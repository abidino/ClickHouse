set (CLICKHOUSE_LOCAL_SOURCES LocalServer.cpp)

set (CLICKHOUSE_LOCAL_LINK
    PRIVATE
        boost::program_options
        clickhouse_aggregate_functions
        clickhouse_common_config
        clickhouse_common_io
        clickhouse_functions
        clickhouse_parsers
        clickhouse_storages_system
        clickhouse_table_functions
)

clickhouse_program_add(local)

target_link_libraries(clickhouse-local-lib PRIVATE clickhouse-server-lib)

if (TARGET ch_rust::skim)
    target_link_libraries(clickhouse-local-lib PRIVATE ch_rust::skim)
endif()

# Always use internal readpassphrase
target_link_libraries(clickhouse-local-lib PRIVATE readpassphrase)

if (ENABLE_FUZZING)
    add_compile_definitions(FUZZING_MODE=1)
    set (WITH_COVERAGE ON)
    target_link_libraries(clickhouse-local-lib PRIVATE ${LIB_FUZZING_ENGINE})
endif ()
