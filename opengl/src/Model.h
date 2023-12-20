#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"
#include <memory>

class GlModel {

public:

	GlMesh& Mesh;
	GlShaderProgram& Shader;


	/*GlModel(std::shared_ptr<GlMesh> _Mesh, std::shared_ptr<GlShaderProgram> _Shader) :
		Mesh(_Mesh), Shader(_Shader){};*/


	GlModel(GlMesh& _Mesh, GlShaderProgram& _Shader) :
		Mesh(_Mesh), Shader(_Shader) {};

	void Draw() const {
		
		Mesh.Draw(Shader);
	}

};