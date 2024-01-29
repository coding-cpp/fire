#####################################
# Define Target Information
#####################################
set (TARGET_NAME fire)
set (TARGET_INCLUDE_FOLDER 
      ${CMAKE_CURRENT_SOURCE_DIR}/lib 
      ${CMAKE_CURRENT_SOURCE_DIR}/include 
    )
# set(TARGET_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB TARGET_SOURCES 
      ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
      ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/linux/*.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/windows/*.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/macos/*.cpp
    )

#####################################
# Support IDE Visualization
#####################################
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_SOURCES})

#####################################
# Define Target according to options
#####################################
add_executable(${TARGET_NAME} ${TARGET_SOURCES})

# include all the necessary directories
target_include_directories(${TARGET_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
  PRIVATE 
    ${TARGET_INCLUDE_FOLDER}
)