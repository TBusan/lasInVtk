add_executable(gpr_demo main.cpp)
target_link_libraries(gpr_demo PRIVATE gpr_parser)

if(EXISTS "${CMAKE_SOURCE_DIR}/testData")
    add_custom_command(TARGET gpr_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/testData"
            "$<TARGET_FILE_DIR:gpr_demo>/testData"
        COMMENT "Copying test data to build directory"
    )
endif()
