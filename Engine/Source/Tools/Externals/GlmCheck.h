#pragma once
#include "GlmTools.h"
#include "ShortHands.h"

#ifdef _DEBUG
#define Check_z(toCheck)  GlmCheck::_Check(toCheck, true, true)
#define Check_nz(toCheck) GlmCheck::_Check(toCheck, false, true)
#define Check_u(toCheck)  GlmCheck::_Check(toCheck, true, false)
#define Check_p(toCheck)  GlmCheck::_Check(toCheck, false, false)
#else
#define Check_z(toCheck)  toCheck
#define Check_nz(toCheck) toCheck
#define Check_u(toCheck)  toCheck
#define Check_p(toCheck)  toCheck
#endif // _DEBUG

namespace GlmCheck
{
	Shorts;
	const float moderatelySmall = glm::pow(10.0f, -6.0f);
	const float moderatelyLarge = glm::pow(10.0f, 6.0f);


	float _Check(float toCheck, bool zeroable, bool signed_);
	/*
	vec2 _Check(vec2 toCheck, bool zeroable, bool signed_);
	vec3 _Check(vec3 toCheck, bool zeroable, bool signed_);
	vec4 _Check(vec4 toCheck, bool zeroable, bool signed_);
	mat4 _Check(mat4 toCheck, bool zeroable, bool signed_);
	quat _Check(quat toCheck, bool zeroable, bool signed_);
	*/
	//template<typename T> void _Check_nz(T glmObjectToCheck, std::string message) {}

};

