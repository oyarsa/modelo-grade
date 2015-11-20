#include "Curso.h"
#include <string>


Curso::Curso(int numDisciplinas, int numPreRequisitos, int numCoRequisitos,
             int numHorarios, int numOfertadas, int numProfessores, int numDiasLetivos,
			 int numPeriodos, int numTurmas)
	: numDisciplinas_(numDisciplinas),
	  numPreRequisitos(numPreRequisitos),
	  numCoRequisitos(numCoRequisitos),
	  numHorarios_(numHorarios),
	  numOfertadas(numOfertadas),
	  numProfessores_(numProfessores),
	  nomeDisciplinas_(),
	  nomeHorarios_(),
	  creditos_(numDisciplinas),
	  ofertadas_(numDisciplinas, false),
	  preRequisitos_(numDisciplinas, std::vector<bool>(numDisciplinas, false)),
	  coRequisitos_(numDisciplinas, std::vector<bool>(numDisciplinas, false)),
	  horarios_(numHorarios, std::vector<bool>(numDisciplinas, false)),
	  professores_(),
	  numDiasLetivos_(numDiasLetivos),
	  numPeriodos_(numPeriodos),
	  equivalencias_(numDisciplinas, std::vector<bool>(numDisciplinas, false)),
	  discTurma_(numDisciplinas),
	  capacidades_(numDisciplinas, 0),
	  alocados_(numDisciplinas, 0),
	  numTurmas_(numTurmas) {

	nomeDisciplinas_.reserve(numDisciplinas);
	nomeHorarios_.reserve(numHorarios);
	professores_.reserve(numProfessores);
}

const std::vector<int>& Curso::creditos() const {
	return creditos_;
}

const std::vector<bool>& Curso::ofertadas() const {
	return ofertadas_;
}

const std::vector<std::vector<bool>>& Curso::preRequisitos() const {
	return preRequisitos_;
}

const std::vector<std::vector<bool>>& Curso::coRequisitos() const {
	return coRequisitos_;
}

const std::vector<std::vector<bool>>& Curso::horarios() const {
	return horarios_;
}

const std::vector<std::string>& Curso::nomeDisciplinas() const {
	return nomeDisciplinas_;
}

const std::vector<std::string>& Curso::nomeHorarios() const {
	return nomeHorarios_;
}

const std::vector<Professor>& Curso::professores() const {
	return professores_;
}

int Curso::numDisciplinas() const {
	return numDisciplinas_;
}

int Curso::numHorarios() const {
	return numHorarios_;
}

void Curso::geraNomeHorarios() {
	for (auto i = 0; i < numHorarios_; i++) {
		nomeHorarios_.push_back("HOR" + std::to_string(i + 1));
	}
}

int Curso::numPeriodos() const {
	return numPeriodos_;
}

int Curso::numDiasLetivos() const {
	return numDiasLetivos_;
}

int Curso::numProfessores() const {
	return numProfessores_;
}

const std::vector<std::vector<bool>>& Curso::equivalencias() const {
	return equivalencias_;
}

const std::vector<std::pair<int, std::string>>& Curso::discTurma() const {
	return discTurma_;
}

const std::vector<int>& Curso::capacidades() const {
	return capacidades_;
}

void Curso::atualiza(const std::vector<bool>& escolhidas) {
	for (auto i = 0; i < numDisciplinas_; i++) {
		if (escolhidas[i])
			alocados_[i]++;

		if (alocados_[i] == capacidades_[i]) {
			ofertadas_[i] = false;
			for (auto h = 0; h < numHorarios_; h++) {
				horarios_[h][i] = false;
			}
		}

	}
}

int Curso::numTurmas() const {
	return numTurmas_;
}

const std::vector<int>& Curso::periodosMinimos() const
{
	return periodosMinimos_;
}