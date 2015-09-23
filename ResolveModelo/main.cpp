#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <Instancia.h>
#include <CursoAleatorio.h>
#include <string>
#include <tuple>
#include <CompFagoc.h>
#include <windows.h>
#include <AlunoAleatorio.h>
#include "SolverHandler.h"
#include <GeraArquivos.h>
#include <ManipulaJson.h>

using CursoPtr = std::unique_ptr<Curso>;
using AlunoPtr = std::unique_ptr<Aluno>;

//! Exibe um menu e retorna as opções selecionadas para o main
//! \return Uma triple de CursoPtr, string e int. Respectivamente, um curso,
//! o nome da pasta de destino e o número de aluno
std::tuple<CursoPtr, std::string, int> menu() {
	// Inicializa as variáveis com o padrão
	int numDisciplinas, numPreRequisitos, numCoRequisitos, numHorarios,
	    numOfertadas, numAlunos, numProfessores, maxMinistradas,
	    numDiasLetivos, numPeriodos;
	int opcao;
	std::string pasta;
	CursoPtr pCurso;

	// Exibe as opções do menu
	std::cout << "\nSOLUCIONADOR DE INSTANCIAS DO MODELO DE GRADE - PROG. INTEIRA\n";
	std::cout << std::string(63, '*') << "\n\n";
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

			std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			std::cin >> pasta;
			std::cout << "Digite o numero de alunos: ";
			std::cin >> numAlunos;


			pCurso = std::unique_ptr<Curso>{new CursoAleatorio(numDisciplinas, numPreRequisitos,
			                                                   numCoRequisitos, numHorarios,
			                                                   numOfertadas, numProfessores,
			                                                   maxMinistradas, numDiasLetivos,
															   numPeriodos)};
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

			if (numDiasLetivos > 7)
				numDiasLetivos = 7;

			pCurso = std::unique_ptr<Curso>{new CursoAleatorio(numDisciplinas, numPreRequisitos,
			                                                   numCoRequisitos, numHorarios,
			                                                   numOfertadas, numProfessores,
			                                                   maxMinistradas, numDiasLetivos,
															   numPeriodos)};

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


bool resolveAlunos(std::string dir, std::string pasta, CursoPtr pCurso, 
				   std::vector<AlunoPtr>& alunos) {
	// Cria o diretório, e encerra a função se for malsucedido
	auto caminho = dir + pasta;
	if (!CreateDirectory(caminho.c_str(), nullptr)) {
		caminho = ".\\" + pasta;
		if (!CreateDirectory(caminho.c_str(), nullptr))
			return false;
	}

	// Nome padrão do aluno
	std::string nome = "aln";
	// Streams de saída para o txt que irá armazenar os dados resumidos
	std::ostringstream saidaTxt{};
	// E para o html que será a compilação de todos os alunos
	std::ostringstream saidaHtml{};
	// Desabilita flushing automático da saída
	saidaTxt << std::nounitbuf;
	saidaHtml << std::nounitbuf;

	// Carrega o arquivo CSS do HTML
	geraArquivo::escolheCSS("etc\\estilos.css");

	for (auto i = 0; i < alunos.size(); i++) {
		SolverHandler solver{pCurso.get(), std::move(alunos[i])};

		// Resolve o problema e imprime os dados nos arquivos
		solver.solve();
		saidaTxt << solver.aluno()->nome() << "\n";
		const auto& nomeDisciplinas = solver.disciplinas();
		const auto& aprovacoesAluno = solver.aluno()->aprovacoes();
		const auto& cursadasAluno = solver.aluno()->cursadas();

		// A cada iteração os resultados são gravados no TXT
		saidaTxt << "Aprovações:\n";
		for (auto j = 0; j < aprovacoesAluno.size(); j++) {
			if (aprovacoesAluno[j]) {
				saidaTxt << pCurso->nomeDisciplinas()[j] << " ";
			}
		}
		saidaTxt << "\n";
		saidaTxt << "Cursadas:\n";
		for (auto j = 0; j < cursadasAluno.size(); j++) {
			if (cursadasAluno[j]) {
				saidaTxt << pCurso->nomeDisciplinas()[j] << " ";
			}
		}
		saidaTxt << "\nDisciplinas escolhidas:\n";
		copy(begin(nomeDisciplinas), end(nomeDisciplinas),
		     std::ostream_iterator<std::string>(saidaTxt, " "));
		saidaTxt << "\nResultado final: " << solver.valorFinal() << "\n\n";

		// Escreve o horário do aluno num HTML separado, mas também escreve no buffer
		saidaHtml << geraArquivo::escreveHTML(pCurso->horarios(), pCurso->nomeDisciplinas(), 
											  solver.solucao(), caminho, solver.aluno()->nome(), 
											  pCurso->numDisciplinas(), pCurso->numHorarios(), 
											  pCurso->numPeriodos(), pCurso->numDiasLetivos())
				<< "\n\n";
	}

	// Cria as streams de escrita dos arquivo
	std::ofstream arquivoSaidaTxt(caminho + "\\" + "resultado.txt");
	std::ofstream arquivoSaidaHtml(caminho + "\\" + "resultado.html");
	arquivoSaidaTxt << std::nounitbuf << saidaTxt.str() << std::endl;
	arquivoSaidaHtml << std::nounitbuf << saidaHtml.str() << std::endl;

	return true;
}

std::vector<AlunoAleatorio> geraAlunosAleatorios(Curso const* curso, int numAlunos) {
	std::vector<AlunoAleatorio> alunos;
	for (auto i = 0; i < numAlunos; i++) {
		auto aln = "aln" + std::to_string(i + 1);
		alunos.push_back(std::move(AlunoAleatorio{
			curso->preRequisitos(), curso->coRequisitos(), aln}));
	}

	return alunos;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	//// Pega as opções retornadas pelo menu
	//auto opcoes = menu();
	//// Ponteiro para um Curso
	//auto pCurso = std::get<0>(move(opcoes));
	//// Pasta de destino
	//auto pasta = std::get<1>(opcoes);
	//// Número de alunos a serem gerados
	//auto numAlunos = std::get<2>(opcoes);
	//// Vector com os alunos gerados aleatoriamente
	//auto alunosAleatorios = geraAlunosAleatorios(pCurso.get(), numAlunos);
	//std::vector<AlunoPtr> alunos;
	//for (auto& aluno : alunosAleatorios) {
	//	alunos.push_back(std::move(std::unique_ptr<Aluno>{new AlunoAleatorio(std::move(aluno))}));
	//}
	
	auto entrada = manipulaJson::lerJson("../input.json");
	auto pCurso = std::unique_ptr<Curso>{new CursoEntrada(std::move(entrada.first))};

	std::vector<AlunoPtr> alunos;
	for (auto& aluno : entrada.second) {
		alunos.push_back(std::move(std::unique_ptr<Aluno>{new AlunoEntrada(std::move(aluno))}));
	}
		
	std::string pasta;
	std::cout << "Digite o nome da pasta: ";
	std::cin >> pasta;

	std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";

	manipulaJson::escreveJson(dir + "out.json", pCurso.get(), alunos);

	// Inicialzia relógio e resolve todos os modelos
	auto begin = std::chrono::system_clock::now();
	std::cout << "\n";
	if (!resolveAlunos(dir, pasta, std::move(pCurso), alunos)) {
		std::cout << "Nao foi possivel criar a pasta.\n\n";
	}
	else {
		std::cout << "Modelos resolvidos com sucesso.\n\n";
	}

	// Imprime o tempo gasto
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diferenca = end - begin;
	std::cout << "Tempo total: " << diferenca.count() << "s\n\n";
}
