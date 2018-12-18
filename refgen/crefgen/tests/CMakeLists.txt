
include_directories(SYSTEM ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS} ${xsimd_INCLUDE_DIRS} ${RG_SRC_DIR})

link_libraries(${TARGET_LIB} xtensor xtl xsimd)

add_executable(utiltest "utiltest")
install(TARGETS utiltest DESTINATION ${${TARGET_LIB}_LIBRARIES})

add_executable(spsatest "spsatest")
install(TARGETS spsatest DESTINATION ${${TARGET_LIB}_LIBRARIES})

add_executable(costtest "costtest")
install(TARGETS costtest DESTINATION ${${TARGET_LIB}_LIBRARIES})

add_executable(refgentest "refgentest")
install(TARGETS refgentest DESTINATION ${${TARGET_LIB}_LIBRARIES})

add_executable(c_api_rgtest "c_api_rgtest")
install(TARGETS c_api_rgtest DESTINATION ${${TARGET_LIB}_LIBRARIES})

message(STATUS "dir: " ${xtensor_INCLUDE_DIRS})
#target_link_libraries(test1 PUBLIC xtensor ${TARGET_LIB})

#target_include_directories(
#    test1 
#        PUBLIC ${xtensor_INCLUDE_DIRS} ${xtl_INCLUDE_DIRS} ${RG_SRC_DIR}
#)


