cmake_minimum_required(VERSION 2.8)

project(ModeloGrade)

# dependência com o CPLEX
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_extensions)
find_package(Cplex)
if (NOT CPLEX_FOUND)
    message(error "Cplex not found")
endif()

include_directories(SYSTEM ${CPLEX_INCLUDE_DIRS})

add_definitions(-DIL_STD)
string (REPLACE "/D_DEBUG" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
endif()

set(MODELO_FILES
    modelo-grade/aluno.h
    modelo-grade/aluno.cpp
    modelo-grade/arquivos.h
    modelo-grade/arquivos.cpp
    modelo-grade/curso.h
    modelo-grade/curso.cpp
    modelo-grade/modelo_solver.h
    modelo-grade/modelo_solver.cpp
    modelo-grade/solucao.h
    modelo-grade/solver.h
    modelo-grade/jsoncpp.cxx
    )

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14 ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  add_definitions("-stdlib=libstdc++")
  set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

add_executable(modelo-grade ${MODELO_FILES} modelo-grade/main.cpp)
add_library(modelo-grade-lib ${MODELO_FILES})

target_link_libraries(modelo-grade ${CPLEX_LIBRARIES})
target_link_libraries(modelo-grade-lib ${CPLEX_LIBRARIES})