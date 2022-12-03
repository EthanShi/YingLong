#pragma once

#include "SystemBase.h"
#include "renderer/Renderer3D.h"
#include "scene/components/BasicComponents.h"
#include "scene/components/DrawableComponents.h"
#include "scene/components/PhongLightingComponent.h"


DECLARE_LOGGER(DrawBasicLightingSystemLog)

namespace YingLong {

	class DrawBasicLightingSystem : public SystemBase
	{
	public:
		DrawBasicLightingSystem() {};
		virtual ~DrawBasicLightingSystem() {};

		virtual void OnOwnerSceneChanged(Scene* OldScene) override {};

		void Draw();
	};
}