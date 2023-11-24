/*
*
*/

#pragma once

#include "math/math.hpp"
#include "shaderprogram.hpp"
#include "thirdparty.hpp"

#define MAX_BONE_INFLUENCE	4

typedef GLuint vo_id_t;
typedef GLuint bo_id_t;
typedef GLuint vertex_id_t;
typedef GLuint texture_id_t;

struct vertex
{
	vec3 position{ };
	vec3 normal{ };
	vec2 texCoords{ };
	vec3 tangent{ };
	vec3 bitangent{ };
	int boneIds[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

class Texture
{
	friend class StaticMesh;
public:
	enum Format
		: GLenum
	{
		Red = GL_RED,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
	};

	enum Type
	{
		None,
		Diffuse,
		Specular,
		Normal,
		Height
	};

	NODISCARD static Texture create(
		const byte_t* _data,
		size_t _width,
		size_t _height,
		Format _format = RGBA,
		Type _type = None
	);

	Texture(texture_id_t _id, Format _format, Type _type) noexcept;

	NODISCARD texture_id_t id( ) const noexcept;

	NODISCARD Format format( ) const noexcept;

	NODISCARD Type type( ) const noexcept;

	void destroy( ) noexcept;

	void bind( ) const noexcept;

	void unbind( ) const noexcept;

private:
	texture_id_t m_id;
	Format m_format;
	Type m_type;

	static GLuint _allocateTexture( ) noexcept;
	static void _bindTexture(GLuint _id) noexcept;
	static void _unbindTexture(GLuint _id) noexcept;
	static void _copyData(
		const GLvoid* _data,
		GLint _format,
		GLsizei _width,
		GLsizei _height);
	static void _setTextureParameters( ) noexcept;
};

class StaticMesh
{
	friend class ModelLoader;
public:
	enum FaceMode
		: GLenum
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		FrontAndBack = GL_FRONT_AND_BACK,
	};

	enum DrawMode
		: GLenum
	{
		Point = GL_POINT,
		Line = GL_LINE,
		Fill = GL_FILL,
	};

	NODISCARD static StaticMesh create(
		const std::pmr::vector<vertex>& _vertices,
		const std::pmr::vector<vertex_id_t>& _indices,
		const std::pmr::vector<Texture>& _textures);

	StaticMesh(
		const std::pmr::vector<vertex>& _vertices,
		const std::pmr::vector<vertex_id_t>& _indices,
		const std::pmr::vector<Texture>& _textures,
		vo_id_t _vao,
		bo_id_t _vbo,
		bo_id_t _ebo);

	NODISCARD const std::pmr::vector<vertex> vertices( ) const noexcept;

	NODISCARD const std::pmr::vector<vertex_id_t> indices( ) const noexcept;

	NODISCARD const std::pmr::vector<Texture> textures( ) const noexcept;

	NODISCARD vo_id_t vao( ) const noexcept;

	NODISCARD bo_id_t vbo( ) const noexcept;

	NODISCARD bo_id_t ebo( ) const noexcept;

	void destroy( ) noexcept;

	void setFaceMode(FaceMode _mode) noexcept;

	void setDrawMode(DrawMode _mode) noexcept;
	
	void bindTextures(const ShaderProgram& _shader);

	void draw(const ShaderProgram& _shader);

	void drawBegin( ) noexcept;

	void drawEnd( ) noexcept;

private:
	std::pmr::vector<vertex> m_vertices;
	std::pmr::vector<vertex_id_t> m_indices;
	std::pmr::vector<Texture> m_textures;
	vo_id_t m_vao;
	bo_id_t m_vbo;
	bo_id_t m_ebo;
	FaceMode m_faceMode;
	DrawMode m_drawMode;

	static GLuint _allocateVertexArray( ) noexcept;
	static void _freeVertexArray(GLuint _vao) noexcept;
	static void _bindVertexArray(GLuint _vao) noexcept;
	static void _unbindVertexArray(GLuint _vao) noexcept;
	static GLuint _allocateBuffer(GLenum _target) noexcept;
	static void _freeBuffer(GLuint _bo) noexcept;
	static void _bindBuffer(GLenum _target, GLuint _bo) noexcept;
	static void _unbindBuffer(GLenum _target, GLuint _bo) noexcept;
	static void _copyData(GLenum _mode, GLsizeiptr _size, const GLvoid* _data) noexcept;
	static void _setAttribPointer(
		GLuint _idx,
		GLsizei _size,
		GLenum _typeHint,
		GLsizei _stride,
		const GLvoid* _offset) noexcept;
};
