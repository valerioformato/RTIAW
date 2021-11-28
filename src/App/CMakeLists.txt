set(APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/App.cpp")

add_library(app STATIC ${APP_SOURCES})
target_include_directories(app PUBLIC ${MAIN_SOURCE_DIR})
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only magic_enum::magic_enum imgui d3d11 d3dcompiler renderer)

add_subdirectory(renderer)