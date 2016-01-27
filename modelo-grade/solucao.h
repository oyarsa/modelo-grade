#pragma once
#include <string>
#include "curso.h"
#include "solver.h"

struct fagoc::Solver::Solucao
{
	Solucao(std::size_t n, double fo, const fagoc::Curso& curso, std::string nome)
		: funcao_objetivo{fo}, nomes_disciplinas{}, solucao_bool(n), curso(curso), nome_aluno(nome) {}

	double funcao_objetivo;
	std::vector<std::string> nomes_disciplinas;
	std::vector<char> solucao_bool;
	const fagoc::Curso& curso;
	std::string nome_aluno;
};
