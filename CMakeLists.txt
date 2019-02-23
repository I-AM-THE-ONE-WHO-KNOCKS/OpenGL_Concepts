# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
    message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
    message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies
add_subdirectory (Libs)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Libs/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

include_directories(
    Libs/glfw-3.1.2/include/
    Libs/glm-0.9.7.1/
    Libs/glew-1.13.0/include/
    common/include/
    Libs/include
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW_1130
    glut
    freeimage
)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

# 1_Basic_Triangle
add_executable(1_Basic_Triangle
    1_Basic_Triangle/1_Basic_Triangle.cpp
    common/Shader.cpp
    common/triangle.cpp

    1_Basic_Triangle/shader.frag
    1_Basic_Triangle/shader.vert
)
target_link_libraries(1_Basic_Triangle
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(1_Basic_Triangle PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/1_Basic_Triangle/")
create_target_launcher(1_Basic_Triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/1_Basic_Triangle/")

# 2_Triangle_VBO
add_executable(2_Triangle_VBO
    2_Triangle_VBO/2_Triangle_VBO.cpp
    common/Shader.cpp
    common/triangle.cpp

    2_Triangle_VBO/shader.frag
    2_Triangle_VBO/shader.vert
)
target_link_libraries(2_Triangle_VBO
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(2_Triangle_VBO PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/2_Triangle_VBO/")
create_target_launcher(2_Triangle_VBO WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/2_Triangle_VBO/")

# 3_Triangle_VAO
add_executable(3_Triangle_VAO
    3_Triangle_VAO/3_Triangle_VAO.cpp
    common/Shader.cpp
    common/triangle.cpp

    3_Triangle_VAO/shader.frag
    3_Triangle_VAO/shader.vert
)
target_link_libraries(3_Triangle_VAO
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(3_Triangle_VAO PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3_Triangle_VAO/")
create_target_launcher(3_Triangle_VAO WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3_Triangle_VAO/")

# 4_Geometry_Shader
add_executable(4_Geometry_Shader
    4_Geometry_Shader/4_Geometry_Shader.cpp
    common/Shader.cpp
    common/triangle.cpp

    4_Geometry_Shader/shader-gs.frag
    4_Geometry_Shader/shader-gs.vert
    4_Geometry_Shader/shader-gs.geom
)
target_link_libraries(4_Geometry_Shader
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(4_Geometry_Shader PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/4_Geometry_Shader/")
create_target_launcher(4_Geometry_Shader WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/4_Geometry_Shader/")

# 5_Tessellation_Shader
add_executable(5_Tessellation_Shader
    5_Tessellation_Shader/5_Tessellation_Shader.cpp
    common/Shader.cpp
    common/triangle.cpp

    5_Tessellation_Shader/shader-Tess.frag
    5_Tessellation_Shader/shader-Tess.vert
    5_Tessellation_Shader/shader-Tess-ctrl.tes
    5_Tessellation_Shader/shader-Tess-eval.tes
)
target_link_libraries(5_Tessellation_Shader
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(5_Tessellation_Shader PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/5_Tessellation_Shader/")
create_target_launcher(5_Tessellation_Shader WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/5_Tessellation_Shader/")

# 6_Blending_Test
add_executable(6_Blending_Test
    6_Blending_Test/6_Blending_Test.cpp
    common/Shader.cpp
    common/triangle.cpp

    6_Blending_Test/shader.frag
    6_Blending_Test/shader.vert
)
target_link_libraries(6_Blending_Test
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(6_Blending_Test PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/6_Blending_Test/")
create_target_launcher(6_Blending_Test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/6_Blending_Test/")

# 7_Culling_Test
add_executable(7_Culling_Test
    7_Culling_Test/7_Culling_Test.cpp
    common/Shader.cpp
    common/triangle.cpp

    7_Culling_Test/shader.frag
    7_Culling_Test/shader.vert
)
target_link_libraries(7_Culling_Test
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(7_Culling_Test PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/7_Culling_Test/")
create_target_launcher(7_Culling_Test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/7_Culling_Test/")

# 8_Clipping_Test
add_executable(8_Clipping_Test
    8_Clipping_Test/8_Clipping_Test.cpp
    common/Shader.cpp
    common/triangle.cpp

    8_Clipping_Test/shader_clipping.frag
    8_Clipping_Test/shader_clipping.vert
)
target_link_libraries(8_Clipping_Test
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(8_Clipping_Test PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/8_Clipping_Test/")
create_target_launcher(8_Clipping_Test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/8_Clipping_Test/")

# 9_Scissor_Test
add_executable(9_Scissor_Test
    9_Scissor_Test/9_Scissor_Test.cpp
    common/Shader.cpp
    common/triangle.cpp

    9_Scissor_Test/shader.frag
    9_Scissor_Test/shader.vert
)
target_link_libraries(9_Scissor_Test
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(9_Scissor_Test PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/9_Scissor_Test/")
create_target_launcher(9_Scissor_Test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/9_Scissor_Test/")

# 10_Stencil_Test
add_executable(10_Stencil_Test
    10_Stencil_Test/10_Stencil_Test.cpp
    common/Shader.cpp
    common/triangle.cpp

    10_Stencil_Test/shader.frag
    10_Stencil_Test/shader.vert
)
target_link_libraries(10_Stencil_Test
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(10_Stencil_Test PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/10_Stencil_Test/")
create_target_launcher(10_Stencil_Test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/10_Stencil_Test/")

# 11_Sphere_MVP
add_executable(11_Sphere_MVP
    11_Sphere_MVP/11_Sphere_MVP.cpp
    common/Shader.cpp
    common/Sphere.cpp

    11_Sphere_MVP/shader_Matrices.frag
    11_Sphere_MVP/shader_Matrices.vert
)
target_link_libraries(11_Sphere_MVP
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(11_Sphere_MVP PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/11_Sphere_MVP/")
create_target_launcher(11_Sphere_MVP WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/11_Sphere_MVP/")

# 12_Sphere_Transformations
add_executable(12_Sphere_Transformations
    12_Sphere_Transformations/12_Sphere_Transformations.cpp
    common/Shader.cpp
    common/Sphere.cpp

    12_Sphere_Transformations/shader_Matrices.frag
    12_Sphere_Transformations/shader_Matrices.vert
)
target_link_libraries(12_Sphere_Transformations
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(12_Sphere_Transformations PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/12_Sphere_Transformations/")
create_target_launcher(12_Sphere_Transformations WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/12_Sphere_Transformations/")

# 13_Lighting_Per_Vertex
add_executable(13_Lighting_Per_Vertex
    13_Lighting_Per_Vertex/13_Lighting_Per_Vertex.cpp
    common/Shader.cpp
    common/Sphere.cpp

    13_Lighting_Per_Vertex/shader_lighting_pervertex.frag
    13_Lighting_Per_Vertex/shader_lighting_pervertex.vert
)
target_link_libraries(13_Lighting_Per_Vertex
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(13_Lighting_Per_Vertex PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/13_Lighting_Per_Vertex/")
create_target_launcher(13_Lighting_Per_Vertex WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/13_Lighting_Per_Vertex/")

# 14_Lighting_Per_Fragment
add_executable(14_Lighting_Per_Fragment
    14_Lighting_Per_Fragment/14_Lighting_Per_Fragment.cpp
    common/Shader.cpp
    common/Sphere.cpp

    14_Lighting_Per_Fragment/shader_lighting_perfragment.frag
    14_Lighting_Per_Fragment/shader_lighting_perfragment.vert
)
target_link_libraries(14_Lighting_Per_Fragment
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(14_Lighting_Per_Fragment PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/14_Lighting_Per_Fragment/")
create_target_launcher(14_Lighting_Per_Fragment WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/14_Lighting_Per_Fragment/")

# 15_Texture_Mapping
add_executable(15_Texture_Mapping
    15_Texture_Mapping/15_Texture_Mapping.cpp
    common/Shader.cpp
    common/Sphere.cpp

    15_Texture_Mapping/shader_texture.frag
    15_Texture_Mapping/shader_texture.vert
)
target_link_libraries(15_Texture_Mapping
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(15_Texture_Mapping PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/15_Texture_Mapping/")
create_target_launcher(15_Texture_Mapping WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/15_Texture_Mapping/")

# 16_Specular_Mapping
add_executable(16_Specular_Mapping
    16_Specular_Mapping/16_Specular_Mapping.cpp
    common/Shader.cpp
    common/Sphere.cpp

    16_Specular_Mapping/shader_specular.frag
    16_Specular_Mapping/shader_specular.vert
)
target_link_libraries(16_Specular_Mapping
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(16_Specular_Mapping PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/16_Specular_Mapping/")
create_target_launcher(16_Specular_Mapping WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/16_Specular_Mapping/")

# 17_FBO
add_executable(17_FBO
    17_FBO/17_FBO.cpp
    common/Shader.cpp
    common/Cube.cpp

    17_FBO/shader_FBO.frag
    17_FBO/shader_FBO.vert
)
target_link_libraries(17_FBO
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(17_FBO PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/17_FBO/")
create_target_launcher(17_FBO WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/17_FBO/")

# 18_Multipass_Rendering
add_executable(18_Multipass_Rendering
    18_Multipass_Rendering/18_Multipass_Rendering.cpp
    common/Shader.cpp
    common/Cube.cpp

    18_Multipass_Rendering/shader_multi.frag
    18_Multipass_Rendering/shader_multi.vert
    18_Multipass_Rendering/shader_multi1.frag
    18_Multipass_Rendering/shader_multi1.vert
)
target_link_libraries(18_Multipass_Rendering
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(18_Multipass_Rendering PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/18_Multipass_Rendering/")
create_target_launcher(18_Multipass_Rendering WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/18_Multipass_Rendering/")

# 19_Compute_Shader
add_executable(19_Compute_Shader
    19_Compute_Shader/19_Compute_Shader.cpp
    common/Shader.cpp
    common/Cube.cpp

    19_Compute_Shader/shader_multiPass_1.frag
    19_Compute_Shader/shader_multiPass_1.vert
    19_Compute_Shader/shader_multiPass_2.frag
    19_Compute_Shader/shader_multiPass_2.vert
)
target_link_libraries(19_Compute_Shader
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(19_Compute_Shader PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/19_Compute_Shader/")
create_target_launcher(19_Compute_Shader WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/19_Compute_Shader/")

# 20_Solar_System
add_executable(20_Solar_System
    20_Solar_System/20_Solar_System.cpp
    common/Shader.cpp
    common/Sphere.cpp

    20_Solar_System/shader_solar.frag
    20_Solar_System/shader_solar.vert
)
target_link_libraries(20_Solar_System
    ${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(20_Solar_System PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/20_Solar_System/")
create_target_launcher(20_Solar_System WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/20_Solar_System/")

if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET 1_Basic_Triangle POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/1_Basic_Triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/1_Basic_Triangle/"
)
add_custom_command(
   TARGET 2_Triangle_VBO POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/2_Triangle_VBO${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/2_Triangle_VBO/"
)
add_custom_command(
   TARGET 3_Triangle_VAO POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/3_Triangle_VAO${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/3_Triangle_VAO/"
)
add_custom_command(
   TARGET 4_Geometry_Shader POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/4_Geometry_Shader${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/4_Geometry_Shader/"
)
add_custom_command(
   TARGET 5_Tessellation_Shader POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/5_Tessellation_Shader${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/5_Tessellation_Shader/"
)
add_custom_command(
   TARGET 6_Blending_Test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/6_Blending_Test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/6_Blending_Test/"
)
add_custom_command(
   TARGET 7_Culling_Test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/7_Culling_Test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/7_Culling_Test/"
)
add_custom_command(
   TARGET 8_Clipping_Test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/8_Clipping_Test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/8_Clipping_Test/"
)
add_custom_command(
   TARGET 9_Scissor_Test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/9_Scissor_Test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/9_Scissor_Test/"
)
add_custom_command(
   TARGET 10_Stencil_Test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/10_Stencil_Test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/10_Stencil_Test/"
)
add_custom_command(
   TARGET 11_Sphere_MVP POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/11_Sphere_MVP${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/11_Sphere_MVP/"
)
add_custom_command(
   TARGET 12_Sphere_Transformations POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/12_Sphere_Transformations${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/12_Sphere_Transformations/"
)
add_custom_command(
   TARGET 13_Lighting_Per_Vertex POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/13_Lighting_Per_Vertex${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/13_Lighting_Per_Vertex/"
)
add_custom_command(
   TARGET 14_Lighting_Per_Fragment POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/14_Lighting_Per_Fragment${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/14_Lighting_Per_Fragment/"
)
add_custom_command(
   TARGET 15_Texture_Mapping POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/15_Texture_Mapping${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/15_Texture_Mapping/"
)
add_custom_command(
   TARGET 16_Specular_Mapping POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/16_Specular_Mapping${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/16_Specular_Mapping/"
)
add_custom_command(
   TARGET 17_FBO POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/17_FBO${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/17_FBO/"
)
add_custom_command(
   TARGET 18_Multipass_Rendering POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/18_Multipass_Rendering${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/18_Multipass_Rendering/"
)
add_custom_command(
   TARGET 19_Compute_Shader POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/19_Compute_Shader${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/19_Compute_Shader/"
)
add_custom_command(
   TARGET 20_Solar_System POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/20_Solar_System${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/20_Solar_System/"
)


elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
