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
#include <iterator>

using CursoPtr = std::unique_ptr<Curso>;
using AlunoPtr = std::unique_ptr<Aluno>;

double funcaoObjetivo(CursoPtr pCurso, std::vector<AlunoPtr>& alunos) {
	double total = 0;

	for (auto i = 0; i < alunos.size(); i++) {
		SolverHandler solver{pCurso.get(), std::move(alunos[i])};
		solver.solve();
		total += solver.valorFinal();
		const auto& nomeDisciplinas = solver.disciplinas();

		/*std::cout << solver.aluno()->nome() << ":\n";
		copy(begin(nomeDisciplinas), end(nomeDisciplinas),
			 std::ostream_iterator<std::string>(std::cout, " "));
		std::cout << "  " << solver.valorFinal() << "\n";*/
	}

	return total;
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

		pCurso->atualiza(solver.solucao());
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
			curso->preRequisitos(), curso->coRequisitos(), aln,
		    curso->numPeriodos(), curso->numTurmas()}));
	}

	return alunos;
}

void entradaPadrao() {
	std::cout << "Lendo Json...\n";
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
	} else {
		std::cout << "Modelos resolvidos com sucesso.\n\n";
	}

	// Imprime o tempo gasto
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diferenca = end - begin;
	std::cout << "Tempo total: " << diferenca.count() << "s\n\n";
}

double entradaArgumentos(std::string arquivoEntrada) {
	auto entrada = manipulaJson::lerJson(arquivoEntrada);
	auto pCurso = std::unique_ptr<Curso>{new CursoEntrada(std::move(entrada.first))};

	std::vector<AlunoPtr> alunos;
	for (auto& aluno : entrada.second) {
		alunos.push_back(std::move(std::unique_ptr<Aluno>{new AlunoEntrada(std::move(aluno))}));
	}

	return funcaoObjetivo(std::move(pCurso), alunos);
}

int main(int argc, char** argv) {
	std::ios_base::sync_with_stdio(false);

	if (argc == 2) {
		std::cout << entradaArgumentos(argv[1]) << "\n";
	} else {
		entradaPadrao();
	}
}
