#pragma once
#ifndef __RESOURCEBONE_H__
#define __RESOURCEBONE_H__

#include <string>
#include <vector>
#include "ComponentMesh.h"
#include "MathGeoLib\MathGeoLib.h"

using namespace std;

class ResourceBone {
public:
	string name;
	// Normal?�?�?
	uint num_weigths = 0;
	uint* indices = nullptr;
	float* weigths = nullptr;
	ComponentMesh* mesh = nullptr;
	GameObject* object;
	// The reason why we need this matrix is because 
	// the vertices are stored in the usual local space. 
	// This means that even without skeletal animation support 
	// our existing code base can load the model and render it correctly.
	float4x4 offsetMat = float4x4::identity; // ?�?�? the website says I need this. EDIT: oh
public:
	void CleanUp();
};

// for later:
// http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html

#endif // !__BONE_H__
#pragma once
