cmake_minimum_required(VERSION 3.17)
project(conclave_daemon C)

set(CMAKE_C_STANDARD 99)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(isDebug TRUE)
else()
  message("Not a debug build")
  set(isDebug FALSE)
endif()

if(isDebug)
  message("Debug build detected")
  add_compile_definitions(CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)
  set(CONFIGURATION_DEBUG 1)
endif()

set(DEPS ../../deps/)

file(GLOB_RECURSE deps_src FOLLOW_SYMLINKS "${DEPS}*/*/src/lib/*.c")

file(GLOB lib_src FOLLOW_SYMLINKS "*.c")

add_executable(
  conclave_daemon
  ${lib_src} ${deps_src}
  "${DEPS}piot/monotonic-time-c/src/platform/posix/monotonic_time_posix.c")

target_compile_options(
  conclave_daemon
  PRIVATE -Wall
          -Wextra
          -Wshadow
          -Wstrict-aliasing
          -ansi
          -pedantic
          -Wno-declaration-after-statement
          -Wno-extra-semi-stmt
          -Wno-undef
          -Wno-unused-variable
          -Wno-unused-parameter
          -Wno-padded
          -Werror=implicit-function-declaration
          -Werror=incompatible-pointer-types
          -Werror=missing-prototypes
          -Werror=int-conversion
          -Werror=return-type
          -Werror=incompatible-function-pointer-types)

if(COMPILER_CLANG)
  target_compile_options(
    conclave_daemon PRIVATE -Wmost -Weverything
                            -Werror=missing-variable-declarations)
endif()

target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/conclave-server-lib/src/include)

target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/flood-c/src/include)
target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/conclave-serialize-c/src/include)
target_include_directories(conclave_daemon PUBLIC ${DEPS}piot/clog/src/include)
target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/tiny-libc/src/include)
target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/imprint/src/include)
target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/secure-random-c/src/include)
target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/udp-server-c/src/include)
target_include_directories(conclave_daemon
                           PUBLIC ${DEPS}piot/monotonic-time-c/src/include)

target_include_directories(conclave_daemon PUBLIC ../include)
