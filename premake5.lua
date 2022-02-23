workspace "YingLong"

    configurations { "Debug", "Release" }
    platforms { "Win64" }

project "YingLong"

    kind "SharedLib"
    language "C++"

    outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"
    targetdir ("bin/" .. outputdir)
    objdir ("intermediate/" .. outputdir)

    files {
        "src/**.h",
        "src/**.c",
        "src/**.cpp",
        "src/**.hpp",
    }

    includedirs {
        "thridparty/GLEW/include",
        "thridparty/GLFW/include",
    }

    -- pchheader "../src/yinglongPCH.h"

    libdirs {
        "thridparty/GLEW/lib/Release/x64",
        "thridparty/GLFW/lib-2022",
    }

    links {
        "glew32s.lib",
        "glfw3.lib",
        "openGL32.lib"
    }

    filter "platforms:Win64"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "YINGLONG_PLATFORM_WINDOWS"
        }

project "test"

        location "../test"

        kind "ConsoleApp"
        language "C++"

        outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"
        targetdir ("bin/" .. outputdir)
        objdir ("intermediate/" .. outputdir)

        files {
            "../test/src/**.h",
            "../test/src/**.c",
            "../test/src/**.cpp",
            "../test/src/**.hpp"
        }

filter "configurations:Debug"
    defines { "YINGLONG_DEBUG" }
    symbols "On"

filter "configurations:Release"
    defines { "YINGLONG_RELEASE" }
    optimize "On"

filter "platforms:Win64"
    system "Windows"
    architecture "x64"