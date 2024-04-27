#pragma once
#include "GlmTools.h"
#include "ShortHands.h"

#ifdef _DEBUG
#define Check_z(toCheck)  GlmCheck::_Check(#toCheck, toCheck, true, true)   // zeroable
#define Check_nz(toCheck) GlmCheck::_Check(#toCheck, toCheck, false, true)  // non-zero
#define Check_u(toCheck)  GlmCheck::_Check(#toCheck, toCheck, true, false)  // unsgined
#define Check_p(toCheck)  GlmCheck::_Check(#toCheck, toCheck, false, false) // positive
#else
#define Check_z(toCheck)  toCheck
#define Check_nz(toCheck) toCheck
#define Check_u(toCheck)  toCheck
#define Check_p(toCheck)  toCheck
#endif // _DEBUG

#define Realistic_nz(toMakeSafe) GlmCheck::_Realistic(toMakeSafe, true)
#define Realistic_p(toMakeSafe) GlmCheck::_Realistic(toMakeSafe, false)


namespace GlmCheck
{
	//Shorts;

	float _Realistic(float toMakeSafe, bool signed_);
	float _Check(std::string variableName, float toCheck, bool zeroable, bool signed_);
	/*
	vec2 _Check(vec2 toCheck, bool zeroable, bool signed_);
	vec3 _Check(vec3 toCheck, bool zeroable, bool signed_);
	vec4 _Check(vec4 toCheck, bool zeroable, bool signed_);
	mat4 _Check(mat4 toCheck, bool zeroable, bool signed_);
	quat _Check(quat toCheck, bool zeroable, bool signed_);
	*/
	//template<typename T> void _Check_nz(T glmObjectToCheck, std::string message) {}

};

