#pragma once

#include "scene/Scene.h"

#include "CameraMove.h"

#include "scene/systems/DrawPhongLightingSystem.h"
#include "renderer/PhongLighting/PhongMaterial.h"

using namespace YingLong;

class PhongLightingScene : public CameraMoveScene
{
public:
	PhongLightingScene();
	virtual ~PhongLightingScene() {}

	virtual std::string GetName() override { return "PhongLightingScene"; }

protected:
	virtual void OnActive(const std::shared_ptr<Engine>& OwnerEngine, const std::shared_ptr<Scene>& This) override;
	virtual void Update(float Deltatime) override;
	virtual void DrawImgui(float Deltatime) override;
	virtual void DrawEntities(float Deltatime) override;

	virtual void CreateACube(const glm::vec3& Position, const glm::vec3& Forward, const glm::vec3& Scale) override;

private:
	std::string GetShaderPath();
	void CreatePhongLights();

	int CurrentMaterialIndex = 0;
	std::string CurrentMaterialName = "";
	std::map<std::string, PhongMaterial> TestMaterials;

private:
	DrawBasicLightingSystem DrawBasicLightingSystem;
};