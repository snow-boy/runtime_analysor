
file(GLOB RT_RECORDER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rt_serializer/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(rt_serializer STATIC ${RT_RECORDER_SRC})

target_include_directories(rt_serializer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rt_serializer
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

