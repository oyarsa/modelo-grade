#include "modelo_solver.h"
#include "solucao.h"
#include <ilcp/cp.h>

std::pair<int, std::string> split_curso_string(const std::string& curso_str)
{
	auto i = curso_str.find_first_of("-");
	return std::make_pair(std::stoi(curso_str.substr(0, i)), curso_str.substr(i + 1));
}


fagoc::Modelo_solver::Modelo_solver(const Curso& curso, const Aluno& aluno)
	: Solver{curso, aluno}, env_{} {}

fagoc::Modelo_solver::~Modelo_solver()
{
	env_.end();
}

void fagoc::Modelo_solver::solve()
{
	/******************************************************
	 *     INICIALIZAÇÃO DE VARIÁVEIS E REFERÊNCIAS       *
	 ******************************************************/

	const auto& nome_disciplina = curso_.nome_disciplinas();
	const auto& creditos = curso_.creditos();
	const auto& prerequisitos = curso_.pre_requisitos();
	const auto& corequisitos = curso_.co_requisitos();
	const auto& horarios = curso_.horario();
	const auto& ofertadas = curso_.ofertadas();
	const auto& equivalencias = curso_.equivalencias();
	const auto& disc_turma = curso_.disc_turma();
	const auto& periodos_minimos = curso_.periodos_minimos();
	const auto num_disciplinas = curso_.num_disciplinas();
	const auto num_horas = curso_.num_horarios();

	// Variáveis do aluno
	const auto& aprovacoes = aluno_.aprovacoes();
	const auto& cursadas = aluno_.cursadas();
	const auto& periodo_aluno = aluno_.periodo();
	const auto& turma_aluno = aluno_.turma();

	auto periodo_alu_num = split_curso_string(periodo_aluno).first;

	// Preferências do aluno (mesma turma/período)
	std::vector<char> pref(num_disciplinas, 0);
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		const auto& periodo_disc = disc_turma[d].first;
		const auto& turma_disc = disc_turma[d].second;

		if (periodo_disc == periodo_aluno && turma_disc == turma_aluno) {
			pref[d] = 1;
		}
	}

	// -------- Elaboração do modelo ---------
	IloModel mod{env_};
	// Variáveis de decisão
	IloBoolVarArray y(env_, num_disciplinas);

	// Carga horária
	IloExpr carga{env_};
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		carga += creditos[d] * y[d];
	}
	// Preferências
	IloExpr mesma_turma{env_};
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		mesma_turma += pref[d] * y[d];
	}
	IloExpr obj{env_};
	// Função objetivo
	obj += carga + 0.1 * mesma_turma;
	mod.add(IloMaximize(env_, obj));
	obj.end();

	// -------- Restrições ---------
	// Período mínimo
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		auto periodo_disc_num = split_curso_string(periodos_minimos[d]).first;
		mod.add(y[d] * periodo_disc_num <= periodo_alu_num);
	}
	// Pré-requisitos
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		auto num_prereq = 0;
		auto prereq_aprov = 0;
		for (std::size_t p = 0; p < num_disciplinas; p++) {
			if (prerequisitos[d][p]) {
				num_prereq++;
			}
			for (std::size_t i = 0; i < num_disciplinas; i++) {
				if (equivalencias[p][i] && aprovacoes[i]) {
					prereq_aprov++;
					break;
				}
			}
		}
		mod.add(num_prereq * y[d] <= prereq_aprov);
	}

	// Co-requisitos
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		for (std::size_t c = 0; c < num_disciplinas; c++) {
			if (corequisitos[d][c]) {
				auto coreq_cumprido = 0;
				for (std::size_t i = 0; i < num_disciplinas; i++) {
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
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		IloExpr disc_equiv(env_);
		for (std::size_t e = 0; e < num_disciplinas; e++) {
			disc_equiv += equivalencias[d][e] * y[e];
		}
		mod.add(disc_equiv <= 1);
		disc_equiv.end();
	}

	// Cria as restrições dos horários
	for (std::size_t h = 0; h < num_horas; h++) {
		IloExpr disc_concorrente(env_);
		for (std::size_t d = 0; d < num_disciplinas; d++) {
			disc_concorrente += horarios[h][d] * y[d];
		}
		mod.add(disc_concorrente <= 1);
		disc_concorrente.end();
	}

	// Disciplinas já aprovadas
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		auto aprovacao_equivalente = false;
		for (std::size_t i = 0; i < num_disciplinas; i++) {
			if (equivalencias[d][i] && aprovacoes[i]) {
				aprovacao_equivalente = true;
				break;
			}
		}
		mod.add(y[d] <= 1 - aprovacao_equivalente);
	}

	// Disciplinas ofertadas
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		mod.add(y[d] <= ofertadas[d]);
	}

	// ----- Resolve o modelo -------

	// Se o número de disciplinas for menor que 30, utiliza
	// o CPLEX. Se for maior, a versão community não resolve, então
	// utiliza-se o CP Optimizer
	IloNumArray solucao_cplex(env_);
	double funcao_objetivo;
	if (num_disciplinas <= 30) {
		IloCplex cplex(mod);
		cplex.setOut(env_.getNullStream());
		cplex.solve();


		funcao_objetivo = cplex.getObjValue();
		cplex.getValues(solucao_cplex, y);
	} else {
		IloCP cp(mod);
		cp.setOut(env_.getNullStream());
		cp.solve();

		funcao_objetivo = cp.getObjValue();
		cp.getValues(y, solucao_cplex);
	}

	solucao_.reset(new Solucao{num_disciplinas, funcao_objetivo, aluno_.nome()});
	// Atribui resposta às variáveis membro
	for (std::size_t d = 0; d < num_disciplinas; d++) {
		if (solucao_cplex[d]) {
			solucao_->solucao_bool[d] = 1;
			solucao_->nomes_disciplinas.push_back(nome_disciplina[d]);
		}
	}
}

std::shared_ptr<fagoc::Solucao> fagoc::Modelo_solver::solucao() const
{
	return solucao_;
}

