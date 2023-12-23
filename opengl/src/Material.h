#pragma once

#include <memory>
#include "Texture.h"
#include "Shader.h"
#include <string>

struct GlMaterial {
	std::shared_ptr<GlShaderProgram> Shader;
	std::shared_ptr<GlTexture> DiffuseTex;
	std::shared_ptr<GlTexture> SpecularTex;
	std::shared_ptr<GlTexture> EmissionTex;
	std::shared_ptr<GlTexture> ReflectionTex;
	float EmissionStrength;
	float Shine;
	float ReflectionStrength;

	std::string ModelMatrixUniform			{ "uModel" };
	std::string ViewMatrixUniform			{ "uView" };
	std::string ProjectionMatrixUniform		{ "uProjection" };
	std::string CameraPositionUniform		{ "uCameraPos" };
	std::string DiffuseTexUniform			{ "Mat.color" };
	std::string SpecularTexUniform			{ "Mat.specular" };
	std::string EmissionTexUniform			{ "Mat.emission" };
	std::string ReflectionTexUniform		{ "Mat.reflection" };
	std::string EmissionStrengthUniform		{ "Mat.emissionStrength" };
	std::string ShineUniform				{ "Mat.shine" };
	std::string ReflectionStrengthUniform	{ "Mat.reflectionStrength" };
	std::string SkyboxUniform				{ "skybox" };
	std::string DirLightDirectionUniform	{ "Light.direction" };
	std::string DirLightColorUniform		{ "Light.color" };
	std::string PointLightPositionUniform	{ "PointLight.position" };
	std::string PointLightColorUniform		{ "PointLight.color" };
	std::string PointLightLinearUniform		{ "PointLight.linear" };
	std::string PointLightQuadraticUniform	{ "PointLight.quadratic" };
	std::string PointLightConstantUniform	{ "PointLight.constant" };

};