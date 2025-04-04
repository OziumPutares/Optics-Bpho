include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(myproject_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project
  include(FetchContent)
  # ImGui
  if(NOT TARGET imgui)
    cpmaddpackage(
      NAME
      imgui
      VERSION
      1.91.7-docking
      GITHUB_REPOSITORY
      ocornut/imgui
      DOWNLOAD_ONLY
      TRUE)

    # CMakeLists.txt from https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4
    file(
      DOWNLOAD
      "https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4/raw/4c2c14374ab878ca2f45daabfed4c156468e4e27/CMakeLists.txt"
      "${imgui_SOURCE_DIR}/CMakeLists.txt"
      EXPECTED_HASH SHA256=fd62f69364ce13a4f7633a9b50ae6672c466bcc44be60c69c45c0c6e225bb086)

    # Options
    set(IMGUI_EXAMPLES FALSE)
    set(IMGUI_DEMO FALSE)
    set(IMGUI_ENABLE_STDLIB_SUPPORT TRUE)
    # FreeType (https://github.com/cpm-cmake/CPM.cmake/wiki/More-Snippets#freetype)
    set(FREETYPE_FOUND TRUE)
    set(FREETYPE_INCLUDE_DIRS "")
    set(FREETYPE_LIBRARIES Freetype::Freetype)
  endif()

  if(NOT TARGET dawn)
    cpmaddpackage(
      NAME
      dawn
      GIT_REPOSITORY
      https://dawn.googlesource.com/dawn
      GIT_TAG
      39a35b8b9dc4b135e30a31f596c5f5d42a4ede30
      OPTIONS
      "DAWN_FETCH_DEPENDENCIES ON" # Automatically fetch Dawn's dependencies
      "DAWN_ENABLE_DESKTOP_GL OFF" # Disable OpenGL if not needed
      "DAWN_BUILD_EXAMPLES OFF" # Exclude examples to reduce build time
    )
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.3.2")
  endif()
  if(NOT TARGET tools::tools)
    cpmaddpackage("gh:lefticus/tools#update_build_system")
  endif()

endfunction()
