# FindCSMAPI.cmake - Find the CSMAPI library

find_path(CSMAPI_INCLUDE_DIR
    NAMES csm/csm.h csm/RasterGM.h
    PATHS
        ${CSMAPI_ROOT}/include
        $ENV{CSMAPI_ROOT}/include
        /usr/local/include
        /usr/include
    PATH_SUFFIXES csm
)

find_library(CSMAPI_LIBRARY
    NAMES csmapi libcsmapi
    PATHS
        ${CSMAPI_ROOT}/lib
        $ENV{CSMAPI_ROOT}/lib
        /usr/local/lib
        /usr/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CSMAPI
    REQUIRED_VARS CSMAPI_LIBRARY CSMAPI_INCLUDE_DIR
)

if(CSMAPI_FOUND AND NOT TARGET CSMAPI::csmapi)
    add_library(CSMAPI::csmapi UNKNOWN IMPORTED)
    set_target_properties(CSMAPI::csmapi PROPERTIES
        IMPORTED_LOCATION "${CSMAPI_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${CSMAPI_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(CSMAPI_INCLUDE_DIR CSMAPI_LIBRARY)
