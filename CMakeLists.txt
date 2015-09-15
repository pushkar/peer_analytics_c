cmake_minimum_required (VERSION 2.6)
project (mcmc)

# include (${CMAKE_INSTALL_PREFIX}/share/dart/DART_config.cmake)
# include (${CMAKE_INSTALL_PREFIX}/share/grip/GRIP_config.cmake)
# This include is used only if you have installed Grip

# Search all header and source files
file (GLOB srcs "*.cpp")
file (GLOB hdrs "*.h")

set (CMAKE_CXX_FLAGS "-std=c++11 -o3")
#add_executable (mcmc mcmc ${hdrs})
#target_link_libraries(mcmc python2.7)

set (CMAKE_CXX_FLAGS "-std=c++11 -o3")
add_executable (simple simplemcmc)
target_link_libraries(simple python2.7)
