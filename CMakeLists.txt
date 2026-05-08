cmake_minimum_required(VERSION 3.16)

project(OpenLOB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPENLOB_BUILD_TESTS "Build OpenLOB tests" ON)
option(OPENLOB_BUILD_BENCHMARKS "Build OpenLOB benchmarks" OFF)

add_library(openlob_core
    src/book/order_book.cpp
    src/book/matching_engine.cpp
    src/sim/event_queue.cpp
    src/sim/simulator.cpp
    src/sim/latency_model.cpp
    src/agents/market_maker_agent.cpp
    src/agents/noise_trader_agent.cpp
    src/agents/informed_trader_agent.cpp
    src/metrics/pnl.cpp
    src/metrics/inventory.cpp
    src/metrics/latency_stats.cpp
)

target_include_directories(openlob_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(openlob src/main.cpp)
target_link_libraries(openlob PRIVATE openlob_core)

if(OPENLOB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(OPENLOB_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
