
project "test"

    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    -- ignoredefaultlibraries { "LIBCMT", "MSVCRT" }

    targetdir ("bin/" .. outputdir)
    objdir ("intermediate/" .. outputdir)

    files {
        "src/**.h",
        "src/**.c",
        "src/**.cpp",
        "src/**.hpp"
    }

    YingLongPath = "../YingLong/"

    includedirs {
        YingLongPath .. "thirdParty/glad/include",
        YingLongPath .. "thirdParty/glfw/include",
        YingLongPath .. "thirdParty/imgui",
        YingLongPath .. "thirdParty/entt",
        YingLongPath .. "thirdParty/glm",
        YingLongPath .. "thirdParty/stb",
        YingLongPath .. "src",
        "src"
    }

    links {
        "YingLong"
    }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        optimize "On"
        runtime "Release"

    filter "platforms:Win64"
        system "Windows"
        architecture "x64"