-- premake5.lua

-- To run you need to set PROJ_LIB=bin\data for the proj database.
project "OpenEAGGRTestHarness" -- testing project name
    kind "ConsoleApp" -- build an exe
    language "C++"
    cppdialect ("C++14")
    externalwarnings "Off" 
    warnings "Off"
    debugdir "../"
    
    vpaths { 
        {["*/Header Files"] = {"**.hpp"}},
        {["*/Source Files"] = {"**.cpp"}}
    } -- creates vs .filter files 

    linkoptions { conan_exelinkflags }
    files {
        "**.hpp", "**.cpp",
        "../EAGGR/**.cpp", "../EAGGR/**.c",
        "../EAGGR/**.hpp", "../EAGGR/**.h",
    } -- files to compile (test files and source files as the tests )

    includedirs {
        "../EAGGR",
        "./",
    } -- search paths for the compiler

    -- links { 
    --     "OpenEAGGR",
    -- }