#pragma once

#include "GameObject.h"


class ComponentTransform;

enum componentType {
	componentType_Mesh = 0,
	componentType_Material,
	componentType_Transform,
	// Unknown ALLWAYS Last
	componentType_Unknown
};

/*
class Component {
	friend GameObject;
public:
	Component() { name = "Component"; };
	virtual ~Component() {};
public:
	virtual void DrawInspectorPanel() {};
public:
	GameObject* gameObject = nullptr;
	ComponentTransform transform;
	char* tag = nullptr;
	componentType type;
	char* name = nullptr;
public:
	virtual bool Update(float dt) { return true; };
	virtual void CleanUp() {};
};*/

struct Transform {
	vec3 position;
	vec4 rotation;
	vec3 scale;
};

class Component {
	friend class GameObject;
public:
	Component(const componentType argtype);
	Component(componentType argtype, GameObject* argparent);
private:
	bool SetParent(GameObject* parent);
public:
	componentType type;
	Transform transform;
protected:
	bool isActive = true;
	GameObject* parent;
public:
	virtual componentType GetType() { return type; }
	virtual void Update(float dt) {}
	virtual void CleanUp() {}

	virtual void Enable() { isActive = true; }
	virtual void Disable() { isActive = false; }
	virtual void OnEditor() {}

	GameObject* GetParent() const { return parent; }
	void SetType(componentType ntype) { type = ntype; }
};