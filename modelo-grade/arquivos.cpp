#include <unordered_map>
#include <fstream>
#include <utility>

#include "json/json.hxx"

#include "arquivos.h"


std::pair<fagoc::Curso, std::vector<fagoc::Aluno>>
fagoc::ler_json(std::string arquivo)
{
    std::ifstream entrada(arquivo);

    // Mapeia os nomes das disciplinas a índices nos vetores e matrizes
    std::unordered_map<std::string, int> nome_to_indice;

    Json::Value raiz;
    entrada >> raiz;

    /**********************************************
     *			   ENTRADA DISCIPLINAS            *
     **********************************************/

    // Lê uma primeira vez para contar o número de disciplinas e associar
    // os nomes a índices
    const auto& disciplinas = raiz["disciplinas"];
    std::vector<std::string> nome_disc;
    std::vector<int> creditos;

    for (auto i = 0u; i < disciplinas.size(); i++) {
        auto nome = disciplinas[i]["id"].asString();
        nome_disc.push_back(nome);
        nome_to_indice[nome] = i;

        creditos.push_back(disciplinas[i]["carga"].asInt());
    }

    auto num_disc = creditos.size();

    // Lê uma segunda vez, dessa vez registrando de fato as disciplinas na memória
    std::vector<std::vector<char>> prerequisitos(num_disc,
                                                 std::vector<char>(num_disc, 0));
    std::vector<std::vector<char>> corequisitos(num_disc,
                                                std::vector<char>(num_disc, 0));
    std::vector<std::vector<char>> equivalencias(num_disc,
                                                 std::vector<char>(num_disc, 0));
    std::vector<std::pair<std::string, std::string>> disc_turma(num_disc);
    std::vector<std::string> periodo_minimo(num_disc);
    std::vector<int> capacidades(num_disc);
    std::vector<char> ofertadas(num_disc, 1);

    for (auto i = 0u; i < disciplinas.size(); i++) {
        const auto& prereq = disciplinas[i]["prerequisitos"];
        const auto& equiv = disciplinas[i]["equivalentes"];
        auto disc_atual = nome_to_indice[disciplinas[i]["id"].asString()];

        for (auto j = 0u; j < prereq.size(); j++) {
            auto prereq_atual = nome_to_indice[prereq[j].asString()];
            prerequisitos[disc_atual][prereq_atual] = 1;
        }

		equivalencias[disc_atual][disc_atual] = 1;
        for (auto j = 0u; j < equiv.size(); j++) {
            auto equiv_atual = nome_to_indice[equiv[j].asString()];
            equivalencias[disc_atual][equiv_atual] = 1;
        }

        disc_turma[disc_atual].first = disciplinas[i]["periodo"].asString();
        disc_turma[disc_atual].second = disciplinas[i]["turma"].asString();
        capacidades[disc_atual] = disciplinas[i]["capacidade"].asInt();
        periodo_minimo[disc_atual] = disciplinas[i]["periodominimo"].asString();
		ofertadas[disc_atual] = disciplinas[i]["ofertada"].asBool();
    }

    /**********************************************
     *			   ENTRADA HORÁRIOS               *
     **********************************************/

    // Registra o número de horários, períodos e dias letivos 
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

    // Volta e lê o horário novamente, desta vez montando de fato na memória
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

    Curso curso(move(creditos), move(prerequisitos),
                move(corequisitos), move(matriz_horario),
                move(ofertadas), move(equivalencias),
                move(disc_turma), move(periodo_minimo),
                move(nome_disc), move(capacidades),
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
            auto disc_index = curso.nome_to_indice()[restantes[j].asString()];
            aprovacoes[disc_index] = 0;
        }

        auto cursadas(aprovacoes);

        const auto& disc_cursadas = alunos[i]["cursadas"];
        for (auto j = 0u; j < disc_cursadas.size(); j++) {
            auto disc_index = curso.nome_to_indice()[disc_cursadas[j].asString()];
            cursadas[disc_index] = 1;
        }

        vet_alunos.emplace_back(nome, move(aprovacoes), move(cursadas), 
								periodo, turma);
    }

    return make_pair(std::move(curso), move(vet_alunos));
}

void fagoc::gen_html(const Curso& curso,
                     const std::vector<std::shared_ptr<Solucao>>& solucoes,
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
        html_pkg << gen_html_aluno(curso, *solucao, destino, css);
    }

    saida << std::nounitbuf << html_pkg.str() << std::endl;
}

std::string fagoc::gen_html_aluno(const Curso& curso, const Solucao& solucao,
                                  const std::string& destino,
                                  const std::string& css)
{
    std::ostringstream html;
    const std::string dias_semana[] = {"Segunda", "Terça", "Quarta", "Quinta",
									   "Sexta", "Sábado", "Domingo"};

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
                            << curso.nome_disciplinas()[k] << "</td>\n";
                    encontrou = true;
                    break;
                }
            }
            // Se chegar ao fim de todas as disciplinas e não encontrar nenhuma
            // que tenha sido escolhida, imprime um traço indicando slot vazio
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

    // E também retorna a string (a mesma que foi escrita) para quem chamou
    return html.str();
}

