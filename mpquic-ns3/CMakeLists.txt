file(GLOB_RECURSE scratch_sources 
      quic-app.cc 
      ./helper/*.cc)

file(GLOB_RECURSE client_sources 
      quic-app-client.cc 
      ./helper/*.cc)

file(GLOB_RECURSE server_sources 
      quic-app-server.cc 
      ./helper/*.cc)

message(${scratch_sources})

build_exec(
  EXECNAME mpquic-ns3
  SOURCE_FILES ${scratch_sources}
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_OUTPUT_DIRECTORY}/scratch/
  LIBRARIES_TO_LINK
    ${libquic}
    ${libcore}
    ${libnetwork}
    ${libfd-net-device}
)
build_exec(
  EXECNAME quic-client
  SOURCE_FILES ${client_sources}
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_OUTPUT_DIRECTORY}/scratch/quic-client/
  LIBRARIES_TO_LINK 
    ${libquic}
    ${libcore}
    ${libnetwork}
    ${libfd-net-device}
)
build_exec(
  EXECNAME quic-server
  SOURCE_FILES ${server_sources}
  EXECUTABLE_DIRECTORY_PATH ${CMAKE_OUTPUT_DIRECTORY}/scratch/quic-server/
  LIBRARIES_TO_LINK
    ${libquic}
    ${libcore}
    ${libnetwork}
    ${libfd-net-device}
)
