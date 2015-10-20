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
#include <map>
#include <ManipulaJson.h>
#include <CursoEntrada.h>

//! Converte um n�mero em um vector de bools, do tamanho de n bits
//! \param x N�mero a ser convertido
//! \param bits N�mero de bits do n�mero
//! \return Um vector bools que representa 'x'
std::vector<bool> geraCandidata(unsigned long long x, int bits) {
	std::vector<bool> candidata;
	// Contador de quantos bits j� foram atribu�dos. Ser� usado pra completar
	// os zeros
	auto bitsAtribuidos = 0;
	// Enquanto x n�o for zero, pega o �ltimo bit, e testa se � 1. Se for, insere
	// 'true' no vector. Se n�o, insere false. Depois faz um shift para a direita, o
	// que descarta o �ltimo bit, e incrementa o contador de bits atribu�dos
	while (x) {
		if (x & 1)
			candidata.push_back(true);
		else
			candidata.push_back(false);
		x >>= 1;
		bitsAtribuidos++;
	}

	// Percorre o restante dos bits n�o atribuitos e insere zeros
	for (auto i = bitsAtribuidos; i < bits; i++) {
		candidata.push_back(false);
	}

	return candidata;
}

using CursoPtr = std::unique_ptr<Curso>;

//! Exibe um menu e retorna as op��es selecionadas para o main
//! \return Uma triple de CursoPtr, string e int. Respectivamente, um curso,
//! o nome da pasta de destino e o n�mero de aluno
std::tuple<CursoPtr, std::string, int> menu() {
	// Inicializa as vari�veis com o padr�o
	int numDisciplinas, numPreRequisitos, numCoRequisitos, numHorarios,
	    numOfertadas, numAlunos, numProfessores, maxMinistradas,
	    numDiasLetivos, numPeriodos, numTurmas, capacidadeMinima, capacidadeMaxima;
	int opcao;
	std::string pasta;
	CursoPtr pCurso;

	// Exibe as op��es do menu
	std::cout << "\nSOLUCIONADOR DE INSTANCIAS DO MODELO DE GRADE - FORCA BRUTA\n";
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

//! Soluciona uma inst�ncia de teste utilizando for�a bruta, e retorna a solu��o
//! mais �tima, o seu valor para a fun��o objetivo e uma lista com todas as solu��es fact�veis
//! \param instancia Refer�ncia para const de um objeto da classe Inst�ncia
//! \return Uma tripla contendo 1) um vector de vectors, que s�o as solu��es fact�veis
//! 2) o �ndice da solu��o mais �tima nesse vector, 3) o valor da fun��o objetivo para 
//! a solu��o mais �tima
std::tuple<std::vector<std::vector<bool>>, int, int>
solucionaAluno(const Instancia& instancia) {
	// Inicializa refer�ncias para as estruturas de dados necess�rias para solucionar
	// o modelo
	const auto& preRequisitos = instancia.curso()->preRequisitos();
	const auto& coRequisitos = instancia.curso()->coRequisitos();
	const auto& horarios = instancia.curso()->horarios();
	const auto& creditos = instancia.curso()->creditos();
	const auto& cursadas = instancia.aluno()->cursadas();
	const auto& aprovacoes = instancia.aluno()->aprovacoes();
	auto numDisciplinas = instancia.curso()->numDisciplinas();
	auto numHorarios = instancia.curso()->numHorarios();
	// Inicializa as vari�veis de controle, de qual � o �ndice da solu��o �tima no
	// vector de solu��es, e qual � o valor da fun��o objetivo para a solu��o mais
	// �tima do momento, assim como qual o n�mero total de poss�veis solu��es para 
	// esse problema (podendo as solu��es ser apenas 0 ou 1, o n�mero de possibilidades
	// � 2^n, onde n � o n�mero de vari�veis de decis�o, nesse caso, as disciplinas
	auto solucaoOtima = 0;
	auto valorFinal = 0;
	auto possibilidades = pow(2, numDisciplinas);
	std::vector<std::vector<bool>> solucoes{};

	// A valia cada possibilidade (for�a bruta), e guarda todas as solu��es fact�veis
	// no vector, e armazena qual a mais �tima at� o momento.
	// NOTA: esse � um dos poucos casos em que o uso do 'goto' � v�lido. Utilizar de
	// outras ferramentas de controle de fluxo s� deixariam o c�digo mais complexo.
	for (auto i = 0; i < possibilidades; i++) {
		// Gera o pr�ximo vector de possibilidades
		auto atual = geraCandidata(i, numDisciplinas);

		for (auto d = 0; d < numDisciplinas; d++) {
			// Verifica se alguma disciplina que a candidata atual marcou como escolhida
			// j� foi cursada pelo aluno (e ele j� foi aprovado). Se for verdadeiro, a 
			// candidata � descartada (um aluno n�o pode cursar uma mat�ria que j� passou)
			// e o programa pula para o fim do bloco da possibilidade
			
			if (atual[d] > (1 - aprovacoes[d])) {
				goto proximo;
			}

			auto numRequisitos = accumulate(begin(preRequisitos[d]),
			                                 end(preRequisitos[d]), 0);
			// Calcula o n�mero de requisitos que disciplina possui
			auto requisitosCumpridos = 0;
			// Calcula o n�mero de disciplinas que o aluno foi aprovado
			// e que s�o pr�-requisitos da disciplina atual
			for (auto j = 0; j < numDisciplinas; j++) {
				if (preRequisitos[d][j] && aprovacoes[j])
					requisitosCumpridos++;
			}

			// Se a disciplina atual est� marcada com escolhida, testa
			// se os requisitos cumpridos s�o insuficientes. Se forem, passa 
			// para a pr�xima solu��o
			if (numRequisitos * atual[d] > requisitosCumpridos) {
				goto proximo;
			}

			// Calcula o n�mero de co-requisitos que a disciplina possui
			auto numCoRequisitos = accumulate(begin(coRequisitos[d]),
							  end(coRequisitos[d]), 0);

			auto coRequisitosCumpridos = 0;

			// Calcula o n�mero de disciplinas que s�o co-requisitos da atual
			// que o aluno j� fez ou ir� fazer, segundo a candidata
			for (auto j = 0; j < numDisciplinas; j++) {
				if (coRequisitos[d][j] && (cursadas[j] || atual[j]))
					coRequisitosCumpridos++;
			}


			// Se a disciplina atual est� marcada como escolhida, testa se os
			// requisitos cumpridos s�o insuficientes. Se forem, passsa para a
			// pr�xima solu��o
			if (numCoRequisitos * atual[d] > coRequisitosCumpridos) {
				goto proximo;
			}
		}

		// Verifica se o as disciplinas do hor�rio do aluno s�o conflitantes
		// (se h� aulas de duas ou mais ou mesmo tempo). Se houver uma colis�o,
		// a solu��o � descartada.
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

		// Se a solu��o passou por todos os testes sem ser descartada, ela � inserida
		// no vector de solu��es fact�veis, e ent�o o valor de sua fun��o objetivo � 
		// calculado, e verifica-se se ela � uma solu��o mais �tima que a atual. Se for,
		// seu �ndice no vector � gravado, assim como seu valor � atribu�do ao atual
		solucoes.push_back(atual);
		auto valorAtual = 0;
		for (auto j = 0; j < numDisciplinas; j++) {
			if (atual[j])
				valorAtual += creditos[j];
		}
		if (valorAtual > valorFinal) {
			valorFinal = valorAtual;
			solucaoOtima++;
		}

	// Se alguma das restri��es n�o for obedecida, a solu��o � descartada
	proximo:
		continue;
	}

	// Depois de analisadas todas as solu��es, � retornado uma tripla com a
	// lista de solu��es, o �ndice da solu��o �tima nessa lista e o valor dela
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

		saidaTxt << "\nDisciplinas escolhidas:\n";
		for (auto j = 0; j < numDisciplinas; j++) {
			if (solucoes[otima][j])
				saidaTxt << curso->nomeDisciplinas()[j] << " ";
		}
		saidaTxt << "\n";

		saidaTxt << "\nResultado final: " << valorFinal << "\n\n";

		// Escreve o hor�rio do aluno num HTML separado, mas tamb�m escreve no buffer
		saidaHtml << geraArquivo::escreveHTML(pCurso->horarios(), pCurso->nomeDisciplinas(),
											  solucoes[otima], caminho, aln, pCurso->numDisciplinas(),
											  pCurso->numHorarios(), pCurso->numPeriodos(),
											  pCurso->numDiasLetivos())
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
	std::ios_base::sync_with_stdio(false);

	//// Pega as op��es retornadas pelo menu
	//auto opcoes = menu();
	//// Ponteiro para um Curso
	//auto pCurso = std::get<0>(move(opcoes));
	//// Pasta de destino
	//auto pasta = std::get<1>(opcoes);
	//// N�mero de alunos a serem gerados
	//auto numAlunos = std::get<2>(opcoes);

	//std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	//// Inicialzia rel�gio e resolve todos os modelos
	//auto begin = std::chrono::system_clock::now();
	//std::cout << "\n";
	//if (!geraAlunos(dir + pasta, move(pCurso), numAlunos)) {
	//	std::cout << "Nao foi possivel criar a pasta.\n\n";
	//} else {
	//	std::cout << "Modelos resolvidos com sucesso.\n\n";
	//}

	//// Imprime o tempo gasto
	//auto end = std::chrono::system_clock::now();
	//std::chrono::duration<double> diferenca = end - begin;
	//std::cout << "Tempo total: " << diferenca.count() << "s\n\n";

	auto entrada = manipulaJson::lerJson("../input.json");
	auto& curso = entrada.first;
	auto& alunos = entrada.second;

	copy(begin(curso.nomeDisciplinas()), end(curso.nomeDisciplinas()), 
		 std::ostream_iterator<std::string>(std::cout, " "));
	std::cout << "\n";
	
	copy(begin(curso.creditos()), end(curso.creditos()),
		 std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\n\n";


	for (auto& aluno : alunos) {
		std::cout << aluno.nome() << "\n";
		for (auto i = 0; i < aluno.aprovacoes().size(); i++) {
			if (!aluno.aprovacoes()[i])
				std::cout << curso.nomeDisciplinas()[i] << " ";
		}
		std::cout << "\n\n";
	}

}
