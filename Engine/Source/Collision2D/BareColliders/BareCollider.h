#pragma once
#include "ErrorChecker.h"
#include <functional>
#include "ShortHands.h"
#include "GlmTools.h"

//struct Bounds { float x, y; };

struct ITransform // this struct serves as an interface for a transform.
{
	Shorts;
	function<vec3()> GetPosition;
	function<vec3()> GetScale;
	function<vec2(vec2, bool)> ToWorldSpace; // (localVector, isPositionvector) -> worldVector
};

class BareCollider
{
public:
	Shorts;
	ITransform iTransform;
	// Bounds bounds;
	
	//void SetupTransform(ITransform iTransform_) { iTransform = iTransform_; };
	virtual pair<float, float> ShadowAlongNormal(glm::vec2 normal) const 
	{ 
		RaiseError("Must be overridden"); 
		return {0.0f, 0.0f}; 
	};
	virtual vector<vec2> Positions() const
	{
		RaiseError("Must be overridden");
		return { };
	};

protected:
	ITransform MakeTransform(vec2 pos = vec2(), quat rot = glm::NoRotation(), vec2 scale = vec2(1.0f));

};




