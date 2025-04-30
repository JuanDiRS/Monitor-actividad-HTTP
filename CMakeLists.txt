# CMakeList.txt: proyecto de CMake para ProyectoLu, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Habilite Recarga activa para los compiladores de MSVC si se admiten.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ProyectoLu")

# TODO: Agregue pruebas y destinos de instalación si es necesario.

# set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/pcapplusplus" ${CMAKE_PREFIX_PATH})
set(PCAPPLUSPLUS_ROOT "/usr/local")
set(CMAKE_PREFIX_PATH "${PCAPPLUSPLUS_ROOT}/lib/cmake/pcapplusplus")

find_package(PcapPlusPlus REQUIRED)
message(STATUS "PcapPlusPlus FOUND: ${PcapPlusPlus_FOUND}")

include_directories(
    ${PcapPlusPlus_INCLUDE_DIRS}
    /usr/local/include/pcapplusplus
)

link_directories(
${PcapPlusPlus_LIBRARIES_DIRS}
/usr/local/lib)


# Agregue un origen al ejecutable de este proyecto.
 add_executable (ProyectoLu "ProyectoLu.cpp" "ProyectoLu.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ProyectoLu PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(ProyectoLu Pcap++ Packet++ Common++ pcap)
