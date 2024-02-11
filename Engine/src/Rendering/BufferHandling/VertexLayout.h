#pragma once


struct VertexLayout
{
	int positionDimension = 2;
	int textureDimension = 2;
	int colorDimension = 0; // color is assumed to be a uniform by default and therefore not stored in the vertices
	int textureID = 0; // textureID is assumed to be a uniform by default and therefore not stored in the vertices
};
