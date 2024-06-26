#pragma once
#include <string>

struct VertexLayout
{
	int positionDimension = 2;
	int textureDimension = 2;
	int colorDimension = 0; // color is assumed to be a uniform by default and therefore not stored in the vertices
	int textureID = 0; // textureID is assumed to be a uniform by default and therefore not stored in the vertices

	std::string to_string() const
	{
		return std::string("{")
			+ "positionDimension: " + std::to_string(positionDimension) + ", "
			+ "textureDimension: "+ std::to_string(textureDimension) + ", "
			+ "colorDimension: "+ std::to_string(colorDimension) + ", "
			+ "textureID: "+ std::to_string(textureID) + ", "
			+ "}";
	}
};
