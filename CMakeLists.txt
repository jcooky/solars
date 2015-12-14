cmake_minimum_required(VERSION 3.3)
project(solars)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenSceneGraph REQUIRED osg osgDB osgUtil osgGA osgViewer)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)
aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_FILES)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
add_executable(solars ${HEADERS} ${SOURCE_FILES})
target_link_libraries(solars ${OPENTHREADS_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})