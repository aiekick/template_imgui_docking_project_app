set(EZLIBS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/ezlibs/include)

set(USE_EZ_LIBS_TESTING ${USE_BUILDING_OF_TESTS} CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ezlibs)

if (TARGET EzLibs_TestOpengl)
	target_link_libraries(EzLibs_TestOpengl PRIVATE glad glfw stb)
endif()
