#pragma once

#include <string>
#include <vector>
#include "Curso.h"
#include "Aluno.h"
#include <memory>
#include "AlunoEntrada.h"
#include "CursoEntrada.h"

namespace manipulaJson {
	using AlunoPtr = std::unique_ptr<Aluno>;

	bool escreveJson(std::string caminho, Curso const* pCurso, std::vector<AlunoPtr>& alunos);

	//! Lê um arquivo no formato JSON e retorna um par de Curso e vetor de Alunos
	//! \param nomeArquivo String com o caminho para o arquivo a ser lido
	std::pair<CursoEntrada, std::vector<AlunoEntrada>> lerJson(std::string nomeArquivo);
}