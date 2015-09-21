#pragma once
#include "Aluno.h"

class AlunoEntrada : public Aluno {
public:
	AlunoEntrada(const std::vector<std::vector<bool>>& preRequisitos,
				 const std::vector<std::vector<bool>>& coRequisitos,
				 const std::string& nome);

	void setAprovacoes(std::vector<bool> aprovacoes);
	void setCursadas(std::vector<bool> cursadas);
	
};
