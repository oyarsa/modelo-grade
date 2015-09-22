#include "GeraJson.h"
#include <json/json.h>
#include <fstream>
#include <iostream>

bool geraJson::escreveJson(std::string caminho, Curso const* pCurso, std::vector<AlunoPtr>& alunos) {
	std::ofstream saida{caminho};

	if (saida.bad())
		return false;

	Json::Value raiz;

	// Inicialização das variáveis do curso
	const auto& nomeDisciplinas = pCurso->nomeDisciplinas();
	const auto& creditos = pCurso->creditos();
	const auto& preRequisitos = pCurso->preRequisitos();
	const auto& horarios = pCurso->horarios();
	const auto& professores = pCurso->professores();
	const auto numDisciplinas = pCurso->numDisciplinas();
	const auto numHorarios = pCurso->numHorarios();
	const auto numProfessores = pCurso->numProfessores();
	const auto numDiasLetivos = pCurso->numDiasLetivos();

	for (auto i = 0; i < numDisciplinas; i++) {
		Json::Value disciplinaAtual;
		disciplinaAtual["id"] = nomeDisciplinas[i];
		disciplinaAtual["nome"] = nomeDisciplinas[i];
		disciplinaAtual["carga"] = creditos[i];
		disciplinaAtual["periodo"] = 0;
		disciplinaAtual["curso"] = "C";

		for (auto j = 0; j < numDisciplinas; j++) {
			if (preRequisitos[i][j])
				disciplinaAtual["prerequisitos"].append(nomeDisciplinas[j]);
		}

		raiz["disciplinas"].append(disciplinaAtual);
	}

	if (numProfessores == 0)
		raiz["professores"] = Json::Value{Json::arrayValue};

	for (auto i = 0; i < numProfessores; i++) {
		Json::Value professorAtual;
		professorAtual["id"] = professores[i].nome();
		professorAtual["nome"] = professores[i].nome();

		auto& ministradas = professores[i].disciplinasMinistradas();

		if (ministradas.empty()) {
			professorAtual["competencias"] = Json::Value(Json::arrayValue);
		}
		else {
			for (auto& disciplina : ministradas) {
				professorAtual["competencias"].append(nomeDisciplinas[disciplina]);
			}
		}

		professorAtual["diasdisponiveis"] = Json::Value{Json::arrayValue};

		raiz["professores"].append(professorAtual);
	}

	raiz["professordisciplinas"] = Json::Value{Json::arrayValue};

	for (auto i = 0; i < alunos.size(); i++) {
		const auto aluno = alunos[i].get();
		Json::Value alunoAtual;
		alunoAtual["id"] = aluno->nome();
		alunoAtual["peso"] = 1;

		const auto& aprovacoes = aluno->aprovacoes();
		const auto& cursadas = aluno->cursadas();

		for (auto j = 0; j < numDisciplinas; j++) {
			if (!aprovacoes[j])
				alunoAtual["restantes"].append(nomeDisciplinas[j]);
			else if (cursadas[j])
				alunoAtual["cursadas"].append(nomeDisciplinas[j]);
		}

		raiz["alunoperfis"].append(alunoAtual);
	}

	auto horariosDia = numHorarios / numDiasLetivos;

	for (auto i = 0; i < numHorarios; i++) {
		for (auto j = 0; j < numDisciplinas; j++) {
			if (!horarios[i][j])
				continue;

			Json::Value horarioAtual;
			horarioAtual["horario"] = i % horariosDia;
			horarioAtual["semana"] = i / horariosDia;
			horarioAtual["camada"] = 0;
			horarioAtual["disciplina"] = nomeDisciplinas[j];

			raiz["horario"].append(horarioAtual);
		}
	}
	saida << raiz;

	return true;
}
