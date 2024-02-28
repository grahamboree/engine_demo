workspace "engine"
	configurations { "Debug", "Release" }
	location "proj"

project "lib"
	kind "StaticLib"
	language "C++"
	architecture "x64"
	files {
		"lib/**.h",
		"lib/**.hpp",
		"lib/**.inl",
		"lib/**.c",
		"lib/**.cpp"
	}
	externalincludedirs { "lib" }
	defines { "GL_SILENCE_DEPRECATION" }

	filter "system:windows"
		externalincludedirs { "./lib/SDL2/include" }
		removefiles { "./lib/SDL2.framework/**" }
	filter "system:macosx"
		removefiles { "./lib/SDL2/**", "./lib/glew/**" }

project "engine"
	language "C++"
	cppdialect "C++20"
	kind "WindowedApp"
	targetdir "bin"
	architecture "x64"

	files {
		"src/**.h",
		"src/**.c",
		"src/**.cpp",
		"src/**.mm"
	}

	links { "lib" }
	externalincludedirs { "lib" }
	includedirs { "src" }

	filter "files:src/**.glsl"
		buildaction "Embed"

	filter "configurations:Debug"
		defines {
			"DEBUG",
			"GL_SILENCE_DEPRECATION"
		}
		symbols "On"
	
	filter "configurations:Release"
		defines {
			"RELEASE",
			"GL_SILENCE_DEPRECATION"
		}
		optimize "On"

	filter "system:windows"
		debugdir "./bin/"
		externalincludedirs {
			"./lib/SDL2/include",
			"./lib/glew/include"
		}
		libdirs {
			"./lib/SDL2/lib/x64",
			"./lib/glew/lib"
		}

		links {
			"glu32",
			"opengl32",
			"SDL2",
			"SDL2main",
			"glew32"
		}
		postbuildcommands {
			-- Copy the SDL2 dll to the Bin folder.
			'{COPY} "%{path.getabsolute("./lib/SDL2/lib/x64/SDL2.dll")}" "%{cfg.targetdir}"',
			'{COPY} "%{path.getabsolute("./lib/glew/lib/glew32.dll")}" "%{cfg.targetdir}"'
		}

	filter "system:macosx"
		kind "ConsoleApp"
		externalincludedirs {"lib/SDL2.framework/Headers"}
		links {
			"Cocoa.framework",
			"OpenGL.framework",
			"./lib/SDL2.framework"
		}
		frameworkdirs { "lib" }
		embed { "SDL2.framework" }
