# handle dependencies
include(FetchContent)

# needed for the fmt and spdlog static libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# https://github.com/Neargye/magic_enum.git
# === magic_enum ===
FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.7.3)
FetchContent_GetProperties(magic_enum)
if(NOT magic_enum_POPULATED)
  FetchContent_Populate(magic_enum)
  add_subdirectory(${magic_enum_SOURCE_DIR} ${magic_enum_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# === fmt ===
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 7.1.3)
FetchContent_GetProperties(fmt)
if(NOT fmt_POPULATED)
  set(FMT_INSTALL ON)
  FetchContent_Populate(fmt)
  add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# === spdlog ===
# Force spdlog to use downloaded fmt library
set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE INTERNAL "") # Forces the value
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.8.5)
FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  set(SPDLOG_INSTALL ON)
  FetchContent_Populate(spdlog)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# === docopt ===
FetchContent_Declare(docopt
GIT_REPOSITORY https://github.com/docopt/docopt.cpp
  GIT_TAG v0.6.3)
FetchContent_GetProperties(docopt)
if(NOT docopt_POPULATED)
  FetchContent_Populate(docopt)
  add_subdirectory(${docopt_SOURCE_DIR} ${docopt_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
install(TARGETS docopt DESTINATION lib)

# === SDL2 ===
# FetchContent_Declare(SDL2
# GIT_REPOSITORY "https://github.com/SDL-mirror/SDL.git"
#   GIT_TAG release-2.0.14)
# FetchContent_GetProperties(SDL2)
# if(NOT sdl2_POPULATED)
#     FetchContent_Populate(SDL2)
#     add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif ()