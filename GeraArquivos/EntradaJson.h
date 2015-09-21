#pragma once
#include <utility>
#include <CursoEntrada.h>
#include <AlunoEntrada.h>
#include <vector>

namespace EntradaJson {
	std::pair<CursoEntrada, std::vector<AlunoEntrada>> lerJson(std::string nomeArquivo);
}