#include "CompFagoc.h"


//! Modelo Computacao - FAGOC
CompFagoc::CompFagoc() : Curso(46, 9, 9, 20, 24, 0, 5, 4) {
	init();
}

void CompFagoc::init() {
	geraCreditos();
	geraPreRequisitos();
	geraCoRequisitos();
	geraOfertadas();
	geraHorario();
	geraNomeHorarios();
	geraNomeDisciplinas();
}

//! Atribui manualmente os valores do créditos
void CompFagoc::geraCreditos() {
	creditos_ = std::vector<int>{4, 6, 2, 4, 2, 2, 4, 5, 3, 4, 2, 2, 5, 2, 5,
		4, 4, 3, 2, 4, 3, 4, 4, 3, 4, 4, 5, 4, 4, 4, 3, 4, 3, 2, 4, 2, 4, 4,
		2, 2, 4, 3, 4, 2, 2, 2};
}

//! Atribui manualmente os pré-requisitos
void CompFagoc::geraPreRequisitos() {
	preRequisitos_[13][7] = true;
	preRequisitos_[19][1] = true;
	preRequisitos_[28][7] = true;
	preRequisitos_[28][16] = true;
	preRequisitos_[33][12] = true;
	preRequisitos_[36][30] = true;
	preRequisitos_[39][33] = true;
	preRequisitos_[40][34] = true;
	preRequisitos_[42][13] = true;
}

//! Atribui manualmente os co-requisitos
void CompFagoc::geraCoRequisitos() {
	coRequisitos_[7][1] = true;
	coRequisitos_[12][1] = true;
	coRequisitos_[15][9] = true;
	coRequisitos_[20][9] = true;
	coRequisitos_[27][22] = true;
	coRequisitos_[29][24] = true;
	coRequisitos_[30][25] = true;
	coRequisitos_[34][35] = true;
	coRequisitos_[37][16] = true;
}

//! Atribui manualmente as disciplinas ofertadas em 2015-2
void CompFagoc::geraOfertadas() {
	ofertadas_ = std::vector<bool>{false, false, false, false, false, true, true,
		true, true, true, true, true, false, false, false, false, false, true, true,
		true, true, true, true, false, false, false, false, false, false, true, true,
		true, true, true, false, false, false, false, false, false, true, false,
		true, true, false, true};
}

//! Atribui manualmente os nomes das disciplinas do curso
void CompFagoc::geraNomeDisciplinas() {
	nomeDisciplinas_ = std::vector<std::string>{"PORT", "ALG1", "LOGI",
		"FMAT", "INTR", "NODI", "ING", "ALG2", "MADI", "CALC1", "CIDA",
		"SISI", "LPRO", "ALG3", "ORCO", "CALC2", "ALIN", "ANPA", "PEST",
		"GRAF", "CANU", "RED1", "BD1", "SIDI", "ENS1", "TEOC", "TWEB",
		"BD2", "CG", "ENS2", "LFA", "SO", "INOP", "LAB1", "PMON", "METC",
		"COMP", "PDI", "GEPR", "LAB2", "MONO", "BD3", "IA", "INOV", "RED2", "TOPC"};
}

//! Não faz nada: não há disciplinas a ser criadas
void CompFagoc::geraDisciplinas() {
	//! Disciplinas já existem
}

//! Atribui manulamente o horário de 2015-2
void CompFagoc::geraHorario() {
	horarios_[0][9] = true;
	horarios_[0][18] = true;
	horarios_[0][31] = true;
	horarios_[0][5] = true;
	horarios_[1][9] = true;
	horarios_[1][18] = true;
	horarios_[1][31] = true;
	horarios_[1][5] = true;
	horarios_[2][7] = true;
	horarios_[2][17] = true;
	horarios_[2][42] = true;
	horarios_[2][43] = true;
	horarios_[3][7] = true;
	horarios_[3][17] = true;
	horarios_[3][42] = true;
	horarios_[3][43] = true;
	horarios_[4][11] = true;
	horarios_[4][21] = true;
	horarios_[4][29] = true;
	horarios_[4][45] = true;
	horarios_[5][11] = true;
	horarios_[5][21] = true;
	horarios_[5][29] = true;
	horarios_[5][45] = true;
	horarios_[6][7] = true;
	horarios_[6][19] = true;
	horarios_[6][31] = true;
	horarios_[6][5] = true;
	horarios_[7][7] = true;
	horarios_[7][19] = true;
	horarios_[7][31] = true;
	horarios_[8][7] = true;
	horarios_[8][21] = true;
	horarios_[8][33] = true;
	horarios_[9][8] = true;
	horarios_[9][21] = true;
	horarios_[9][33] = true;
	horarios_[10][10] = true;
	horarios_[10][19] = true;
	horarios_[10][30] = true;
	horarios_[11][10] = true;
	horarios_[11][19] = true;
	horarios_[11][30] = true;
	horarios_[12][9] = true;
	horarios_[12][22] = true;
	horarios_[12][42] = true;
	horarios_[13][9] = true;
	horarios_[13][22] = true;
	horarios_[13][42] = true;
	horarios_[14][6] = true;
	horarios_[14][20] = true;
	horarios_[14][32] = true;
	horarios_[15][6] = true;
	horarios_[15][17] = true;
	horarios_[15][32] = true;
	horarios_[16][8] = true;
	horarios_[16][22] = true;
	horarios_[16][29] = true;
	horarios_[17][8] = true;
	horarios_[17][22] = true;
	horarios_[17][29] = true;
	horarios_[18][6] = true;
	horarios_[18][20] = true;
	horarios_[18][30] = true;
	horarios_[19][6] = true;
	horarios_[19][20] = true;
	horarios_[19][32] = true;
}

void CompFagoc::geraProfessores() {
	//! Não implementado
}
