
project "imgui"

    location "imgui"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    targetdir ("imgui/bin/" .. outputdir)
    objdir ("imgui/intermediate/" .. outputdir)

    files {
        "imgui/imconfig.h",
        "imgui/imgui.h",
        "imgui/imgui_internal.h",
        "imgui/imgui.cpp",
        "imgui/imgui_draw.cpp",
        "imgui/imgui_demo.cpp",
        "imgui/imgui_tables.cpp",
        "imgui/imgui_widgets.cpp",
        "imgui/imstb_rectpack.h",
        "imgui/imstb_textedit.h",
        "imgui/imstb_truetype.h",
        "imgui/backends/imgui_impl_glfw.h",
        "imgui/backends/imgui_impl_glfw.cpp",
        "imgui/backends/imgui_impl_opengl3.h",
        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/backends/imgui_impl_opengl3_loader.h"
    }

    includedirs {
        "imgui",
        "imgui/backends",
        
        "glfw/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"