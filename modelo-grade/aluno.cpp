#include "aluno.h"

fagoc::Aluno::Aluno(std::string nome, 
					std::vector<char>&& aprovacoes, 
					std::vector<char>&& cursadas, 
					std::string periodo, 
					std::string turma)
	: nome_(nome)
	, aprovacoes_(std::move(aprovacoes))
	, cursadas_(std::move(cursadas))
	, periodo_(periodo)
	, turma_(turma) {}

std::string fagoc::Aluno::nome() const
{
	return nome_;
}

std::string fagoc::Aluno::turma() const
{
	return turma_;
}

std::string fagoc::Aluno::periodo() const
{
	return periodo_;
}

const std::vector<char>& fagoc::Aluno::aprovacoes() const
{
	return aprovacoes_;
}

const std::vector<char>& fagoc::Aluno::cursadas() const
{
	return aprovacoes_;
}
