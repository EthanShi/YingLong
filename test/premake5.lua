
project "test"

    kind "ConsoleApp"
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
        "src/**.hpp"
    }

    YingLongPath = "../YingLong/"

    includedirs {
        YingLongPath .. "thirdParty/glad/include",
        YingLongPath .. "thirdParty/glfw/include",
        YingLongPath .. "thirdParty/imgui",
        YingLongPath .. "thirdParty/glm",
        YingLongPath .. "thirdParty",
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