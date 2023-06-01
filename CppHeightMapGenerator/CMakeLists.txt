# CMakeList.txt : CMake project for CppHeightMapGenerator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(CppHeightMapGenerator)

set(SOURCE_FILES "CppHeightMapGenerator.cpp" 
	"src/generationMethods/BillowNoiseMethod.cpp" 
	"src/generationMethods/CosineNoiseMethod.cpp" 
	"src/generationMethods/GenerationMethodBase.cpp" 
	"src/generationMethods/GenerationSettings.cpp" 
	"src/generationMethods/PerlinNoiseMethod.cpp" 
	"src/generationMethods/RidgedNoiseMethod.cpp" 
	"src/generationMethods/SineNoiseMethod.cpp" 
	"src/utilities/CsvHandler.cpp" 
	"src/utilities/IniHandler.cpp" 
)
set(HEADER_DIRS "./" "include/.")

# Add source to this project's executable.
add_executable (CppHeightMapGenerator ${SOURCE_FILES})
target_include_directories(CppHeightMapGenerator PRIVATE ${HEADER_DIRS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CppHeightMapGenerator PROPERTY CXX_STANDARD 17)
endif()

  add_custom_command(TARGET CppHeightMapGenerator
                     PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                     "${PROJECT_SOURCE_DIR}/config.ini"
                     $<TARGET_FILE_DIR:CppHeightMapGenerator>)

# TODO: Add tests and install targets if needed.
