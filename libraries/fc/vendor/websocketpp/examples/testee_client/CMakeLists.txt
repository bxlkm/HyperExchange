
file (GLOB SOURCE_FILES *.cpp)
file (GLOB HEADER_FILES *.hpp)

if (ZLIB_FOUND)

init_target (testee_client)

build_executable (${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})

link_boost ()
link_zlib()
final_target ()

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")

endif()
