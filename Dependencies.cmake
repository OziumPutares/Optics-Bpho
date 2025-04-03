include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(myproject_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project
  include(FetchContent)
  #Fetch imgui
  FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.7-docking
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL SYSTEM)
  FetchContent_MakeAvailable(ImGui)
  FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
  #Fetch SFML and sfml-imgui
  FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL SYSTEM)
  FetchContent_MakeAvailable(SFML)

  set(IMGUI_SFML_FIND_SFML OFF)
  FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL SYSTEM)
  FetchContent_MakeAvailable(ImGui-SFML)

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.3.2")
  endif()
  if(NOT TARGET tools::tools)
    cpmaddpackage("gh:lefticus/tools#update_build_system")
  endif()

endfunction()
