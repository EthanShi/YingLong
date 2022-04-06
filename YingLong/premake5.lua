
outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

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

        "thirdParty/src/glm/**.hpp",
        "thirdParty/src/glm/**.inl",
    }

    includedirs {
        "thirdParty/lib/GLEW/include",
        "thirdParty/lib/GLFW/include",
        "thirdParty/src",
        "src"
    }

    pchheader "YingLongPCH.h"
    pchsource "src/YingLongPCH.cpp"

    libdirs {
        "thirdParty/lib/GLEW/lib/Release/x64",
        "thirdParty/lib/GLFW/lib-vs2022",
    }

    links {
        "imgui",
        "glew32s.lib",
        "glfw3.lib",
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
        debuglevel (2)

    filter "configurations:Release"
        defines { "YINGLONG_RELEASE" }
        optimize "On"
        runtime "Release"
        debuglevel (0)

    filter "platforms:Win64"
        system "Windows"
        architecture "x64"
