/*
* 
*/

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "utils/asserts.hpp"
#include <algorithm>
#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <limits>

using glm::vec1;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::bvec1;
using glm::bvec2;
using glm::bvec3;
using glm::bvec4;
using glm::dvec1;
using glm::dvec2;
using glm::dvec3;
using glm::dvec4;
using glm::ivec1;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::uvec1;
using glm::uvec2;
using glm::uvec3;
using glm::uvec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::mat2x2;
using glm::mat2x3;
using glm::mat2x4;
using glm::mat3x2;
using glm::mat3x3;
using glm::mat3x4;
using glm::mat4x2;
using glm::mat4x3;
using glm::mat4x4;
using glm::dmat2x2;
using glm::dmat2x3;
using glm::dmat2x4;
using glm::dmat3x2;
using glm::dmat3x3;
using glm::dmat3x4;
using glm::dmat4x2;
using glm::dmat4x3;
using glm::dmat4x4;

using glm::quat;

template <glm::length_t _Size, typename _Ty, glm::qualifier _QlyTy>
std::ostream& operator<<(std::ostream& _lhs, const glm::vec<_Size, _Ty, _QlyTy>& _rhs)
{
	_lhs << to_string(_rhs);

	return _lhs;
}

template <glm::length_t _Size, glm::length_t _Ext, typename _Ty, glm::qualifier _QlyTy>
std::ostream& operator<<(std::ostream& _lhs, const glm::mat<_Size, _Ext, _Ty, _QlyTy>& _rhs)
{
	_lhs << to_string(_rhs);

	return _lhs;
}
