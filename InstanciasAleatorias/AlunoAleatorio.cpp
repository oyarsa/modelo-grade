#include "AlunoAleatorio.h"
#include "Aluno.h"
#include "aleatorio.h"
#include <algorithm>
#include <queue>

using aleatorio::randomInt;

AlunoAleatorio::AlunoAleatorio(const std::vector<std::vector<bool>>& preRequisitos,
             const std::vector<std::vector<bool>>& coRequisitos,
             std::string nome)
	: Aluno(preRequisitos, coRequisitos, nome) {

	init();
}

void AlunoAleatorio::init() {
	geraAprovacoes();
	geraCursadas();
}


void AlunoAleatorio::geraAprovacoes() {
	//! Gera aleatoriamente o número de disciplinas em que o aluno foi aprovado,
	//! sendo esse número no máximo metade do número de disciplinas do curso
	numAprovacoes = randomInt() % (numDisciplinas / 3);

	//! Preenche o vetor de aprovações com tantos 1's quanto o número de disciplinas
	//! que o aluno foi aprovado e depois sorteia
	fill(begin(aprovacoes_), begin(aprovacoes_) + numAprovacoes, true);
	shuffle(begin(aprovacoes_), end(aprovacoes_), aleatorio::geradorAleatorio());

	std::queue<int> preReqAnalisar{};
	//! Percorre o vetor de apravoções enfileirando as disciplinas aprovadas para analisar
	//! seus requisitos
	for (size_t i = 0; i < aprovacoes_.size(); i++) {
		if (aprovacoes_[i]) {
			preReqAnalisar.push(i);
		}
	}
	
	//! Perocorre a lista de disciplinas aprovadas, marcando seus requisitos como aprovados
	//! e inserindo-os na lista também para análise
	while (!preReqAnalisar.empty()) {
		auto discAtual = preReqAnalisar.front();
		preReqAnalisar.pop();
		for (size_t j = 0; j < preRequisitos_.size(); j++) {
			if (preRequisitos_[discAtual][j] || coRequisitos_[discAtual][j]) {
				aprovacoes_[j] = true;
				preReqAnalisar.push(j);
			}
			
		}
	}
}

void AlunoAleatorio::geraCursadas() {
	//! Gera aleatoriamente o número de disciplinas cursadas, e depois subtrai o número de
	//! aprovadas para verificar se sobrou alguma, ou seja, se o aluno já foi reprovado em alguma.
	//! Testa-se então se o número é negativo. Se for, ele passa a ser igual ao número de
	//! aprovações (o aluno nunca foi reprovado) e a função termina
	//! O vector de cursadas precisa ser, no mínimo, igual ao de aprovações.
	//! Então ele o copia;
	cursadas_ = aprovacoes_;

	numCursadas = randomInt() % (numDisciplinas / 2);
	auto numCursadasAlocar = numCursadas - numAprovacoes;

	if (numCursadasAlocar < 0) {
		numCursadas = numAprovacoes;
		return;
	}

	auto cursadasAlocadas = 0;

	//! Percorre o vetor, escolhendo aleatoriamente quais disciplinas marcar como cursadas
	//! Percorre então seus pré-requisitos, marcando-os como cursados também. O loop para
	//! quando acaba o vetor de cursadas ou se o número de cursadas alocadas se igualar ao esperado
	for (size_t i = 0; i < cursadas_.size(); i++) {
		if (cursadasAlocadas >= numCursadasAlocar) {
			if (!aprovacoes_[i])
				break;
		}
		if (!cursadas_[i] && randomInt() % 3) {
			cursadas_[i] = true;
			for (size_t j = 0; j < preRequisitos_.size(); j++) {
				if (preRequisitos_[i][j] && !aprovacoes_[j]) {
					cursadas_[i] = false;
					break;
				}
				if (coRequisitos_[i][j] && !cursadas_[j]) {
					cursadas_[j] = true;
					cursadasAlocadas++;
				}
			}
			cursadasAlocadas++;
		}
	}
}
