set (LIBWDB_SOURCES
    process.cpp
    pipe.cpp
)

add_library(libwdb 
    ${LIBWDB_SOURCES}
)

add_library(wdb::libwdb 
    ALIAS libwdb
)

set_target_properties(libwdb
    PROPERTIES OUTPUT_NAME wdb
)

target_compile_features(libwdb
    PUBLIC cxx_std_23
)

target_compile_options(libwdb
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_include_directories(libwdb
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/include
)

include(GNUInstallDirs)

install(TARGETS libwdb
    EXPORT wdb-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT wdb-targets
    FILE wdb-config.cmake
    NAMESPACE wdb::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wdb
)

