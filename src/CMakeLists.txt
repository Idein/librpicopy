set(RPICOPY_INCLUDE_DIRS ${RPIMEMMGR_INCLUDE_DIRS} ${VC4REGMAP_INCLUDE_DIRS})
set(RPICOPY_CFLAGS_OTHER ${RPIMEMMGR_CFLAGS_OTHER} ${VC4REGMAP_CFLAGS_OTHER})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -pipe -g -O2 -W -Wall -Wextra")
include_directories(${CMAKE_SOURCE_DIR}/include ${RPICOPY_INCLUDE_DIRS})

set(rpicopy_SOURCES rpicopy.c dma.c)

add_library(rpicopy        SHARED ${rpicopy_SOURCES})
target_compile_options(rpicopy PRIVATE "${RPICOPY_CFLAGS_OTHER}")

add_library(rpicopy-static STATIC ${rpicopy_SOURCES})
target_compile_options(rpicopy PRIVATE "${RPICOPY_CFLAGS_OTHER}")
set_target_properties(rpicopy-static PROPERTIES OUTPUT_NAME "rpicopy")

install(TARGETS rpicopy        LIBRARY DESTINATION lib)
install(TARGETS rpicopy-static ARCHIVE DESTINATION lib)
