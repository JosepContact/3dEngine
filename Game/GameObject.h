#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "MathGeoLib\MathGeoLib.h"
#include "glmath.h"
#include <vector>
#include <string>
#include "Globals.h"

class Component;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

class ModuleScene;

enum componentType;

class GameObject {
public:
	GameObject(GameObject* parent);
	GameObject();
	virtual ~GameObject();
private:
	GameObject* parent = nullptr;
	std::string name;


public:
	std::vector<Component*> components; // Children Components
	std::vector<GameObject*> children; // Children GameObjects
	AABB aabb;

	bool selected = false;

private:
	ModuleScene* scene = nullptr;
	bool isRoot = false;
public:
	void Start();
	void Update(float dt); // Called every ModuleScene->Update(dt) (if they are insied the scene)
	void CleanUp(); // Called at App CleanUp

public:
	void SetRoot(bool root);
	bool IsRoot() const;

	std::vector<Component*> FindComponents(componentType type);
	Component* AddComponent(const componentType type, Component* componentPointer = nullptr, bool fromReference = true);
	void DeleteChild(GameObject* go);
	void DestroyComponent(Component* componentPointer = nullptr);
	GameObject* GetParent() const { return parent; }
	void SetParent(GameObject* p) { parent = p; }
	std::string GetName() const { return name; }
	void CreateAABBFromMesh(ComponentMesh* mesh);
	void UpdateAABBFromMesh(ComponentMesh* mesh);
	void SetName(const char* str) { name = str; }
	void SetToGlobalTransform();
	ComponentTransform* GetTransform();

	void SetScene(ModuleScene* sce);

	void OnEditor();
	void OnHierarchyTree(bool skip_root = false);

};

#endif // !__GAMEOBJECT_H__

