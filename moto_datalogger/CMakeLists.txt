add_library(moto_datalogger INTERFACE)

target_sources(moto_datalogger INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/moto_datalogger.cpp
)

target_include_directories(moto_datalogger INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(moto_datalogger INTERFACE pico_stdlib pico_time)