
set(TARGET_LIB "crefgen")
add_library(
    ${TARGET_LIB} SHARED
        "rgcommon.h"
		"c_api_comm.h"
)



set(${TARGET_LIB}_INCLUDE_DIRS "${WIN_INST_PREFIX}include/${PROJ_NAME}")#/${TARGET_LIB}")
set(${TARGET_LIB}_LIBRARIES "${WIN_INST_PREFIX}lib/${PROJ_NAME}")

target_compile_definitions(${TARGET_LIB} PRIVATE RG_EXPORTS XTENSOR_USE_XSIMD)
target_link_libraries(${TARGET_LIB} xtensor xtl xsimd)

message(STATUS "${TARGET_LIB} LIBRARIES: " ${${TARGET_LIB}_LIBRARIES})


set(RG_SRC_DIR "${CMAKE_SOURCE_DIR}/${PROJ_NAME}")

message(STATUS "source dir: " ${RG_SRC_DIR})

target_include_directories(
    ${TARGET_LIB} 
        PUBLIC ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS} ${xsimd_INCLUDE_DIRS} ${RG_SRC_DIR}
)


if (WIN32)
	add_custom_command(TARGET ${TARGET_LIB} POST_BUILD			# Adds a post-build event to pyvlib
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
			"$<TARGET_FILE:${TARGET_LIB}>"      # <--this is in-file
	        "$<TARGET_FILE_DIR:${TARGET_LIB}>/${TARGET_LIB}/tests")
endif()
		
add_subdirectory(${TARGET_LIB})


install(FILES rgcommon.h c_api_comm.h DESTINATION ${${TARGET_LIB}_INCLUDE_DIRS})
install(TARGETS ${TARGET_LIB} DESTINATION ${${TARGET_LIB}_LIBRARIES})

set(Config_IN ${CMAKE_CURRENT_SOURCE_DIR}/targetConfig.cmake)
set(Config_OUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_LIB}Config.cmake)

# request to configure the file
configure_file(${Config_IN} ${Config_OUT} @ONLY)

install(FILES ${Config_OUT} DESTINATION ${WIN_INST_PREFIX}lib/cmake/${PROJ_NAME})
install(FILES "LICENSE.md" DESTINATION ${WIN_INST_PREFIX}doc/${PROJ_NAME})
install(FILES "README.md" DESTINATION ${WIN_INST_PREFIX}doc/${PROJ_NAME})