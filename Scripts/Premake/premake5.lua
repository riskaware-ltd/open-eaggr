-- premake5.lua

include("conanbuildinfo.premake.lua")

openEAGGRLibType = "SharedLib"

workspace "OpenEAGGR" -- created solution file name
    conan_basic_setup()
    configurations { "Debug", "Release", "ReleaseNoTests" } 
    platforms { "Win64", "unix" } -- optional, does not need to specify platforms, can be used for ddl/lib 
    cppdialect ("C++14")
    targetdir ( "../../bin/outputdir/%{prj.name}")
    includedirs {"./"}
    debugdir "../../"
    startproject "EAGGRTestHarness"
    filter {"platforms:unix"}
        system "linux" -- specify this plaform is for only x platform (full list is here https://premake.github.io/docs/system/)
        defines { "BUILDING_ON_LINUX" }

    filter {"platforms:unix", "configurations:Debug" }
        buildoptions { "-fprofile-arcs", "-ftest-coverage" }
        linkoptions { "-lgcov" }

    filter {"platforms:Win*" } 
        system "windows"
        architecture "x86_64"
        defines { "BUILDING_ON_WINDOWS" }

    -- Source code project file
    include("../../EAGGR/premake5.lua")

    include("../../EAGGRTestHarness/premake5.lua")