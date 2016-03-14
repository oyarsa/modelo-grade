#include "curso.h"

fagoc::Curso::Curso(std::vector<Disciplina>&& disciplinas,
	                std::vector<std::vector<char>>&& horario,
                    std::vector<char>&& ofertadas,
					std::unordered_map<std::string, int>&& nome_to_indice,
                    int num_dias_letivos,
                    int num_periodos)
	  : disciplinas_(std::move(disciplinas))
      , horario_(std::move(horario))
      , ofertadas_(std::move(ofertadas))
      , alocados_(ofertadas_.size())
	  , id_to_indice_(std::move(nome_to_indice))
      , num_dias_letivos_{num_dias_letivos}
      , num_periodos_{num_periodos} {}


int fagoc::Curso::num_periodos() const
{
    return num_periodos_;
}

void fagoc::Curso::atualiza_ofertadas(const std::vector<char>& escolhidas)
{
	atualiza_ofertadas(escolhidas, horario_);
}


void fagoc::Curso::atualiza_ofertadas(const std::vector<char>& escolhidas,
									  std::vector<std::vector<char>>& horario)
{
	for (const auto& disc : disciplinas_) {
		auto index = id_to_indice_[disc.id];
		if (escolhidas[index]) {
			alocados_[index]++;
		}

		if (alocados_[index] == disc.capacidade) {
			ofertadas_[index] = 0;
			for (auto h = 0u; h < horario.size(); h++) {
				horario[h][index] = 0;
			}
		}
	}
}

const std::vector<fagoc::Disciplina>& fagoc::Curso::disciplinas() const
{
	return disciplinas_;
}

std::size_t fagoc::Curso::num_disciplinas() const
{
    return disciplinas_.size();
}

std::size_t fagoc::Curso::num_horarios() const
{
    return horario_.size();
}

int fagoc::Curso::num_dias_letivos() const
{
    return num_dias_letivos_;
}

const std::vector<std::vector<char>>& fagoc::Curso::horario() const
{
    return horario_;
}

const std::vector<char>& fagoc::Curso::ofertadas() const
{
    return ofertadas_;
}

std::unordered_map<std::string, int>& fagoc::Curso::id_to_indice()
{
	return id_to_indice_;
}
