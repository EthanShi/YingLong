
project "imgui"

    location "imgui"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    targetdir ("%{prj.location}/bin/" .. outputdir)
    objdir ("%{prj.location}/intermediate/" .. outputdir)

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_demo.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"