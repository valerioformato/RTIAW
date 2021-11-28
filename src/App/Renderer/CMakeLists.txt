set(RENDERER_SOURCES 
"${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Ray.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Shapes/Sphere.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Shapes/HittableObjectList.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/Materials/Lambertian.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Materials/Metal.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Materials/Dielectric.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/Scenes.cpp"
)

add_library(renderer STATIC ${RENDERER_SOURCES})
target_include_directories(renderer PUBLIC ${MAIN_SOURCE_DIR})
target_include_directories(renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(renderer PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only glm)