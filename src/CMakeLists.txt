set(MAIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(IMGUI_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_win32.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

set(SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_subdirectory(App)

add_executable(demo ${SOURCES})
target_include_directories(demo PRIVATE "${MAIN_SOURCE_DIR}")
target_link_libraries(demo PRIVATE fmt::fmt-header-only spdlog::spdlog_header_only docopt app)
