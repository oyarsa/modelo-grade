#include "CursoAleatorio.h"
#include "CompFagoc.h"
#include "Instancia.h"
#include <iostream>
#include <tuple>
#include <windows.h>
#include <memory>
#include <string>
#include <ManipulaJson.h>

using CursoPtr = std::unique_ptr<Curso>;
using AlunoPtr = std::unique_ptr<Aluno>;

//! Exibe um menu e retorna as opções selecionadas para o main
//! \return Uma triple de CursoPtr, string e int. Respectivamente, um curso,
//! o nome da pasta de destino e o número de aluno
std::tuple<CursoPtr, std::string, int> menu() {
	int numDisciplinas, numPreRequisitos, numCoRequisitos, numHorarios,
	    numOfertadas, numAlunos, numProfessores, maxMinistradas,
	    numDiasLetivos, numPeriodos, numTurmas, capacidadeMaxima, capacidadeMinima;
	int opcao;
	std::string pasta;
	CursoPtr pCurso;

	// Exibe as opções do menu
	std::cout << "\n\tGERADOR DE INSTNACIAS ALEATORIAS EM JSON\n";
	std::cout << std::string(65, '*') << "\n\n";
	std::cout << "Selecione a opcao desejada:\n\n";
	std::cout << "1 - Valores aleatorios padrao\n";
	std::cout << "2 - Valores aleatorios customizados\n";
	std::cout << "3 - Modelo FAGOC\n";
	std::cout << "\nOpcao: ";

	while (true) {
		std::cin >> opcao;
		switch (opcao) {
			// Se a opção for a padrão, utiliza esses valores e gera um curso aleatório
		case 1:
			numDisciplinas = 50;
			numPreRequisitos = 10;
			numCoRequisitos = 5;
			numHorarios = 20;
			numOfertadas = 20;
			numProfessores = 20;
			maxMinistradas = 10;
			numDiasLetivos = 5;
			numPeriodos = 4;
			numTurmas = 2;
			capacidadeMaxima = 4000;
			capacidadeMinima = 40;

			std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			std::cin >> pasta;
			std::cout << "Digite o numero de alunos: ";
			std::cin >> numAlunos;


			pCurso = std::unique_ptr<Curso>{new CursoAleatorio(numDisciplinas, numPreRequisitos,
			                                                   numCoRequisitos, numHorarios,
			                                                   numOfertadas, numProfessores,
			                                                   maxMinistradas, numDiasLetivos,
			                                                   numPeriodos, numTurmas, capacidadeMinima,
															   capacidadeMaxima)};
			return make_tuple(move(pCurso), pasta, numAlunos);

			// Caso seja desejado fornecer os valores, capitura-os e cria o curso aleatório
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

			pCurso = std::unique_ptr<Curso>{new CursoAleatorio(numDisciplinas, numPreRequisitos,
															   numCoRequisitos, numHorarios,
															   numOfertadas, numProfessores,
															   maxMinistradas, numDiasLetivos,
															   numPeriodos, numTurmas, capacidadeMinima,
															   capacidadeMaxima)};

			return make_tuple(move(pCurso), pasta, numAlunos);

			// Se for escolhido o modelo da fagoc, pede-se apenas o número de alunos
		case 3:
			std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			std::cin >> pasta;
			std::cout << "Numero de alunos: ";
			std::cin >> numAlunos;

			pCurso = std::unique_ptr<Curso>{new CompFagoc{}};

			return make_tuple(move(pCurso), pasta, numAlunos);

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

bool escreveArquivoJson(std::string destino, Curso const* pCurso, std::vector<AlunoPtr>& alunos) {
	if (!CreateDirectory(destino.c_str(), nullptr))
		return false;

	return manipulaJson::escreveJson(destino + "out.json", pCurso, alunos);
}

std::vector<AlunoAleatorio> geraAlunosAleatorios(Curso const* curso, int numAlunos) {
	std::vector<AlunoAleatorio> alunos;
	for (auto i = 0; i < numAlunos; i++) {
		auto aln = "aln" + std::to_string(i + 1);
		alunos.push_back(std::move(AlunoAleatorio{
			curso->preRequisitos(), curso->coRequisitos(), aln, curso->numPeriodos(),
		    curso->numTurmas()}));
	}

	return alunos;
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
	auto pCurso = std::get<0>(move(opcoes));
	auto pasta = std::get<1>(opcoes);
	auto numAlunos = std::get<2>(opcoes);

	// Vector com os alunos gerados aleatoriamente
	auto alunosAleatorios = geraAlunosAleatorios(pCurso.get(), numAlunos);
	std::vector<AlunoPtr> alunos;
	for (auto& aluno : alunosAleatorios) {
		alunos.push_back(std::move(std::unique_ptr<Aluno>{new AlunoAleatorio(std::move(aluno))}));
	}

	std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	auto caminho = dir + pasta + "\\";

	std::cout << "\n";
	if (escreveArquivoJson(caminho, pCurso.get(), alunos)) {
		std::cout << "Arquivo criado com sucesso.\n";
	}
	else {
		std::cout << "Falha ao criar os arquivos.\n";
	}

	//if (moveArquivos(caminho, engine))
	//	std::cout << "\nArquivos movidos com sucesso.\n";
	//else {
	//	std::cout << "\nFalha ao mover os arquivos. Tente novamente.\n";
	//	exit(1);
	//}

	//if (geraAlunos(caminho, std::move(curso), numAlunos))
	//	std::cout << "\nAlunos gerados com sucesso.\n";
	//else {
	//	std::cout << "\nFalha ao gerar os alunos. Tente novamente.\n";
	//	exit(1);
	//}
}
