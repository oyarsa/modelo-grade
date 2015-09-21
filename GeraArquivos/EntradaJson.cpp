#include "EntradaJson.h"
#include <json/json.h>
#include <fstream>
#include <unordered_map>

namespace EntradaJson {

	std::tuple<CursoEntrada, std::vector<AlunoEntrada>> lerJson(std::string nomeArquivo) {
		std::ifstream entrada(nomeArquivo);

		std::unordered_map<std::string, int> discToInt{};

		Json::Value raiz;
		entrada >> raiz;

		const auto disciplinas = raiz["disciplinas"];
		std::vector<std::string> nomeDisc{};
		std::vector<int> creditos{};

		for (auto i = 0; i < disciplinas.size(); i++) {
			const auto nome = disciplinas[i]["id"].asString();
			nomeDisc.push_back(nome);
			discToInt[nome] = i;

			creditos.push_back(disciplinas[i]["carga"].asInt());
		}
		auto numDisciplinas = creditos.size();
		auto numPreRequisitos = 0;
		auto numCoRequisitos = 0;
		std::vector<std::vector<bool>> preRequisitos(numDisciplinas,
		                                             std::vector<bool>(numDisciplinas, false));
		std::vector<std::vector<bool>> coRequisitos(numDisciplinas,
		                                            std::vector<bool>(numDisciplinas, false));
		for (auto i = 0; i < disciplinas.size(); i++) {
			const auto preReq = disciplinas[i]["prerequisitos"];
			auto discAtual = discToInt[disciplinas[i]["id"].asString()];

			numPreRequisitos += preReq.size();
			for (auto j = 0; j < preReq.size(); j++) {
				auto preReqAtual = discToInt[preReq[j].asString()];
				preRequisitos[discAtual][preReqAtual] = true;
			}
		}

		auto numProfessores = raiz["professores"].size();

		auto horarios = raiz["horario"];
		auto numHorariosDia = 0;
		auto numDiasLetivos = 0;
		auto numPeriodos = 0;

		for (auto i = 0; i < horarios.size(); i++) {
			auto horarioAtual = horarios[i]["horario"].asInt();
			if (horarioAtual + 1 > numHorariosDia)
				numHorariosDia = horarioAtual + 1;

			auto diaLetivoAtual = horarios[i]["semana"].asInt();
			if (diaLetivoAtual + 1 > numDiasLetivos)
				numDiasLetivos = diaLetivoAtual + 1;

			auto periodoAtual = horarios[i]["camada"].asInt();
			if (periodoAtual + 1 > numPeriodos)
				numPeriodos = periodoAtual + 1;
		}

		auto numHorarios = numHorariosDia * numDiasLetivos;
		std::vector<std::vector<bool>> matrizHorario(numHorarios,
		                                             std::vector<bool>(numDisciplinas, false));

		for (auto i = 0; i < horarios.size(); i++) {
			auto horarioAtual = horarios[i]["horario"].asInt();
			auto diaLetivoAtual = horarios[i]["semana"].asInt();

			auto horario = numHorariosDia * diaLetivoAtual + horarioAtual;
			auto discIndex = discToInt[horarios[i]["disciplinas"].asString()];

			matrizHorario[horario][discIndex] = true;
		}

		CursoEntrada curso(numDisciplinas, numPreRequisitos, 0, numHorarios, 0, 
						   numProfessores, numDiasLetivos, numPeriodos);

	}
}
