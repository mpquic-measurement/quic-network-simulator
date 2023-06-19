build_example(
  NAME mpquic-ns3
  SOURCE_FILES mpquic.cc "./helper/*.cc"
  LIBRARIES_TO_LINK
    ${libquic}
)
