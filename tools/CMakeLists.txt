set(WDBCLI_SOURCES
    wdbcli.cpp
)

add_executable(wdbcli
    ${WDBCLI_SOURCES}
)

target_link_libraries(wdbcli
    PRIVATE 
        wdb::libwdb
        PkgConfig::libedit
)

include(GNUInstallDirs)

install(
    TARGETS wdbcli
    RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR}
)

