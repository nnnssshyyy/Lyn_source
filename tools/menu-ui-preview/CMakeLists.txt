cmake_minimum_required(VERSION 3.24)
project(menu_ui_preview LANGUAGES CXX RC)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
get_filename_component(WORKSPACE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
# Standalone UI verification. It never starts the DLL entry point or opens the game.
file(GLOB OBJECTS "${WORKSPACE}/build-current/lyn.dir/Release/*.obj")
list(FILTER OBJECTS EXCLUDE REGEX "/dllmain\\.obj$")
set_source_files_properties(${OBJECTS} PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)
add_executable(preview preview.cpp ${OBJECTS} "${WORKSPACE}/build-current/hunter_resources.rc")
add_executable(ui_performance performance.cpp ${OBJECTS} "${WORKSPACE}/build-current/hunter_resources.rc")
foreach(target preview ui_performance)
target_compile_definitions(${target} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
target_compile_options(${target} PRIVATE /utf-8)
target_include_directories(${target} PRIVATE "${WORKSPACE}/src" "${WORKSPACE}/vendor/imgui")
target_link_libraries(${target} PRIVATE "${WORKSPACE}/build-current/Release/imgui.lib"
    "${WORKSPACE}/build-current/vendor/freetype/Release/freetype.lib"
    "${WORKSPACE}/build-current/vendor/minhook/Release/minhook.x32.lib"
    d3d9 d3dcompiler dwmapi imm32 user32 windowscodecs ole32 advapi32 bcrypt)
endforeach()
