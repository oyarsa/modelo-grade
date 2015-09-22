#pragma once

#include <string>
#include <vector>
#include "Curso.h"
#include "Aluno.h"
#include <memory>

namespace geraJson {
	using AlunoPtr = std::unique_ptr<Aluno>;

	bool escreveJson(std::string caminho, Curso const* pCurso, std::vector<AlunoPtr>& alunos);
}