#pragma once
#include <vector>
#include <string>

namespace fagoc {

struct Solucao
{
	Solucao() = default;
	Solucao(std::size_t n, double fo, std::string nome)
		: funcao_objetivo{fo}, solucao_bool(n), nome_aluno(nome) {}

	double funcao_objetivo;
	std::vector<std::string> nomes_disciplinas;
	std::vector<char> solucao_bool;
	std::string nome_aluno;
};

}
