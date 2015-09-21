#include "CursoEntrada.h"

CursoEntrada::CursoEntrada(int numDisciplinas, int numPreRequisitos,
			 int numCoRequisitos, int numHorarios,
			 int numOfertadas, int numProfessores,
			 int numDiasLetivos, int numPeriodos)
	: Curso(numDisciplinas, numPreRequisitos,
			numCoRequisitos, numHorarios,
			numOfertadas, numProfessores,
			numDiasLetivos, numPeriodos) {}
void CursoEntrada::setDisciplinas(std::vector<std::string>&& nomeDisciplinas, 
								  std::vector<std::vector<bool>>&& preRequisitos, 
								  std::vector<std::vector<bool>>&& coRequisitos,
								  std::vector<bool>&& ofertadas,
								  std::vector<int>&& creditos) {
	nomeDisciplinas_ = std::move(nomeDisciplinas);
	preRequisitos_ = std::move(preRequisitos);
	coRequisitos_ = std::move(coRequisitos);
	ofertadas_ = std::move(ofertadas);
	creditos_ = std::move(creditos);
}

void CursoEntrada::setHorarios(std::vector<std::vector<bool>>&& horarios) {
	horarios_ = std::move(horarios);
	geraNomeHorarios();
}
