
workspace "YingLong"

    configurations { "Debug", "Release" }
    platforms { "Win64" }

    include "YingLong/premake5"
    include "test/premake5"