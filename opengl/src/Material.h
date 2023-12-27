#pragma once

#include <memory>
#include "Texture.h"
#include "Shader.h"
#include <string>
#include "LightSources.h"
#include "RendererContext.h"

struct GlMaterial {
	std::shared_ptr<GlShaderProgram> Shader;
	std::shared_ptr<GlTexture> DiffuseTex;
	std::shared_ptr<GlTexture> EmissionTex;
	std::shared_ptr<GlTexture> ReflectionTex;
	std::shared_ptr<GlTexture> LightmapTex;

	glm::vec3 ColorTint{ 1.0f };
	float EmissionStrength{};
	float Roughness{ 0.1f };
	float Metalic{ 0.0f };
	float IOR{ 1.4f };
	float ReflectionStrength{};

	std::string ModelMatrixUniform			{ "uModel" };
	std::string ViewMatrixUniform			{ "uView" };
	std::string ProjectionMatrixUniform		{ "uProjection" };
	std::string CameraPositionUniform		{ "uCameraPos" };
	std::string DiffuseTexUniform			{ "Mat.color" };
	std::string ColorTintUniform			{ "Mat.colorTint" };
	std::string EmissionTexUniform			{ "Mat.emission" };
	std::string ReflectionTexUniform		{ "Mat.reflection" };
	std::string EmissionStrengthUniform		{ "Mat.emissionStrength" };
	std::string RoughnessUniform			{ "Mat.roughness" };
	std::string MetalicUniform				{ "Mat.metalic" };
	std::string IORUniform					{ "Mat.IOR" };
	std::string ReflectionStrengthUniform	{ "Mat.reflectionStrength" };
	std::string LightmapTexUniform          { "Mat.lightmap" };
	std::string SkyboxUniform				{ "skybox" };
	std::string DirLightDirectionUniform	{ "Light.direction" };
	std::string DirLightColorUniform		{ "Light.color" };
	std::string DirLightIntensityUniform	{ "Light.intensity" };
	std::string PointLightPositionUniform	{ "PointLight.position" };
	std::string PointLightColorUniform		{ "PointLight.color" };
	std::string PointLightLinearUniform		{ "PointLight.linear" };
	std::string PointLightQuadraticUniform	{ "PointLight.quadratic" };
	std::string PointLightConstantUniform	{ "PointLight.constant" };

	void SetUniforms(GlRendererContext& RenderContext, glm::mat4 ModelMatrix) {

		Shader->Bind();

		Shader->SetMatrix4f(ModelMatrixUniform, ModelMatrix);
		Shader->SetMatrix4f(ViewMatrixUniform, RenderContext.Cam.view);
		Shader->SetMatrix4f(ProjectionMatrixUniform, RenderContext.Cam.projection);
		Shader->SetVec3(DirLightDirectionUniform, RenderContext.DirLightSource.Direction);
		Shader->SetVec3(DirLightColorUniform, RenderContext.DirLightSource.Color);
		Shader->SetFloat(DirLightIntensityUniform, RenderContext.DirLightSource.Intensity);
		Shader->SetVec3(PointLightColorUniform, RenderContext.PointLightSource.Color);
		Shader->SetVec3(PointLightPositionUniform, RenderContext.PointLightSource.Position);
		Shader->SetFloat(PointLightConstantUniform, RenderContext.PointLightSource.Constant);
		Shader->SetFloat(PointLightLinearUniform, RenderContext.PointLightSource.Linear);
		Shader->SetFloat(PointLightQuadraticUniform, RenderContext.PointLightSource.Quadratic);
		Shader->SetVec3(CameraPositionUniform, RenderContext.Cam.Position);
		Shader->SetFloat(RoughnessUniform, Roughness);
		Shader->SetFloat(MetalicUniform, Metalic);
		Shader->SetFloat(IORUniform, IOR);
		Shader->SetFloat(EmissionStrengthUniform, EmissionStrength);
		Shader->SetFloat(ReflectionStrengthUniform, ReflectionStrength);
		Shader->SetVec3(ColorTintUniform, ColorTint);

		if (DiffuseTex)
		{
			Shader->SetTexture(DiffuseTexUniform, *DiffuseTex, 0);
		}
	
		if (EmissionTex)
		{
			Shader->SetTexture(EmissionTexUniform, *EmissionTex, 2);
		}

		if (LightmapTex)
		{
			Shader->SetTexture(LightmapTexUniform, *LightmapTex, 3);
		}

		if (RenderContext.Skybox) {
			Shader->SetCubemapTexture(SkyboxUniform, *RenderContext.Skybox, 4);
		}


		Shader->Unbind();

	};
};