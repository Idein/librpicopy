set(RPICOPY_INCLUDE_DIRS ${RPIMEMMGR_INCLUDE_DIRS} ${VC4REGMAP_INCLUDE_DIRS})
set(RPICOPY_CFLAGS_OTHER ${RPIMEMMGR_CFLAGS_OTHER} ${VC4REGMAP_CFLAGS_OTHER})
set(RPICOPY_LDFLAGS ${RPIMEMMGR_LDFLAGS} ${VC4REGMAP_LDFLAGS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -g -O2 -W -Wall -Wextra")
include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(bench bench.c)
target_include_directories(bench PRIVATE "${RPICOPY_INCLUDE_DIRS}")
target_compile_options(bench PRIVATE "${RPICOPY_CFLAGS_OTHER}")
target_link_libraries(bench rpicopy "${RPICOPY_LDFLAGS}")
