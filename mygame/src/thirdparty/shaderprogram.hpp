/*
*
*/

#pragma once

#include "math/math.hpp"
#include "object/object.hpp"
#include "thirdparty.hpp"
#include "utils/exception.hpp"
#include <vector>
#include <shader_resource.hpp>
#include <string>

typedef GLuint program_id_t;

struct variable_info
{
	static constexpr size_t NSize = static_cast<size_t>(-1);

	std::string name{ };
	size_t size{ NSize };
	GLenum typeHint{ GL_NONE };
};

struct attrib_info : variable_info { };

struct uniform_info : variable_info { };

struct lightSource
{
	vec3 pos;
	vec3 color;
};

class ProgramShaderException
	: public std::exception
{
public:
	ProgramShaderException(const char* _msg) noexcept
		: exception(_msg)
	{ }
};

class CompileError
	: public ProgramShaderException
{
public:
	CompileError(const char* _msg) noexcept
		: ProgramShaderException(_msg)
	{ }
};

class LinkError
	: public ProgramShaderException
{
public:
	LinkError(const char* _msg) noexcept
		: ProgramShaderException(_msg)
	{ }
};

class ShaderProgram
	: public Object
{
public:
	OBJIMPL( );

	NODISCARDRAWPTR static ShaderProgram* create(
		const std::string_view& _vertexShaderSrc,
		const std::string_view& _fragmentShaderSrc);

	static void xCompileError(const std::string_view& _msg);

	static void xLinkError(const std::string_view& _msg);

	ShaderProgram(GLuint _programId) noexcept;

	NODISCARD program_id_t programId( ) const noexcept;

	NODISCARD const std::pmr::vector<attrib_info> activeAttribs( ) const noexcept;

	NODISCARD const std::pmr::vector<uniform_info> activeUniforms( ) const noexcept;

	NODISCARD bool hasAttrib(const attrib_info& _attribInfo) const noexcept;

	NODISCARD bool hasUniform(const uniform_info& _uniformInfo) const noexcept;

	NODISCARD attrib_info getAttribInfo(const std::string_view& _name) const noexcept;

	NODISCARD uniform_info getUniformInfo(const std::string_view& _name) const noexcept;

	void destroy( ) noexcept;

	bool use( ) const noexcept;

	bool setFloat(const std::string_view& _name, float _val) const noexcept;

	bool setFloatVec2(const std::string_view& _name, const vec2& _val) const noexcept;

	bool setFloatVec3(const std::string_view& _name, const vec3& _val) const noexcept;

	bool setFloatVec4(const std::string_view& _name, const vec4& _val) const noexcept;

	// ... double types only gl >= v4.1

	bool setInt(const std::string_view& _name, int _val) const noexcept;
	
	bool setIntVec2(const std::string_view& _name, const ivec2& _val) const noexcept;

	bool setIntVec3(const std::string_view& _name, const ivec3& _val) const noexcept;

	bool setIntVec4(const std::string_view& _name, const ivec4& _val) const noexcept;

	bool setUInt(const std::string_view& _name, unsigned int _val) const noexcept;
	
	bool setUIntVec2(const std::string_view& _name, const uvec2& _val) const noexcept;

	bool setUIntVec3(const std::string_view& _name, const uvec3& _val) const noexcept;

	bool setUIntVec4(const std::string_view& _name, const uvec4& _val) const noexcept;

	bool setBool(const std::string_view& _name, bool _val) const noexcept;
	
	bool setBoolVec2(const std::string_view& _name, const bvec2& _val) const noexcept;

	bool setBoolVec3(const std::string_view& _name, const bvec3& _val) const noexcept;

	bool setBoolVec4(const std::string_view& _name, const bvec4& _val) const noexcept;

	bool setFloatMat2(const std::string_view& _name, const mat2& _val) const noexcept;

	bool setFloatMat3(const std::string_view& _name, const mat3& _val) const noexcept;

	bool setFloatMat4(const std::string_view& _name, const mat4& _val) const noexcept;

	bool setFloatMat2x3(const std::string_view& _name, const mat2x3& _val) const noexcept;

	bool setFloatMat2x4(const std::string_view& _name, const mat2x4& _val) const noexcept;

	bool setFloatMat3x2(const std::string_view& _name, const mat3x2& _val) const noexcept;

	bool setFloatMat3x4(const std::string_view& _name, const mat3x4& _val) const noexcept;

	bool setFloatMat4x2(const std::string_view& _name, const mat4x2& _val) const noexcept;

	bool setFloatMat4x3(const std::string_view& _name, const mat4x3& _val) const noexcept;

	// ... samplers

	// ... images types only gl >= 4.2

private:
	program_id_t m_programId;

	static GLuint _createProgram( ) noexcept;
	static GLuint _compile(const char* _shader, GLenum _type) noexcept;
	static std::string _getCompileLog(GLuint _shaderId) noexcept;
	static GLuint _link(GLuint _programId) noexcept;
	static std::string _getLinkLog(GLuint _programId) noexcept;
	static void _attachShader(GLuint _programId, GLuint _shaderId) noexcept;
	static void _detachShader(GLuint _programId, GLuint _shaderId) noexcept;
	static void _deleteShader(GLuint _shaderId) noexcept;
	static bool _validateProgram(GLuint _programId) noexcept;
	static bool _validateShader(GLuint _shaderId) noexcept;

	size_t _getActiveAttribCount( ) const noexcept;
	attrib_info _getActiveAttribInfo(GLuint _idx) const noexcept;
	size_t _getActiveUniformCount( ) const noexcept;
	uniform_info _getActiveUniformInfo(GLuint _idx) const noexcept;
};
OBJDECL(ShaderProgram, Object);
