#pragma once
#include "curso.h"
#include "aluno.h"
#include "solucao.h"
#include "solver.h"

#include <memory>
#include <ilcplex/ilocplexi.h>

namespace fagoc {

class Modelo_solver : public Solver
{
public:
	Modelo_solver(const Curso& curso, const Aluno& aluno);

	void solve() override;
	std::shared_ptr<Solucao> solucao() const override;
private:
	IloEnv env_;
};



}