#pragma once

#include <vector>
#include <utility>
#include "curso.h"
#include "aluno.h"

namespace fagoc {
	
std::pair<Curso, std::vector<Aluno>> ler_json(std::string arquivo);

}
