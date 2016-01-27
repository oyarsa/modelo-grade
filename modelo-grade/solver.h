#pragma once
#include "curso.h"
#include "aluno.h"

#include <memory>
#include <ilcplex/ilocplexi.h>

namespace fagoc {

class Solver
{
public:
	struct Solucao;

	Solver(const Curso& curso, const Aluno& aluno);

	void solve();
	std::shared_ptr<Solucao> solucao() const;
private:
	std::shared_ptr<Solucao> solucao_ = nullptr;
	const Curso& curso_;
	const Aluno& aluno_;
	IloEnv env_;
};

double soluciona_alunos(Curso& curso, const std::vector<Aluno>& alunos, std::vector<std::shared_ptr<Solver::Solucao>>& solucoes);


}