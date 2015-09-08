#pragma warning(disable: 4996)

#include "ResolveInstancia.h"
#include <Instancia.h>
#include <ilcp/cp.h>

ResolveInstancia::ResolveInstancia(Curso const* curso, AlunoPtr aluno)
	: curso_{curso},
	  aluno_{move(aluno)},
	  env{},
	  solucao_{},
	  valorFinal_{} {}

void ResolveInstancia::solve() {

	// Inicialização das variáveis do curso
	const auto& nomeDisciplinas = curso_->nomeDisciplinas();
	const auto& creditos = curso_->creditos();
	const auto& preRequisitos = curso_->preRequisitos();
	const auto& coRequisitos = curso_->coRequisitos();
	const auto& horarios = curso_->horarios();
	const auto& ofertadas = curso_->ofertadas();
	const auto numDisciplinas = creditos.size();
	const auto numHorarios = horarios.size();

	// Variáveis do aluno
	const auto& aprovacoes = aluno_->aprovacoes();
	const auto& cursadas = aluno_->cursadas();

	// ------ Elaboração do modelo ----------
	IloModel mod(env);

	// Variáveis de decisão
	IloBoolVarArray y(env, numDisciplinas);

	// Função objetivo
	IloExpr obj(env);
	for (size_t d = 0; d < numDisciplinas; d++) {
		obj += creditos[d] * y[d];
	}
	mod.add(IloMaximize(env, obj));
	obj.end();

	// ------ Restrições --------------------

	// Pré-requisitos
	for (size_t d = 0; d < numDisciplinas; d++) {
		auto numPreRequisitos = 0;
		auto preReqAprovados = 0;
		for (size_t p = 0; p < numDisciplinas; p++) {
			if (preRequisitos[d][p]) {
				numPreRequisitos++;
				if (aprovacoes[p]) {
					preReqAprovados++;
				}
			}
		}
		mod.add(numPreRequisitos * y[d] <= preReqAprovados);
	}

	// Co-requisitos
	for (size_t d = 0; d < numDisciplinas; d++) {
		for (size_t c = 0; c < numDisciplinas; c++) {
			if (coRequisitos[d][c]) {
				mod.add(y[d] <= y[c] + cursadas[c]);
			}
		}
	}

	// Horários, cria as restrições dois a dois
	for (size_t h = 0; h < numHorarios - 1; h += 2) {
		IloExpr disciplinasConcorrentes(env);
		for (size_t d = 0; d < numDisciplinas; d++) {
			disciplinasConcorrentes += 
				(horarios[h][d] + horarios[h + 1][d]) * y[d];
		}
		mod.add(disciplinasConcorrentes <= 2);
		disciplinasConcorrentes.end();
	}

	// Disciplinas já aprovadas
	for (size_t d = 0; d < numDisciplinas; d++) {
		mod.add(y[d] <= 1 - aprovacoes[d]);
	}

	// Disciplinas ofertadas
	for (size_t d = 0; d < numDisciplinas; d++) {
		mod.add(y[d] <= ofertadas[d]);
	}

	// ----- Resolve o modelo -------

	// Se o número de disciplinas for menor que 30, utiliza
	// o CPLEX. Se for maior, a versão community não resolve, então
	// utiliza-se o CP Optimizer
	IloNumArray solucao(env);
	if (numDisciplinas <= 30) {
		IloCplex cplex(mod);
		cplex.setOut(env.getNullStream());
		cplex.solve();

		valorFinal_ = cplex.getObjValue();
		cplex.getValues(solucao, y);
	}
	else {
		IloCP cp(mod);
		cp.setOut(env.getNullStream());
		cp.solve();

		valorFinal_ = cp.getObjValue();
		cp.getValues(y, solucao);
	}

	// Atribui resposta às variáveis membro
	for (size_t d = 0; d < numDisciplinas; d++) {
		if (solucao[d]) {
			solucao_.push_back(d);
			disciplinas_.push_back(nomeDisciplinas[d]);
		}
	}
}

std::vector<int> ResolveInstancia::solucao() const {
	return solucao_;
}

std::vector<std::string> ResolveInstancia::disciplinas() const {
	return disciplinas_;
}

int ResolveInstancia::valorFinal() const {
	return static_cast<int>(valorFinal_);
}

ResolveInstancia::~ResolveInstancia() {
	env.end();
}
