#include "shaderprogram.hpp"
#include "utils/logger.hpp"
#include <glm/gtc/type_ptr.hpp>

#define INLINE_BUFFER_SIZE	512

ShaderProgram ShaderProgram::create(
	const std::string_view& _vertexShaderSrc,
	const std::string_view& _fragmentShaderSrc)
{
	GLuint vertexShaderId = _compile(_vertexShaderSrc.data( ), GL_VERTEX_SHADER);
	if (!_validateShader(vertexShaderId))
	{
		xCompileError("Could not compile vertex shader, got GL error info: "
			+ _getCompileLog(vertexShaderId));
	}

	GLuint fragmentShaderId = _compile(_fragmentShaderSrc.data( ), GL_FRAGMENT_SHADER);
	if (!_validateShader(fragmentShaderId))
	{
		xCompileError("Could not compile fragment shader, got GL error info: "
			+ _getCompileLog(fragmentShaderId));
	}

	GLuint programId = _createProgram( );
	_attachShader(programId, vertexShaderId);
	_attachShader(programId, fragmentShaderId);
	_link(programId);
	_detachShader(programId, vertexShaderId);
	_detachShader(programId, fragmentShaderId);
	if (!_validateProgram(programId))
	{
		xLinkError("Could not compile fragment shader, got GL error info: "
			+ _getLinkLog(programId));
	}

	return ShaderProgram{ programId };
}

void ShaderProgram::xCompileError(const std::string_view& _msg)
{
	throw CompileError(_msg.data( ));
}

void ShaderProgram::xLinkError(const std::string_view& _msg)
{
	throw LinkError(_msg.data( ));
}

ShaderProgram::ShaderProgram(GLuint _programId) noexcept
	: m_programId{ _programId }
{ }

program_id_t ShaderProgram::programId( ) const noexcept
{
	return m_programId;
}

const std::pmr::vector<attrib_info> ShaderProgram::activeAttribs( ) const noexcept
{
	std::pmr::vector<attrib_info> attribs{ singleton_memory::getPool( ) };
	for (GLuint i = 0; i < _getActiveAttribCount( ); i++)
		attribs.push_back(_getActiveAttribInfo(i));

	return attribs;
}

const std::pmr::vector<uniform_info> ShaderProgram::activeUniforms( ) const noexcept
{
	std::pmr::vector<uniform_info> uniforms{ singleton_memory::getPool( ) };
	for (GLuint i = 0; i < _getActiveUniformCount( ); i++)
		uniforms.push_back(_getActiveUniformInfo(i));

	return uniforms;
}

bool ShaderProgram::hasAttrib(const attrib_info& _attribInfo) const noexcept
{
	GLint id = glGetAttribLocation(m_programId, _attribInfo.name.data( ));
	if (id < 0)
		return false;

	attrib_info info = _getActiveAttribInfo(id);
	if (_attribInfo.size != info.size
		|| _attribInfo.typeHint != info.typeHint)
		return false;

	return true;
}

bool ShaderProgram::hasUniform(const uniform_info& _uniformInfo) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _uniformInfo.name.data( ));
	if (id < 0)
		return false;

	uniform_info info = _getActiveUniformInfo(id);
	if (_uniformInfo.size != info.size
		|| _uniformInfo.typeHint != info.typeHint)
		return false;

	return true;
}

attrib_info ShaderProgram::getAttribInfo(const std::string_view& _name) const noexcept
{
	GLint id = glGetAttribLocation(m_programId, _name.data( ));
	if (id < 0)
		return { };

	return _getActiveAttribInfo(id);
}

uniform_info ShaderProgram::getUniformInfo(const std::string_view& _name) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	if (id < 0)
		return { };

	return _getActiveUniformInfo(id);
}

void ShaderProgram::destroy( ) noexcept
{
	glDeleteProgram(m_programId);
	m_programId = GL_FALSE;
}

bool ShaderProgram::use( ) const noexcept
{
	glUseProgram(m_programId);

	return m_programId != 0;
}

bool ShaderProgram::setFloat(const std::string_view& _name, float _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform1f(id, _val);

	return id > -1;
}

bool ShaderProgram::setFloatVec2(const std::string_view& _name, const vec2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform2fv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatVec3(const std::string_view& _name, const vec3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform3fv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatVec4(const std::string_view& _name, const vec4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform4fv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setInt(const std::string_view& _name, int _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform1i(id, _val);

	return id > -1;
}

bool ShaderProgram::setIntVec2(const std::string_view& _name, const ivec2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform2iv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setIntVec3(const std::string_view& _name, const ivec3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform3iv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setIntVec4(const std::string_view& _name, const ivec4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform4iv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setUInt(const std::string_view& _name, unsigned int _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform1ui(id, _val);

	return id > -1;
}

bool ShaderProgram::setUIntVec2(const std::string_view& _name, const uvec2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform2uiv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setUIntVec3(const std::string_view& _name, const uvec3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform3uiv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setUIntVec4(const std::string_view& _name, const uvec4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform4uiv(id, 1, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setBool(const std::string_view& _name, bool _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform1i(id, _val);

	return id > -1;
}

bool ShaderProgram::setBoolVec2(const std::string_view& _name, const bvec2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform2iv(id, 1, reinterpret_cast<const GLint*>(glm::value_ptr(_val)));

	return id > -1;
}

bool ShaderProgram::setBoolVec3(const std::string_view& _name, const bvec3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform3iv(id, 1, reinterpret_cast<const GLint*>(glm::value_ptr(_val)));

	return id > -1;
}

bool ShaderProgram::setBoolVec4(const std::string_view& _name, const bvec4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniform4iv(id, 1, reinterpret_cast<const GLint*>(glm::value_ptr(_val)));

	return id > -1;
}

bool ShaderProgram::setFloatMat2(const std::string_view& _name, const mat2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix2fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat3(const std::string_view& _name, const mat3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat4(const std::string_view& _name, const mat4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat2x3(const std::string_view& _name, const mat2x3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix2x3fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat2x4(const std::string_view& _name, const mat2x4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix2x4fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat3x2(const std::string_view& _name, const mat3x2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix3x2fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat3x4(const std::string_view& _name, const mat3x4& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix3x4fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat4x2(const std::string_view& _name, const mat4x2& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix4x2fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

bool ShaderProgram::setFloatMat4x3(const std::string_view& _name, const mat4x3& _val) const noexcept
{
	GLint id = glGetUniformLocation(m_programId, _name.data( ));
	glUniformMatrix4x3fv(id, 1, GL_FALSE, glm::value_ptr(_val));

	return id > -1;
}

GLuint ShaderProgram::_createProgram( ) noexcept
{
	return glCreateProgram( );
}

GLuint ShaderProgram::_compile(
	const char* _shader,
	GLenum _type
) noexcept
{
	GLuint outShader = glCreateShader(_type);
	glShaderSource(outShader, 1, &_shader, nullptr);
	glCompileShader(outShader);

	return outShader;
}

std::string ShaderProgram::_getCompileLog(GLuint _shaderId) noexcept
{
	char buffer[INLINE_BUFFER_SIZE];
	GLsizei size;
	glGetShaderInfoLog(_shaderId, INLINE_BUFFER_SIZE, &size, buffer);

	return std::string(buffer, size);
}

GLuint ShaderProgram::_link(GLuint _program) noexcept
{
	glLinkProgram(_program);

	return _program;
}

std::string ShaderProgram::_getLinkLog(GLuint _programId) noexcept
{
	char buffer[INLINE_BUFFER_SIZE];
	GLsizei size;
	glGetProgramInfoLog(_programId, INLINE_BUFFER_SIZE, &size, buffer);

	return std::string(buffer, size);
}

void ShaderProgram::_attachShader(GLuint _programId, GLuint _shaderId) noexcept
{
	glAttachShader(_programId, _shaderId);
}

void ShaderProgram::_detachShader(GLuint _programId, GLuint _shaderId) noexcept
{
	glDetachShader(_programId, _shaderId);
	_deleteShader(_shaderId);
}

void ShaderProgram::_deleteShader(GLuint _shaderId) noexcept
{
	glDeleteShader(_shaderId);
}

bool ShaderProgram::_validateProgram(GLuint _program) noexcept
{
	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);

	return status == GL_TRUE;
}

bool ShaderProgram::_validateShader(GLuint _shader) noexcept
{
	GLint status;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &status);

	return status == GL_TRUE;
}

size_t ShaderProgram::_getActiveAttribCount( ) const noexcept
{
	GLsizei count;
	glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &count);

	return static_cast<size_t>(count);
}

attrib_info ShaderProgram::_getActiveAttribInfo(GLuint _idx) const noexcept
{
	char buffer[INLINE_BUFFER_SIZE];
	GLsizei size;
	GLint typeSize;
	GLenum typeHint;
	glGetActiveAttrib(
		m_programId,
		_idx,
		INLINE_BUFFER_SIZE,
		&size,
		&typeSize,
		&typeHint,
		buffer);

	return {
		std::string(buffer, size),
		static_cast<size_t>(typeSize),
		typeHint
	};
}

size_t ShaderProgram::_getActiveUniformCount( ) const noexcept
{
	GLsizei count;
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &count);

	return static_cast<size_t>(count);
}

uniform_info ShaderProgram::_getActiveUniformInfo(GLuint _idx) const noexcept
{
	char buffer[INLINE_BUFFER_SIZE];
	GLsizei size;
	GLint typeSize;
	GLenum typeHint;
	glGetActiveUniform(
		m_programId,
		_idx,
		INLINE_BUFFER_SIZE,
		&size,
		&typeSize,
		&typeHint,
		buffer);

	return {
		std::string(buffer, size),
		static_cast<size_t>(typeSize),
		typeHint
	};
}
