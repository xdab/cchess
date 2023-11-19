cmake_minimum_required(VERSION 3.13)
project(cchess VERSION 0.0.1 LANGUAGES C)

include_directories(include)

file(GLOB headers include/*.h)
file(GLOB sources src/*.c)
list(FILTER sources EXCLUDE REGEX ".*main\\.c$")
add_executable(cchess src/main.c ${sources} ${headers})
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
 
enable_testing()

macro(add_unit_test test_file)
	get_filename_component(test_name ${test_file} NAME_WE)
	file(GLOB test_headers test/*.h)
	list(APPEND test_headers ${headers})
	add_executable(${test_name} ${test_file} ${sources} ${test_headers})
	add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
endmacro(add_unit_test)

file(GLOB test_files test/*.c)
foreach(test_file ${test_files})
	add_unit_test(${test_file})
endforeach(test_file)
