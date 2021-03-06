#include <unordered_map>
#include <fstream>
#include <utility>

#include "json/json.hxx"

#include "arquivos.h"


std::pair<fagoc::Curso, std::vector<fagoc::Aluno>>
fagoc::ler_json(std::string arquivo)
{
    std::ifstream entrada(arquivo);

    // Mapeia os nomes das disciplinas a �ndices nos vetores e matrizes
    std::unordered_map<std::string, int> nome_to_indice;

    Json::Value raiz;
    entrada >> raiz;

    /**********************************************
     *			   ENTRADA DISCIPLINAS            *
     **********************************************/

    // L� uma primeira vez para contar o n�mero de disciplinas e associar
    // os nomes a �ndices
    const auto& disciplinas = raiz["disciplinas"];
	auto num_disc = disciplinas.size();

    for (auto i = 0u; i < num_disc; i++) {
        nome_to_indice[disciplinas[i]["id"].asString()] = i;
    }

    // L� uma segunda vez, dessa vez registrando de fato as disciplinas na mem�ria
    std::vector<char> ofertadas(num_disc, 1);
	std::vector<Disciplina> disciplinas_vector;

    for (auto i = 0u; i < num_disc; i++) {
        auto disc_atual = nome_to_indice[disciplinas[i]["id"].asString()];
        const auto& prereq = disciplinas[i]["prerequisitos"];
        const auto& equiv = disciplinas[i]["equivalentes"];
		Disciplina disc{num_disc};

        for (auto j = 0u; j < prereq.size(); j++) {
            auto prereq_atual = nome_to_indice[prereq[j].asString()];
            disc.prerequisitos[prereq_atual] = 1;
        }

		disc.equivalentes[disc_atual] = 1;
        for (auto j = 0u; j < equiv.size(); j++) {
            auto equiv_atual = nome_to_indice[equiv[j].asString()];
            disc.equivalentes[equiv_atual] = 1;
        }

		disc.id = disciplinas[i]["id"].asString();
		disc.nome = disciplinas[i]["nome"].asString();
        disc.periodo = disciplinas[i]["periodo"].asString();
        disc.turma = disciplinas[i]["turma"].asString();
        disc.capacidade = disciplinas[i]["capacidade"].asInt();
        disc.periodo_minimo = disciplinas[i]["periodominimo"].asString();
		disc.credito = disciplinas[i]["carga"].asInt();
		ofertadas[disc_atual] = disciplinas[i]["ofertada"].asBool();

		disciplinas_vector.emplace_back(std::move(disc));
    }

    /**********************************************
     *			   ENTRADA HOR�RIOS               *
     **********************************************/

    // Registra o n�mero de hor�rios, per�odos e dias letivos 
    const auto& horarios = raiz["horario"];
    auto num_horarios_dia = 0;
    auto num_dias_letivos = 0;
    auto num_periodos = 0;

    for (auto i = 0u; i < horarios.size(); i++) {
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

    // Volta e l� o hor�rio novamente, desta vez montando de fato na mem�ria
    auto num_horarios = num_horarios_dia * num_dias_letivos;
    std::vector<std::vector<char>> matriz_horario(
        num_horarios, std::vector<char>(num_disc, 0));

    for (auto i = 0u; i < horarios.size(); i++) {
        auto horario_atual = horarios[i]["horario"].asInt();
        auto dia_letivo_atual = horarios[i]["semana"].asInt();

        auto horario = num_horarios_dia * dia_letivo_atual + horario_atual;
        auto disc_index = nome_to_indice[horarios[i]["professordisciplina"].asString()];

        matriz_horario[horario][disc_index] = 1;
    }

    Curso curso(move(disciplinas_vector),
		        move(matriz_horario),
                move(ofertadas),
				move(nome_to_indice),
                num_dias_letivos, num_periodos);


    /*********************************************
     *			   ENTRADA ALUNOS                *
     *********************************************/

    const auto& alunos = raiz["alunoperfis"];
    std::vector<Aluno> vet_alunos;

    for (auto i = 0u; i < alunos.size(); i++) {
        auto nome = alunos[i]["id"].asString();
        auto periodo = alunos[i]["periodo"].asString();
        auto turma = alunos[i]["turma"].asString();

        std::vector<char> aprovacoes(num_disc, 1);

        const auto& restantes = alunos[i]["restantes"];
        for (auto j = 0u; j < restantes.size(); j++) {
            auto disc_index = curso.id_to_indice().find(restantes[j].asString());
			if (disc_index != curso.id_to_indice().end()) {
				aprovacoes[disc_index->second] = 0;
			}
        }

        auto cursadas = aprovacoes;

        const auto& disc_cursadas = alunos[i]["cursadas"];
        for (auto j = 0u; j < disc_cursadas.size(); j++) {
            auto disc_index = curso.id_to_indice().find(disc_cursadas[j].asString());
			if (disc_index != curso.id_to_indice().end()) {
				cursadas[disc_index->second] = 1;
			}
        }

        vet_alunos.emplace_back(nome, move(aprovacoes), move(cursadas), 
								periodo, turma);
    }

    return make_pair(std::move(curso), move(vet_alunos));
}

void fagoc::gen_html(const Curso& curso,
                     const std::vector<Solucao>& solucoes,
                     std::string destino)
{
#if defined(_WIN32)
	std::string comando{"mkdir " + destino + " 2> NUL"};
#else
	std::string comando{"mkdir -p " + destino + " > /dev/null 2>&1"};
#endif
	system(comando.c_str()); 

    std::ofstream saida{destino + "resultado.html"};
    std::ifstream arq_css{arquivo_css};
    std::ostringstream buffer{};
    buffer << arq_css.rdbuf();
    auto css = buffer.str();
    std::ostringstream html_pkg;

    for (const auto& solucao : solucoes) {
        html_pkg << gen_html_aluno(curso, solucao, destino, css);
    }

    saida << std::nounitbuf << html_pkg.str() << std::endl;
}

std::string fagoc::gen_html_aluno(const Curso& curso, const Solucao& solucao,
                                  const std::string& destino,
                                  const std::string& css)
{
    std::ostringstream html;
    const std::string dias_semana[] = {"Segunda", "Ter�a", "Quarta", "Quinta",
									   "Sexta", "S�bado", "Domingo"};

    html << std::nounitbuf;
    html << "<!DOCTYPE html>\n"
            << "<html>\n"
            << "<style type=\"text/css\">"
            << css
            << "</style>"
            << "<body>\n"
            << "<h1 align='center' id='nome'>" + solucao.nome_aluno + ":<br></h1>"
            << "<table align='center' id='horarios'>\n";

    html << "<tr>\n";
    for (auto i = 0; i < curso.num_dias_letivos(); i++) {
        html << "<th>" << dias_semana[i] << "</th>\n";
    }
    html << "</tr>\n";

    auto horarios_dia = static_cast<std::size_t>(curso.num_horarios() / curso.num_dias_letivos());

    for (auto i = 0u; i < horarios_dia; i++) {
        html << "<tr>\n";
		for (auto j = 0; j < curso.num_dias_letivos(); j++) {
            auto encontrou = false;
            for (auto k = 0u; k < curso.num_disciplinas(); k++) {
				auto slot = j * horarios_dia + i;
                if (curso.horario()[slot][k] && solucao.solucao_bool[k]) {
                    html << "<td>"
                         << curso.disciplinas()[k].id 
						 << "</td>\n";
                    encontrou = true;
                    break;
                }
            }
            // Se chegar ao fim de todas as disciplinas e n�o encontrar nenhuma
            // que tenha sido escolhida, imprime um tra�o indicando slot vazio
            if (!encontrou) {
                html << "<td> ----- </td>\n";
            }
        }
        html << "</tr>\n";
    }

    html << "</table>\n"
            << "</body>\n"
            << "</html>\n";

#if defined(_WIN32)
    auto nome_arquivo = destino + "\\" + solucao.nome_aluno + ".html";
#else
    auto nome_arquivo = destino + "/" + solucao.nome_aluno + ".html";
#endif

    std::ofstream arquivo_saida(nome_arquivo);
    arquivo_saida << std::nounitbuf << html.str() << std::endl;

    // E tamb�m retorna a string (a mesma que foi escrita) para quem chamou
    return html.str();
}

