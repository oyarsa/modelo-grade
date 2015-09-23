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

	//! Recebe um ponteiro para um Curso e um vector de unique_ptrs para Alunos,
	//! e gera um arquivo Json a partir deles
	//! \param caminho Caminho (pasta + nome) do arquivo a ser escrito
	//! \param pCurso Ponteiro const para um Curso
	//! \param aluno Vector de AlunoPtrs contendo os alunos do curso
	//! \return Verdadeiro se a escrita foi bem sucedida
	bool escreveJson(std::string caminho, Curso const* pCurso, 
					 std::vector<AlunoPtr>& alunos);

	//! Lê um arquivo no formato JSON e retorna um par de Curso e vetor de Alunos.
	//! Lança uma exception std::runtime_error se o arquivo não for encontrado
	//! \param nomeArquivo String com o caminho para o arquivo a ser lido
	std::pair<CursoEntrada, std::vector<AlunoEntrada>> 
		lerJson(std::string nomeArquivo);
}