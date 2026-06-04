set(GPR_SOURCES
    gpr_head_parser.cpp
    gpr_cor_parser.cpp
    gpr_data_parser.cpp
    gpr_dataset.cpp
    gpr_vtk_renderer.cpp
)

add_library(gpr_parser ${GPR_SOURCES})
target_include_directories(gpr_parser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(gpr_parser PUBLIC ${VTK_LIBRARIES})
