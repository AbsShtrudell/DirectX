#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Skybox.h"

class App
{
public:
	App();
	~App();

	int Go();

private:
	void DoFrame();

private:
	ImguiManager imgui;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	static constexpr size_t nDrawables = 180;
	Skybox skybox;
};

