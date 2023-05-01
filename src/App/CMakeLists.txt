set(APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/ApplicationLayer.cpp")

add_subdirectory(Renderer)

add_library(app STATIC ${APP_SOURCES})
target_include_directories(app PUBLIC ${MAIN_SOURCE_DIR})
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PUBLIC tl::optional fmt::fmt-header-only spdlog::spdlog_header_only magic_enum::magic_enum imgui renderer Walnut)