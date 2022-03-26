
outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

include "/thirdParty/YingLongStatic/dependencies"

project "test"

    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    ignoredefaultlibraries { "LIBCMT" }

    targetdir ("%{prj.location}/bin/" .. outputdir)
    objdir ("%{prj.location}/intermediate/" .. outputdir)

    files {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.c",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.hpp"
    }

    YingLongLibPath = "%{prj.location}/thirdParty/"

    _includeDirs = { "%{prj.location}/src" }
    for k,v in ipairs(YingLongIncludePaths) do
        table.insert(_includeDirs, YingLongLibPath .. v)
    end

    includedirs(_includeDirs)

    _libDirs = {}
    for k,v in ipairs(YingLongLibDirs) do
        table.insert(_libDirs, YingLongLibPath .. v)
    end

    libdirs(_libDirs)

    _linksLibs = {}
    for k,v in ipairs(YingLongLinkLibs) do
        table.insert(_linksLibs, v)
    end

    links(_linksLibs)

    linkoptions { "-IGNORE:4099" }

    defines {
        "GLEW_STATIC"
    }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
        debuglevel (2)

    filter "configurations:Release"
        optimize "On"
        runtime "Release"
        debuglevel (0)

    filter "platforms:Win64"
        system "Windows"
        architecture "x64"