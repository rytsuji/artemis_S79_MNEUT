set(LIB_NAME art_test)

set(SOURCES
  TCmdCheckYAML.cc
  TCmdLoopAdd.cc
  TCmdShowKey.cc
  TCmdShowYAML.cc
  CmdYAMLUtil.cc
   TModuleDecoderMDPPQDC.cc
)

set(HEADERS
  TCmdCheckYAML.h
  TCmdLoopAdd.h
  TCmdShowKey.h
  TCmdShowYAML.h
 TModuleDecoderMDPPQDC.h
)

add_library(${LIB_NAME} SHARED ${SOURCES})

target_include_directories(${LIB_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIB_NAME}
  PRIVATE
    artemis::catcore
    artemis::catloop
    artemis::catcmd
    artemis::artshare
    ${ROOT_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
)

ROOT_GENERATE_DICTIONARY(G__${LIB_NAME}
  ${HEADERS}
  LINKDEF "linkdef_user.h"
  MODULE ${LIB_NAME}
)

set_target_properties(${LIB_NAME}
  PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

install(TARGETS ${LIB_NAME}
  INCLUDES DESTINATION include
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  LIBRARY DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${LIB_NAME}_rdict.pcm
  DESTINATION lib
)
