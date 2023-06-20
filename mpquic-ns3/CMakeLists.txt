file(GLOB_RECURSE scratch_sources 
      mpquic.cc 
      ./helper/*.cc)

message(${scratch_sources})

build_exec(
  EXECNAME mpquic-ns3
  SOURCE_FILES ${scratch_sources}
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_OUTPUT_DIRECTORY}/scratch/
  LIBRARIES_TO_LINK
    ${libquic}
    ${libcore}
    ${libfd-net-device}
)
