#pragma once

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include "core/Macros.h"

namespace YingLong {

	class YINGLONG_API Scene
	{
	public:
		Scene();
		virtual ~Scene();

		void Tick(float deltatime);
		const glm::vec4& GetBackgroundColor() const { return m_BackgroundColor; }
		void SetBackgroundColor(const glm::vec4& color) { m_BackgroundColor = color; }

		virtual void Update(float deltatime);
		virtual void DrawEntities(float deltatime);
		virtual void DrawImgui(float deltatime);

	private:
		void InnerUpdate(float deltatime);
		void InnerDrawEntities(float deltatime);
		void InnerDrawImgui(float deltatime);

	private:
		entt::registry m_Registry;

		glm::vec4 m_BackgroundColor;
	};

	using Scene_SPtr = std::shared_ptr<Scene>;
}