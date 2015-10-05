#pragma warning(disable: 4996)

#include "SolverHandler.h"
#include <Instancia.h>
#include <ilcp/cp.h>
#include <cstdio>

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
	const auto numDisciplinas = curso_->numDisciplinas();
	const auto numHorarios = curso_->numHorarios();

	// Variáveis do aluno
	const auto& aprovacoes = aluno_->aprovacoes();
	const auto& cursadas = aluno_->cursadas();
	const auto& periodoAluno = aluno_->periodo();
	const auto& turmaAluno = aluno_->turma();

	std::vector<bool> pref(numDisciplinas, false);

	printf("Aluno: %s\n", aluno_->nome().c_str());
	printf("Periodo: %d Turma: %s\n", periodoAluno, turmaAluno.c_str());

	for (auto i = 0; i < numDisciplinas; i++) {
		const auto& periodoDisc = discTurma[i].first;
		const auto& turmaDisc = discTurma[i].second;

		printf("Periodo disc: %d Turma disc: %s\n", periodoDisc, turmaDisc.c_str());

		if (periodoDisc == periodoAluno && turmaDisc == turmaAluno) {
			pref[i] = true;
			std::cout << "Entrou\n";
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

	// Pré-requisitos
	for (auto d = 0; d < numDisciplinas; d++) {
		auto numPreRequisitos = 0;
		auto preReqAprovados = 0;
		for (auto p = 0; p < numDisciplinas; p++) {
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
	for (auto d = 0; d < numDisciplinas; d++) {
		for (auto c = 0; c < numDisciplinas; c++) {
			if (coRequisitos[d][c]) {
				mod.add(y[d] <= y[c] + cursadas[c]);
			}
		}
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
		mod.add(y[d] <= 1 - aprovacoes[d]);
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

	std::cout << aluno_->nome() << "> " << valorFinal_ << '\n';
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