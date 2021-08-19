workspace "ngrok-helper"
   startproject "ngrok-helper"
   systemversion "10.0"
   cppdialect "C++20"
   toolset "v143"
   defines { "_CRT_SECURE_NO_WARNINGS" }
   configurations { "Debug", "Release" }

project "ngrok-helper"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   location "ngrok-helper"
   characterset "MBCS"
   
   includedirs { "include/ImGui", "include/ImGui/backends", "include/rapidjson/include/rapidjson", "include/HTTPRequest/include", "ngrok-helper/src" }

   files { "ngrok-helper/src/**.hpp", "ngrok-helper/src/**.cpp" }
   
   links { "ImGui" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      
project "ImGui"
   kind "StaticLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   location "include/ImGui"
   
   includedirs { "include/ImGui" }

   files { "include/ImGui/backends/imgui_impl_dx9.cpp", "include/ImGui/backends/imgui_impl_win32.cpp", "include/ImGui/imgui.cpp", "include/ImGui/imgui_tables.cpp", "include/ImGui/imgui_demo.cpp", "include/ImGui/imgui_widgets.cpp", "include/ImGui/imgui_draw.cpp", "include/ImGui/misc/cpp/imgui_stdlib.cpp" }