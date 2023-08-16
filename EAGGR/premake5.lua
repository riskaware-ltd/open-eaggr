-- premake5.lua

project "OpenEAGGR" -- testing project name
    kind(openEAGGRLibType)
    language "C++"
    linkoptions { conan_exelinkflags }
    cppdialect ("C++14")
    -- targetdir "../bin/%{cfg.buildcfg}" -- where to compile to
    includedirs { "./", "3rdParty/include/"}
    files { "**.cpp", "**.hpp", "**.h", "**.inl", "**.c" }
    removefiles { "**/3rdParty/**", "./3rdParty/**" } -- Don't compile with these (but they are included)
    warnings "Off"
    vpaths {
        {["*/Header Files"] = {"**.hpp", "**.h"}},
        {["*/Source Files"] = {"**.cpp", "**.inl", "**.c"}},
    } -- creates vs .filter files
