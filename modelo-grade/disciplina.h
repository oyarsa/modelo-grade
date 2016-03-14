#pragma once
#include <vector>

namespace fagoc {
	
struct Disciplina
{
	explicit Disciplina(std::size_t n)
		: credito{}
		, prerequisitos(n, 0)
		, corequisitos(n, 0)
		, equivalentes(n, 0)
		, capacidade{}
	{}

	int credito;
	std::vector<char> prerequisitos;
	std::vector<char> corequisitos;
	std::vector<char> equivalentes;
	std::string turma;
	std::string periodo;
	std::string periodo_minimo;
	std::string id;
	std::string nome;
	int capacidade;
};

}
