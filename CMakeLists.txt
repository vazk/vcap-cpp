cmake_minimum_required (VERSION 2.6)
project (vcap-cpp)

cmake_policy(SET CMP0015 NEW)

set(LIBRARY_OUTPUT_PATH lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY examples)

# Documentation
find_package(Doxygen)

if(DOXYGEN_FOUND)
	add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} doxygen.conf
        WORKING_DIRECTORY .
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc") 
endif(DOXYGEN_FOUND)

find_library(VCAP vcap)

if (VCAP)
	# Library
	include_directories(include)
	
	add_definitions(-Wall -std=c++11 -D_GNU_SOURCE)
	add_library (vcap-cpp SHARED "src/Vcap.cpp")
	
	# Examples
	
	# Info example
	add_executable(Info "examples/Info.cpp")
	target_link_libraries(Info vcap-cpp ${VCAP})
	
	# Grab example
	add_executable(Grab "examples/Grab.cpp")
	target_link_libraries(Grab vcap-cpp ${VCAP})
	
	# PNG example
	find_package(PNG)
	
	if(PNG_FOUND)
		include_directories(${PNG_INCLUDE_DIR})
		add_executable(Png "examples/Png.cpp")
		target_link_libraries(Png vcap-cpp ${VCAP} ${PNG_LIBRARY})
	else(PNG_FOUND)
		message(WARNING "You need libPNG install in order to build examples/Png.cpp")
	endif(PNG_FOUND)
	
	# SDL example
	find_package(SDL)
	
	if(SDL_FOUND)
		include_directories(${SDL_INCLUDE_DIR})
		add_executable(Sdl "examples/Sdl.cpp")
		target_link_libraries(Sdl vcap-cpp ${VCAP} ${SDL_LIBRARY})
	else(SDL_FOUND)
		message(WARNING "You need SDL1.2 install in order to build examples/Sdl.cpp")
	endif(SDL_FOUND)
	
	# Installation
	install(DIRECTORY include/Vcap DESTINATION include)
	install(FILES lib/libvcap-cpp.so DESTINATION lib)
else(VCAP)
	message(FATAL_ERROR "The C version of the Vcap library must be installed. Aborting...")
endif(VCAP)
