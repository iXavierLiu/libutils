function(libutils_build_module name)

	set(_LIB_NAME libutils_${name})

	aux_source_directory("src" _SRC)
	
	add_library(${_LIB_NAME} ${_SRC})
	add_library(libutils::${name} ALIAS ${_LIB_NAME})
	
	target_include_directories(${_LIB_NAME} PUBLIC "include")
	target_include_directories(${_LIB_NAME} PRIVATE "src")
	
	if(ARGC GREATER 1)
		list(SUBLIST ARGV 1 -1 _DEPENDENCIES)
		foreach(_DEPENDENCY IN LISTS _DEPENDENCIES)
			target_link_libraries(${_LIB_NAME} PUBLIC libutils::${_DEPENDENCY})
			message("added dependency libutils::${_DEPENDENCY} to ${_LIB_NAME}")
		endforeach()
	endif()
	
	target_compile_options(${_LIB_NAME} PRIVATE "$<$<C_COMPILER_ID:MSVC>:/utf-8>" "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	
	install(TARGETS ${_LIB_NAME} LIBRARY)
	install(DIRECTORY "include/libutils" TYPE INCLUDE)

endfunction()

function(libutils_build_module_interface name)

	set(_LIB_NAME libutils_${name})

	add_library(${_LIB_NAME} INTERFACE)
	add_library(libutils::${name} ALIAS ${_LIB_NAME})

	target_include_directories(${_LIB_NAME} INTERFACE "include")

	install(DIRECTORY "include/libutils" TYPE INCLUDE)

endfunction()