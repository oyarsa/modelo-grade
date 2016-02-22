#pragma once

#include <memory>
#include <vector>

#include "curso.h"
#include "aluno.h"
#include "solucao.h"
#include "solver.h"

namespace fagoc {

class Modelo_solver : public Solver
{
public:
	Modelo_solver(const Curso& curso, const Aluno& aluno, 
				  const std::vector<std::vector<char>>& horario);
	Modelo_solver(const Curso& curso, const Aluno& aluno);
	Modelo_solver(const Modelo_solver&) = delete;
	virtual ~Modelo_solver();

	void solve() override;
	const Solucao& solucao() const override;
private:
	struct impl;
	std::unique_ptr<impl> impl_;
};


}

