#pragma once

#include "Curso.h"
#include "AlunoAleatorio.h"

//! Gera aleatoriamente a estrutura de um curso de ensino superior, com horários, disciplinas etc
class CursoAleatorio : public Curso {
public:
	//! \brief Constroi um curso aleatório a partir de parâmetros arbitrários
	//!
	//! \param numDisciplinas_ Número total de disciplinas do curso
	//! \param numPreRequisitos Soma total do número de pré-requisitos de cada matéria do curso
	//! \param numCoRequisitos Soma total do número de co-requisitos de cada matéria do curso
	//! \param numHorarios_ Número de horários (perído de 1 hora ou 50 minutos) em uma semana
	//! \param numOfertadas Número de disciplinas ofertadas no período
	//! \param numProfessores_ Número de professores do curso
	//! \param maxMinistradas Número máximo de matérias que um professor ministra
	//! \param numDiasLetivos Número de dias letivos por semana
	//! \param numPeriodos Número de períodos simultaneamente oferecidos pelo curso
	CursoAleatorio(int numDisciplinas, int numPreRequisitos,
	               int numCoRequisitos, int numHorarios,
	               int numOfertadas, int numProfessores,
	               int maxMinistradas, int numDiasLetivos,
				   int numPeriodos, int numTurmas, int capMinima,
				   int capMaxima);
	//! Executa as subrotinas que geram os elementos aleatórios
	void init();
private:
	//! Gera aleatoriamente os créditos de cada matéria do curso
	void geraCreditos();
	//! Gera aleatoriamente os pré-requisitos de cada matéria do curso
	void geraPreRequisitos();
	//! Gera aleatoriamente os co-requisitos de cada matéria do curso
	void geraCoRequisitos();
	//! Gera aleatoriamente as matérias ofertadas no período
	void geraOfertadas();
	//! Gera sequencialmente os nomes das disciplinas do curso (DISC1, DISC2...)
	void geraNomeDisciplinas();
	//! Gera aleatoriamente os créditos, os pré-requisitos e os co-requisitos de cada disciplina do curso
	void geraDisciplinas();
	//! Gera aleatoriamente os horários do curso
	void geraHorario();
	//! Gera aleatoriamente os professores do curso
	void geraProfessores();
	bool isEquivalente(int disc1, int disc2) const;
	//! Gera aleatoriamente o número de disciplinas que cada professor ministra
	void distribuiProfessores();
	//! Gera a matriz de equivalências das disciplinas
	void geraEquivalencia();
	//! Gera a capacidade das disciplinas
	void geraCapacidades();
	//! Gera disciplinas equivalentes a diferentes turmas para uma mesma disciplina base
	void geraTurmas();
	//! Configura os pré- e co-requisitos para as outras turmas das disciplinas
	void finalizaDependencias();

	//! Número máximo de disciplinas que um professor pode ministrar
	int maxMinistradas;
	//! Número máximo de professores que podem ministrar uma disciplina
	int maxProfessores;
	//! Número total de créditos oferecidos no período
	int creditosTotaisOfertados;
	//! Funções para geração de números aleatórios
	Aleatorio rand;
	//! Capacidade máxima de uma aula do curso
	int capacidadeMaxima_;
	//! Quantidade mínima de alunos em uma aula
	int capacidadeMinima_;
};
