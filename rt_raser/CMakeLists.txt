
file(GLOB TEST_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(rt_raser ${TEST_SRC})

target_link_libraries(rt_raser
    PRIVATE
       rt_serializer
)
