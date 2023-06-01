# CMakeList.txt : CMake project for CppHeightMapGenerator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("GANHeightMapGenerator")

set(SOURCE_FILES "GANHeightMapGenerator.cpp" 
"src/TrainingHandler.cpp")
set(HEADER_DIRS "./"
"include/")

IF(CMAKE_BUILD_TYPE MATCHES Debug)
set(CMAKE_PREFIX_PATH 
    "D:/Programming/libraries/libtorch-d"
    "D:/Programming/libraries/opencv/build")	
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
IF(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_PREFIX_PATH 
    "D:/Programming/libraries/libtorch"
    "D:/Programming/libraries/opencv/build")	
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)




find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


# Add source to this project's executable.
add_executable (GANHeightMapGenerator ${SOURCE_FILES})
target_include_directories(GANHeightMapGenerator PRIVATE ${HEADER_DIRS})

target_link_libraries(GANHeightMapGenerator "${TORCH_LIBRARIES}")
target_link_libraries(GANHeightMapGenerator "${OpenCV_LIBS}")
set_property(TARGET GANHeightMapGenerator PROPERTY CXX_STANDARD 20)

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET GANHeightMapGenerator
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:GANHeightMapGenerator>)
endif (MSVC)

get_target_property(__dll_dbg opencv_world IMPORTED_LOCATION_DEBUG)
get_target_property(__dll_release opencv_world  IMPORTED_LOCATION_RELEASE)

add_custom_command(TARGET GANHeightMapGenerator POST_BUILD        # Adds a post-build event the TARGET
    COMMAND ${CMAKE_COMMAND} -E copy_if_different           # which executes "cmake - E copy_if_different..."
    "$<$<CONFIG:debug>:${__dll_dbg}>$<$<CONFIG:release>:${__dll_release}>"      # <--this is in-file
    $<TARGET_FILE_DIR:GANHeightMapGenerator>                        # <--this is out-file path
        # another dll copy if needed here
    COMMENT "    [1718_34_ATK] copy dlls realsense2 and opencv_world")
 

#file(GLOB DATASET_FILES ${CMAKE_CURRENT_SOURCE_DIR}/assets/dataset/*)
#message(STATUS "DATASET_FILES=${DATASET_FILES}")

#foreach(CurrentDatasetFile IN LISTS DATASET_FILES)
#    add_custom_command(
#                TARGET GANHeightMapGenerator PRE_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CurrentDatasetFile} ${CMAKE_CURRENT_BINARY_DIR}/assets/dataset/
#                COMMENT "Copying header: ${CurrentDatasetFile}")
#endforeach()

# TODO: Add tests and install targets if needed.
