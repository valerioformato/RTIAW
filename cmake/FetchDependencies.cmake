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
  GIT_TAG 9.1.0)
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
  GIT_TAG v1.11.0)
FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  set(SPDLOG_INSTALL ON)
  FetchContent_Populate(spdlog)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# === tl::optional ===
FetchContent_Declare(
  tloptional
  GIT_REPOSITORY https://github.com/TartanLlama/optional
  GIT_TAG v1.1.0)
FetchContent_GetProperties(tloptional)
if(NOT tloptional_POPULATED)
  set(OPTIONAL_BUILD_PACKAGE OFF)
  set(OPTIONAL_BUILD_TESTS OFF)
  FetchContent_Populate(tloptional)
  add_subdirectory(${tloptional_SOURCE_DIR} ${tloptional_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# === openexr ===
FetchContent_Declare(
  openexr
  GIT_REPOSITORY https://github.com/AcademySoftwareFoundation/openexr.git
  GIT_TAG v3.1.7)
FetchContent_GetProperties(openexr)
if(NOT openexr_POPULATED)
  set(OPENEXR_INSTALL_EXAMPLES OFF)
  FetchContent_Populate(openexr)
  add_subdirectory(${openexr_SOURCE_DIR} ${openexr_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
