#include "AlunoEntrada.h"
#include <numeric>

AlunoEntrada::AlunoEntrada(const std::vector<std::vector<bool>>& preRequisitos,
                           const std::vector<std::vector<bool>>& coRequisitos,
                           const std::string& nome)
	: Aluno(preRequisitos, coRequisitos, nome) {}

AlunoEntrada::AlunoEntrada(AlunoEntrada&& outro)
	: Aluno(outro.preRequisitos_,
	        outro.coRequisitos_,
	        outro.nome_) {
	numDisciplinas = outro.numDisciplinas;
	numAprovacoes = outro.numAprovacoes;
	numCursadas = outro.numCursadas;
	aprovacoes_ = move(outro.aprovacoes_);
	cursadas_ = move(outro.cursadas_);
}

void AlunoEntrada::setAprovacoes(std::vector<bool>&& aprovacoes) {
	aprovacoes_ = std::move(aprovacoes);
	numAprovacoes = std::accumulate(begin(aprovacoes_), end(aprovacoes_), 0);
}

void AlunoEntrada::setCursadas(std::vector<bool>&& cursadas) {
	cursadas_ = std::move(cursadas);
	numCursadas = std::accumulate(begin(cursadas_), end(cursadas_), 0);
}
