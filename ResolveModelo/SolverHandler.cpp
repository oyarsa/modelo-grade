#include "SolverHandler.h"
#include <Instancia.h>
#include <ilcp/cp.h>

SolverHandler::SolverHandler(Curso const* curso, AlunoPtr aluno)
	: curso_{curso},
	  aluno_{move(aluno)},
	  env{},
	  solucao_(curso->numDisciplinas(), false),
	  valorFinal_{} {}

void SolverHandler::solve() {

	// Inicialização das variáveis do curso
	const auto& nomeDisciplinas = curso_->nomeDisciplinas();
	const auto& creditos = curso_->creditos();
	const auto& preRequisitos = curso_->preRequisitos();
	const auto& coRequisitos = curso_->coRequisitos();
	const auto& horarios = curso_->horarios();
	const auto& ofertadas = curso_->ofertadas();
	const auto& equivalencias = curso_->equivalencias();
	const auto& discTurma = curso_->discTurma();
	const auto& periodosMinimos = curso_->periodosMinimos();
	const auto numDisciplinas = curso_->numDisciplinas();
	const auto numHorarios = curso_->numHorarios();

	// Variáveis do aluno
	const auto& aprovacoes = aluno_->aprovacoes();
	const auto& cursadas = aluno_->cursadas();
	const auto& periodoAluno = aluno_->periodo();
	const auto& turmaAluno = aluno_->turma();

	std::vector<bool> pref(numDisciplinas, false);

	for (auto i = 0; i < numDisciplinas; i++) {
		const auto& periodoDisc = discTurma[i].first;
		const auto& turmaDisc = discTurma[i].second;

		if (periodoDisc == periodoAluno && turmaDisc == turmaAluno) {
			pref[i] = true;
		}
	}


	// ------ Elaboração do modelo ----------
	IloModel mod(env);

	// Variáveis de decisão
	IloBoolVarArray y(env, numDisciplinas);

	// Função objetivo
	IloExpr carga(env);
	for (auto d = 0; d < numDisciplinas; d++) {
		carga += creditos[d] * y[d];
	}
	IloExpr mesmaTurma(env);
	for (auto d = 0; d < numDisciplinas; d++) {
		mesmaTurma += y[d] * pref[d];
	}
	IloExpr obj(env);
	obj += carga + 0.1 * mesmaTurma;
	mod.add(IloMaximize(env, obj));
	obj.end();

	// ------ Restrições --------------------

	// Período mínimo
	for (auto d = 0; d < numDisciplinas; d++) {
		mod.add(y[d] * periodosMinimos[d] <= periodoAluno);
	}

	// Pré-requisitos
	for (auto d = 0; d < numDisciplinas; d++) {
		auto numPreRequisitos = 0;
		auto preReqAprovados = 0;
		for (auto p = 0; p < numDisciplinas; p++) {
			if (preRequisitos[d][p]) {
				numPreRequisitos++;
				for (auto i = 0; i < numDisciplinas; i++) {
					if (equivalencias[p][i] && aprovacoes[i]) {
						preReqAprovados++;
						break;
					}
				}
			}
		}
		mod.add(numPreRequisitos * y[d] <= preReqAprovados);
	}

	// Co-requisitos
	for (auto d = 0; d < numDisciplinas; d++) {
		for (auto c = 0; c < numDisciplinas; c++) {
			if (coRequisitos[d][c]) {
				auto coReqCumprido = false;
				for (auto i = 0; i < numDisciplinas; i++) {
					if (equivalencias[c][i] && cursadas[i]) {
						coReqCumprido = true;
						break;
					}
				}
				mod.add(y[d] <= y[c] + coReqCumprido);
			}
		}
	}

	// Disciplinas equivalentes
	for (auto d = 0; d < numDisciplinas; d++) {
		IloExpr disciplinasEquivalentes(env);
		for (auto e = 0; e < numDisciplinas; e++) {
			disciplinasEquivalentes += equivalencias[d][e] * y[e];
		}
		mod.add(disciplinasEquivalentes <= 1);
		disciplinasEquivalentes.end();
	}

	// Cria as restrições dos horários
	for (auto h = 0; h < numHorarios; h++) {
		IloExpr disciplinasConcorrentes(env);
		for (auto d = 0; d < numDisciplinas; d++) {
			disciplinasConcorrentes += horarios[h][d] * y[d];
		}
		mod.add(disciplinasConcorrentes <= 1);
		disciplinasConcorrentes.end();
	}

	// Disciplinas já aprovadas
	for (auto d = 0; d < numDisciplinas; d++) {
		auto aprovadoEquivalente = false;
		for (auto i = 0; i < numDisciplinas; i++) {
			if (equivalencias[d][i] && aprovacoes[i]) {
				aprovadoEquivalente = true;
				break;
			}
		}
		mod.add(y[d] <= 1 - aprovadoEquivalente);
	}

	// Disciplinas ofertadas
	for (auto d = 0; d < numDisciplinas; d++) {
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
	for (auto d = 0; d < numDisciplinas; d++) {
		if (solucao[d]) {
			solucao_[d] = true;
			disciplinas_.push_back(nomeDisciplinas[d]);
		}
	}
}

std::vector<bool> SolverHandler::solucao() const {
	return solucao_;
}

std::vector<std::string> SolverHandler::disciplinas() const {
	return disciplinas_;
}

double SolverHandler::valorFinal() const {
	return valorFinal_;
}

SolverHandler::~SolverHandler() {
	env.end();
}

Aluno const* SolverHandler::aluno() const {
	return aluno_.get();
}