#include "arquivos.h"
#include <unordered_map>
#include <fstream>
#include "json/json.hxx"

std::pair<fagoc::Curso, std::vector<fagoc::Aluno>> fagoc::ler_json(std::string arquivo)
{
	std::ifstream entrada(arquivo);

	std::unordered_map<std::string, int> disc_to_int;

	Json::Value raiz;
	entrada >> raiz;

	const auto& disciplinas = raiz["disciplinas"];
	std::vector<std::string> nome_disc;
	std::vector<int> creditos;

	for (std::size_t i = 0; i < disciplinas.size(); i++) {
		auto nome = disciplinas[i]["id"].asString();
		nome_disc.push_back(nome);
		disc_to_int[nome] = i;

		creditos.push_back(disciplinas[i]["carga"].asInt());
	}

	auto num_disc = creditos.size();

	std::vector<std::vector<char>> prerequisitos(num_disc, std::vector<char>(num_disc, 0));
	std::vector<std::vector<char>> corequisitos(num_disc, std::vector<char>(num_disc, 0));
	std::vector<std::vector<char>> equivalencias(num_disc, std::vector<char>(num_disc, 0));
	std::vector<std::pair<int, std::string>> disc_turma(num_disc);
	std::vector<int> periodo_minimo(num_disc);
	std::vector<int> capacidades(num_disc);

	for (std::size_t i = 0; i < disciplinas.size(); i++) {
		const auto& prereq = disciplinas[i]["prerequisitos"];
		const auto& equiv = disciplinas[i]["equivalentes"];
		auto disc_atual = disc_to_int[disciplinas[i]["id"].asString()];

		for (std::size_t j = 0; j < prereq.size(); j++) {
			auto prereq_atual = disc_to_int[prereq[j].asString()];
			prerequisitos[disc_atual][prereq_atual] = 1;
		}

		for (std::size_t j = 0; j < equiv.size(); j++) {
			auto equiv_atual = disc_to_int[equiv[j].asString()];
			equivalencias[disc_atual][equiv_atual] = 1;
		}

		disc_turma[disc_atual].first = disciplinas[i]["periodo"].asInt();
		disc_turma[disc_atual].second = disciplinas[i]["turma"].asString();
		capacidades[disc_atual] = disciplinas[i]["capacidade"].asInt();
		periodo_minimo[disc_atual] = disciplinas[i]["periodominimo"].asInt();
	}

	const auto& horarios = raiz["horario"];
	auto num_horarios_dia = 0;
	auto num_dias_letivos = 0;
	auto num_periodos = 0;

	for (std::size_t i = 0; i < horarios.size(); i++) {
		auto horario_atual = horarios[i]["horario"].asInt();
		if (horario_atual + 1 > num_horarios_dia) {
			num_horarios_dia = horario_atual + 1;
		}

		auto dia_letivo_atual = horarios[i]["semana"].asInt();
		if (dia_letivo_atual + 1 > num_dias_letivos) {
			num_dias_letivos = dia_letivo_atual + 1;
		}

		auto periodo_atual = horarios[i]["camada"].asInt();
		if (periodo_atual + 1 > num_periodos) {
			num_periodos = periodo_atual + 1;
		}
	}

	auto num_horarios = num_horarios_dia * num_dias_letivos;
	std::vector<std::vector<char>> matriz_horario(num_horarios, std::vector<char>(num_disc, 0));
	std::vector<char> ofertadas(num_disc, 0);

	for (std::size_t i = 0; i < horarios.size(); i++) {
		auto horario_atual = horarios[i]["horario"].asInt();
		auto dia_letivo_atual = horarios[i]["semana"].asInt();

		auto horario = num_horarios_dia * dia_letivo_atual + horario_atual;
		auto discIndex = disc_to_int[horarios[i]["professordisciplina"].asString()];

		matriz_horario[horario][discIndex] = 1;
		ofertadas[discIndex] = 1;
	}

	fagoc::Curso curso(std::move(creditos), std::move(prerequisitos), std::move(corequisitos),
					   std::move(matriz_horario), std::move(ofertadas), std::move(equivalencias),
					   std::move(disc_turma), std::move(periodo_minimo), std::move(nome_disc),
					   std::move(capacidades));


	const auto& alunos = raiz["alunoperfis"];
	std::vector<fagoc::Aluno> vet_alunos;

	for (std::size_t i = 0; i < alunos.size(); i++) {
		auto nome = alunos[i]["id"].asString();
		auto periodo = alunos[i]["periodo"].asInt();
		auto turma = alunos[i]["turma"].asString();

		std::vector<char> aprovacoes(num_disc, 1);

		const auto& restantes = alunos[i]["restantes"];
		for (std::size_t j = 0; j < restantes.size(); j++) {
			auto disc_index = disc_to_int[restantes[j].asString()];
			aprovacoes[disc_index] = 0;
		}

		auto cursadas(aprovacoes);

		const auto& disc_cursadas = alunos[i]["cursadas"];
		for (std::size_t j = 0; j < disc_cursadas.size(); j++) {
			auto disc_index = disc_to_int[disc_cursadas[j].asString()];
			cursadas[disc_index] = 1;
		}

		vet_alunos.push_back(fagoc::Aluno(nome, std::move(aprovacoes), std::move(cursadas), periodo, turma));
	}

	return std::make_pair(std::move(curso), std::move(vet_alunos));
}
