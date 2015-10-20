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

//! Converte um número em um vector de bools, do tamanho de n bits
//! \param x Número a ser convertido
//! \param bits Número de bits do número
//! \return Um vector bools que representa 'x'
std::vector<bool> geraCandidata(unsigned long long x, int bits) {
	std::vector<bool> candidata;
	// Contador de quantos bits já foram atribuídos. Será usado pra completar
	// os zeros
	auto bitsAtribuidos = 0;
	// Enquanto x não for zero, pega o último bit, e testa se é 1. Se for, insere
	// 'true' no vector. Se não, insere false. Depois faz um shift para a direita, o
	// que descarta o último bit, e incrementa o contador de bits atribuídos
	while (x) {
		if (x & 1)
			candidata.push_back(true);
		else
			candidata.push_back(false);
		x >>= 1;
		bitsAtribuidos++;
	}

	// Percorre o restante dos bits não atribuitos e insere zeros
	for (auto i = bitsAtribuidos; i < bits; i++) {
		candidata.push_back(false);
	}

	return candidata;
}

using CursoPtr = std::unique_ptr<Curso>;

//! Exibe um menu e retorna as opções selecionadas para o main
//! \return Uma triple de CursoPtr, string e int. Respectivamente, um curso,
//! o nome da pasta de destino e o número de aluno
std::tuple<CursoPtr, std::string, int> menu() {
	// Inicializa as variáveis com o padrão
	int numDisciplinas, numPreRequisitos, numCoRequisitos, numHorarios,
	    numOfertadas, numAlunos, numProfessores, maxMinistradas,
	    numDiasLetivos, numPeriodos, numTurmas, capacidadeMinima, capacidadeMaxima;
	int opcao;
	std::string pasta;
	CursoPtr pCurso;

	// Exibe as opções do menu
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

//! Soluciona uma instância de teste utilizando força bruta, e retorna a solução
//! mais ótima, o seu valor para a função objetivo e uma lista com todas as soluções factíveis
//! \param instancia Referência para const de um objeto da classe Instância
//! \return Uma tripla contendo 1) um vector de vectors, que são as soluções factíveis
//! 2) o índice da solução mais ótima nesse vector, 3) o valor da função objetivo para 
//! a solução mais ótima
std::tuple<std::vector<std::vector<bool>>, int, int>
solucionaAluno(const Instancia& instancia) {
	// Inicializa referências para as estruturas de dados necessárias para solucionar
	// o modelo
	const auto& preRequisitos = instancia.curso()->preRequisitos();
	const auto& coRequisitos = instancia.curso()->coRequisitos();
	const auto& horarios = instancia.curso()->horarios();
	const auto& creditos = instancia.curso()->creditos();
	const auto& cursadas = instancia.aluno()->cursadas();
	const auto& aprovacoes = instancia.aluno()->aprovacoes();
	auto numDisciplinas = instancia.curso()->numDisciplinas();
	auto numHorarios = instancia.curso()->numHorarios();
	// Inicializa as variáveis de controle, de qual é o índice da solução ótima no
	// vector de soluções, e qual é o valor da função objetivo para a solução mais
	// ótima do momento, assim como qual o número total de possíveis soluções para 
	// esse problema (podendo as soluções ser apenas 0 ou 1, o número de possibilidades
	// é 2^n, onde n é o número de variáveis de decisão, nesse caso, as disciplinas
	auto solucaoOtima = 0;
	auto valorFinal = 0;
	auto possibilidades = pow(2, numDisciplinas);
	std::vector<std::vector<bool>> solucoes{};

	// A valia cada possibilidade (força bruta), e guarda todas as soluções factíveis
	// no vector, e armazena qual a mais ótima até o momento.
	// NOTA: esse é um dos poucos casos em que o uso do 'goto' é válido. Utilizar de
	// outras ferramentas de controle de fluxo só deixariam o código mais complexo.
	for (auto i = 0; i < possibilidades; i++) {
		// Gera o próximo vector de possibilidades
		auto atual = geraCandidata(i, numDisciplinas);

		for (auto d = 0; d < numDisciplinas; d++) {
			// Verifica se alguma disciplina que a candidata atual marcou como escolhida
			// já foi cursada pelo aluno (e ele já foi aprovado). Se for verdadeiro, a 
			// candidata é descartada (um aluno não pode cursar uma matéria que já passou)
			// e o programa pula para o fim do bloco da possibilidade
			
			if (atual[d] > (1 - aprovacoes[d])) {
				goto proximo;
			}

			auto numRequisitos = accumulate(begin(preRequisitos[d]),
			                                 end(preRequisitos[d]), 0);
			// Calcula o número de requisitos que disciplina possui
			auto requisitosCumpridos = 0;
			// Calcula o número de disciplinas que o aluno foi aprovado
			// e que são pré-requisitos da disciplina atual
			for (auto j = 0; j < numDisciplinas; j++) {
				if (preRequisitos[d][j] && aprovacoes[j])
					requisitosCumpridos++;
			}

			// Se a disciplina atual está marcada com escolhida, testa
			// se os requisitos cumpridos são insuficientes. Se forem, passa 
			// para a próxima solução
			if (numRequisitos * atual[d] > requisitosCumpridos) {
				goto proximo;
			}

			// Calcula o número de co-requisitos que a disciplina possui
			auto numCoRequisitos = accumulate(begin(coRequisitos[d]),
							  end(coRequisitos[d]), 0);

			auto coRequisitosCumpridos = 0;

			// Calcula o número de disciplinas que são co-requisitos da atual
			// que o aluno já fez ou irá fazer, segundo a candidata
			for (auto j = 0; j < numDisciplinas; j++) {
				if (coRequisitos[d][j] && (cursadas[j] || atual[j]))
					coRequisitosCumpridos++;
			}


			// Se a disciplina atual está marcada como escolhida, testa se os
			// requisitos cumpridos são insuficientes. Se forem, passsa para a
			// próxima solução
			if (numCoRequisitos * atual[d] > coRequisitosCumpridos) {
				goto proximo;
			}
		}

		// Verifica se o as disciplinas do horário do aluno são conflitantes
		// (se há aulas de duas ou mais ou mesmo tempo). Se houver uma colisão,
		// a solução é descartada.
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

		// Se a solução passou por todos os testes sem ser descartada, ela é inserida
		// no vector de soluções factíveis, e então o valor de sua função objetivo é 
		// calculado, e verifica-se se ela é uma solução mais ótima que a atual. Se for,
		// seu índice no vector é gravado, assim como seu valor é atribuído ao atual
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

	// Se alguma das restrições não for obedecida, a solução é descartada
	proximo:
		continue;
	}

	// Depois de analisadas todas as soluções, é retornado uma tripla com a
	// lista de soluções, o índice da solução ótima nessa lista e o valor dela
	return make_tuple(solucoes, solucaoOtima, valorFinal);
}

bool geraAlunos(std::string caminho, CursoPtr pCurso, int numAlunos) {
	Instancia instancia(move(pCurso));
	auto curso = instancia.curso();
	// Cria o diretório, e encerra a função se for malsucedido
	if (!CreateDirectory(caminho.c_str(), nullptr))
		return false;

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

	for (auto i = 0; i < numAlunos; i++) {
		// Gera um aluno aleatório e o coloca dentr do Solver
		auto aln = nome + std::to_string(i + 1);
		instancia.novoAlunoAleatorio(aln);
		auto numDisciplinas = instancia.curso()->numDisciplinas();
		auto resultado = solucionaAluno(instancia);

		// Resolve o problema e imprime os dados nos arquivos
		saidaTxt << aln << "\n";
		const auto& aprovacoesAluno = instancia.aluno()->aprovacoes();
		const auto& cursadasAluno = instancia.aluno()->cursadas();

		// A cada iteração os resultados são gravados no TXT
		saidaTxt << "Aprovações:\n";
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

		// Escreve o horário do aluno num HTML separado, mas também escreve no buffer
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

	//// Pega as opções retornadas pelo menu
	//auto opcoes = menu();
	//// Ponteiro para um Curso
	//auto pCurso = std::get<0>(move(opcoes));
	//// Pasta de destino
	//auto pasta = std::get<1>(opcoes);
	//// Número de alunos a serem gerados
	//auto numAlunos = std::get<2>(opcoes);

	//std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	//// Inicialzia relógio e resolve todos os modelos
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
