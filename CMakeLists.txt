# For CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
cmake_minimum_required(VERSION 3.7.2)

project(librpicopy)
include(CTest)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "Sugizaki Yukimasa <ysugi@idein.jp>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
        "Fast implementations of memory functions for Raspberry Pi")

# See https://semver.org/ .
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 1)

# CMAKE_INSTALL_PREFIX and CPACK_PACKAGING_INSTALL_PREFIX must be identical
# because the install prefix is used in the pkg-config file which will be
# auto-generated.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install prefix" FORCE)
endif ()
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
message(STATUS "Install prefix is set to ${CMAKE_INSTALL_PREFIX}")
include(CPack)

find_package(PkgConfig)

function (pkg_check_modules_opt_vc prefix spec)
    pkg_check_modules(${prefix} ${spec})
    if (NOT "${${prefix}_FOUND}")
        message(STATUS "Adding /opt/vc/lib/pkgconfig to PKG_CONFIG_PATH")
        set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/opt/vc/lib/pkgconfig")
        pkg_check_modules(${prefix} ${spec})
        if (NOT "${${prefix}_FOUND}")
            message(FATAL_ERROR "${spec} is not found even in /opt/vc.")
        endif ()
    endif ()
endfunction (pkg_check_modules_opt_vc)

pkg_check_modules_opt_vc(BCM_HOST bcm_host=1)
pkg_check_modules_opt_vc(RPIMEMMGR librpimemmgr>=3.0.0)
pkg_check_modules_opt_vc(VC4REGMAP libvc4regmap>=3.0.0)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(example)

configure_file(librpicopy.pc.in librpicopy.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/librpicopy.pc"
        DESTINATION lib/pkgconfig)
