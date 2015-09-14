#pragma once

#include "Curso.h"
#include "AlunoAleatorio.h"

//! Gera aleatoriamente a estrutura de um curso de ensino superior, com horários, disciplinas etc
class CursoAleatorio : public Curso {
public:
	/*!
	* \brief Constroi um curso aleatório a partir de parâmetros arbitrários
	*
	* \param numDisciplinas_ Número total de disciplinas do curso
	* \param numPreRequisitos Soma total do número de pré-requisitos de cada matéria do curso
	* \param numCoRequisitos Soma total do número de co-requisitos de cada matéria do curso
	* \param numHorarios_ Número de horários (perído de 1 hora ou 50 minutos) em uma semana
	* \param numOfertadas Número de disciplinas ofertadas no período
	* \param numProfessores Número de professores do curso
	* \param maxMinistradas Número máximo de matérias que um professor ministra
	*/
	CursoAleatorio(int numDisciplinas, int numPreRequisitos,
	               int numCoRequisitos, int numHorarios,
	               int numOfertadas, int numProfessores,
	               int maxMinistradas, int numDiasLetivos,
				   int numPeriodos);
	//! Executa as subrotinas que geram os elementos aleatórios
	void init();
private:
	//! Gera aleatoriamente os créditos de cada matéria do curso
	void geraCreditos() override;
	//! Gera aleatoriamente os pré-requisitos de cada matéria do curso
	void geraPreRequisitos() override;
	//! Gera aleatoriamente os co-requisitos de cada matéria do curso
	void geraCoRequisitos() override;
	//! Gera aleatoriamente as matérias ofertadas no período
	void geraOfertadas() override;
	//! Gera sequencialmente os nomes das disciplinas do curso (DISC1, DISC2...)
	void geraNomeDisciplinas() override;
	//! Gera aleatoriamente os créditos, os pré-requisitos e os co-requisitos de cada disciplina do curso
	void geraDisciplinas() override;
	//! Gera aleatoriamente os horários do curso
	void geraHorario() override;
	//! Gera aleatoriamente os professores do curso
	void geraProfessores() override;
	//! Gera aleatoriamente o número de disciplinas que cada professor ministra
	void distribuiProfessores();
	//! Número máximo de disciplinas que um professor pode ministrar
	int maxMinistradas;
	//! Número máximo de professores que podem ministrar uma disciplina
	int maxProfessores;
	//! Número total de créditos oferecidos no período
	int creditosTotaisOfertados;
	//! Funções para geração de números aleatórios
	Aleatorio rand;
};
