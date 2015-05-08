solution "MySolution"
	configurations {"Debug", "Release"}
	
	project "app"
		kind "ConsoleApp"
		language "C++"
		buildoptions { "-std=c++11" }
		links { "SDL2", "SDL2_ttf", "GL" }
		includedirs{ "../src" }
		files {
			"../src/**.cpp"
		}

