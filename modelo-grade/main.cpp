#include <iostream>
#include <memory>
#include <chrono>

#include "arquivos.h"
#include "solucao.h"
#include "solver.h"
#include "modelo_solver.h"

void
print_solutions(const std::vector<fagoc::Solucao>& solucoes)
{
	for (const auto& solucao : solucoes) {
		std::cout << "Aluno " << solucao.nome_aluno << "\n";
		for (const auto& disc : solucao.nomes_disciplinas) {
			std::cout << disc << " ";
		}
		std::cout << solucao.funcao_objetivo << "\n\n";
	}
}

int main(int argc, char* argv[])
{
	std::string arquivo_entrada, caminho_destino;
	if (argc != 3) {
#if defined(_WIN32)
		caminho_destino = "..\\saida\\";
		arquivo_entrada = "..\\input\\input.json";
#else
		caminho_destino = "../saida/";
		arquivo_entrada = "../input/input.json";
#endif
	} else {
		arquivo_entrada = argv[1];
		caminho_destino = argv[2];
	}

	auto ret = fagoc::ler_json(arquivo_entrada);
	auto curso = ret.first;
	auto alunos = ret.second;
	std::vector<fagoc::Solucao> solucoes{};

	auto inicio = std::chrono::steady_clock::now();
	auto funcao_objetivo = fagoc::soluciona_alunos<fagoc::Modelo_solver>(
		curso, alunos, solucoes);
	auto fim = std::chrono::steady_clock::now();

	print_solutions(solucoes);
	std::cout << "Tempo: " << std::chrono::duration_cast<std::chrono::milliseconds>
		(fim - inicio).count() << "ms\n";
	std::cout << "Resultado final: " << funcao_objetivo << "\n\n";

	gen_html(curso, solucoes, caminho_destino);
	std::cout << "HTMLs em: " << caminho_destino << "\n\n";
}

