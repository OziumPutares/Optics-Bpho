add_executable(intro main.cpp)

target_link_libraries(intro PRIVATE myproject::myproject_options myproject::myproject_warnings)

target_link_system_libraries(
  intro
  PRIVATE
  lefticus::tools
  dawn::webgpu_dawn)

if(EMSCRIPTEN)
  set_target_properties(intro PROPERTIES SUFFIX ".html")
endif()
target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
