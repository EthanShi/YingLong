
workspace "YingLong"

    configurations { "Debug", "Release" }
    platforms { "Win64" }

    system ("Windows")

    outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

    group "thirdParty"
        include "YingLong/thirdParty/glfw_premake5"
        include "YingLong/thirdParty/glad_premake5"
        include "YingLong/thirdParty/imgui_premake5"
    group ""

    include "YingLong"
    include "test"