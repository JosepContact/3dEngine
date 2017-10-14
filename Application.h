#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleImGui.h"
#include "mmgr\mmgr.h"
#include "ModuleParson.h"
#include "ModuleAssimp.h"
#include "ModuleTextures.h"

#define LOG(format, ...) App->mylog(__FILE__, __LINE__, format, __VA_ARGS__);

class ModuleImGui;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModulePlayer* player;
	ModuleImGui* imgui;
	ModuleParson* parson;
	ModuleAssimp* assimp;
	ModuleTextures* tex;
	
private:

	Timer	ms_timer;
	float	dt;
	list<Module*> list_modules;

	char* EngineName;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawConfigPanel();
	
	string Logs;
	void mylog(const char file[], int line, const char* format, ...);

private:
	vector<float> fps_container;
	vector<float> ms_container;
	vector<float> mem_container;
	sMStats stats;

	
	string ms_title;

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};