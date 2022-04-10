
project "glad"

    location "glad"

    kind "StaticLib"
    language "C"
    staticruntime "on"
    systemversion "latest"

    targetdir ("glad/bin/" .. outputdir)
    objdir ("glad/intermediate/" .. outputdir)

    files {
        "glad/include/glad/gl.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/gl.c"
    }

    includedirs {
        "glad/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"