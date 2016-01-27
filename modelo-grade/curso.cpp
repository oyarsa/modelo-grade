#include "curso.h"

fagoc::Curso::Curso(std::vector<int>&& creditos, 
					std::vector<std::vector<char>>&& pre_reqs, 
					std::vector<std::vector<char>>&& co_reqs, 
					std::vector<std::vector<char>>&& horario, 
					std::vector<char>&& ofertadas, 
					std::vector<std::vector<char>>&& equivs, 
					std::vector<std::pair<int, std::string>>&& disc_turma, 
					std::vector<int>&& periodos_min, 
					std::vector<std::string>&& nome_discs,
					std::vector<int>&& capacidades) 
	: creditos_(std::move(creditos))
	, pre_requisitos_(std::move(pre_reqs))
	, co_requisitos_(std::move(co_reqs))
	, horario_(std::move(horario))
	, ofertadas_(std::move(ofertadas))
	, equivalencias_(std::move(equivs))
	, disc_turma_(std::move(disc_turma))
	, periodos_minimos_(std::move(periodos_min))
	, nome_disciplinas_(std::move(nome_discs))
	, capacidades_(std::move(capacidades))
	, alocados_(pre_requisitos_.size())
{}

void fagoc::Curso::atualiza_ofertadas(const std::vector<char>& escolhidas)
{
	for (std::size_t d = 0; d < nome_disciplinas_.size(); d++) {
		if (escolhidas[d]) {
			alocados_[d]++;
		}

		if (alocados_[d] == capacidades_[d]) {
			ofertadas_[d] = 0;
			for (std::size_t h = 0; h < horario_.size(); h++) {
				horario_[h][d] = 0;
			}
		}
	}
}

std::size_t fagoc::Curso::num_disciplinas() const
{
	return nome_disciplinas_.size();
}

std::size_t fagoc::Curso::num_horarios() const
{
	return horario_.size();
}

const std::vector<int>& fagoc::Curso::creditos() const
{
	return creditos_;
}

const std::vector<std::vector<char>>& fagoc::Curso::pre_requisitos() const
{
	return pre_requisitos_;
}

const std::vector<std::vector<char>>& fagoc::Curso::co_requisitos() const
{
	return co_requisitos_;
}

const std::vector<std::vector<char>>& fagoc::Curso::horario() const
{
	return horario_;
}

const std::vector<char>& fagoc::Curso::ofertadas() const
{
	return ofertadas_;
}

const std::vector<std::vector<char>>& fagoc::Curso::equivalencias() const
{
	return equivalencias_;
}

const std::vector<std::pair<int, std::string>>& fagoc::Curso::disc_turma() const
{
	return disc_turma_;
}

const std::vector<int>& fagoc::Curso::periodos_minimos() const
{
	return periodos_minimos_;
}

const std::vector<std::string>& fagoc::Curso::nome_disciplinas() const
{
	return nome_disciplinas_;
}
