#pragma once
#include "Curso.h"

class CursoEntrada : public Curso {
public:
	CursoEntrada(int numDisciplinas, int numPreRequisitos,
				 int numCoRequisitos, int numHorarios,
				 int numOfertadas, int numProfessores,
				 int numDiasLetivos, int numPeriodos);

	void setDisciplinas(std::vector<std::string>&& nomeDisciplinas,
	                    std::vector<std::vector<bool>>&& preRequisitos,
	                    std::vector<std::vector<bool>>&& coRequisitos,
						std::vector<bool>&& ofertadas,
	                    std::vector<int>&& creditos);
	void setHorarios(std::vector<std::vector<bool>>&& horarios);
};
