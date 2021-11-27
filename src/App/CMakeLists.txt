set(WINDOW_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/App.cpp")

add_library(app STATIC ${WINDOW_SOURCES})
target_include_directories(app PUBLIC ${MAIN_SOURCE_DIR})
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PUBLIC imgui d3d11 d3dcompiler)