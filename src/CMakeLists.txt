add_executable (demo "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
target_link_libraries(demo fmt::fmt-header-only spdlog::spdlog_header_only docopt)
