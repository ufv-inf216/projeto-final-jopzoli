#include "memory/monotonicstorage.hpp"
#include "staticmesh.hpp"

Texture Texture::create(
	const byte_t* _data,
	size_t _width,
	size_t _height,
	Format _format,
	Type _type)
{
	GLuint id = _allocateTexture( );
	_bindTexture(id);
	_copyData(_data, _format, _width, _height);
	_setTextureParameters( );
	_unbindTexture(id);

	return Texture{ id, _format, _type };
}

Texture::Texture(texture_id_t _id, Format _format, Type _type) noexcept
	: m_id{ _id },
	m_format{ _format },
	m_type{ _type }
{ }

texture_id_t Texture::id( ) const noexcept
{
	return m_id;
}

Texture::Format Texture::format( ) const noexcept
{
	return m_format;
}

Texture::Type Texture::type( ) const noexcept
{
	return m_type;
}

void Texture::destroy( ) noexcept
{
	glDeleteTextures(1, &m_id);
	m_id = GL_NONE;
}

void Texture::bind( ) const noexcept
{
	_bindTexture(m_id);
}

void Texture::unbind( ) const noexcept
{
	_unbindTexture(m_id);
}

GLuint Texture::_allocateTexture( ) noexcept
{
	GLuint id;
	glGenTextures(1, &id);

	return id;
}

void Texture::_bindTexture(GLuint _id) noexcept
{
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::_unbindTexture(GLuint _id) noexcept
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::_copyData(const GLvoid* _data, GLint _format, GLsizei _width, GLsizei _height)
{
	glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::_setTextureParameters( ) noexcept
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// TODO: mesh binder
NODISCARD StaticMesh StaticMesh::create(
	const std::pmr::vector<vertex>& _vertices,
	const std::pmr::vector<vertex_id_t>& _indices,
	const std::pmr::vector<Texture>& _textures)
{
	vo_id_t vao = _allocateVertexArray( );
	bo_id_t vbo = _allocateBuffer(GL_ARRAY_BUFFER);
	bo_id_t ebo = _allocateBuffer(GL_ELEMENT_ARRAY_BUFFER);

	_bindVertexArray(vao);

	_bindBuffer(GL_ARRAY_BUFFER, vbo);
	_copyData(
		GL_ARRAY_BUFFER,
		_vertices.size( ) * sizeof(vertex),
		_vertices.data( )); // TODO: revisar

	_bindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	_copyData(
		GL_ELEMENT_ARRAY_BUFFER,
		_indices.size( ) * sizeof(vertex_id_t),
		_indices.data( ));

#define BIND_VERTEX_WITH(x, idx, typeHint)	_setAttribPointer(	\
	idx,														\
	sizeof(vertex::x) / sizeof(vertex::x[0]),					\
	typeHint,													\
	sizeof(vertex),												\
	(GLvoid*) (offsetof(vertex, x)))

	BIND_VERTEX_WITH(position, 0, GL_FLOAT);
	BIND_VERTEX_WITH(normal, 1, GL_FLOAT);
	BIND_VERTEX_WITH(texCoords, 2, GL_FLOAT);
	BIND_VERTEX_WITH(tangent, 3, GL_FLOAT);
	BIND_VERTEX_WITH(bitangent, 4, GL_FLOAT);
	BIND_VERTEX_WITH(boneIds, 5, GL_INT);
	BIND_VERTEX_WITH(weights, 6, GL_FLOAT);

	_unbindVertexArray(vao);
	_unbindBuffer(GL_ARRAY_BUFFER, vbo);
	_unbindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	return StaticMesh{ _vertices, _indices, _textures, vao, vbo, ebo };
}

StaticMesh::StaticMesh(
	const std::pmr::vector<vertex>& _vertices,
	const std::pmr::vector<vertex_id_t>& _indices,
	const std::pmr::vector<Texture>& _textures,
	vo_id_t _vao,
	bo_id_t _vbo,
	bo_id_t _ebo)
	: m_vertices{ _vertices },
	m_indices{ _indices },
	m_textures{ _textures },
	m_vao{ _vao },
	m_vbo{ _vbo },
	m_ebo{ _ebo },
	m_faceMode{ Front },
	m_drawMode{ Fill }
{ }

const std::pmr::vector<vertex> StaticMesh::vertices( ) const noexcept
{
	return m_vertices;
}

const std::pmr::vector<vertex_id_t> StaticMesh::indices( ) const noexcept
{
	return m_indices;
}

const std::pmr::vector<Texture> StaticMesh::textures( ) const noexcept
{
	return m_textures;
}

vo_id_t StaticMesh::vao( ) const noexcept
{
	return m_vao;
}

bo_id_t StaticMesh::vbo( ) const noexcept
{
	return m_vbo;
}

bo_id_t StaticMesh::ebo( ) const noexcept
{
	return m_ebo;
}

void StaticMesh::destroy( ) noexcept
{
	_freeBuffer(m_vbo);
	m_vbo = 0;
	_freeBuffer(m_ebo);
	m_ebo = 0;
	_freeVertexArray(m_vao);
	m_vao = 0;
}

void StaticMesh::setFaceMode(FaceMode _mode) noexcept
{
	m_faceMode = _mode;
}

void StaticMesh::setDrawMode(DrawMode _mode) noexcept
{
	m_drawMode = _mode;
}

void StaticMesh::bindTextures(const ShaderProgram& _shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;

	GLuint number;
	std::string_view name;
	// deveria ser i = 1???
	for (size_t i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		switch (m_textures[i].m_type)
		{
		case Texture::Diffuse:
		{
			name = "texture_diffuse";
			number = diffuseNr++;
			break;
		}
		case Texture::Specular:
		{
			name = "texture_specular";
			number = specularNr++;
			break;
		}
		case Texture::Normal:
		{
			name = "texture_normal";
			number = normalNr++;
			break;
		}
		case Texture::Height:
		{
			name = "texture_height";
			number = heightNr++;
			break;
		}
		default:
			number = 0;
			break;
		}

		_shader.setInt(name.data( ) + std::to_string(number), i);
		m_textures[i].bind( );
	}
}

void StaticMesh::draw(const ShaderProgram& _shader)
{
	bindTextures(_shader);
	drawBegin( );
	drawEnd( );
}

void StaticMesh::drawBegin( ) noexcept
{
	//glPolygonMode(m_faceMode, m_drawMode);
	_bindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size( ), GL_UNSIGNED_INT, (const GLvoid*) (0));
}

void StaticMesh::drawEnd( ) noexcept
{
	_unbindVertexArray(m_vao);
	glActiveTexture(GL_TEXTURE0);
}

GLuint StaticMesh::_allocateVertexArray( ) noexcept
{
	GLuint vao;
	glGenVertexArrays(1, &vao);

	return vao;
}

void StaticMesh::_freeVertexArray(GLuint _vao) noexcept
{
	glDeleteVertexArrays(1, &_vao);
}

void StaticMesh::_bindVertexArray(GLuint _vao) noexcept
{
	glBindVertexArray(_vao);
}

void StaticMesh::_unbindVertexArray(GLuint _vao) noexcept
{
	glBindVertexArray(GL_FALSE);
}

GLuint StaticMesh::_allocateBuffer(GLenum _target) noexcept
{
	GLuint buffer;
	glGenBuffers(1, &buffer);

	return buffer;
}

void StaticMesh::_freeBuffer(GLuint _bo) noexcept
{
	glDeleteBuffers(1, &_bo);
}

void StaticMesh::_bindBuffer(GLenum _target, GLuint _bo) noexcept
{
	glBindBuffer(_target, _bo);
}

void StaticMesh::_unbindBuffer(GLenum _target, GLuint _bo) noexcept
{
	glBindBuffer(_target, GL_FALSE);
}

void StaticMesh::_copyData(GLenum _mode, GLsizeiptr _size, const GLvoid* _data) noexcept
{
	glBufferData(_mode, _size, _data, GL_STATIC_DRAW);
}

void StaticMesh::_setAttribPointer(
	GLuint _idx,
	GLsizei _size,
	GLenum _typeHint,
	GLsizei _stride,
	const GLvoid* _offset) noexcept
{
	switch (_typeHint)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_INT:
	case GL_UNSIGNED_INT:
	{
		glVertexAttribIPointer(_idx, _size, _typeHint, _stride, _offset);
		break;
	}
	case GL_DOUBLE:
	{
		glVertexAttribLPointer(_idx, _size, _typeHint, _stride, _offset);
		break;
	}
	default:
	{
		glVertexAttribPointer(_idx, _size, _typeHint, GL_FALSE, _stride, _offset);
		break;
	}
	}
	glEnableVertexAttribArray(_idx);
}
