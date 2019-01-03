
file(GLOB TEST_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(test_runtime ${TEST_SRC})

target_link_libraries(test_runtime
    PRIVATE
       runtime_recorder
)
