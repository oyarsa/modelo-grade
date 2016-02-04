#pragma once

#include <vector>
#include <utility>
#include "curso.h"
#include "aluno.h"
#include "solucao.h"
#include <memory>

namespace fagoc {

// Arquivo CSS utilizado para gerar a grade 
const std::string arquivo_css = "res\\estilos.css";

// L� um JSON e gera uma inst�ncia de Curso e um vector com todos os seus Alunos
std::pair<Curso, std::vector<Aluno>> ler_json(std::string arquivo);

// Gera uma representa��o em HTML dos hor�rios de todos os alunos, assim como
// um arquivo com o hor�rio de todos juntos
void gen_html(const Curso& curso, 
			  const std::vector<std::shared_ptr<fagoc::Solucao>>& solucoes, 
			  std::string destino);

// Escreve um arquivo HTML com o hor�rio do aluno, e retorna a string gerada 
std::string gen_html_aluno(const Curso& curso, const Solucao& solucao,
						   const std::string& destino, const std::string& css);

}

