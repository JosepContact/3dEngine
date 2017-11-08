#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "MathGeoLib\MathGeoLib.h"
#include "glmath.h"
#include "MathGeoLib\Geometry\Frustum.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	main_camera = new ComponentCamera();
	name = "Camera3D";
}

ModuleCamera3D::~ModuleCamera3D()
{
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	curr_camera = main_camera;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	delete main_camera;
	curr_camera = nullptr;
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (!ImGui::IsAnyWindowHovered() && active_camera == true)
	{
		// Mouse wheel motion
		if (App->input->IsMouseWheelActive()) {
			CameraZoom(dt);
		}

		// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			MoveCamera(dt);
			RotateCamera(dt);
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			GameObject* pick = Pick();
			//if (pick != nullptr)
				//App->editor->SetSelected(pick, (App->editor->selected == pick));
		}
	}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return main_camera->GetViewMatrix();
}

void ModuleCamera3D::FocusMesh(const float* vertices, const uint &num_vertices)
{
	/*
	uint	y_test = 0;
	float	highest_vertex_y = 0.0f, highest_vertex_x = 0.0f, highest_vertex_z = 0.0f;

	for (uint i = 0; i < num_vertices; ++i) {
		switch (++y_test) {
		case 1:
			if (highest_vertex_x < vertices[i])
				highest_vertex_x = vertices[i];
			break;
		case 2:
			if (highest_vertex_y < vertices[i])
				highest_vertex_y = vertices[i];
			break;
		case 3:
			if (highest_vertex_z < vertices[i])
				highest_vertex_z = vertices[i];
			y_test = 0;
			break;
		}
	}

	vec3 first_v(vertices[0], vertices[1], vertices[2]);


	Position.y = Reference.y = highest_vertex_y * PROPORTION_CAMERA;
	Position.z = Reference.z = highest_vertex_z * PROPORTION_CAMERA;
	Position.x = Reference.x = highest_vertex_x * PROPORTION_CAMERA;

	vec3 closest_vertex(first_v), furthest_vertex;

	float dist_closest = dot(Position, vec3(vertices[0], vertices[1], vertices[2]));
	float dist_furthest = 0.0f;

	for (uint i = 0; i < num_vertices; i += 3)
	{
		if (dot(Position, vec3(vertices[i], vertices[i + 1], vertices[i + 2])) < dist_closest)
		{
			closest_vertex = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			dist_closest = dot(Position, closest_vertex);
		}
		else if (dot(Position, vec3(vertices[i], vertices[i + 1], vertices[i + 2])) > dist_furthest)
		{
			furthest_vertex = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			dist_furthest = dot(Position, closest_vertex);
		}
	}

	vec3 midpoint = (closest_vertex + furthest_vertex) / 2;
	*/
	//LookAt(midpoint);

}

void ModuleCamera3D::LookAt(const float3 & spot)
{
	float3 direction = spot - main_camera->GetFrustum().pos;

	float3x3 matrix = float3x3::LookAt(main_camera->GetFrustum().front, direction.Normalized(), main_camera->GetFrustum().up, float3::unitY);

	main_camera->GetFrustum().front = matrix.MulDir(main_camera->GetFrustum().front).Normalized();
	main_camera->GetFrustum().up = matrix.MulDir(main_camera->GetFrustum().up).Normalized();
}

void ModuleCamera3D::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("Camera")) {
		float curr_fov = main_camera->GetFOV();
		if (ImGui::SliderFloat("Set FOV (60 by default)", &curr_fov, 1.f, 200.f)) {
			if (curr_fov != main_camera->GetFOV()) main_camera->SetFov(curr_fov);
		}
	}

}


void ModuleCamera3D::RotateCamera(float dt)
{
	float Sensitivity = 0.25f;

	float dx = -App->input->GetMouseXMotion() * Sensitivity * dt;
	float dy = -App->input->GetMouseYMotion() * Sensitivity * dt;

	main_camera->Rotate(dx, dy);
}

void ModuleCamera3D::MoveCamera(float dt)
{
	float speed = 2.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) main_camera->MoveForwards(speed);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) main_camera->MoveBackwards(speed);


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) main_camera->MoveLeft(speed);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) main_camera->MoveRight(speed);

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) main_camera->MoveUp(speed);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) main_camera->MoveDown(speed);
}

void ModuleCamera3D::CameraZoom(float dt)
{
	main_camera->MoveForwards(App->input->GetMouseWheelMotion());
}

GameObject * ModuleCamera3D::Pick()
{
	GameObject* ret = nullptr;

	float mousex = App->input->GetMouseX(), mousey = App->input->GetMouseY();

	float dist_w = -(1.0f - (float (mousex * 2.0f)) / App->window->GetWidth());
	float dist_y = 1.0f - (float (mousey * 2.0f)) / App->window->GetHeight();

	LineSegment picker = main_camera->GetFrustum().UnProjectLineSegment(dist_w , dist_y);

	float distance = 0.f;

	ret = App->picker->RayCast(picker, distance);
	if (ret != nullptr && distance != FLOAT_INF)
	{
		pickingat = picker.GetPoint(distance);
		string PickedObject = "You selected: " + ret->GetName();
		LOG(PickedObject.c_str());
	}


	return ret;
}
