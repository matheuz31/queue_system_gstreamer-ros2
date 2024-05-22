cmake_minimum_required(VERSION 3.8)
project(queue_system)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)  

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTAPP REQUIRED gstreamer-app-1.0)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(${PROJECT_NAME}_exe src/main.cpp)

# Vinculando as bibliotecas do GStreamer ao executável
target_include_directories(${PROJECT_NAME}_exe PRIVATE ${GSTREAMER_INCLUDE_DIRS} ${GSTAPP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_exe ${GSTREAMER_LIBRARIES} ${GSTAPP_LIBRARIES})

ament_target_dependencies(
  ${PROJECT_NAME}_exe 
  rclcpp 
  geometry_msgs 
  sensor_msgs 
  cv_bridge 
  OpenCV
  gazebo_ros
)

ament_package()

