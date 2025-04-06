add_executable(intro main.cpp)

target_link_libraries(
  intro
  PRIVATE myproject::myproject_options
          myproject::myproject_warnings
          lefticus::tools
          SFML::Graphics
          ImGui-SFML::ImGui-SFML)

if(EMSCRIPTEN)
  set_target_properties(intro PROPERTIES SUFFIX ".html")
endif()
target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
