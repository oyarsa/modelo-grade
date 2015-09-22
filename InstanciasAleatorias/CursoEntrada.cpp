#include "CursoEntrada.h"

CursoEntrada::CursoEntrada(int numDisciplinas, int numPreRequisitos,
                           int numCoRequisitos, int numHorarios,
                           int numOfertadas, int numProfessores,
                           int numDiasLetivos, int numPeriodos)
	: Curso(numDisciplinas, numPreRequisitos,
	        numCoRequisitos, numHorarios,
	        numOfertadas, numProfessores,
	        numDiasLetivos, numPeriodos) {}

CursoEntrada::CursoEntrada(CursoEntrada&& outro)
	: Curso(outro.numDisciplinas_, outro.numPreRequisitos,
	        outro.numCoRequisitos, outro.numHorarios_,
	        outro.numOfertadas, outro.numProfessores_,
	        outro.numDiasLetivos_, outro.numPeriodos_) {
	nomeDisciplinas_ = move(outro.nomeDisciplinas_);
	preRequisitos_ = move(outro.preRequisitos_);
	coRequisitos_ = move(outro.coRequisitos_);
	ofertadas_ = move(outro.ofertadas_);
	creditos_ = move(outro.creditos_);
	horarios_ = move(outro.horarios_);
	nomeHorarios_ = move(outro.nomeHorarios_);
}

void CursoEntrada::setDisciplinas(std::vector<std::string>&& nomeDisciplinas,
                                  std::vector<std::vector<bool>>&& preRequisitos,
                                  std::vector<std::vector<bool>>&& coRequisitos,
                                  std::vector<bool>&& ofertadas,
                                  std::vector<int>&& creditos) {
	nomeDisciplinas_ = move(nomeDisciplinas);
	preRequisitos_ = move(preRequisitos);
	coRequisitos_ = move(coRequisitos);
	ofertadas_ = move(ofertadas);
	creditos_ = move(creditos);
}

void CursoEntrada::setHorarios(std::vector<std::vector<bool>>&& horarios) {
	horarios_ = move(horarios);
	geraNomeHorarios();
}
