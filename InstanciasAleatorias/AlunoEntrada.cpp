#include "AlunoEntrada.h"

AlunoEntrada::AlunoEntrada(const std::vector<std::vector<bool>>& preRequisitos,
			 const std::vector<std::vector<bool>>& coRequisitos,
			 const std::string& nome)
	: Aluno(preRequisitos, coRequisitos, nome) {}

void AlunoEntrada::setAprovacoes(std::vector<bool> aprovacoes) {
	aprovacoes_ = std::move(aprovacoes);
}

void AlunoEntrada::setCursadas(std::vector<bool> cursadas) {
	cursadas_ = std::move(cursadas);
}