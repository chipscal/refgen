cmake_minimum_required(VERSION 2.6)

set(PROJ_NAME refgen)


project(${PROJ_NAME})

find_package(xtl REQUIRED)
find_package(xtensor REQUIRED)
find_package(xsimd REQUIRED)




if(UNIX)
    message(STATUS "Linux env...")
	# Note: to compile on ARM (or cross compile), you may need to add the following:
	# -DTARGET_ARCH="armv8-a -mfpu=neon -mfloat-abi=softfp -target arm-linux-gnueabi"
	#set(TARGET_ARCH "native" CACHE STRING "Target architecture arguments")
	#set(ARM_ARCH_DIRECTORY "arm-linux-gnueabi" CACHE STRING "ARM arch header dir")
	#set(ARM_GCC_VER "6.3.0" CACHE STRING "ARM GCC header dir")

	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -march=${TARGET_ARCH} -Wunused-parameter -Wextra -Wreorder -std=c++14")

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 ${ARCH_SPECIFIC}")
endif()

if (WIN32)
	set(WIN_INST_PREFIX "${PROJ_NAME}/")
endif()

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX -gd)
endif()

add_subdirectory("${PROJ_NAME}")

find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API        
documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
	
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc_doxygen/html DESTINATION ${WIN_INST_PREFIX}doc/${PROJ_NAME})
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc_doxygen/latex DESTINATION ${WIN_INST_PREFIX}doc/${PROJ_NAME})
endif()
