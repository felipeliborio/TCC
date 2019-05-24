workspace "FibonacciHeap"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "FibonacciHeap"
	location "FibonacciHeap"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	links {
		"FibonacciHeap"
	}

	includedirs {
		"FibonacciHeap",
		"FibonacciHeap/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"K_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines {
			"K_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		defines {
			"K_RELEASE"
		}
		optimize "On"
