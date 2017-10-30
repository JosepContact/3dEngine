#pragma once

#include "Component.h"

class ComponentMesh : public Component {
public:
	ComponentMesh();
	ComponentMesh(GameObject* argparent);
	ComponentMesh(componentType argtype, GameObject* argparent);
	virtual ~ComponentMesh() {};

public:
	void Update(float dt);
	void Draw(const ComponentMaterial* mat = nullptr);
	void OnEditor();
public:
	//Vertices
	unsigned int id_vertices = 0; // id in VRAM
	unsigned int num_indices = 0;
	unsigned int* indices = nullptr;
	//Indices
	unsigned int id_indices = 0; // id in VRAM
	unsigned int num_vertices = 0;
	float* vertices = nullptr;
	//Triangles
	unsigned int num_triangles = 0; // A.K.A. num_faces
	//UVs
	unsigned int id_UV = 0; // id in VRAM
	float* textureCoords; // UV Coords
	unsigned int num_UV = 0;
	unsigned int num_UVChannels = 0;

	float material_index = 0; // material ID

	bool visible = true;
};