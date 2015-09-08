#include "Aluno.h"

Aluno::Aluno(const std::vector<std::vector<bool>>& preRequisitos,
							   const std::vector<std::vector<bool>>& coRequisitos,
							   std::string nome)
	: numDisciplinas(preRequisitos.size()),
	numAprovacoes(),
	numCursadas(),
	preRequisitos_(preRequisitos),
	coRequisitos_(coRequisitos),
	nome_(nome),
	aprovacoes_(numDisciplinas),
	cursadas_() {}

std::string Aluno::nome() const {
	return nome_;
}

const std::vector<bool>& Aluno::aprovacoes() const {
	return aprovacoes_;
}

const std::vector<bool>& Aluno::cursadas() const {
	return cursadas_;
}
