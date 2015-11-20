#pragma once


#include <vector>
#include "Professor.h"

//! Curso que possui disciplinas e matrizes de horários, pré e co-requisitos
class Curso {
public:
	/*!
	* \brief Constroi um curso aleatório a partir de parâmetros arbitrários
	*
	* \param numDisciplinas_ Número total de disciplinas do curso
	* \param numPreRequisitos Soma total do número de pré-requisitos de cada matéria do curso
	* \param numCoRequisitos Soma total do número de co-requisitos de cada matéria do curso
	* \param numHorarios_ Número de horários (perído de 1 hora ou 50 minutos) em uma semana
	* \param numOfertadas Número de disciplinas ofertadas no período
	* \param numProfessores_ Número de professores do curso
	* \param numDiasLetivos Número de dias letivos por semana do curso
	* \param numPeriodos Número de períodos ofertados simultaneamente no curso
	*/
	Curso(int numDisciplinas, int numPreRequisitos, int numCoRequisitos,
	      int numHorarios, int numOfertadas, int numProfessores, int numDiasLetivos,
		  int numPeriodos, int numTurmas);

	virtual ~Curso() {}

	//! Retorna uma referência const ao vetor de créditos das disciplinas
	//! \return Referência const para um vector de ints
	const std::vector<int>& creditos() const;

	//! Retorna uma referência const ao vetor de disciplinas ofertadas no período
	//! \return Referência const para um vector de booleanos
	const std::vector<bool>& ofertadas() const;

	//! Retorna uma referência const a uma matriz de pré-requisitos das disciplinas do curso
	//! \return Referência const para uma matriz de booleanos
	const std::vector<std::vector<bool>>& preRequisitos() const;

	//! Retorna uma referência const a uma matriz de co-requisitos das disciplinas do curso
	//! \return Referência const para uma matriz de booleanos
	const std::vector<std::vector<bool>>& coRequisitos() const;

	//! Retorna uma referência const a uma matriz de horários do curso
	//! \return Referência const para uma matriz de booleanos
	const std::vector<std::vector<bool>>& horarios() const;

	//! Retorna uma referência const a um vetor de strings, os nomes das disciplinas do curso
	//! \return Referência const para um vector de strings
	const std::vector<std::string>& nomeDisciplinas() const;

	//! Retorna uma referência const a um vetor de strings, os nomes dos horários do curso
	//! \return Referência const para um vector de strings
	const std::vector<std::string>& nomeHorarios() const;

	//! Retorna uma referência const a um vetor de Professores curso
	//! \return Referência const para um vector de Professor
	const std::vector<Professor>& professores() const;

	//! Retorna o número de disciplinas do curso
	//! \return Inteiro, número de disciplinsa do curso
	int numDisciplinas() const;

	//! Retorna o número de horários por semana do curso
	//! \return Inteiro, número de horários por semana
	int numHorarios() const;

	//! Retorna o número de períodos simultaneamente ofertados pelo curso
	//! \return Inteiro, número de períodos simultaneamente ofertados pelo curso
	int numPeriodos() const;

	//! Retorna o número de dias letivos por semana do curso
	//! \return Inteiro, número de dias letivos por semana
	int numDiasLetivos() const;

	//! Retorna o número de professores associados ao curso
	//! \return Inteiro, número de professores do curso
	int numProfessores() const;

	//! Retorna uma referência const para a matriz de equivalências do curso
	//! \return Const ref para um vector de vector de bools, indicando quais disciplinas
	//!			são equivalente umas às outras
	const std::vector<std::vector<bool>>& equivalencias() const;

	//! Retorna referência const para um vetor de turmas para cada disciplina
	//! \return Const ref para vector de strings
	const std::vector<std::pair<int, std::string>>& discTurma() const;

	//! Retorna um vector de capacidades de cada disciplina do curso
	//! \return Cosnt ref para vector de ints
	const std::vector<int>& capacidades() const;

	//! Atualiza as disciplinas ofertadas pelo curso, de acordo com as que foram
	//! escolhidas para o aluno, desabilitando as que alcançarem a capacidade máxima
	void atualiza(const std::vector<bool>& escolhidas);

	//! Retorna o número de turmas do curso
	//! \return Int, número de turmas do curso
	int numTurmas() const;

	//! Retorna referência const para um vetor de períodos mínimos para cada disciplina
	//! \return Const ref para um vector de ints, indicando quantos períodos um aluno
	//!		    precisa ter cursado para fazer aquela disciplina
	const std::vector<int>& periodosMinimos() const;

protected:
	//! Número de disciplinas do curso
	int numDisciplinas_;
	//! Número de pré-requisitos do curso
	int numPreRequisitos;
	//! Número de co-requisitos do curso
	int numCoRequisitos;
	//! Número de horários do curso
	int numHorarios_;
	//! Número de disciplinas ofertadas no período
	int numOfertadas;
	//! Número de professores do curso
	int numProfessores_;
	//! Vetor de nomes das disciplinas do curso
	std::vector<std::string> nomeDisciplinas_;
	//! Vetor de nomes dos horários do curso
	std::vector<std::string> nomeHorarios_;
	//! Vetor com os valores dos créditos das disciplinas do curso
	std::vector<int> creditos_;
	//! Vetor com as disciplinas ofertadas naquele período
	std::vector<bool> ofertadas_;
	//! Matriz dos pré-requisitos das disciplinas
	std::vector<std::vector<bool>> preRequisitos_;
	//! Matriz dos co-requisitos das disciplinas
	std::vector<std::vector<bool>> coRequisitos_;
	//! Matriz dos horários do curso
	std::vector<std::vector<bool>> horarios_;
	//! Vetor de professores do curso
	std::vector<Professor> professores_;
	//! Dias letivos por semana
	int numDiasLetivos_;
	//! Número de períodos simultâneos no curso
	int numPeriodos_;
	//! Matriz de equivalência entre disciplinas
	std::vector<std::vector<bool>> equivalencias_;
	//! Vetor de pares disciplina - turma
	std::vector<std::pair<int, std::string>> discTurma_;
	//! Vetor de capacidade de cada disciplina
	std::vector<int> capacidades_;
	//! Alunos alocados em cada disciplina
	std::vector<int> alocados_;
	//! Número máximo de turmas por disciplina
	int numTurmas_;
	//! Número mínimo de períodos que um aluno precisa ter cursado
	//! para poder cursar a disciplina
	std::vector<int> periodosMinimos_;

	//! Gera os nomes dos horários sequencialmente (HOR1, HOR2, HOR3...)
	virtual void geraNomeHorarios();
};
