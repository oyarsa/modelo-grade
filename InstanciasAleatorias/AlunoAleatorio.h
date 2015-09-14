#pragma once

#include <vector>
#include "Aluno.h"
#include "aleatorio.h"

//! \brief Representa um aluno de um Curso, e tem suas apravoções e
//! disciplinas cursadas geradas aleatoriamente a partir da matriz
//! de pré-requisitos do curso.
class AlunoAleatorio : public Aluno {
public:
	/*!
	* \brief Recebe o nome do aluno e a matriz de pré-requisitos de seu curso
	* e partir dela gera aleatoriamente as matérias cursadas e aprovadas
	*
	* \param preRequisitos Referência const para uma matriz booleana, os pré-requisitos
	* do curso em que o aluno estuda
	* \param coRequisitos Referência const para uma matriz booleana, os co-requisitos
	* do curso em que o aluno estuda
	* \param nome Nome do aluno
	*/
	AlunoAleatorio(const std::vector<std::vector<bool>>& preRequisitos,
		  const std::vector<std::vector<bool>>& coRequisitos,
	      std::string nome);

	
private:
	//! Incializa os membros aleatórios
	void init();
	//! Gera aleatoriamente as disciplinas que o aluno cursou
	void geraCursadas() override;
	//! Gera aleatoriamente as disciplinas em que o aluno foi aprovado
	void geraAprovacoes() override;
	//! Funções para geração de números aleatórios
	Aleatorio rand;
};
