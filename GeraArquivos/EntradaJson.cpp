#include "EntradaJson.h"
#include "json/json.h"
#include <fstream>
#include <unordered_map>
#include <numeric>

namespace EntradaJson {

	std::pair<CursoEntrada, std::vector<AlunoEntrada>> lerJson(std::string nomeArquivo) {
		// Abre o arquivo e testa se a leitura foi bem sucedida.
		std::ifstream entrada(nomeArquivo);
		if (entrada.bad())
			throw 1;

		// Cria um map que vai guardar o índice de uma determinada matéria
		// nos vetores
		std::unordered_map<std::string, int> discToInt{};

		// Cria um objeto para armazenar a estrutura do arquivo, e depois lê
		// o arquivo pra ele. Esse objeto guarda a estrutura do arquivo desde a raiz.
		Json::Value raiz;
		entrada >> raiz;

		// Cria um objeto que aponta para o objeto disciplinas do arquivo JSON
		const auto disciplinas = raiz["disciplinas"];
		// Cria um vector de strings que irá guardar os nomes das disciplinas do curso
		std::vector<std::string> nomeDisc{};
		// E outro que irá guardar os créditos de cada uma
		std::vector<int> creditos{};

		for (auto i = 0; i < disciplinas.size(); i++) {
			const auto nome = disciplinas[i]["id"].asString();
			nomeDisc.push_back(nome);
			discToInt[nome] = i;

			creditos.push_back(disciplinas[i]["carga"].asInt());
		}

		// Cria as matrizes de pré-requisitos e co-requisitos, e volta lendo a lista
		// de disciplinas para estabelecê-los 
		// 21/09/2015: o arquivo atual não possui os co-requisitos
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

		// Captura o número de professores
		auto numProfessores = raiz["professores"].size();

		// Estabelece qual o número de horários, períodos (camadas) e dias letivos
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

		// Percorre novamente a lista, criando a matriz de horários e marcando as
		// matérias ofertadas
		auto numHorarios = numHorariosDia * numDiasLetivos;
		std::vector<std::vector<bool>> matrizHorario(numHorarios,
		                                             std::vector<bool>(numDisciplinas, false));
		std::vector<bool> ofertadas(numDisciplinas, false);

		for (auto i = 0; i < horarios.size(); i++) {
			auto horarioAtual = horarios[i]["horario"].asInt();
			auto diaLetivoAtual = horarios[i]["semana"].asInt();

			auto horario = numHorariosDia * diaLetivoAtual + horarioAtual;
			auto discIndex = discToInt[horarios[i]["disciplina"].asString()];

			matrizHorario[horario][discIndex] = true;
			ofertadas[discIndex] = true;
		}

		// Soma o número de matérias marcadas como ofertadas
		auto numOfertadas = accumulate(begin(ofertadas), end(ofertadas), 0);
		// Cria o curso usando todos os parâmetros encontrados acima
		CursoEntrada curso(numDisciplinas, numPreRequisitos, numCoRequisitos, 
						   numHorarios, numOfertadas, numProfessores, numDiasLetivos,
						   numPeriodos);

		// Configura as estruturas das disciplinas e os horários do curso
		curso.setDisciplinas(move(nomeDisc), move(preRequisitos), move(coRequisitos),
							 move(ofertadas), move(creditos));
		curso.setHorarios(move(matrizHorario));

		// Lê a lista de alunos, criando objetos que armazenam os nomes e
		// as matérias cursadas e aprovadas
		auto alunos = raiz["alunoperfis"];
		std::vector<AlunoEntrada> vetorAlunos;

		for (auto i = 0; i < alunos.size(); i++) {
			// Pega o nome do aluno e cria um objeto com esse nome e as disciplinas do
			// curso que acabou de ser lido
			auto nome = alunos[i]["id"].asString();
			AlunoEntrada aluno{curso.preRequisitos(), curso.coRequisitos(), nome};

			// Inicializa uma lista de aprovações, com todas as disciplinas verdadeiras
			// aquelas na lista de "restantes" serão marcadas com falso
			std::vector<bool> aprovacoes(numDisciplinas, true);

			auto restantes = alunos[i]["restantes"];
			for (auto j = 0; j < restantes.size(); j++) {
				auto discIndex = discToInt[restantes[j].asString()];
				aprovacoes[discIndex] = false;
			}

			// Cria agora uma lista de cursadas, que inicialmente copia as aprovadas
			std::vector<bool> cursadas(aprovacoes);

			// As disciplinas que o aluno cursou mas foi reprovado estão marcadas como 
			// falso. Agora elas serão marcadas como verdadeiro na lista de cursadas.
			auto discCursadas = alunos[i]["cursadas"];
			for (auto j = 0; j < discCursadas.size(); j++) {
				auto discIndex = discToInt[restantes[j].asString()];
				cursadas[discIndex] = true;
			}

			// Estabelece as estruturas do aluno como as lidas agora
			aluno.setAprovacoes(move(aprovacoes));
			aluno.setCursadas(move(cursadas));

			// Move o aluno para dentro do vector
			vetorAlunos.push_back(std::move(aluno));
		}

		// Cria um par com o curso e o vector de alunos e o retorna
		return make_pair(curso, vetorAlunos);
	}
}
