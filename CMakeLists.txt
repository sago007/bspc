cmake_minimum_required(VERSION 3.10.0..4.1.0)
project (bspc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Dstricmp=strcasecmp -DCom_Memcpy=memcpy -DCom_Memset=memset \
	-DMAC_STATIC= -DQDECL= -DBSPC -D_FORTIFY_SOURCE=2 \
	-I. -Ideps -Wall -std=gnu11 -Wsuggest-attribute=noreturn \
	-Wsuggest-attribute=format -ffast-math")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG} -g -O1")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dstricmp=strcasecmp -DCom_Memcpy=memcpy -DCom_Memset=memset \
	-DMAC_STATIC= -DQDECL= -DBSPC -D_FORTIFY_SOURCE=2 \
	-I. -Ideps -std=c++11 -Wall -Wsuggest-attribute=noreturn \
	-Wsuggest-attribute=format -ffast-math")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG} -g -O1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} -O3")

file(GLOB SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "deps/*/*.c" "deps/*/*.cpp")
add_executable(bspc ${SOURCES})
target_link_libraries( bspc m pthread)
