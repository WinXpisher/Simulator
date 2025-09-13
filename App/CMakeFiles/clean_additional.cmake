# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\myApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\myApp_autogen.dir\\ParseCache.txt"
  "myApp_autogen"
  )
endif()
