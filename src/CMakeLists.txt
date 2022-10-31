set(MAIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_subdirectory(App)

add_executable(demo ${SOURCES})
target_include_directories(demo PRIVATE "${MAIN_SOURCE_DIR}")
target_link_libraries(demo PRIVATE app)
