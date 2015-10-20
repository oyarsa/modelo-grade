#include "CursoAleatorio.h"
#include "CompFagoc.h"
#include "Instancia.h"
#include <iostream>
#include <tuple>
#include <windows.h>
#include <memory>
#include <iterator>
#include <string>

using CursoPtr = std::unique_ptr<Curso>;

std::tuple<CursoPtr, std::string, std::string, int> menu() {
	int numDisciplinas, numPreRequisitos, numCoRequisitos, numHorarios,
	    numOfertadas, numAlunos, numProfessores, maxMinistradas,
	    numPeriodos, numDiasLetivos, numTurmas, capacidadeMinima, capacidadeMaxima;
	int opcao;
	std::string pasta, engine;
	CursoPtr curso;

	std::cout << "Selecione a opcao desejada:\n\n";
	std::cout << "1 - Valores padrao\n";
	std::cout << "2 - Valores customizados\n";
	std::cout << "3 - Modelo FAGOC\n";
	std::cout << "\nOpcao: ";
	
	while (true) {
		std::cin >> opcao;
		switch (opcao) {
			case 1:
				numDisciplinas = 50;
				numPreRequisitos = 10;
				numCoRequisitos = 5;
				numHorarios = 20;
				numOfertadas = numDisciplinas / 2;
				numProfessores = 20;
				maxMinistradas = 10;
				numPeriodos = 4;
				numDiasLetivos = 5;
				numTurmas = 2;
				capacidadeMaxima = 4000;
				capacidadeMinima = 40;

				std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
				std::cin >> pasta;
				std::cout << "Digite o número de alunos: ";
				std::cin >> numAlunos;

				engine = numDisciplinas <= 15 ? "CPLEX" : "CP";

				curso = CursoPtr{new CursoAleatorio(numDisciplinas, numPreRequisitos,
													numCoRequisitos, numHorarios,
													numOfertadas, numProfessores,
													maxMinistradas, numDiasLetivos,
													numPeriodos, numTurmas, capacidadeMinima,
													capacidadeMaxima)};
				return make_tuple(std::move(curso), pasta, engine, numAlunos);

			case 2:
				std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
				std::cin >> pasta;
				std::cout << "\nNumero de disciplinas: ";
				std::cin >> numDisciplinas;
				std::cout << "Numero de pre-requisitos: ";
				std::cin >> numPreRequisitos;
				std::cout << "Numero de co-requisitos: ";
				std::cin >> numCoRequisitos;
				std::cout << "Numero de horarios: ";
				std::cin >> numHorarios;
				std::cout << "Numero de disciplinas ofertadas no perido: ";
				std::cin >> numOfertadas;
				std::cout << "Numero de alunos: ";
				std::cin >> numAlunos;
				std::cout << "Numero de professores: ";
				std::cin >> numProfessores;
				std::cout << "Numero maximo de disciplinas que um professor pode ministrar: ";
				std::cin >> maxMinistradas;
				std::cout << "Numero de dias letivos por semana: ";
				std::cin >> numDiasLetivos;
				std::cout << "Numero de peridos simultaneamente ofertados: ";
				std::cin >> numPeriodos;
				std::cout << "Numero maximo de turmas por disciplina: ";
				std::cin >> numTurmas;
				std::cout << "Capacidade minima de uma turma: ";
				std::cin >> capacidadeMinima;
				std::cout << "Capacidade maxima de uma turma: ";
				std::cin >> capacidadeMaxima;

				if (numDiasLetivos > 7)
					numDiasLetivos = 7;
				if (numTurmas > 26)
					numTurmas = 26;

				engine = numDisciplinas <= 15 ? "CPLEX" : "CP";

				curso = CursoPtr{new CursoAleatorio(numDisciplinas, numPreRequisitos,
													numCoRequisitos, numHorarios,
													numOfertadas, numProfessores,
													maxMinistradas, numDiasLetivos,
													numPeriodos, numTurmas, capacidadeMinima,
													capacidadeMaxima)};

				return make_tuple(std::move(curso), pasta, engine, numAlunos);

			case 3:
				std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
				std::cin >> pasta;
				std::cout << "Numero de alunos: ";
				std::cin >> numAlunos;

				engine = "CP";

				curso = CursoPtr{new CompFagoc()};

				return make_tuple(move(curso), pasta, engine, numAlunos);

			default:
				std::cout << "\nOpcao invalida. Tente de novo: ";
		}
	}
}

bool moveArquivos(std::string caminho, std::string engine) {
	if (!CreateDirectory(caminho.c_str(), nullptr))
		return false;

	auto comandoOpl = "cd etc && oplrun -o grade.opl grade" + engine + ".mod > nul";

	system(comandoOpl.c_str());

	if (CopyFile("etc//solve.py", std::string(caminho + "solve.py").c_str(), TRUE) &&
		CopyFile("etc//grade.opl", std::string(caminho + "\\grade.opl").c_str(), TRUE)) {
		system("cd etc && del grade.opl");
		return true;
	}
	
	return false;
}

bool geraAlunos(std::string destino, CursoPtr curso, int numAlunos) {
	Instancia inst(move(curso));

	std::string ext = ".dat";
	std::string nome = "aln";
	for (auto i = 0; i < numAlunos; i++) {
		auto aln = destino + nome;
		inst.novoAlunoAleatorio(aln + std::to_string(i + 1) + ext);
		if (!inst.gravaAluno("Italo"))
			return false;
	}
	return true;
}

int main() {
	//! Desliga a sincronização entre as streams do C++ e a stdio do C
	std::ios_base::sync_with_stdio(false);

	auto opcoes = menu();
	auto curso = std::get<0>(move(opcoes));

	auto& professores = curso->professores();
	std::cout << "Lista de professores:\n";
	for (auto professor : professores) {
		std::cout << professor.nome() << ": ";
		auto& ministradas = professor.disciplinasMinistradas();
		copy(begin(ministradas), end(ministradas),
		          std::ostream_iterator<int>(std::cout, " "));
		std::cout << "\n";
	}

	auto pasta = std::get<1>(opcoes);
	auto engine = std::get<2>(opcoes);
	auto numAlunos = std::get<3>(opcoes);

	std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	auto caminho = dir + pasta + "\\";

	if (moveArquivos(caminho, engine))
		std::cout << "\nArquivos movidos com sucesso.\n";
	else {
		std::cout << "\nFalha ao mover os arquivos. Tente novamente.\n";
		exit(1);
	}

	if (geraAlunos(caminho, std::move(curso), numAlunos))
		std::cout << "\nAlunos gerados com sucesso.\n";
	else {
		std::cout << "\nFalha ao gerar os alunos. Tente novamente.\n";
		exit(1);
	}
}
