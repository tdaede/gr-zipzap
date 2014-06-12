INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_ZIPZAP zipzap)

FIND_PATH(
    ZIPZAP_INCLUDE_DIRS
    NAMES zipzap/api.h
    HINTS $ENV{ZIPZAP_DIR}/include
        ${PC_ZIPZAP_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    ZIPZAP_LIBRARIES
    NAMES gnuradio-zipzap
    HINTS $ENV{ZIPZAP_DIR}/lib
        ${PC_ZIPZAP_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZIPZAP DEFAULT_MSG ZIPZAP_LIBRARIES ZIPZAP_INCLUDE_DIRS)
MARK_AS_ADVANCED(ZIPZAP_LIBRARIES ZIPZAP_INCLUDE_DIRS)

