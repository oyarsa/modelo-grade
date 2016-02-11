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

# set(FAPTP_FILES
#     faptp/src/Aleatorio.cpp
#     faptp/src/Aleatorio.h
#     faptp/src/AlunoPerfil.cpp
#     faptp/src/AlunoPerfil.h
#     faptp/src/Disciplina.cpp
#     faptp/src/Disciplina.h
#     faptp/src/Grade.cpp
#     faptp/src/Grade.h
#     faptp/src/Horario.cpp
#     faptp/src/Horario.h
#     faptp/src/Output.cpp
#     faptp/src/Output.h
#     faptp/src/Professor.cpp
#     faptp/src/Professor.h
#     faptp/src/ProfessorDisciplina.cpp
#     faptp/src/ProfessorDisciplina.h
#     faptp/src/Representacao.cpp
#     faptp/src/Representacao.h
#     faptp/src/Resolucao.cpp
#     faptp/src/Resolucao.h
#     faptp/src/Semana.h
#     faptp/src/Solucao.cpp
#     faptp/src/Solucao.h
#     faptp/src/Util.cpp
#     faptp/src/Util.h
#     faptp/src/UUID.cpp
#     faptp/src/UUID.h
#     faptp/src/includes/parametros.cpp
#     faptp/src/includes/parametros.h
#     faptp/src/includes/jsoncpp.cpp
#     faptp/template/Algorithms.h
#     )

add_executable(modelo-grade ${MODELO_FILES} modelo-grade/main.cpp)
# add_executable(faptp ${FAPTP_FILES} faptp/main.cpp)

target_link_libraries(modelo-grade ${CPLEX_LIBRARIES})