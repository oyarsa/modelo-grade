#include "Aluno.h"

Aluno::Aluno(const std::vector<std::vector<bool>>& preRequisitos,
             const std::vector<std::vector<bool>>& coRequisitos,
             std::string nome, int periodo, std::string turma)
	: numDisciplinas(preRequisitos.size()),
	  numAprovacoes(),
	  numCursadas(),
	  preRequisitos_(preRequisitos),
	  coRequisitos_(coRequisitos),
	  nome_(nome),
	  aprovacoes_(numDisciplinas),
	  cursadas_(),
	  periodo_(periodo),
	  turma_(turma) {}

std::string Aluno::nome() const {
	return nome_;
}

int Aluno::periodo() const {
	return periodo_;
}

const std::vector<bool>& Aluno::aprovacoes() const {
	return aprovacoes_;
}

const std::vector<bool>& Aluno::cursadas() const {
	return cursadas_;
}

std::string Aluno::turma() const {
	return turma_;
}