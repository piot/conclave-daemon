cmake_minimum_required(VERSION 3.16.3)
project(conclave-daemon C)

add_subdirectory(deps/piot/clog/src/lib)
add_subdirectory(deps/piot/conclave-serialize-c/src/lib)
add_subdirectory(deps/piot/conclave-server-lib/src/lib)
add_subdirectory(deps/piot/datagram-transport-c/src/lib)
add_subdirectory(deps/piot/flood-c/src/lib)
add_subdirectory(deps/piot/guise-client-c/src/lib)
add_subdirectory(deps/piot/guise-client-udp-c/src/lib)
add_subdirectory(deps/piot/guise-serialize-c/src/lib)
add_subdirectory(deps/piot/guise-session-client-c/src/lib)
add_subdirectory(deps/piot/imprint/src/lib)
add_subdirectory(deps/piot/monotonic-time-c/src/lib)
add_subdirectory(deps/piot/secure-random-c/src/lib)
add_subdirectory(deps/piot/stats-c/src/lib)
add_subdirectory(deps/piot/tiny-libc/src/lib)
add_subdirectory(deps/piot/udp-client-c/src/lib)
add_subdirectory(deps/piot/udp-server-c/src/lib)


add_subdirectory(lib)


