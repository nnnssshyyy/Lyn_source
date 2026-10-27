cmake_minimum_required(VERSION 3.24)
project(visuals_preview LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
get_filename_component(ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
add_executable(preview preview.cpp "${ROOT}/src/velocity_ui/visuals_window.cpp"
    "${ROOT}/src/esp_font.cpp" "${ROOT}/src/game.cpp" "${ROOT}/src/source.cpp"
    "${ROOT}/src/memory.cpp" "${ROOT}/src/log.cpp")
target_compile_definitions(preview PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
target_compile_options(preview PRIVATE /utf-8 /EHsc)
target_include_directories(preview PRIVATE "${ROOT}/src" "${ROOT}/vendor/imgui")
target_link_libraries(preview PRIVATE "${ROOT}/build-current/Release/imgui.lib"
    d3d9 dwmapi imm32 windowscodecs ole32 user32)
