build_example(
  NAME mpquic-ns3
  SOURCE_FILES mpquic.cc
  LIBRARIES_TO_LINK
    ${libquic}
)
