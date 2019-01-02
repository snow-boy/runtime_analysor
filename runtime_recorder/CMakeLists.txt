
file(GLOB RUNTIME_RECORDER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/runtime_recorder/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
# serializer
add_library(runtime_recorder STATIC ${RUNTIME_RECORDER_SRC})

target_include_directories(runtime_recorder
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/runtime_recorder
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

