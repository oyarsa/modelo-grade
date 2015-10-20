#include "CursoAleatorio.h"
#include "Curso.h"
#include "Professor.h"
#include <algorithm>
#include <sstream>
#include <unordered_set>

CursoAleatorio::CursoAleatorio(int numDisciplinas, int numPreRequisitos,
                               int numCoRequisitos, int numHorarios,
                               int numOfertadas, int numProfessores,
                               int maxMinistradas, int numDiasLetivos,
                               int numPeriodos)
	: Curso(numDisciplinas, numPreRequisitos,
	        numCoRequisitos, numHorarios,
	        numOfertadas, numProfessores,
	        numDiasLetivos, numPeriodos),
	  maxMinistradas(maxMinistradas),
	  maxProfessores(numProfessores / 3),
	  rand() {
	init();
}

void CursoAleatorio::init() {
	geraDisciplinas();
	geraHorario();
	geraNomeHorarios();
	geraProfessores();
}

void CursoAleatorio::geraCreditos() {
	//! Gera a quantidade de disciplinas que recebem 2, 4 e 6 créditos a partir de porcentagens
	//! Se o número de dias letivos for pequeno, não gera disciplinas de 6 créditos
	auto discSeisCreditos = int(0.2 * numDisciplinas_ * (numDiasLetivos_ > 3 ? 1 : 0));
	auto discQuatroCreditos = int(0.6 * numDisciplinas_);
	auto discDoisCreditos = numDisciplinas_ - discSeisCreditos - discQuatroCreditos;
	int i, j, k;

	//! Aloca cada grupo de disciplinas linearmente
	for (i = 0; i < discSeisCreditos; i++)
		creditos_[i] = 6;

	for (j = discSeisCreditos; j < i + discQuatroCreditos; j++)
		creditos_[j] = 4;

	for (k = discQuatroCreditos; k < j + discDoisCreditos; k++)
		creditos_[k] = 2;

	//! Embaralha o vector para aleatorizar a distribuiçãp
	shuffle(begin(creditos_), end(creditos_), rand.geradorAleatorio());
}

void CursoAleatorio::geraPreRequisitos() {
	auto requisitosAlocados = 0;

	//! Enquanto não foram alocados todos os pré-requisitos
	while (requisitosAlocados < numPreRequisitos) {
		//! Percorre a lista de disciplinas
		for (auto i = 0; i < numDisciplinas_; i++) {
			if (discTurma_[i].second != "A")
				continue;
			//! Gera uma chance de essa disciplina receber um pré-requisito
			if (rand.randomInt() % 100 < 25) {
				//! Percorre um número igual ao número de disciplinas
				for (auto j = 0; j < numDisciplinas_; j++) {
					//! Gera o índice de uma disciplina para ser o pré-requisito
					auto preReq = rand.randomInt() % numDisciplinas_;

					//! Se a gerada não for um pré-requisito desta (isso geraria um deadlock)
					//! e não for ela mesma, escolhe como pré-requisito
					if (preReq != i && !preRequisitos_[preReq][i]) {
						requisitosAlocados++;
						preRequisitos_[i][preReq] = true;
						break;
					}
				}
			}
		}
		//! Se todos os pré-requisitos tiverem sido alocados, termina o laço
		if (requisitosAlocados >= numPreRequisitos)
			break;
	}
}

void CursoAleatorio::geraCoRequisitos() {
	auto coRequisitosAlocados = 0;

	//! Enquanto não foram alocados todos os pré-requisitos
	while (coRequisitosAlocados < numCoRequisitos) {
		//! Percorre a lista de disciplinas
		for (auto i = 0; i < numDisciplinas_; i++) {
			if (discTurma_[i].second != "A")
				continue;
			//! Gera uma chance desta disciplina receber um co-requisito
			if (rand.randomInt() % 100 < 10) {
				for (auto j = 0; j < numDisciplinas_; j++) {
					//! Gera o índice de uma disciplina
					auto coReq = rand.randomInt() % numDisciplinas_;

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
	std::unordered_set<int> disciplinasEscolhidas;

	//! Enquanto não tiverem sido escolhidas todas as ofertadas,
	//! gera uma e insere no conjunto. Se já tiver sido escolhida
	//! o tamanho não será afetado
	size_t numOfertadaSizeT(numOfertadas);
	while (disciplinasEscolhidas.size() < numOfertadaSizeT) {
		auto disc = rand.randomInt() % numDisciplinas_;
		auto inserido = disciplinasEscolhidas.insert(disc).second;
		if (inserido)
			creditosTotaisOfertados += creditos_[disc];
	}

	//! Percorre o conjunto de disciplinas escolhidas e as marca como
	//! verdadeiras no vector de ofertadas
	for (auto disc : disciplinasEscolhidas)
		ofertadas_[disc] = true;
}

//! Executa subrotinas para gerar cada aspecto das disciplinas
void CursoAleatorio::geraDisciplinas() {
	geraCreditos();
	geraTurmas();
	geraPreRequisitos();
	geraCoRequisitos();
	geraCapacidades();
	finalizaDependencias();
	geraEquivalencia();
	geraNomeDisciplinas();
}

void CursoAleatorio::geraNomeDisciplinas() {
	//! Percorre a lista de disciplinas e atribui nomes genéricos
	//! baseados no seu índice
	for (auto i = 0; i < numDisciplinas_; i++) {
		nomeDisciplinas_.push_back("DISC" + std::to_string(i + 1));
	}
}

void CursoAleatorio::geraHorario() {
	geraOfertadas();
	auto disciplinasAlocadasTotal = 0;
	auto creditosAlocadosTotal = 0;
	std::vector<int> creditosAlocados(numDisciplinas_, 0);
	std::vector<int> disciplinasAlocadas(numHorarios_, 0);

	// Executa o laço até todas as disciplinas ofertadas terem sido alocadas
	// em um horário
	while (disciplinasAlocadasTotal < numOfertadas
		|| creditosAlocadosTotal < creditosTotaisOfertados) {
		// Percorre todos os horários
		for (auto i = 0; i < numHorarios_; i++) {
			// Gera uma disciplina para ser alocada naquele horário
			auto discAtual = rand.randomInt() % numDisciplinas_;
			// Testa se essa disciplina já teve todos os seus créditos alocados,
			// ou se já foi alocada nesse horário, ou então se não está sendo ofertada
			if (!ofertadas_[discAtual]
				|| creditosAlocados[discAtual] >= creditos_[discAtual]
				|| horarios_[i][discAtual]
				|| disciplinasAlocadas[i] >= numPeriodos_) {
				continue;
			}
			// Se passar nos testes, ela é alocada para esse horário. Se ela ainda
			// não havia sido alocada antes, incrementa o contador de disciplinas alocadas
			// Depois incrementa o contador de créditos alocados dessa disciplina
			horarios_[i][discAtual] = true;
			if (creditosAlocados[discAtual] == 0)
				disciplinasAlocadasTotal++;

			disciplinasAlocadas[i]++;
			creditosAlocados[discAtual]++;
			creditosAlocadosTotal++;
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
	while (totalDistribuidas < numDisciplinas_) {
		//! Limpa os números distribuídos e começa do zero
		professores_.erase(professores_.begin(), professores_.end());
		for (auto i = 0; i < numProfessores_; i++) {
			//! Gera um número para o professor em questão e soma às distribuidas
			auto ministradas = (rand.randomInt() % maxMinistradas) + 1;
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
	std::vector<int> disciplinasAlocadas(numDisciplinas_);
	std::vector<int> contadorMinistradas(numProfessores_);
	auto numAlocadas = 0;

	distribuiProfessores();

	for (auto i = 0; i < numProfessores_; i++) {
		//! Pega a referência ao professor naquele índice para poder adicionar disciplinas
		auto& professor = professores_[i];
		//! Enquanto o professor ainda precisar receber disciplinas
		//! Percorre a lista de disciplinas, parando se elas acabarem ou o professor já
		//! estiver cheio
		for (auto j = 0; j < disciplinasAlocadas.size() &&
		     contadorMinistradas[i] < professor.numMinistradas(); j++) {
			//! Verifica se o professor já ministra aquela disciplina,
			//! gera uma chance de o professor ministrá-la e testa se ela já está sendo ministrada
			//! pelo número máximo de professores que podem simultaneamente ministrar uma disciplina
			if (disciplinasAlocadas[j] < maxProfessores &&
				!professor.ministra(j) && rand.randomInt() % 100 < 25) {

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

bool CursoAleatorio::isEquivalente(int disc1, int disc2) const {
	// Executa um 'xor' dos pré-requisitos e co-requisitos
	// Se as disciplinas tiverem exatamente os mesmos pré-
	// e co-requisitos, elas são equivalentes

	auto incompativel = [&, this](int d) -> bool {
		// Essa linha é um xor. Se os dois forem falsos, o resultado
		// é 0, assim como se os dois forem verdadeiros. Se apenas
		// um for verdadeiro, o resutlado é 1
		auto incompatPreReq = (preRequisitos_[disc1][d]
			+ preRequisitos_[disc2][d]) % 2;
		auto incompatCoReq = (coRequisitos_[disc1][d]
			+ coRequisitos_[disc2][d]) % 2;

		// Como duas disciplinas são compatíveis para uma terceira
		// se e apenas se essa terceira for prerequisito das duas,
		// ou corequisitos das duas, ou não for nade de nenhuma das duas,
		// qualquer coisa diferente é uma incompatibilidade
		return incompatCoReq || incompatPreReq;
	};

	for (auto i = 0; i < numDisciplinas_; i++) {
		if (incompativel(i)) {
			return false;
		}
	}
	return true;
}

void CursoAleatorio::geraEquivalencia() {
	// Percorre todas as combinações possíveis de disciplinas, e verifica
	// se seus pre- e co-requisitos são idênticos. Se forem, marca
	// como equivalentes
	for (auto i = 0; i < numDisciplinas_; i++) {
		for (auto j = 0; j < numDisciplinas_; j++) {
			if (isEquivalente(i, j)) {
				equivalencias_[i][j] = true;
			}
		}
	}
}

void CursoAleatorio::geraTurmas() {
	// Converte um número maior ou igual a zero em uma letra
	auto numToLetra = [](int num) {
		char letra = 'A' + num;
		return std::string{letra};
	};

	for (auto i = 0; i < numDisciplinas_;) {
		auto turmasDiscAtual = rand.randomInt() % maxTurmas_;
		auto periodo = rand.randomInt() % numPeriodos_;

		discTurma_[i].first = periodo;
		discTurma_[i].second = numToLetra(0);

		auto n = std::min(turmasDiscAtual, numDisciplinas_ - i);

		for (auto j = 1; j < n; j++) {
			discTurma_[j].first = periodo;
			discTurma_[j].second = numToLetra(j);
		}

		i += n;
	}
}

void CursoAleatorio::finalizaDependencias() {
	int anterior = -1;

	for (auto i = 0; i < numDisciplinas_; i++) {
		if (discTurma_[i].second == "A") {
			anterior = i;
			continue;
		}

		for (auto j = 0; j < numDisciplinas_; j++) {
			preRequisitos_[i][j] = preRequisitos_[anterior][j];
			coRequisitos_[i][j] = coRequisitos_[anterior][j];
		}
	}
}

void CursoAleatorio::geraCapacidades() {
	// Gera um número no intervalo [min, max] e atribui à capacidade
	// da disciplina
	auto diferenca = capacidadeMaxima_ - capacidadeMinima_;
	for (auto i = 0; i < numDisciplinas_; i++) {
		capacidades_[i] = capacidadeMinima_ +
				(rand.randomInt() % (diferenca + 1));
	}
}
