cmake_minimum_required( VERSION 3.6 )

project( HDK_Project )

# CMAKE_PREFIX_PATH must contain the path to the toolkit/cmake subdirectory of
# the Houdini installation. See the "Compiling with CMake" section of the HDK
# documentation for more details, which describes several options for
# specifying this path.
# list( APPEND CMAKE_PREFIX_PATH "$ENV{HFS}/toolkit/cmake" )

# Please set HDK_PATH environment variable
list( APPEND CMAKE_PREFIX_PATH "$ENV{HDK_PATH}/toolkit/cmake" )

# Locate Houdini's libraries and header files.
# Registers an imported library target named 'Houdini'.
find_package( Houdini REQUIRED )

set( library_name OBJ_RokokoFrontend )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# No optimization.
# Turn on debugging information instead.



# Add a library and its source files.
add_library( ${library_name} SHARED
    OBJ_RokokoFrontend.cpp
    OBJ_RokokoFrontend.h
    RokokoDataParser.cpp
    RokokoDataParser.h
    RokokoSocketReader.cpp
    RokokoSocketReader.h
    RokokoReceiver.cpp
    RokokoReceiver.h
    TransformInfo.h
    PropTrackerInfo.h
    BoneInfo.h
    ActorInfo.h
)

# Link against the Houdini libraries, and add required include directories and
# compile definitions.
target_link_libraries( ${library_name} Houdini )

# Include ${CMAKE_CURRENT_BINARY_DIR} for the generated header.
target_include_directories( ${library_name} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories( ${library_name} PRIVATE 
    "$ENV{HDK_PATH}/toolkit/include"
)

# Sets several common target properties, such as the library's output directory.
houdini_configure_target( ${library_name} )
