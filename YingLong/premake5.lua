
outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

project "YingLong"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir ("%{prj.location}/bin/" .. outputdir)
    objdir ("%{prj.location}/intermediate/" .. outputdir)

    files {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.c",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.hpp",
    }

    includedirs {
        "%{prj.location}/thirdParty/lib/GLEW/include",
        "%{prj.location}/thirdParty/lib/GLFW/include",
        "%{prj.location}/thirdParty/src",
        "%{prj.location}/src"
    }

    pchheader "YingLongPCH.h"
    pchsource "%{prj.location}/src/YingLongPCH.cpp"

    libdirs {
        "%{prj.location}/thirdParty/lib/GLEW/lib/Release/x64",
        "%{prj.location}/thirdParty/lib/GLFW/lib-vs2022",
    }

    links {
        "glew32s.lib",
        "glfw3.lib",
        "openGL32.lib"
    }

    defines {
        "YINGLONG_BUILD_LIB",
        "GLEW_STATIC"
    }

    filter { "system:windows" }
        postbuildcommands { "python createLibraryPackage.py" }

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
