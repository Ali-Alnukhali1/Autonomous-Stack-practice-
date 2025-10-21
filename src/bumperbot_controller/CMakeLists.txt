cmake_minimum_required(VERSION 3.5)
project(bumperbot_controller)

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
  include
  ${EIGEN3_INCLUDE_DIRS}
)

add_executable(simple_controller src/simple_controller.cpp)
ament_target_dependencies(simple_controller rclcpp geometry_msgs std_msgs ${Eigen_LIBERARIES})

install(
  DIRECTORY include
  DESTINATION include
)

install(TARGETS
  simple_controller
  DESTINATION lib/${PROJECT_NAME}

)

install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()