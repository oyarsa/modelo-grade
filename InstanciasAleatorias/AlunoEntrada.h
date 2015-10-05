#pragma once
#include "Aluno.h"

class AlunoEntrada : public Aluno {
public:
	//! Construtor padrão do Aluno
	//! \see Aluno.h
	AlunoEntrada(const std::vector<std::vector<bool>>& preRequisitos,
				 const std::vector<std::vector<bool>>& coRequisitos,
				 const std::string& nome, int periodo, std::string turma);

	//! Move constructor para a classe
	AlunoEntrada(AlunoEntrada&& outro);

	//! Setter para o vetor de disciplinas aprovadas do aluno.
	//! Utilize std::move para move as estruturas para a função, 
	//! a fim de evitar cópias
	//! \param aprovacoes Rvalue reference para um vetor de booleanos
	void setAprovacoes(std::vector<bool>&& aprovacoes);

	//! Setter para o vetor de disciplinas cursadas do aluno
	//! Utilize std::move para move as estruturas para a função, 
	//! a fim de evitar cópias
	//! \param cursadas Rvalue reference para um vetor de booleanos
	void setCursadas(std::vector<bool>&& cursadas);
	
};
