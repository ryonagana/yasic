#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Allegro::allegro_monolith" for configuration "Release"
set_property(TARGET Allegro::allegro_monolith APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Allegro::allegro_monolith PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/liballegro_monolith-static.a"
  )

list(APPEND _cmake_import_check_targets Allegro::allegro_monolith )
list(APPEND _cmake_import_check_files_for_Allegro::allegro_monolith "${_IMPORT_PREFIX}/lib/liballegro_monolith-static.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
