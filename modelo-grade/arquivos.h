#pragma once

#include <vector>
#include <utility>
#include "curso.h"
#include "aluno.h"
#include "solucao.h"
#include <memory>

namespace fagoc {

const std::string arquivo_css = "res\\estilos.css";

std::pair<Curso, std::vector<Aluno>> ler_json(std::string arquivo);
void gen_html(const Curso& curso, const std::vector<std::shared_ptr<fagoc::Solucao>>& solucoes, std::string destino);
std::string gen_html_aluno(const Curso& curso, const Solucao& solucao,
						   const std::string& destino, const std::string& css);

}
