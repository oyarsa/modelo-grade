#pragma once

#include "Curso.h"


//! Representação do Curso de Ciências da Computação da FAGOC no mesmo modelo dos cursos gerados aleatoriamente
class CompFagoc : public Curso {
public:
	//! Executa o construtor da superclasse Curso com os valores de Ciências da Computação da FAGOC 
	CompFagoc();
	//! Executa as subrotinas que preenchem os membros da classe
	void init();

private:
	//! Gera um vetor de créditos de disciplinas a partir do curso de Ciências da Computação da FAGOC
	void geraCreditos() override;
	//! Gera uma matriz de pré-requisitos das disciplinas a partir do curso de Ciências da Computação da FAGOC
	void geraPreRequisitos() override;
	//! Gera uma matriz de co-requisitos das disciplinas a partir do curso de Ciências da Computação da FAGOC
	void geraCoRequisitos() override;
	//! Gera um vetor das disciplinas ofertadas no périodo a partir do curso de Ciências da Computação da FAGOC
	void geraOfertadas() override;
	//! Gera um vetor com os nomes das disciplinas do curso de Ciências da Computação da FAGOC
	void geraNomeDisciplinas() override;
	//! Gera os créditos, os pré-requisitos e os co-requisitos de cada disciplina do curso de Ciências da Computação da FAGOC
	void geraDisciplinas() override;
	//! Gera uma matriz de horários a partir do curso de Ciências da Computação da FAGOC
	void geraHorario() override;
	//! Gera um vetor de professores a partir do curso de Ciências da Computação da FAGOC
	void geraProfessores() override;
};
