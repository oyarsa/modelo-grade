#include <iostream>
#include <vector>
#include <Instancia.h>
#include <tuple>
#include <string>
#include <CursoAleatorio.h>
#include <CompFagoc.h>
#include <GeraArquivos.h>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <numeric>
#include <cstdio>

std::vector<bool> convert(unsigned long long x) {
	std::vector<bool> ret;
	while (x) {
		if (x & 1)
			ret.push_back(true);
		else
			ret.push_back(false);
		x >>= 1;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

std::vector<std::vector<bool>> geraSolucoes(int numDisc) {
	std::vector<std::vector<bool>> out{};
	auto numCombinacoes = pow(2, unsigned long long(numDisc));

	for (unsigned long long i = 0; i < numCombinacoes; i++) {
		out.push_back(convert(i));
		std::cout << i << "\n";
	}

	return out;
}

using CursoPtr = std::unique_ptr<Curso>;

//! Exibe um menu e retorna as op��es selecionadas para o main
//! \return Uma triple de CursoPtr, string e int. Respectivamente, um curso,
//! o nome da pasta de destino e o n�mero de aluno
std::tuple<CursoPtr, std::string, int> menu() {
	// Inicializa as vari�veis com o padr�o
	int numDisciplinas, numPreRequisitos, numCoRequisitos, numHorarios,
	    numOfertadas, numAlunos, numProfessores, maxMinistradas,
	    numDiasLetivos, numPeriodos;
	int opcao;
	std::string pasta;
	CursoPtr pCurso;

	// Exibe as op��es do menu
	std::cout << "\nSOLUCIONADOR DE INSTANCIAS DO MODELO DE GRADE\n";
	std::cout << std::string(60, '*') << "\n\n";
	std::cout << "Selecione a opcao desejada:\n\n";
	std::cout << "1 - Valores aleatorios padrao\n";
	std::cout << "2 - Valores aleatorios customizados\n";
	std::cout << "3 - Modelo FAGOC\n";
	std::cout << "\nOpcao: ";

	while (true) {
		std::cin >> opcao;
		switch (opcao) {
			// Se a op��o for a padr�o, utiliza esses valores e gera um curso aleat�rio
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

			// Caso seja desejado fornecer os valores, capitura-os e cria o curso aleat�rio
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

			// Se for escolhido o modelo da fagoc, pede-se apenas o n�mero de alunos
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

std::tuple<std::vector<std::vector<bool>>, int, int>
solucionaAluno(const Instancia& instancia) {
	const auto& preRequisitos = instancia.curso()->preRequisitos();
	const auto& coRequisitos = instancia.curso()->coRequisitos();
	const auto& horarios = instancia.curso()->horarios();
	const auto& creditos = instancia.curso()->creditos();
	const auto& cursadas = instancia.aluno()->cursadas();
	const auto& aprovacoes = instancia.aluno()->aprovacoes();
	auto numDisciplinas = instancia.curso()->numDisciplinas();
	auto numHorarios = instancia.curso()->numHorarios();
	auto candidatas = geraSolucoes(numDisciplinas);
	auto solucaoOtima = 0;
	auto valorFinal = 0;
	std::vector<std::vector<bool>> solucoes{};

	for (auto i = 0; i < candidatas.size(); i++) {
		const auto& atual = candidatas[i];
		std::cout << "Atual: " << i << "\n";
		for (auto d = 0; d < numDisciplinas; d++) {
			if (atual[d] > (1 - aprovacoes[d])) {
				goto proximo;
			}

			auto numRequisitos = accumulate(begin(preRequisitos[d]),
			                                 end(preRequisitos[d]), 0);
			
			auto requisitosCumpridos = 0;
			for (auto j = 0; j < numDisciplinas; j++) {
				if (preRequisitos[d][j] && aprovacoes[j])
					requisitosCumpridos++;
			}

			if (numRequisitos > requisitosCumpridos) {
				goto proximo;
			}

			auto numCoRequisitos = accumulate(begin(coRequisitos[d]),
											 end(coRequisitos[d]), 0);
			auto coRequisitosCumpridos = 0;
			for (auto j = 0; j < numDisciplinas; j++) {
				if (coRequisitos[d][j] && (cursadas[j] || atual[j]))
					coRequisitosCumpridos++;
			}

			if (numCoRequisitos > coRequisitosCumpridos) {
				goto proximo;
			}
		}

		for (auto h = 0; h < numHorarios - 1; h += 2) {
			auto disciplinasConcorrentes = 0;
			for (auto d = 0; d < numDisciplinas; d++) {
				disciplinasConcorrentes +=
					(horarios[h][d] + horarios[h + 1][d]) * atual[d];
			}
			if (disciplinasConcorrentes > 2) {
				goto proximo;
			}
		}

		solucoes.push_back(candidatas[i]);
		auto valorAtual = 0;
		for (auto j = 0; j < numDisciplinas; j++) {
			if (atual[j])
				valorAtual += creditos[j];
		}
		if (valorAtual > valorFinal) {
			valorFinal = valorAtual;
			solucaoOtima = i;
		}

	proximo:
		continue;
	}

	return make_tuple(solucoes, solucaoOtima, valorFinal);
}

bool geraAlunos(std::string caminho, CursoPtr pCurso, int numAlunos) {
	Instancia instancia(move(pCurso));
	auto curso = instancia.curso();
	// Cria o diret�rio, e encerra a fun��o se for malsucedido
	if (!CreateDirectory(caminho.c_str(), nullptr))
		return false;

	// Nome padr�o do aluno
	std::string nome = "aln";
	// Streams de sa�da para o txt que ir� armazenar os dados resumidos
	std::ostringstream saidaTxt{};
	// E para o html que ser� a compila��o de todos os alunos
	std::ostringstream saidaHtml{};
	// Desabilita flushing autom�tico da sa�da
	saidaTxt << std::nounitbuf;
	saidaHtml << std::nounitbuf;

	// Carrega o arquivo CSS do HTML
	geraArquivo::escolheCSS("etc\\estilos.css");

	for (auto i = 0; i < numAlunos; i++) {
		// Gera um aluno aleat�rio e o coloca dentr do Solver
		auto aln = nome + std::to_string(i + 1);
		instancia.novoAlunoAleatorio(aln);
		auto numDisciplinas = instancia.curso()->numDisciplinas();
		auto resultado = solucionaAluno(instancia);

		// Resolve o problema e imprime os dados nos arquivos
		saidaTxt << aln << "\n";
		const auto& aprovacoesAluno = instancia.aluno()->aprovacoes();
		const auto& cursadasAluno = instancia.aluno()->cursadas();

		// A cada itera��o os resultados s�o gravados no TXT
		saidaTxt << "Aprova��es:\n";
		for (auto j = 0; j < numDisciplinas; j++) {
			if (aprovacoesAluno[j]) {
				saidaTxt << curso->nomeDisciplinas()[j] << " ";
			}
		}
		saidaTxt << "\n";
		saidaTxt << "Cursadas:\n";
		for (auto j = 0; j < numDisciplinas; j++) {
			if (cursadasAluno[j]) {
				saidaTxt << curso->nomeDisciplinas()[j] << " ";
			}
		}

		auto solucoes = std::get<0>(resultado);
		auto otima = std::get<1>(resultado);
		auto valorFinal = std::get<2>(resultado);

		saidaTxt << "\nDisciplinas escolhidas: ";
		for (auto j = 0; j < numDisciplinas; j++) {
			if (solucoes[otima][j])
				saidaTxt << curso->nomeDisciplinas()[j] << " ";
		}
		saidaTxt << "\n";

		saidaTxt << "\nResultado final: " << valorFinal << "\n\n";

		// Escreve o hor�rio do aluno num HTML separado, mas tamb�m escreve no buffer
		saidaHtml << geraArquivo::escreveHTML(curso, solucoes[otima], caminho, aln,
		                                      curso->numPeriodos(), curso->numDiasLetivos())
				<< "\n\n";
	}

	// Cria as streams de escrita dos arquivo
	std::ofstream arquivoSaidaTxt(caminho + "\\" + "resultado.txt");
	std::ofstream arquivoSaidaHtml(caminho + "\\" + "resultado.html");
	arquivoSaidaTxt << std::nounitbuf << saidaTxt.str() << std::endl;
	arquivoSaidaHtml << std::nounitbuf << saidaHtml.str() << std::endl;

	return true;

}


int main() {
	//std::ios_base::sync_with_stdio(false);
	//freopen("out", "w", stdout);
	// Pega as op��es retornadas pelo menu
	auto opcoes = menu();
	// Ponteiro para um Curso
	auto pCurso = std::get<0>(move(opcoes));
	// Pasta de destino
	auto pasta = std::get<1>(opcoes);
	// N�mero de alunos a serem gerados
	auto numAlunos = std::get<2>(opcoes);

	std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	// Inicialzia rel�gio e resolve todos os modelos
	auto begin = std::chrono::system_clock::now();
	std::cout << "\n";
	if (!geraAlunos(dir + pasta, move(pCurso), numAlunos)) {
		std::cout << "Nao foi possivel criar a pasta.\n\n";
	} else {
		std::cout << "Modelos resolvidos com sucesso.\n\n";
	}

	// Imprime o tempo gasto
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diferenca = end - begin;
	std::cout << "Tempo total: " << diferenca.count() << "s\n\n";

}
