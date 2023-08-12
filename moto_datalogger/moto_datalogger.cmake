add_library(moto_datalogger)

target_sources(moto_datalogger ${CMAKE_CURRENT_LIST_DIR}/moto_datalogger.cpp)

target_include_directories(moto_datalogger ${CMAKE_CURRENT_LIST_DIR})

# Pull in pico libraries that we need
target_link_libraries(moto_datalogger pico_stdlib pico_time)