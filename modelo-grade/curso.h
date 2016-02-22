#pragma once

#include <vector>
#include <unordered_map>

#include "disciplina.h"

namespace fagoc {

class Curso
{
public:
	Curso(std::vector<Disciplina>&& disciplinas,
		  std::vector<std::vector<char>>&& horario,
	      std::vector<char>&& ofertadas,
		  std::unordered_map<std::string, int>&& nome_to_indice,
	      int num_dias_letivos,
	      int num_periodos);

	// Atualiza as disciplinas ofertadas pelo curso com base do número máximo
	// de alunos, conforme a disciplina entrar nas grades geradas.
	void atualiza_ofertadas(const std::vector<char>& escolhidas);
	void atualiza_ofertadas(const std::vector<char>& escolhidas,
							std::vector<std::vector<char>>& horario);

	const std::vector<Disciplina>& disciplinas() const;
	std::size_t num_disciplinas() const;
	std::size_t num_horarios() const;
	const std::vector<std::vector<char>>& horario() const;
	const std::vector<char>& ofertadas() const;
	std::unordered_map<std::string, int>& nome_to_indice();
	int num_dias_letivos() const;
	int num_periodos() const;

private:
	std::vector<Disciplina> disciplinas_;
	std::vector<std::vector<char>> horario_;
	std::vector<char> ofertadas_;
	std::vector<int> alocados_;
	std::unordered_map<std::string, int> nome_to_indice_;
	int num_dias_letivos_;
	int num_periodos_;
};

}

