
project "YingLong"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. outputdir)
    objdir ("intermediate/" .. outputdir)

    files {
        "src/**.h",
        "src/**.c",
        "src/**.cpp",
        "src/**.hpp",

        "thirdParty/glm/glm/**.hpp",
        "thirdParty/glm/glm/**.inl",

        "thirdParty/entt/entt.hpp",

        "thirdParty/tinyobjloader/**.h",
        "thirdParty/tinyobjloader/**.cpp",

        "thirdParty/stb/**.h",
        "thirdParty/stb/**.cpp",
    }

    includedirs {
        "thirdParty/glad/include",
        "thirdParty/glfw/include",
        "thirdParty/imgui",
        "thirdParty/glm",
        "thirdParty",
        "src",
    }

    pchheader "YingLongPCH.h"
    pchsource "src/YingLongPCH.cpp"

    links {
        "imgui",
        "glad",
        "glfw",
        "openGL32.lib"
    }

    defines {
        "YINGLONG_BUILD_LIB",
        "GLEW_STATIC"
    }

    filter "configurations:Debug"
        defines { "YINGLONG_DEBUG" }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines { "YINGLONG_RELEASE" }
        optimize "On"
        runtime "Release"

    filter "platforms:Win64"
        system "Windows"
        architecture "x64"
