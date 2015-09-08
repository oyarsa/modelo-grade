#pragma once

#include "Curso.h"
#include "Aluno.h"
#include <memory>

class Instancia {
	//! Aliases para os smartpointers usados pela classe
	using CursoPtr = std::unique_ptr<Curso>;
	using AlunoPtr = std::unique_ptr<Aluno>;
public:
	//! Construtor que recebe um ponteiro para um Curso, que será usado com parte
	//! principal da geração das instâncias
	explicit Instancia(CursoPtr curso);
	//! Gera um novo aluno, a partir um nome (padrão é vazio)
	//! \param nome Nome do aluno
	void novoAluno(std::string nome = "");
	//! Grava o aluno em um arquivo, cujo autor é especificado
	//! no argumento (padrão é vazio)
	//! \param autor Autor do arquivo
	bool gravaAluno(std::string autor = "") const;
	//! Retorna um ponteiro const para o aluno atualmente apontado por AlunoPtr
	Aluno* aluno() const;
	//! Retorna um ponteiro const para o curso atualmente apontado por CursoPtr
	Curso* curso() const;
private:
	//! Ponteiro para um curso (smart pointer - unique_ptr)
	CursoPtr curso_;
	//! Ponteiro para um aluno (smart pointer - unique_ptr)
	AlunoPtr aluno_;

	//! TODO: Função que retorna referências para os membros internos do Curso e do ALuno
};