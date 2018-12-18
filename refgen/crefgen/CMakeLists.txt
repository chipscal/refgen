message(STATUS "-------> Entering dir: " ${CMAKE_CURRENT_LIST_DIR})

file(GLOB pub_header "*.h" "*.hpp")
file(GLOB_RECURSE priv_sources "src/*.cpp" "src/*.h" "src/*.hpp")

foreach(hdr ${pub_header})
	message(STATUS "---------------> founded header: " ${hdr})
endforeach()

foreach(psrc ${priv_sources})
	message(STATUS "---------------> founded source: " ${psrc})
endforeach()

target_sources(${TARGET_LIB}
	PUBLIC ${pub_header}
	PRIVATE ${priv_sources}
)

add_subdirectory("tests")

install(FILES ${pub_header} DESTINATION ${${TARGET_LIB}_INCLUDE_DIRS}/${TARGET_LIB})
