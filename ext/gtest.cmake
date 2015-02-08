########################### GTEST
# Enable ExternalProject CMake module
INCLUDE(ExternalProject)
 
# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ext)

# Add gtest
ExternalProject_Add(
    googletest
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk
    SVN_REVISION -r HEAD
    TIMEOUT 10
    # # Force separate output paths for debug and release builds to allow easy
    # # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    # CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    #            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    #            -Dgtest_force_shared_crt=ON
		# Disable updating
		UPDATE_COMMAND ""
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)
 
# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for googletest")
 
# Library
ExternalProject_Get_Property(googletest binary_dir)
set(GTEST_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a CACHE INTERNAL "Path to lib folder for googletest")
set(GTEST_LIBRARY gtest)
add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION ${GTEST_LIBRARY_PATH})
add_dependencies(${GTEST_LIBRARY} googletest)
