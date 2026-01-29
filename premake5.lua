workspace ("ngrok-helper")
   startproject ("ngrok-helper")
   systemversion ("10.0")
   cppdialect ("C++17")
   toolset ("v143")
   defines { "_CRT_SECURE_NO_WARNINGS" }
   configurations { "Debug", "Release" }

project ("imgui")
   location ("vendor/%{prj.name}")
   kind ("StaticLib")
   language ("C++")
   targetdir ("bin/%{cfg.buildcfg}")
   includedirs { "vendor/%{prj.name}" }
   files { "vendor/%{prj.name}/backends/imgui_impl_dx9.cpp", "vendor/%{prj.name}/backends/imgui_impl_win32.cpp", "vendor/%{prj.name}/imgui.cpp", "vendor/%{prj.name}/imgui_tables.cpp", "vendor/%{prj.name}/imgui_demo.cpp", "vendor/%{prj.name}/imgui_widgets.cpp", "vendor/%{prj.name}/imgui_draw.cpp", "vendor/%{prj.name}/misc/cpp/imgui_stdlib.cpp" }

project ("ngrok-helper")
   location ("ngrok-helper")
   kind ("ConsoleApp")
   language ("C++")
   targetdir ("bin/%{cfg.buildcfg}")
   objdir ("bin/obj/%{cfg.buildcfg}/%{prj.name}")
   characterset ("MBCS")

   disablewarnings { "4244" }
   PrecompiledHeaderInclude = "common.hpp"
   PrecompiledHeaderSource = "%{prj.name}/src/common.cpp"

    filter { "toolset:msc*" }
      buildoptions { "/utf-8" }

   includedirs { "%{prj.name}/src", "vendor/HTTPRequest/include", "vendor/json/single_include", "vendor/spdlog/include", "vendor/imgui", "vendor/imgui/backends" }
   files { "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.hpp" }

   links { "imgui" }
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
