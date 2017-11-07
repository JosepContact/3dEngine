#pragma once

#include "Component.h"

#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"

class ComponentTransform : public Component {
public:
	ComponentTransform();
	ComponentTransform(GameObject* argparent);
	ComponentTransform(componentType argtype, GameObject* argparent);
	ComponentTransform(GameObject* argparent, float3 position, Quat rotation, float3 scale);
	~ComponentTransform() {};
	void Update(float dt);
public:
	//void Update(float dt);
	void OnEditor();
	float4x4 GetWorldMatrix();
	float4x4 GetLocalMatrix() const;
public:
	float* GetMatrix4x4() const;
public:
	float3 position;
	Quat rotation;
	float3 scale;
	float4x4 WorldMatrix = float4x4::zero;
public:
	void UpdateMatrix();
	float3 GlobalPosition;
	Quat GlobalRotation;
	float3 GlobalScale;
	bool Transformed = false;
private:
};
