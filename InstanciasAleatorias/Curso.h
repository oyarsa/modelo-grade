#pragma once


#include <vector>
#include "Professor.h"

//! Curso que possui disciplinas e matrizes de horários, pré e co-requisitos
class Curso {
public:
	/*!
	* \brief Constroi um curso aleatório a partir de parâmetros arbitrários
	*
	* \param numDisciplinas_ Número total de disciplinas do curso
	* \param numPreRequisitos Soma total do número de pré-requisitos de cada matéria do curso
	* \param numCoRequisitos Soma total do número de co-requisitos de cada matéria do curso
	* \param numHorarios_ Número de horários (perído de 1 hora ou 50 minutos) em uma semana
	* \param numOfertadas Número de disciplinas ofertadas no período
	* \param numProfessores_ Número de professores do curso
	* \param numDiasLetivos Número de dias letivos por semana do curso
	* \param numPeriodos Número de períodos ofertados simultaneamente no curso
	*/
	Curso(int numDisciplinas, int numPreRequisitos, int numCoRequisitos,
	      int numHorarios, int numOfertadas, int numProfessores, int numDiasLetivos,
		  int numPeriodos);

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

	//! Retorna o número de disciplinas do curso
	//! \return Inteiro, número de disciplinsa do curso
	int numDisciplinas() const;

	//! Retorna o número de horários por semana do curso
	//! \return Inteiro, número de horários por semana
	int numHorarios() const;

	//! Retorna o número de períodos simultaneamente ofertados pelo curso
	//! \return Inteiro, número de períodos simultaneamente ofertados pelo curso
	int numPeriodos() const;

	//! Retorna o número de dias letivos por semana do curso
	//! \return Inteiro, número de dias letivos por semana
	int numDiasLetivos() const;

	//! Retorna o número de professores associados ao curso
	//! \return Inteiro, número de professores do curso
	int numProfessores() const;

protected:
	//! Número de disciplinas do curso
	int numDisciplinas_;
	//! Número de pré-requisitos do curso
	int numPreRequisitos;
	//! Número de co-requisitos do curso
	int numCoRequisitos;
	//! Número de horários do curso
	int numHorarios_;
	//! Número de disciplinas ofertadas no período
	int numOfertadas;
	//! Número de professores do curso
	int numProfessores_;
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
	//! Dias letivos por semana
	int numDiasLetivos_;
	//! Número de períodos simultâneos no curso
	int numPeriodos_;

	//! Gera os nomes dos horários sequencialmente (HOR1, HOR2, HOR3...)
	virtual void geraNomeHorarios();
};
