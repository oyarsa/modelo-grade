#pragma once

#include <string>
#include <vector>

namespace fagoc {

class Aluno
{
public:
	Aluno(std::string nome,
	      std::vector<char>&& aprovacoes,
	      std::vector<char>&& cursadas,
	      std::string periodo,
	      std::string turma);

	std::string nome() const;
	std::string turma() const;
	std::string periodo() const;
	const std::vector<char>& aprovacoes() const;
	const std::vector<char>& cursadas() const;
private:
	std::string nome_;
	std::vector<char> aprovacoes_;
	std::vector<char> cursadas_;
	std::string periodo_;
	std::string turma_;
};

}

