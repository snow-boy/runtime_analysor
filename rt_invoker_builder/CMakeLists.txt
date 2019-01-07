
file(GLOB SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(rt_invoker_builder ${SRC})

target_link_libraries(rt_invoker_builder
    PRIVATE
       rt_serializer
)
