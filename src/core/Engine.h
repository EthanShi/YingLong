#pragma once

#include "YingLongPCH.h"

#include "Macros.h"

namespace YingLong {

	class YINGLONG_API Engine
	{
	public:
		Engine();
		~Engine();

		void MainLoop();

	public:

	private:

	private:
		entt::registry m_Registry;
	};

}
