#pragma once
#include <tuple>
#include <CursoEntrada.h>
#include <AlunoEntrada.h>
#include <vector>

namespace EntradaJson {
	std::tuple<CursoEntrada, std::vector<AlunoEntrada>> lerJson(std::string nomeArquivo);
}