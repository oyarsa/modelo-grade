#pragma once
#include "Curso.h"

//! Classe que representa um Curso cujas disciplinas e horários serão lidos
//! de alguma fonte, e não gerados aleatoriamente
class CursoEntrada : public Curso {
public:
	//! Construtor normal dos Cursos
	//! \see Curso.h
	CursoEntrada(int numDisciplinas, int numPreRequisitos,
				 int numCoRequisitos, int numHorarios,
				 int numOfertadas, int numProfessores,
				 int numDiasLetivos, int numPeriodos);

	CursoEntrada(CursoEntrada&& outro);

	//! Setter de todas as estruturas relativas a disciplinas do curso,
	//! recebendo referências rvalue como parâmetro. Utilize std::move para
	//! mover as estruturas para a função, a fim de evitar cópias
	//! \param nomeDisciplinas Rvalue reference para um vetor de strings
	//! \param preRequisitos Rvalue reference para uma matriz de booleanos
	//! \param coRequisitos Rvalue reference para uma matriz de booleanos
	//! \param ofertadas Rvalue reference para um vetor de booleanos
	//! \param creditos Rvalue reference para um vetor de inteiros
	void setDisciplinas(std::vector<std::string>&& nomeDisciplinas,
	                    std::vector<std::vector<bool>>&& preRequisitos,
	                    std::vector<std::vector<bool>>&& coRequisitos,
						std::vector<bool>&& ofertadas,
	                    std::vector<int>&& creditos,
						std::vector<std::vector<bool>>&& equivalencias,
						std::vector<std::pair<int, std::string>>&& discTurma,
						std::vector<int>&& capacidades,
						std::vector<int>&& periodosMinimos);

	//! Setter da matriz de horários do curso, recebendo uma referência rvalue
	//! para uma matriz de booleanos. Utilize std::move para mover as estruturas 
	//! para a função, a fim de evitar cópias
	//! \param horarios Rvalue reference para uma matriz de booleanos
	void setHorarios(std::vector<std::vector<bool>>&& horarios);

	//! Seter da lista de professores do curso, recebdno uma referência rvalue
	//! para um vector de Professor. Utilize std::move para mover as estruturas 
	//! para a função, a fim de evitar cópias
	void setProfessores(std::vector<Professor>&& professores);
};
