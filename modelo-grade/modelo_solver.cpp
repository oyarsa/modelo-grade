#include "solucao.h"
#include <ilcp/cp.h>
#include <ilcplex/ilocplexi.h>

#include "curso.h"
#include "aluno.h"
#include "modelo_solver.h"

struct fagoc::Modelo_solver::impl
{
	explicit impl(const Solver& parent);
	~impl();

	void solve();
	std::shared_ptr<Solucao> solucao = nullptr;
	IloEnv env;
	const Solver& parent;
};

std::pair<int, std::string> split_curso_string(const std::string& curso_str)
{
	if (curso_str == "") {
		return {0, ""};
	}
	auto i = curso_str.find_first_of("-");
	return {stoi(curso_str.substr(0, i)), curso_str.substr(i + 1)};
}


fagoc::Modelo_solver::impl::impl(const Solver& parent)
	: env{}, parent{parent} 
{}

fagoc::Modelo_solver::impl::~impl()
{
	env.end();
}

void fagoc::Modelo_solver::impl::solve()
{
	/*******************************************************
	 *     INICIALIZAÇÃO DE VARIÁVEIS E REFERÊNCIAS        *
	 ******************************************************/

	const auto& nome_disciplina = parent.curso().nome_disciplinas();
	const auto& creditos = parent.curso().creditos();
	const auto& prerequisitos = parent.curso().pre_requisitos();
	const auto& corequisitos = parent.curso().co_requisitos();
	const auto& horario = parent.horario();
	const auto& ofertadas = parent.curso().ofertadas();
	const auto& equivalencias = parent.curso().equivalencias();
	const auto& disc_turma = parent.curso().disc_turma();
	const auto& periodos_minimos = parent.curso().periodos_minimos();
	const auto num_disciplinas = parent.curso().num_disciplinas();
	const auto num_horas = parent.curso().num_horarios();

	// Variáveis do aluno
	const auto& aprovacoes = parent.aluno().aprovacoes();
	const auto& cursadas = parent.aluno().cursadas(); 
	const auto& periodo_aluno = parent.aluno().periodo();
	const auto& turma_aluno = parent.aluno().turma();

	auto periodo_alu_num = split_curso_string(periodo_aluno).first;

	// Preferências do aluno (mesma turma/período)
	std::vector<char> pref(num_disciplinas, 0);
	for (auto d = 0u; d < num_disciplinas; d++) {
		const auto& periodo_disc = disc_turma[d].first;
		const auto& turma_disc = disc_turma[d].second;

		if (periodo_disc == periodo_aluno && turma_disc == turma_aluno) {
			pref[d] = 1;
		}
	}

	/******************************************************
	 *                ELABORAÇÃO DO MODELO                *
	 ******************************************************/
	
	IloModel mod{env};
	// Variáveis de decisão
	IloBoolVarArray y(env, num_disciplinas);

	// Carga horária
	IloExpr carga{env};
	for (auto d = 0u; d < num_disciplinas; d++) {
		carga += creditos[d] * y[d];
	}
	// Preferências
	IloExpr mesma_turma{env};
	for (auto d = 0u; d < num_disciplinas; d++) {
		mesma_turma += pref[d] * y[d];
	}
	IloExpr obj{env};
	// Função objetivo
	obj += carga + 0.1 * mesma_turma;
	mod.add(IloMaximize(env, obj));
	obj.end();

	/*****************************************************
	 *                   RESTRIÇÕES					     *
	 *****************************************************/

	// Período mínimo
	for (auto d = 0u; d < num_disciplinas; d++) {
		auto periodo_disc_num = split_curso_string(periodos_minimos[d]).first;
		mod.add(y[d] * periodo_disc_num <= periodo_alu_num);
	}
	// Pré-requisitos
	for (auto d = 0u; d < num_disciplinas; d++) {
		auto num_prereq = 0;
		auto prereq_aprov = 0;
		for (auto p = 0u; p < num_disciplinas; p++) {
			if (!prerequisitos[d][p]) {
				continue;
			}
			num_prereq++;
			for (auto i = 0u; i < num_disciplinas; i++) {
				if (equivalencias[p][i] && aprovacoes[i]) {
					prereq_aprov++;
					break;
				}
			}
		}
		mod.add(num_prereq * y[d] <= prereq_aprov);
	}

	// Co-requisitos
	for (auto d = 0u; d < num_disciplinas; d++) {
		for (auto c = 0u; c < num_disciplinas; c++) {
			if (corequisitos[d][c]) {
				auto coreq_cumprido = 0;
				for (auto i = 0u; i < num_disciplinas; i++) {
					if (equivalencias[c][i] && cursadas[i]) {
						coreq_cumprido = 1;
						break;
					}
				}
				mod.add(y[d] <= y[c] + coreq_cumprido);
			}
		}
	}

	// Disciplinas equivalentes
	for (auto d = 0u; d < num_disciplinas; d++) {
		IloExpr disc_equiv(env);
		for (auto e = 0u; e < num_disciplinas; e++) {
			disc_equiv += equivalencias[d][e] * y[e];
		}
		mod.add(disc_equiv <= 1);
		disc_equiv.end();
	}

	// Cria as restrições dos horários
	for (auto h = 0u; h < num_horas; h++) {
		IloExpr disc_concorrente(env);
		for (auto d = 0u; d < num_disciplinas; d++) {
			disc_concorrente += horario[h][d] * y[d];
		}
		mod.add(disc_concorrente <= 1);
		disc_concorrente.end();
	}

	// Disciplinas já aprovadas
	for (auto d = 0u; d < num_disciplinas; d++) {
		auto aprovacao_equivalente = 0;
		for (auto i = 0u; i < num_disciplinas; i++) {
			if (equivalencias[d][i] && aprovacoes[i]) {
				aprovacao_equivalente = 1;
				break;
			}
		}
		mod.add(y[d] <= 1 - aprovacao_equivalente);
	}

	// Disciplinas ofertadas
	for (auto d = 0u; d < num_disciplinas; d++) {
		mod.add(y[d] <= ofertadas[d]);
	}

	/********************************************************
	 *                RESOLUÇÃO DO MODELO                   *
	 ********************************************************/

	// Se o número de disciplinas for menor que 30, utiliza
	// o CPLEX. Se for maior, a versão community não resolve, então
	// utiliza-se o CP Optimizer
	IloNumArray solucao_cplex(env);
	double funcao_objetivo;
	if (num_disciplinas <= 30) {
		IloCplex cplex(mod);
		cplex.setOut(env.getNullStream());
		cplex.solve();


		funcao_objetivo = cplex.getObjValue();
		cplex.getValues(solucao_cplex, y);
	} else {
		IloCP cp(mod);
		cp.setOut(env.getNullStream());
		cp.solve();

		funcao_objetivo = cp.getObjValue();
		cp.getValues(y, solucao_cplex);
	}

	solucao.reset(new Solucao{num_disciplinas, funcao_objetivo, parent.aluno().nome()});
	// Atribui resposta às variáveis membro
	for (auto d = 0u; d < num_disciplinas; d++) {
		if (solucao_cplex[d]) {
			solucao->solucao_bool[d] = 1;
			solucao->nomes_disciplinas.push_back(nome_disciplina[d]);
		}
	}
}

std::shared_ptr<fagoc::Solucao> fagoc::Modelo_solver::solucao() const
{
	return impl_->solucao;
}


fagoc::Modelo_solver::Modelo_solver(const Curso& curso, const Aluno& aluno) 
	: Solver(curso, aluno), impl_{std::make_unique<impl>(*this)}
{}

fagoc::Modelo_solver::Modelo_solver(const Curso& curso, const Aluno& aluno,
									const std::vector<std::vector<char>>& horario) 
	: Solver(curso, aluno, horario), impl_{std::make_unique<impl>(*this)}
{}

void fagoc::Modelo_solver::solve()
{
	impl_->solve();
}

fagoc::Modelo_solver::~Modelo_solver() {}
