set(RENDERER_SOURCES 
"${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Ray.cpp"
)

add_library(renderer STATIC ${RENDERER_SOURCES})
target_include_directories(renderer PUBLIC ${MAIN_SOURCE_DIR})
target_include_directories(renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(renderer PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only glm)