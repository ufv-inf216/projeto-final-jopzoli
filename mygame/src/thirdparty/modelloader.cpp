#include "memory/memory.hpp"
#include "modelloader.hpp"
#include <assimp/Importer.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace /* anonymous */
{
std::pmr::vector<std::string> loadedTextures{ singleton_memory::getPool( ) };
}

// TODO: exceptions
Image Image::load(const std::string_view& _path)
{
	int width, height, channels;
	byte_t* data = static_cast<byte_t*>(_loadImage(
		_path.data( ),
		&channels,
		&width,
		&height));
	if (!data)
	{
		xImageLoadingError(std::string("Could not load image file, stbi got error: ")
			+ stbi_failure_reason( ));
	}

	return Image{
		data,
		static_cast<size_t>(width),
		static_cast<size_t>(height),
		static_cast<size_t>(channels)
	};
}

void Image::xImageLoadingError(const std::string_view& _msg)
{
	throw ImageLoadingError(_msg.data( ));
}

Image::Image(
	byte_t* _data,
	size_t _width,
	size_t _height,
	size_t _channels) noexcept
	: m_data{ _data },
	m_width{ _width },
	m_height{ _height },
	m_channels{ _channels }
{ }

Image::~Image( ) noexcept
{
	destroy( );
}

void Image::destroy( ) noexcept
{
	_freeImage(m_data);
}

const byte_t* Image::data( ) const noexcept
{
	return m_data;
}

size_t Image::channels( ) const noexcept
{
	return m_channels;
}

size_t Image::width( ) const noexcept
{
	return m_width;
}

size_t Image::height( ) const noexcept
{
	return m_height;
}

void* Image::_loadImage(
	const char* _path,
	int* _channels,
	int* _width,
	int* _height)
{
	return stbi_load(_path, _width, _height, _channels, 0);
}

void Image::_freeImage(void* _data) noexcept
{
	stbi_image_free(_data);
}

std::pmr::vector<StaticMesh> ModelLoader::load(const std::string_view _path)
{
	stbi_set_flip_vertically_on_load(true);
	std::pmr::vector<StaticMesh> buffer;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		_path.data( ),
		aiProcess_Triangulate | aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene
		|| scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
		|| !scene->mRootNode)
	{
		xReadModelFileError("Could not create model file, assimp got error: "
			+ std::string(importer.GetErrorString( )));
	}

	loadedTextures.clear( );
	_processNode(_path, buffer, scene->mRootNode, scene);

	return buffer;
}

void ModelLoader::xReadModelFileError(const std::string_view& _msg)
{
	throw ReadModelFileError(_msg.data( ));
}

void ModelLoader::_processNode(
	const std::string_view& _path,
	std::pmr::vector<StaticMesh>& _buffer,
	aiNode* _node,
	const aiScene* _scene)
{
	_buffer.reserve(_node->mNumMeshes);
	aiMesh* mesh;
	for (size_t i = 0; i < _node->mNumMeshes; i++)
	{
		mesh = _scene->mMeshes[_node->mMeshes[i]];
		_buffer.push_back(_processMesh(_path, mesh, _scene));
	}

	for (size_t i = 0; i < _node->mNumChildren; i++)
		_processNode(_path, _buffer, _node->mChildren[i], _scene);
}

StaticMesh ModelLoader::_processMesh(
	const std::string_view& _path,
	aiMesh* _mesh,
	const aiScene* _scene)
{
	std::pmr::vector<vertex> vertices{ singleton_memory::getPool( ) };
	std::pmr::vector<vertex_id_t> indices{ singleton_memory::getPool( ) };
	std::pmr::vector<Texture> textures{ singleton_memory::getPool( ) };

	vertices.reserve(_mesh->mNumVertices);
	for (size_t i = 0; i < _mesh->mNumVertices; i++)
	{
		vertex vert;
		vert.position = {
			_mesh->mVertices[i].x,
			_mesh->mVertices[i].y,
			_mesh->mVertices[i].z
		};
		if (_mesh->HasNormals( ))
		{
			vert.normal = {
				_mesh->mNormals[i].x,
				_mesh->mNormals[i].y,
				_mesh->mNormals[i].z
			};
		}
		if (_mesh->mTextureCoords[0])
		{
			vert.texCoords = {
				_mesh->mTextureCoords[0][i].x,
				_mesh->mTextureCoords[0][i].y
			};
			vert.tangent = {
				_mesh->mTangents[i].x,
				_mesh->mTangents[i].y,
				_mesh->mTangents[i].z,
			};
			vert.bitangent = {
				_mesh->mBitangents[i].x,
				_mesh->mBitangents[i].y,
				_mesh->mBitangents[i].z
			};
		}
		vertices.push_back(vert);
	}

	aiFace face;
	for (size_t i = 0; i < _mesh->mNumFaces; i++)
	{
		face = _mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = _scene->mMaterials[_mesh->mMaterialIndex];
	_loadTextureMaterial(
		_path,
		textures,
		material,
		aiTextureType_DIFFUSE);
	_loadTextureMaterial(
		_path,
		textures,
		material,
		aiTextureType_SPECULAR);
	_loadTextureMaterial(
		_path,
		textures,
		material,
		aiTextureType_HEIGHT);
	_loadTextureMaterial(
		_path,
		textures,
		material,
		aiTextureType_AMBIENT);

	return StaticMesh::create(vertices, indices, textures);
}

void ModelLoader::_loadTextureMaterial(
	const std::string_view& _path,
	std::pmr::vector<Texture>& _buffer,
	aiMaterial* _material,
	aiTextureType _type)
{
	aiString str;
	for (size_t i = 0; i < _material->GetTextureCount(_type); i++)
	{
		_material->GetTexture(_type, i, &str);
		auto it = std::find(loadedTextures.begin( ),
			loadedTextures.end( ),
			str.C_Str( ));
		if (it != loadedTextures.end( ))
			continue;

		Texture::Type texType;
		switch (_type)
		{
		case aiTextureType_DIFFUSE:
		{
			texType = Texture::Diffuse;
			break;
		}
		case aiTextureType_SPECULAR:
		{
			texType = Texture::Specular;
			break;
		}
		case aiTextureType_AMBIENT:
		{
			texType = Texture::Height;
			break;
		}
		case aiTextureType_HEIGHT:
		{
			texType = Texture::Normal;
			break;
		}
		/*case aiTextureType_NONE:
		{
			break;
		}
		case aiTextureType_EMISSIVE:
		{
			break;
		}
		case aiTextureType_NORMALS:
		{
			break;
		}
		case aiTextureType_SHININESS:
		{
			break;
		}
		case aiTextureType_OPACITY:
		{
			break;
		}
		case aiTextureType_DISPLACEMENT:
		{
			break;
		}
		case aiTextureType_LIGHTMAP:
		{
			break;
		}
		case aiTextureType_REFLECTION:
		{
			break;
		}
		case aiTextureType_BASE_COLOR:
		{
			break;
		}
		case aiTextureType_NORMAL_CAMERA:
		{
			break;
		}
		case aiTextureType_EMISSION_COLOR:
		{
			break;
		}
		case aiTextureType_METALNESS:
		{
			break;
		}
		case aiTextureType_DIFFUSE_ROUGHNESS:
		{
			break;
		}
		case aiTextureType_AMBIENT_OCCLUSION:
		{
			break;
		}
		case aiTextureType_SHEEN:
		{
			break;
		}
		case aiTextureType_CLEARCOAT:
		{
			break;
		}
		case aiTextureType_TRANSMISSION:
		{
			break;
		}
		case aiTextureType_UNKNOWN:
		{
			break;
		}
		case _aiTextureType_Force32Bit:
		{
			break;
		}*/
		default:
		{
			texType = Texture::None;
			break;
		}
		}

		size_t at = _path.find_last_of('/');
		_ENSURES(at != std::string_view::npos, "invalid path given for texture file");
		std::string path(_path.substr(0, at + 1));
		path += str.C_Str( );
		Image img = Image::load(path);

		_buffer.push_back(Texture::create(
			img.data( ),
			img.width( ),
			img.height( ),
			img.channels( ) == 1
			? Texture::Red : img.channels( ) == 3
			? Texture::RGB : Texture::RGBA,
			texType));
		loadedTextures.push_back(str.C_Str( ));
	}
}
