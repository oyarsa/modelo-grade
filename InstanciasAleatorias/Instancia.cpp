﻿#include "Instancia.h"
#include <GeraArquivos.h>
#include "Aluno.h"
#include "AlunoAleatorio.h"

//! Inicializa a classe recebendo um rvalue de um unique_ptr para o curso, e
//! o aluno apontando para o nulo
//! \param curso unique_ptr<Curso>, passado por rvalue
Instancia::Instancia(CursoPtr curso) : curso_(std::move(curso)), aluno_(nullptr) {}

Instancia::Instancia(Instancia&& outro) {
	curso_ = move(outro.curso_);
	aluno_ = move(outro.aluno_);
}

void Instancia::novoAlunoAleatorio(std::string nome) {
	//! Cria um novo objeto da classe AlunoAleatorio dentro do unique_ptr,
	//! apontando sua matriz de pré-requisitos para a do curso
	aluno_ = AlunoPtr{new AlunoAleatorio(curso_->preRequisitos(),
									     curso_->coRequisitos(), nome,
										 curso_->numPeriodos(),
										 curso_->numTurmas())};
}

void Instancia::novoAluno(AlunoPtr aluno) {
	aluno_ = move(aluno);
}

bool Instancia::gravaAluno(std::string autor) const {
	//! Chama a função geraArquivoCplex para criar um arquivo .dat do CPLEX a partir
	//! das estruturas dos objetos aluno_ e curso_
	return geraArquivo::geraArquivoCplex(aluno_->nome(), autor, curso_->nomeDisciplinas(), curso_->creditos(),
	                                     curso_->ofertadas(), curso_->preRequisitos(), curso_->coRequisitos(),
	                                     curso_->nomeHorarios(), curso_->horarios(),
	                                     aluno_->aprovacoes(), aluno_->cursadas());
}

Aluno* Instancia::aluno() const {
	return aluno_.get();
}

Curso* Instancia::curso() const {
	return curso_.get();
}