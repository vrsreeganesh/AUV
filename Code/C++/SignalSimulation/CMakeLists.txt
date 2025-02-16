# INCLUDING OPEN-MP ============================================================
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(SignalSimulation)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Find Python
find_package(Python3 REQUIRED COMPONENTS Development)

# Find Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Find OpenMP
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Create the executable
add_executable(SignalSimulation SignalSimulation.cpp)

# Link against Python, Torch, and OpenMP
target_include_directories(SignalSimulation PRIVATE ${Python3_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
target_link_libraries(SignalSimulation "${TORCH_LIBRARIES}" Python3::Python OpenMP::OpenMP_CXX)

set_property(TARGET SignalSimulation PROPERTY CXX_STANDARD 17)

# Windows-specific fix
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET SignalSimulation
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:SignalSimulation>)
endif (MSVC)

























# cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
# project(SignalSimulation)

# find_package(Torch REQUIRED)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# # Find Python
# find_package(Python3 REQUIRED COMPONENTS Development)

# # Add Eigen
# find_package(Eigen3 REQUIRED)
# include_directories(${EIGEN3_INCLUDE_DIR})

# add_executable(SignalSimulation SignalSimulation.cpp)

# # Link against Python and Torch
# target_include_directories(SignalSimulation PRIVATE ${Python3_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
# target_link_libraries(SignalSimulation "${TORCH_LIBRARIES}" Python3::Python)

# set_property(TARGET SignalSimulation PROPERTY CXX_STANDARD 17)

# # Windows-specific fix
# if (MSVC)
#   file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
#   add_custom_command(TARGET SignalSimulation
#                      POST_BUILD
#                      COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                      ${TORCH_DLLS}
#                      $<TARGET_FILE_DIR:SignalSimulation>)
# endif (MSVC)








# cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
# project(SignalSimulation)

# find_package(Torch REQUIRED)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# # Find Python
# find_package(Python3 REQUIRED COMPONENTS Development) 

# add_executable(SignalSimulation SignalSimulation.cpp)

# # Link against Python
# target_include_directories(SignalSimulation PRIVATE ${Python3_INCLUDE_DIRS})
# target_link_libraries(SignalSimulation "${TORCH_LIBRARIES}" Python3::Python)

# set_property(TARGET SignalSimulation PROPERTY CXX_STANDARD 17)

# # Windows-specific fix
# if (MSVC)
#   file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
#   add_custom_command(TARGET SignalSimulation
#                      POST_BUILD
#                      COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                      ${TORCH_DLLS}
#                      $<TARGET_FILE_DIR:SignalSimulation>)
# endif (MSVC)























# cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
# project(SignalSimulation)

# find_package(Torch REQUIRED)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# add_executable(SignalSimulation SignalSimulation.cpp)
# target_link_libraries(SignalSimulation "${TORCH_LIBRARIES}")
# set_property(TARGET SignalSimulation PROPERTY CXX_STANDARD 17)

# # The following code block is suggested to be used on Windows.
# # According to https://github.com/pytorch/pytorch/issues/25457,
# # the DLLs need to be copied to avoid memory errors.
# if (MSVC)
#   file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
#   add_custom_command(TARGET SignalSimulation
#                      POST_BUILD
#                      COMMAND ${CMAKE_COMMAND} -E copy_if_different
#                      ${TORCH_DLLS}
#                      $<TARGET_FILE_DIR:SignalSimulation>)
# endif (MSVC)