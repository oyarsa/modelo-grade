#pragma once

#include <vector>
#include <unordered_map>

namespace fagoc {

class Curso
{
public:
	Curso(std::vector<int>&& creditos,
	      std::vector<std::vector<char>>&& prerequisitos,
	      std::vector<std::vector<char>>&& corequisitos,
	      std::vector<std::vector<char>>&& horario,
	      std::vector<char>&& ofertadas,
	      std::vector<std::vector<char>>&& equivalentes,
	      std::vector<std::pair<std::string, std::string>>&& disc_turma,
	      std::vector<std::string>&& periodos_minimos,
	      std::vector<std::string>&& nome_disciplinas,
	      std::vector<int>&& capacidades,
		  std::unordered_map<std::string, int>&& nome_to_indice,
	      int num_dias_letivos,
	      int num_periodos);

	// Atualiza as disciplinas ofertadas pelo curso com base do número máximo
	// de alunos, conforme a disciplina entrar nas grades geradas.
	void atualiza_ofertadas(const std::vector<char>& escolhidas);

	std::size_t num_disciplinas() const;
	std::size_t num_horarios() const;
	const std::vector<int>& creditos() const;
	const std::vector<std::vector<char>>& pre_requisitos() const;
	const std::vector<std::vector<char>>& co_requisitos() const;
	const std::vector<std::vector<char>>& horario() const;
	const std::vector<char>& ofertadas() const;
	const std::vector<std::vector<char>>& equivalencias() const;
	const std::vector<std::pair<std::string, std::string>>& disc_turma() const;
	const std::vector<std::string>& periodos_minimos() const;
	const std::vector<std::string>& nome_disciplinas() const;
	std::unordered_map<std::string, int> nome_to_indice();
	int num_dias_letivos() const;
	int num_periodos() const;

private:
	std::vector<int> creditos_;
	std::vector<std::vector<char>> pre_requisitos_;
	std::vector<std::vector<char>> co_requisitos_;
	std::vector<std::vector<char>> horario_;
	std::vector<char> ofertadas_;
	std::vector<std::vector<char>> equivalencias_;
	std::vector<std::pair<std::string, std::string>> disc_turma_;
	std::vector<std::string> periodos_minimos_;
	std::vector<std::string> nome_disciplinas_;
	std::vector<int> capacidades_;
	std::vector<int> alocados_;
	std::unordered_map<std::string, int> nome_to_indice_;
	int num_dias_letivos_;
	int num_periodos_;
};

}

