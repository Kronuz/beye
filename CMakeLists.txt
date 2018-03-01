cmake_minimum_required(VERSION 3.2)

project(beye CXX)

set(PACKAGE_NAME "Beye")
set(PACKAGE_VERSION "1.0.0")
set(PACKAGE_TARNAME "${PROJECT_NAME}")

########################################################################

include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CMakePushCheckState)
include(TestBigEndian)


########################################################################
# Options
########################################################################

# option(static      "use static linkage"                              ON)
option(fastcall    "use regparm method on x86 systems"               ON)
option(constfunc   "use const modifier for some functions"           ON)
option(purefunc    "use pure modifier for some functions"            ON)
option(mouse       "build mouse support on *nix systems"             ON)
option(iconv       "build iconv support"                             ON)
# option(shared      "use dynamic linkage"                            OFF)
option(gomp        "use GNU OpenMP (requires gcc-4.3+)"             OFF)
# option(gcov        "compile gnu coverage information into PROGRAM"  OFF)
option(profile     "compile profiling information into PROGRAM"     OFF)


########################################################################
# Compiler Flags
########################################################################

# Default value for CMAKE_BUILD_TYPE.
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Setup CMAKE_CXX_FLAGS Debug flags.
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")

# Check for fdiagnostics-color flag.
check_cxx_compiler_flag("-fdiagnostics-color=always" COMPILER_SUPPORTS_DIAG_COLOR)
if(COMPILER_SUPPORTS_DIAG_COLOR)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# Check for warnings flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -W")
check_cxx_compiler_flag("-Wall" COMPILER_SUPPORTS_WALL)
if(COMPILER_SUPPORTS_WALL)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
endif()
check_cxx_compiler_flag("-Wextra" COMPILER_SUPPORTS_WEXTRA)
if(COMPILER_SUPPORTS_WEXTRA)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")
endif()
check_cxx_compiler_flag("-Wshadow" COMPILER_SUPPORTS_SHADOW)
if(COMPILER_SUPPORTS_SHADOW)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wshadow")
endif()
check_cxx_compiler_flag("-Warray-bounds" COMPILER_SUPPORTS_ARRAY_BOUNDS)
if(COMPILER_SUPPORTS_ARRAY_BOUNDS)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Warray-bounds")
endif()
check_cxx_compiler_flag("-Wreturn-type" COMPILER_SUPPORTS_RETURN_TYPE)
if(COMPILER_SUPPORTS_RETURN_TYPE)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wreturn-type")
endif()
check_cxx_compiler_flag("-Wuninitialized" COMPILER_SUPPORTS_UNINITIALIZED)
if(COMPILER_SUPPORTS_UNINITIALIZED)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wuninitialized")
endif()
check_cxx_compiler_flag("-Wlogical-op" COMPILER_SUPPORTS_LOGICAL_OP)
if(COMPILER_SUPPORTS_LOGICAL_OP)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wlogical-op")
endif()
check_cxx_compiler_flag("-Waddress" COMPILER_SUPPORTS_ADDRESS)
if(COMPILER_SUPPORTS_ADDRESS)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Waddress")
endif()
# check_cxx_compiler_flag("-Wnon-virtual-dtor" COMPILER_SUPPORTS_WNON_VIRTUAL_DTOR)
# if(COMPILER_SUPPORTS_WNON_VIRTUAL_DTOR)
# 	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wnon-virtual-dtor")
# endif()
# check_cxx_compiler_flag("-Wsuggest-final-types" COMPILER_SUPPORTS_WSUGGEST_FINAL_TYPES)
# if(COMPILER_SUPPORTS_WSUGGEST_FINAL_TYPES)
# 	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wsuggest-final-types")
# endif()
# check_cxx_compiler_flag("-pedantic" COMPILER_SUPPORTS_PEDANTIC)
# if(COMPILER_SUPPORTS_PEDANTIC)
# 	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic")
# endif()

# fixes MAC OS X compilation:
check_cxx_compiler_flag("-mdynapic-no-pic" COMPILER_DYNAPIC_NO_PIC)
if(COMPILER_DYNAPIC_NO_PIC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mdynapic-no-pic")
endif()
check_cxx_compiler_flag("-fno-pic" COMPILER_SUPPORTS_NO_PIC)
if(COMPILER_SUPPORTS_NO_PIC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-pic")
endif()


########################################################################

# check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
# if(COMPILER_SUPPORTS_CXX11)
# 	set(HAVE_CXX11 1)
# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# endif()

set(CMAKE_REQUIRED_FLAGS_CACHE ${CMAKE_REQUIRED_FLAGS})
set(CMAKE_REQUIRED_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
check_cxx_compiler_flag("-fsanitize=address -fno-omit-frame-pointer" COMPILER_SUPPORTS_ASAN)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_CACHE})

if(COMPILER_SUPPORTS_ASAN)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
else()
	message(WARNING "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} doesn't support AddressSanitizer")
endif()

########################################################################


if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
	set(ARCH "X86_64")
	set(__CPU__ "686")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ia32")
	set(ARCH "IA32")
	set(__CPU__ "IA32")
else()
	set(ARCH "X86_32")
	set(ARCH_X86 1)
	set(CAN_COMPILE_X86_GAS 1)
	set(__CPU__ "386")
endif()
message(STATUS "Architecture: ${ARCH}")

set(SLIBSUFFIX ".so")
set(HAVE_TERMINAL_OUT 1)
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(OS "DARWIN")
	set(__UNIX__ 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(OS "LINUX")
	set(__UNIX__ 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	set(OS "FREEBSD")
	set(__UNIX__ 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
	set(OS "OPENBSD")
	set(__UNIX__ 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "NetBSD")
	set(OS "NETBSD")
	set(__UNIX__ 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "(Solaris|SunOS)")
	set(OS "SOLARIS")
	set(__UNIX__ 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "OS2")
	set(OS "OS2")
	set(HAVE_TERMINAL_OUT 0)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows|Win32")
	set(OS "WINDOWS")
	set(HAVE_TERMINAL_OUT 0)
	set(SLIBSUFFIX ".dll")
endif()
message(STATUS "Target: ${OS}")

test_big_endian(WORDS_BIGENDIAN)
message(STATUS "Big endian: ${WORDS_BIGENDIAN}")


########################################################################

if(fastcall AND NOT profile)
	check_cxx_source_compiles("
		int __attribute__ ((regparm (3))) func(int a) { return a; }
		int main(void) { return func(0); }
	" USE_FASTCALL)
endif()

if(constfunc)
	check_cxx_source_compiles("
		int func(int a) { return a; } __attribute__ ((const))
		int main(void) { return func(0); }
	" USE_CONSTFUNC)
endif()

if(purefunc)
	check_cxx_source_compiles("
		int func(int a) { return a; } __attribute__ ((pure))
		int main(void) { return func(0); }
	" USE_PUREFUNC)
endif()

# Checking for GOMP
if(gomp)
	find_library(HAVE_GOMP gomp)
	if (HAVE_GOMP)
		check_cxx_compiler_flag("-fopenmp" COMPILER_SUPPORTS_OPENMP)
		if(COMPILER_SUPPORTS_NO_PIC)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
		endif()
		check_cxx_source_compiles("
			#include <omp.h>
			int main(void) { return (int)omp_get_thread_num; }
		" HAVE_GOMP)
	endif()
endif()


check_cxx_source_compiles("
	#pragma pack(1)
	int main(void) { return 0; }
" HAVE_PPACK)
message(STATUS "HAVE_PPACK: ${HAVE_PPACK}")


cmake_push_check_state()
set(CMAKE_REQUIRED_QUIET 1)
check_cxx_source_runs("
	#include <unistd.h>
	int main(void) { return getpagesize() / 1024; }
" VM_PAGESIZE)
cmake_pop_check_state()
if(VM_PAGESIZE_COMPILED)
	math(EXPR VM_PAGESIZE "${VM_PAGESIZE_EXITCODE} * 1024")
else()
	set(VM_PAGESIZE 4096)
endif()


########################################################################

foreach(_rnd_var RND0 RND1 RND2 RND3 RND4 RND5 RND6 RND7 RND8 RND9
	            SRND0 SRND1 SRND2 SRND3 SRND4 SRND5 SRND6 SRND7 SRND8 SRND9
	            FRND0 FRND1 FRND2 FRND3 FRND4 FRND5 FRND6 FRND7 FRND8 FRND9)
	if(ARCH STREQUAL "X86_64")
		# 4294967295
		string(RANDOM LENGTH 1 ALPHABET 01234 _first)
		string(RANDOM LENGTH 9 ALPHABET 0123456789 _rnd)
	else()
		# 65535
		string(RANDOM LENGTH 1 ALPHABET 0123456 _first)
		string(RANDOM LENGTH 4 ALPHABET 0123456789 _rnd)
	endif()
	string(CONCAT _rnd "${_first}" "${_rnd}")
	string(REGEX REPLACE "^0+" "" _rnd "${_rnd}")
	if(_rnd STREQUAL "")
		set("${_rnd_var}" "0")
	else()
		set("${_rnd_var}" "${_rnd}")
	endif()
	# message(STATUS "${_rnd_var}: ${_rnd}")
endforeach(_rnd_var)
foreach(_rnd_var CRND0 CRND1 CRND2 CRND3 CRND4 CRND5 CRND6 CRND7 CRND8 CRND9)
	string(RANDOM LENGTH 1 ALPHABET 012 _first)
	string(RANDOM LENGTH 2 ALPHABET 0123456789 _rnd)
	string(CONCAT _rnd "${_first}" "${_rnd}")
	string(REGEX REPLACE "^0+" "" _rnd "${_rnd}")
	if(_rnd STREQUAL "")
		set("${_rnd_var}" "0")
	else()
		set("${_rnd_var}" "${_rnd}")
	endif()
	# message(STATUS "${_rnd_var}: ${_rnd}")
endforeach(_rnd_var)


########################################################################

check_include_file_cxx("endian.h" HAVE_ENDIAN_H)
check_include_file_cxx("sys/resource.h" HAVE_SYS_RESOURCE_H)
check_include_file_cxx("byteswap.h" HAVE_BYTESWAP_H)

check_function_exists("backtrace" HAVE_BACKTRACE)  # execinfo.h
check_function_exists("popen" HAVE_POPEN)  # stdio.h
check_function_exists("pclose" HAVE_PCLOSE)  # stdio.h
check_function_exists("mremap" HAVE_MREMAP)  # sys/mman.h
if(HAVE_MREMAP)
	check_function_exists("mmap" HAVE_MMAP)  # sys/mman.h
endif()
check_function_exists("mprotect" HAVE_MPROTECT)  # sys/mman.h
check_function_exists("memalign" HAVE_MEMALIGN)  # malloc.h

if(iconv)
	check_library_exists(iconv "iconv_open" "" HAVE_ICONV)
	if(HAVE_ICONV)
		list(APPEND BEYE_LIBRARIES iconv)
	endif()
	check_library_exists(giconv "iconv_open" "" HAVE_GICONV)
	if(HAVE_GICONV)
		list(APPEND BEYE_LIBRARIES giconv)
	endif()
endif()

if(mouse)
	check_library_exists(gpm "Gpm_GetEvent" "" HAVE_MOUSE)
	if (HAVE_MOUSE)
		list(APPEND BEYE_LIBRARIES giconv)
	endif()
endif()

########################################################################
# Configuration files
########################################################################

configure_file(
	"${PROJECT_SOURCE_DIR}/version.h.in"
	"${PROJECT_BINARY_DIR}/src/version.h"
	@ONLY)

configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/src/config.h"
	@ONLY)


########################################################################
# Check CMAKE_BUILD_TYPE:  Debug Release RelWithDebInfo MinSizeRel
########################################################################

# Remove duplicates compilation flags
separate_arguments(CMAKE_CXX_FLAGS)
list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

string(TOLOWER ${CMAKE_BUILD_TYPE} _build_type)
if(_build_type STREQUAL "debug")
	set(CMAKE_BUILD_TYPE "Debug")
	separate_arguments(CMAKE_CXX_FLAGS_DEBUG)
	list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS_DEBUG)
	string(REPLACE ";" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	message(STATUS "Debug build")
	message(STATUS "Compile flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")

elseif(_build_type STREQUAL "relwithdebinfo")
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
	separate_arguments(CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	string(REPLACE ";" " " CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	message(STATUS "Release With Debug Info build")
	message(STATUS "Compile flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

elseif(_build_type STREQUAL "minsizerel")
	set(CMAKE_BUILD_TYPE "MinSizeRel")
	separate_arguments(CMAKE_CXX_FLAGS_MINSIZEREL)
	list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS_MINSIZEREL)
	string(REPLACE ";" " " CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
	message(STATUS "Minimum Size Release build")
	message(STATUS "Compile flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_MINSIZEREL}")

else()
	set(CMAKE_BUILD_TYPE "Release")
	separate_arguments(CMAKE_CXX_FLAGS_RELEASE)
	list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS_RELEASE)
	string(REPLACE ";" " " CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	message(STATUS "Release build")
	message(STATUS "Compile flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
endif()


########################################################################
# Targets
########################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

if(OS STREQUAL "DARWIN")
	set(OSDEP "unix")
elseif(OS STREQUAL "FREEBSD")
	set(OSDEP "unix")
elseif(OS STREQUAL "LINUX")
	set(OSDEP "linux")
elseif(OS STREQUAL "WINDOWS")
	set(OSDEP "win32")
elseif(OS STREQUAL "OS2")
	set(OSDEP "os2")
else()
	message(FATAL_ERROR "Cannot find OS dependencies.")
endif()

if(ARCH STREQUAL "X86_64")
	set(SYSDEP "x86_64")
elseif(ARCH STREQUAL "IA32")
	set(SYSDEP "ia32")
elseif(ARCH STREQUAL "X86")
	set(SYSDEP "generic")
else()
	message(FATAL_ERROR "Cannot find System dependencies.")
endif()

file(GLOB LIBBEYE_SRC_LIST
	${PROJECT_SOURCE_DIR}/src/libbeye/*.cpp ${PROJECT_SOURCE_DIR}/src/libbeye/*.h
	${PROJECT_SOURCE_DIR}/src/libbeye/sysdep/*.cpp ${PROJECT_SOURCE_DIR}/src/libbeye/sysdep/*.h
	${PROJECT_SOURCE_DIR}/src/libbeye/sysdep/${SYSDEP}/*.cpp ${PROJECT_SOURCE_DIR}/src/libbeye/sysdep/${SYSDEP}/*.h
	${PROJECT_SOURCE_DIR}/src/libbeye/osdep/*.cpp ${PROJECT_SOURCE_DIR}/src/libbeye/osdep/*.h
	${PROJECT_SOURCE_DIR}/src/libbeye/osdep/${OSDEP}/*.cpp ${PROJECT_SOURCE_DIR}/src/libbeye/osdep/${OSDEP}/*.h
)
add_library(LIBBEYE_OBJ OBJECT ${LIBBEYE_SRC_LIST})

file(GLOB BEYE_SRC_LIST
	${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/bin/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/bin/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/nls/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/nls/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/disasm/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/disasm/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/disasm/ix86/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/disasm/ix86/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/disasm/avr/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/disasm/avr/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/disasm/ppc/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/disasm/ppc/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/disasm/java/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/disasm/java/*.h
	${PROJECT_SOURCE_DIR}/src/plugins/disasm/arm/*.cpp ${PROJECT_SOURCE_DIR}/src/plugins/disasm/arm/*.h
	${PROJECT_SOURCE_DIR}/src/addons/*.cpp ${PROJECT_SOURCE_DIR}/src/addons/*.h
	${PROJECT_SOURCE_DIR}/src/addons/tools/*.cpp ${PROJECT_SOURCE_DIR}/src/addons/tools/*.h
	${PROJECT_SOURCE_DIR}/src/addons/sysinfo/*.cpp ${PROJECT_SOURCE_DIR}/src/addons/sysinfo/*.h
)

add_executable(${PROJECT_NAME}
	${BEYE_SRC_LIST}
	$<TARGET_OBJECTS:LIBBEYE_OBJ>
)

target_link_libraries(${PROJECT_NAME} ${BEYE_LIBRARIES})

include_directories(
	"${PROJECT_BINARY_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/plugins"
	"${PROJECT_SOURCE_DIR}/hlp"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/bin_rc/skn/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/beye/skn)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/bin_rc/syntax/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/beye/syntax)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/bin_rc/xlt/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/beye/xlt)
install(FILES
	${CMAKE_SOURCE_DIR}/doc/beye.txt
	${CMAKE_SOURCE_DIR}/doc/ru/beye.txt
	${CMAKE_SOURCE_DIR}/doc/release.txt
	${CMAKE_SOURCE_DIR}/doc/unix.txt
	DESTINATION ${CMAKE_INSTALL_DOCDIR}/beye)
install(FILES ${CMAKE_SOURCE_DIR}/man/beye.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
