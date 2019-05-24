workspace "Kruskal_UF"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Kruskal_UF"
	location "Kruskal_UF"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	links {
		"Kruskal_UF"
	}

	includedirs {
		"Kruskal_UF",
		"Kruskal_UF/src"
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