#pragma once
#include <utility>
#include <CursoEntrada.h>
#include <AlunoEntrada.h>
#include <vector>

namespace EntradaJson {
	//! Lê um arquivo no formato JSON e retorna um par de Curso e vetor de Alunos
	//! \param nomeArquivo String com o caminho para o arquivo a ser lido
	std::pair<CursoEntrada, std::vector<AlunoEntrada>> lerJson(std::string nomeArquivo);
}