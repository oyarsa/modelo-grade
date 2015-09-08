#include "CursoAleatorio.h"
#include "aleatorio.h"
#include "Curso.h"
#include "Professor.h"
#include <algorithm>
#include <sstream>
#include <set>

using aleatorio::randomInt;

CursoAleatorio::CursoAleatorio(int numDisciplinas, int numPreRequisitos,
                               int numCoRequisitos, int numHorarios,
                               int numOfertadas, int numProfessores,
                               int maxMinistradas)
	: Curso(numDisciplinas, numPreRequisitos,
	        numCoRequisitos, numHorarios,
	        numOfertadas, numProfessores),
	  maxMinistradas(maxMinistradas),
	  maxProfessores(numProfessores / 3) {
	init();
}

void CursoAleatorio::init() {
	geraDisciplinas();
	geraHorario();
	geraProfessores();
	geraNomeHorarios();
}

void CursoAleatorio::geraCreditos() {
	//! Gera a quantidade de disciplinas que recebem 2, 4 e 6 créditos a partir de porcentagens
	auto discSeisCreditos = int(0.2 * numDisciplinas);
	auto discQuatroCreditos = int(0.6 * numDisciplinas);
	auto discDoisCreditos = numDisciplinas - discSeisCreditos - discQuatroCreditos;
	int i, j, k;

	//! Aloca cada grupo de disciplinas linearmente
	for (i = 0; i < discSeisCreditos; i++)
		creditos_[i] = 6;

	for (j = discSeisCreditos; j < i + discQuatroCreditos; j++)
		creditos_[j] = 4;

	for (k = discQuatroCreditos; k < j + discDoisCreditos; k++)
		creditos_[k] = 2;

	//! Embaralha o vector para aleatorizar a distribuiçãp
	shuffle(begin(creditos_), end(creditos_), aleatorio::geradorAleatorio());
}

void CursoAleatorio::geraPreRequisitos() {
	auto requisitosAlocados = 0;

	//! Enquanto não foram alocados todos os pré-requisitos
	while (requisitosAlocados < numPreRequisitos) {
		//! Percorre a lista de disciplinas
		for (auto i = 0; i < numDisciplinas; i++) {
			//! Gera uma chance de essa disciplina receber um pré-requisito
			if (randomInt() % 100 < 25) {
				//! Percorre um número igual ao número de disciplinas
				for (auto j = 0; j < numDisciplinas; j++) {
					//! Gera o índice de uma disciplina para ser o pré-requisito
					auto preReq = randomInt() % numDisciplinas;

					//! Se a gerada não for um pré-requisito desta (isso geraria um deadlock)
					//! e não for ela mesma, escolhe como pré-requisito
					if (preReq != i && !preRequisitos_[preReq][i]) {
						requisitosAlocados++;
						preRequisitos_[i][preReq] = true;
						break;
					}
				}
			}
			//! Se todos os pré-requisitos tiverem sido alocados, termina o laço
			if (requisitosAlocados >= numPreRequisitos)
				break;
		}
	}
}

void CursoAleatorio::geraCoRequisitos() {
	auto coRequisitosAlocados = 0;

	//! Enquanto não foram alocados todos os pré-requisitos
	while (coRequisitosAlocados < numCoRequisitos) {
		//! Percorre a lista de disciplinas
		for (auto i = 0; i < numDisciplinas; i++) {
			//! Gera uma chance desta disciplina receber um co-requisito
			if (randomInt() % 100 < 10) {
				for (auto j = 0; j < numDisciplinas; j++) {
					//! Gera o índice de uma disciplina
					auto coReq = randomInt() % numDisciplinas;

					//! Testa se não é ela mesma, e se essa disciplina já não é
					//! co-requisito nem pré-requisito
					if (coReq != i && !preRequisitos_[i][coReq]
						&& !coRequisitos_[coReq][i]) {
						coRequisitosAlocados++;
						coRequisitos_[i][coReq] = true;
						break;
					}
				}
			}
			//! Se todos os co-requisitos tiverem sido alocados, termina o laço
			if (coRequisitosAlocados >= numCoRequisitos)
				break;
		}
	}
}

void CursoAleatorio::geraOfertadas() {
	//! Conjunto de disciplinas que serão ofertadas
	std::set<int> disciplinasEscolhidas;

	//! Enquanto não tiverem sido escolhidas todas as ofertadas,
	//! gera uma e insere no conjunto. Se já tiver sido escolhida
	//! o tamanho não será afetado
	size_t numOfertadas(numOfertadas);
	while (disciplinasEscolhidas.size() < numOfertadas) {
		auto disc = randomInt() % numDisciplinas;
		disciplinasEscolhidas.insert(disc);
	}

	//! Percorre o conjunto de disciplinas escolhidas e as marca como
	//! verdadeiras no vector de ofertadas
	for (auto disc : disciplinasEscolhidas)
		ofertadas_[disc] = true;
}

//! Executa subrotinas para gerar cada aspecto das disciplinas
void CursoAleatorio::geraDisciplinas() {
	geraCreditos();
	geraPreRequisitos();
	geraCoRequisitos();
	geraNomeDisciplinas();
}

void CursoAleatorio::geraNomeDisciplinas() {
	//! Percorre a lista de disciplinas e atribui nomes genéricos
	//! baseados no seu índice
	for (auto i = 0; i < numDisciplinas; i++) {
		nomeDisciplinas_.push_back("DISC" + std::to_string(i + 1));
	}
}

void CursoAleatorio::geraHorario() {
	geraOfertadas();

	auto disciplinasAlocadas = 0;
	std::vector<int> creditosAlocados(numDisciplinas, 0);

	//! Executa o laço até todas as disciplinas ofertadas terem sido alocadas
	//! em um horário
	while (disciplinasAlocadas < numOfertadas) {
		//! Percorre todos os horários
		for (auto i = 0; i < numHorarios; i++) {
			//! Gera uma disciplina para ser alocada naquele horário
			auto discAtual = randomInt() % numDisciplinas;
			//! Testa se essa disciplina já teve todos os seus créditos alocados,
			//! ou se já foi alocada nesse horário, ou então se não está sendo ofertada
			if (!ofertadas_[discAtual]
				|| creditosAlocados[discAtual] > creditos_[discAtual]
				|| horarios_[i][discAtual]) {
				continue;
			}
			//! Se passar nos testes, ela é alocada para esse horário. Se ela ainda
			//! não havia sido alocada antes, incrementa o contador de disciplinas alocadas
			//! Depois incrementa o contador de créditos alocados dessa disciplina
			horarios_[i][discAtual] = true;
			if (creditosAlocados[discAtual] == 0)
				disciplinasAlocadas++;

			creditosAlocados[discAtual]++;
		}
	}
}

void CursoAleatorio::distribuiProfessores() {
	/*!
	* Distribui aleatoriamente a quantidade de disciplinas ministradas por cada
	* professor. Se o número dsitribuído não se igual ou superar o número de disciplinas
	* (ou seja, se ficarem disciplinas sem professores), o algoritmo reinicia
	*/
	auto totalDistribuidas = 0;
	while (totalDistribuidas < numDisciplinas) {
		//! Limpa os números distribuídos e começa do zero
		professores_.erase(professores_.begin(), professores_.end());
		for(auto i = 0; i < numProfessores; i++) {
			//! Gera um número para o professor em questão e soma às distribuidas
			auto ministradas = (randomInt() % maxMinistradas) + 1;
			totalDistribuidas += ministradas;

			//! Cria um objeto professor com um nome genérico e com a quantidade gerada
			auto nome = "PROF" + std::to_string(i + 1);
			Professor professor(nome, ministradas);

			//! Insere no vector por rvalue
			professores_.push_back(std::move(professor));
		}
	}
}

void CursoAleatorio::geraProfessores() {
	//! Aloca as disciplinas ao professores, de forma que não excedam o número que possam ministrar
	std::vector<int> disciplinasAlocadas(numDisciplinas);
	std::vector<int> contadorMinistradas(numProfessores);
	auto numAlocadas = 0;

	distribuiProfessores();

	for (auto i = 0; i < numProfessores; i++) {
		//! Pega a referência ao professor naquele índice para poder adicionar disciplinas
		auto& professor = professores_[i];
		//! Enquanto o professor ainda precisar receber disciplinas
		while (contadorMinistradas[i] < professor.numMinistradas()) {
			//! Percorre a lista de disciplinas, parando se elas acabarem ou o professor já
			//! estiver cheio
			for (size_t j = 0; j < disciplinasAlocadas.size() &&
			     contadorMinistradas[i] < professor.numMinistradas(); j++) {
				//! Verifica se o professor já ministra aquela disciplina,
				//! gera uma chance de o professor ministrá-la e testa se ela já está sendo ministrada
				//! pelo número máximo de professores que podem simultaneamente ministrar uma disciplina
				if (disciplinasAlocadas[j] < maxProfessores &&
					!professor.ministra(j) && randomInt() % 100 < 25) {

					//! Se todas as condições forem satisfeitas, testa-se se a disciplina não havia
					//! sido alocada antes. Se não, incrementa o número de alocadas
					if (disciplinasAlocadas[j] == 0)
						numAlocadas++;

					//! Incrementa o número de vezes que a disciplina foi alocada a um professor
					disciplinasAlocadas[j]++;
					//! Adiciona a disciplina à lista de ministradas do professor
					professor.adicionaDisciplina(j);
					//! Incrementa o contador de disciplinas que aquele professor recebeu
					contadorMinistradas[i]++;
				}
			}
		}
	}
}
