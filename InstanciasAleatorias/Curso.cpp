#include "Curso.h"
#include <string>


Curso::Curso(int numDisciplinas, int numPreRequisitos, int numCoRequisitos,
             int numHorarios, int numOfertadas, int numProfessores, int numDiasLetivos,
			 int numPeriodos)
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
	  numPeriodos_(numPeriodos) {

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