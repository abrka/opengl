#pragma once
#include "Camera.h"
#include "CubemapTexture.h"
#include "LightSources.h"
#include <memory>


struct GlRendererContext {
	Camera Cam;
	DirLight DirLightSource;
	PointLight PointLightSource;
	std::shared_ptr<GlCubemapTexture> Skybox = nullptr;
};