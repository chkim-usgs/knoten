# FindUSGSCSM.cmake - Find the USGSCSM library

find_path(USGSCSM_INCLUDE_DIR
    NAMES usgscsm/UsgsAstroPlugin.h
    PATHS
        ${USGSCSM_ROOT}/include
        $ENV{USGSCSM_ROOT}/include
        /usr/local/include
        /usr/include
)

find_library(USGSCSM_LIBRARY
    NAMES usgscsm libusgscsm
    PATHS
        ${USGSCSM_ROOT}/lib
        $ENV{USGSCSM_ROOT}/lib
        ${USGSCSM_ROOT}/lib/csmplugins
        $ENV{USGSCSM_ROOT}/lib/csmplugins
        $ENV{CONDA_PREFIX}/lib/csmplugins
        /usr/local/lib
        /usr/local/lib/csmplugins
        /usr/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(USGSCSM
    REQUIRED_VARS USGSCSM_LIBRARY USGSCSM_INCLUDE_DIR
)

if(USGSCSM_FOUND AND NOT TARGET USGSCSM::usgscsm)
    add_library(USGSCSM::usgscsm UNKNOWN IMPORTED)
    set_target_properties(USGSCSM::usgscsm PROPERTIES
        IMPORTED_LOCATION "${USGSCSM_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${USGSCSM_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(USGSCSM_INCLUDE_DIR USGSCSM_LIBRARY)
