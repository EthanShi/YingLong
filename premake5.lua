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
        "src/**.hpp"
    }

    includedirs {
        "thridparty/include"
    }

    pchheader "../src/yinglongPCH.h"

    libdirs {
        "libs"
    }

    links {

    }

    filter "platforms:Win64"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "YINGLONG_PLATFORM_WINDOWS"
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