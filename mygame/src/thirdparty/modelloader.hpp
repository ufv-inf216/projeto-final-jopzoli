/*
* 
*/

#pragma once

#include "staticmesh.hpp"
#include "utils/exception.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class ImageException
	: public std::exception
{
public:
	ImageException(const char* _msg) noexcept
		: std::exception(_msg)
	{ }
};

class ImageLoadingError
	: public ImageException
{
public:
	ImageLoadingError(const char* _msg) noexcept
		: ImageException(_msg)
	{ }
};

class ModelLoaderException
	: public std::exception
{
public:
	ModelLoaderException(const char* _msg) noexcept
		: std::exception(_msg)
	{ }
};

class ReadModelFileError
	: public ModelLoaderException
{
public:
	ReadModelFileError(const char* _msg) noexcept
		: ModelLoaderException(_msg)
	{ }
};

class Image
{
public:
	NODISCARD static Image load(const std::string_view& _path);

	static void xImageLoadingError(const std::string_view& _msg);

	Image(
		byte_t* _data,
		size_t _width,
		size_t _height,
		size_t _channels) noexcept;

	~Image( ) noexcept;

	void destroy( ) noexcept;

	NODISCARD const byte_t* data( ) const noexcept;

	NODISCARD size_t channels( ) const noexcept;

	NODISCARD size_t width( ) const noexcept;

	NODISCARD size_t height( ) const noexcept;

private:
	byte_t* m_data;
	size_t m_channels;
	size_t m_width;
	size_t m_height;

	static void* _loadImage(
		const char* _path,
		int* _channels,
		int* _width,
		int* _height);
	static void _freeImage(void* _data) noexcept;
};

class ModelLoader
{
public:
	NODISCARD static std::pmr::vector<StaticMesh> load(const std::string_view _path);

	static void xReadModelFileError(const std::string_view& _msg);

private:
	static void _processNode(
		const std::string_view& _path,
		std::pmr::vector<StaticMesh>& _buffer,
		aiNode* _node,
		const aiScene* _scene);
	static StaticMesh _processMesh(
		const std::string_view& _path,
		aiMesh* _mesh,
		const aiScene* _scene);
	static void _loadTextureMaterial(
		const std::string_view& _path,
		std::pmr::vector<Texture>& _buffer,
		aiMaterial* _material,
		aiTextureType _type);
};
