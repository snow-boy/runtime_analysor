
file(GLOB TEST_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(runtime_analysor ${TEST_SRC})

target_link_libraries(runtime_analysor
    PRIVATE
       runtime_recorder
)
