add_executable(wdb-tests
    wdbtests.cpp
)

target_link_libraries(wdb-tests
    PRIVATE
        wdb::libwdb
        Catch2::Catch2WithMain
)
