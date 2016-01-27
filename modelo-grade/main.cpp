#pragma warning(disable: 4996)

#include <iostream>
#include "arquivos.h"
#include "solucao.h"
#include "solver.h"
#include "modelo_solver.h"
#include <memory>

void print_solutions(const std::vector<std::shared_ptr<fagoc::Solucao>>& solucoes)
{
	for (const auto solucao : solucoes) {
		std::cout << solucao->nome_aluno << "\n";
		for (const auto& disc : solucao->nomes_disciplinas) {
			std::cout << disc << " ";
		}
		std::cout << solucao->funcao_objetivo << "\n\n";
	}
}

int main(int argc, char *argv[])
{
	std::string arquivo_entrada, caminho_destino;
	if (argc != 3) {
		arquivo_entrada = "input.json";
		caminho_destino = "saida/";
	} else {
		arquivo_entrada = argv[1];
		caminho_destino = argv[2];
	}

	auto ret = fagoc::ler_json(arquivo_entrada);
	auto curso = ret.first;
	auto alunos = ret.second;
	std::vector<std::shared_ptr<fagoc::Solucao>> solucoes(alunos.size());
	auto funcao_objetivo = fagoc::soluciona_alunos<fagoc::Modelo_solver>(curso, alunos, solucoes);
	print_solutions(solucoes);
	std::cout << "Resultado final: " << funcao_objetivo << "\n\n";
	fagoc::gen_html(curso, solucoes, caminho_destino);
	std::cout << "\n\n\n" << "HTMLs em: " << caminho_destino << "\n";
}