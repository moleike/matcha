find_package(Boost) 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    message(STATUS "Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})
endif()


