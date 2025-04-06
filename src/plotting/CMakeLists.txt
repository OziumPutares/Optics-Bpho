include(GenerateExportHeader)

add_library(plotting plotting.cpp)

add_library(myproject::plotting ALIAS plotting)

target_link_libraries(plotting PRIVATE myproject_options myproject_warnings SFML::graphics)

target_include_directories(plotting ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(plotting PUBLIC cxx_std_26)

set_target_properties(
  plotting
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(plotting EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/myproject/plotting_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(plotting PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif()
