cmake_minimum_required(VERSION 3.5)
project(forces_pro_server)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()
add_compile_options(-DCMAKE_BUILD_TYPE=Release)

find_package(OpenMP) # For the Forces solver
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  message(WARNING "Compiling with OPENMP")
endif()

set(SYSTEM_TO_USE "Jackal")

## DEPENDENCIES ##
set(DEPENDENCIES
  rclcpp
  std_msgs
)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)

find_package(std_msgs REQUIRED)

## BUILD ##
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/CallForcesPro.srv"
  DEPENDENCIES std_msgs
 )

include_directories(
  include
  solver/${SYSTEM_TO_USE}FORCESNLPsolver/include
)

# The service provider
add_executable(${PROJECT_NAME}_node 
  src/solver_service_node.cpp  
  solver/${SYSTEM_TO_USE}FORCESNLPsolver/${SYSTEM_TO_USE}FORCESNLPsolver_interface.c
  solver/${SYSTEM_TO_USE}FORCESNLPsolver/${SYSTEM_TO_USE}FORCESNLPsolver_model.c)
target_include_directories(${PROJECT_NAME}_node
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)
ament_target_dependencies(${PROJECT_NAME}_node
  rclcpp
)
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(${PROJECT_NAME}_node # Link against the solver
  ${PROJECT_SOURCE_DIR}/solver/${SYSTEM_TO_USE}FORCESNLPsolver/lib/lib${SYSTEM_TO_USE}FORCESNLPsolver.so
  "${cpp_typesupport_target}"
)

## INSTALL ##
install(
  TARGETS ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME})
  # EXPORT export_${PROJECT_NAME}_node
  # LIBRARY DESTINATION lib
  # ARCHIVE DESTINATION lib
  # RUNTIME DESTINATION bin


install(DIRECTORY include/ 
  DESTINATION include/${PROJECT_NAME})
  

install(DIRECTORY solver/${SYSTEM_TO_USE}FORCESNLPsolver/include/
  DESTINATION include/${PROJECT_NAME})

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

## FINALIZE
ament_export_include_directories(include/${PROJECT_NAME})
# ament_export_targets(export_${PROJECT_NAME}_node)
ament_export_dependencies(${DEPENDENCIES})
ament_package()
