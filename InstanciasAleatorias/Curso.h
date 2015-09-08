#pragma once


#include <vector>
#include "Professor.h"

class Curso {
public:
	/*!
	* \brief Constroi um curso aleatório a partir de parâmetros arbitrários
	*
	* \param numDisciplinas Número total de disciplinas do curso
	* \param numPreRequisitos Soma total do número de pré-requisitos de cada matéria do curso
	* \param numCoRequisitos Soma total do número de co-requisitos de cada matéria do curso
	* \param numHorarios Número de horários (perído de 1 hora ou 50 minutos) em uma semana
	* \param numOfertadas Número de disciplinas ofertadas no período
	* \param numProfessores Número de professores do curso
	*/
	Curso(int numDisciplinas, int numPreRequisitos, int numCoRequisitos,
	      int numHorarios, int numOfertadas, int numProfessores);

	virtual ~Curso() {}

	//! Retorna uma referência const ao vetor de créditos das disciplinas
	//! \return Referência const para um vector de ints
	const std::vector<int>& creditos() const;

	//! Retorna uma referência const ao vetor de disciplinas ofertadas no período
	//! \return Referência const para um vector de booleanos
	const std::vector<bool>& ofertadas() const;

	//! Retorna uma referência const a uma matriz de pré-requisitos das disciplinas do curso
	//! \return Referência const para uma matriz de booleanos
	const std::vector<std::vector<bool>>& preRequisitos() const;

	//! Retorna uma referência const a uma matriz de co-requisitos das disciplinas do curso
	//! \return Referência const para uma matriz de booleanos
	const std::vector<std::vector<bool>>& coRequisitos() const;

	//! Retorna uma referência const a uma matriz de horários do curso
	//! \return Referência const para uma matriz de booleanos
	const std::vector<std::vector<bool>>& horarios() const;

	//! Retorna uma referência const a um vetor de strings, os nomes das disciplinas do curso
	//! \return Referência const para um vector de strings
	const std::vector<std::string>& nomeDisciplinas() const;

	//! Retorna uma referência const a um vetor de strings, os nomes dos horários do curso
	//! \return Referência const para um vector de strings
	const std::vector<std::string>& nomeHorarios() const;

	//! Retorna uma referência const a um vetor de Professores curso
	//! \return Referência const para um vector de Professor
	const std::vector<Professor>& professores() const;

protected:
	//! Número de disciplinas do curso
	int numDisciplinas;
	//! Número de pré-requisitos do curso
	int numPreRequisitos;
	//! Número de co-requisitos do curso
	int numCoRequisitos;
	//! Número de horários do curso
	int numHorarios;
	//! Número de disciplinas ofertadas no período
	int numOfertadas;
	//! Número de professores do curso
	int numProfessores;
	//! Vetor de nomes das disciplinas do curso
	std::vector<std::string> nomeDisciplinas_;
	//! Vetor de nomes dos horários do curso
	std::vector<std::string> nomeHorarios_;
	//! Vetor com os valores dos créditos das disciplinas do curso
	std::vector<int> creditos_;
	//! Vetor com as disciplinas ofertadas naquele período
	std::vector<bool> ofertadas_;
	//! Matriz dos pré-requisitos das disciplinas
	std::vector<std::vector<bool>> preRequisitos_;
	//! Matriz dos co-requisitos das disciplinas
	std::vector<std::vector<bool>> coRequisitos_;
	//! Matriz dos horários do curso
	std::vector<std::vector<bool>> horarios_;
	//! Vetor de professores do curso
	std::vector<Professor> professores_;

	//! Gera os nomes dos horários sequencialmente (HOR1, HOR2, HOR3...)
	virtual void geraNomeHorarios();
	//! Pure virtual - definida pelas class filhas
	virtual void geraCreditos() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraPreRequisitos() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraCoRequisitos() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraOfertadas() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraNomeDisciplinas() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraDisciplinas() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraHorario() = 0;
	//! Pure virtual - definida pelas class filhas
	virtual void geraProfessores() = 0;
};
