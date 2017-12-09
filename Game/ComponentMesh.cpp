#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

#include "ComponentTransform.h"

#include "md5.h"

ComponentMesh::ComponentMesh() : Component(componentType_Mesh)
{
	name = "Mesh";
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
}

ComponentMesh::ComponentMesh(GameObject* argparent) : Component(componentType_Mesh, argparent)
{
	name = "Mesh";
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
}

ComponentMesh::ComponentMesh(componentType argtype, GameObject * argparent) : Component(componentType_Mesh, argparent)
{
	name = "Mesh";
	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;
}

void ComponentMesh::Start()
{
	GetParent()->CreateAABBFromMesh(this);
}

void ComponentMesh::Update(float dt)
{
}

void ComponentMesh::CleanUp()
{
	if (vertices != nullptr)
		mdelete[] vertices;
	if (indices != nullptr)
		mdelete[] indices;
	if (textureCoords != nullptr)
		mdelete[] textureCoords;
	/*if (normals != nullptr)
		mdelete[] normals;*/
	if (skin != nullptr)
		mdelete skin;

	indices = nullptr;
	vertices = nullptr;
	textureCoords = nullptr;

}

void ComponentMesh::Draw(const ComponentTransform* trans, const ComponentMaterial * mat)
{
}

void ComponentMesh::OnEditor()
{
	ImGui::TextColored(COLOR_YELLOW, "Mesh:");
		std::string str = "Triangles: " + std::to_string(this->GetTriangles());
		ImGui::Text(str.c_str());
		str = "Vertices: " + std::to_string(this->num_vertices);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Indices: " + std::to_string(this->num_indices);
		ImGui::Text(str.c_str());
}
void ComponentMesh::Serialize(JSON_Doc * doc)
{
	if (doc == NULL)
		return;

	char* buffer;
	App->fs->Load(path.c_str(), &buffer);

	std::string meshdoc = md5(buffer).c_str();

	doc->SetNumber("type", type);
	doc->SetNumber("parentUID", (parent != nullptr) ? parent->GetUID() : -1);
	doc->SetString("path", path.c_str());
	doc->SetString("rawpath", raw_path.c_str());
	doc->SetString("meshdoc", meshdoc.c_str());

	if (buffer != nullptr)
		mdelete[] buffer;
}

// ------------------------- ANIMATION -------------------------

void ComponentMesh::SetSkin()
{
	if (skin != nullptr)
	{
		skin = new ComponentMesh();
		// fill the skin here with something like:
		// skin->FillYourself();
	}
}

void ComponentMesh::DoSkin(GameObject* go)
{
	//vector<Component*> anims = GetParent()->FindComponents(componentType_Animation);
	//I need the bones to attach them

	//after this the skin should be re-copied
	SetSkin();
	// we send it to the gpu here again
	// i have no idea how to do that but just a reminder I guess

}

void ComponentMesh::ResetDeformableMesh()
{
	if (skin != nullptr)
	{
		ComponentMesh* copy = this;

		memset(skin->indices, 0, copy->num_indices * sizeof(uint));

		memcpy(skin->vertices, copy->vertices, skin->num_vertices * sizeof(float) * 3);

		// we still don't have this loaded rip
		if (copy->normals != nullptr)
		{
			memcpy(skin->normals, copy->normals, skin->num_vertices * sizeof(float) * 3);
		}
	}
}

