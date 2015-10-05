#pragma once

#include <vector>

//! Representa um aluno de um Curso, e tem suas apravoções e
//! disciplinas cursadas geradas aleatoriamente a partir da matriz
//! de pré-requisitos do curso.
class Aluno {
public:
	virtual ~Aluno() {}

	//! Recebe o nome do aluno e a matriz de pré-requisitos de seu curso
	//! e partir dela gera aleatoriamente as matérias cursadas e aprovadas
	//!
	//! \param preRequisitos Referência const para uma matriz booleana, os pré-requisitos
	//! do curso em que o aluno estuda
	//! \param coRequisitos Referência const para uma matriz booleana, os co-requisitos
	//! do curso em que o aluno estuda
	//! \param nome Nome do aluno
	Aluno(const std::vector<std::vector<bool>>& preRequisitos,
	      const std::vector<std::vector<bool>>& coRequisitos,
	      std::string nome);

	//! \brief Retorna um referência const para o vetor de aprovações do aluno
	//!  \return Referência const para o vetor de aprovações
	//!
	const std::vector<bool>& aprovacoes() const;

	//! \brief Retorna um referência const para o vetor de disciplinas cursadas do aluno
	//! \return Referência const para o vetor de disciplinas cursadas
	const std::vector<bool>& cursadas() const;

	//! \brief Retorna o nome do aluno
	//! \return Cópia de uma string, o nome do aluno
	std::string nome() const;

	//! \brief Retorna o período em que o aluno está
	//! \return Int, período do aluno
	int periodo() const;

	//! \brief Retorna a turma em que o aluno começou o curso
	std::string turma() const;

protected:
	//! Número de disciplinas do curso
	int numDisciplinas;
	//! Número de disciplinas em que o aluno foi aprovado (aleatório)
	int numAprovacoes;
	//! Número de disciplinas que o aluno cursou ( >= a numAprovacoes)
	int numCursadas;
	//! Referência const para matriz de pré-requisitos do curso
	const std::vector<std::vector<bool>>& preRequisitos_;
	//! Referência const para matriz de co-requisitos do curso
	const std::vector<std::vector<bool>>& coRequisitos_;
	//! Nome do aluno
	std::string nome_;
	//! Vetor de disciplinas em que o aluno foi aprovado
	std::vector<bool> aprovacoes_;
	//! Vetor de disciplinas que o aluno cursou
	std::vector<bool> cursadas_;
	//! Período em que o aluno está
	int periodo_;
	//! Turma em que o aluno começou o curso
	std::string turma_;
};
