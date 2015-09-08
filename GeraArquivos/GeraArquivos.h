#pragma once

#include <vector>

namespace geraArquivo
{
	/*!
	* \brief Recebe estruturas de dados e nomes de arquivo e autor e gera um
	* arquivo .dat da OPL, para ser lido pelo CPLEX
	*
	* \param nomeDoArquivo Nome do arquivo final
	* \param autor Nome do autor do arquivo
	* \param nomeDisciplinas Vector de strings contendo os nomes das disciplinas do curso
	* \param creditos Vector de inteiros contentdo um n�mero de cr�ditos semanais de cada disciplina
	* \param ofertadas Vector de booleanos contendo as disciplinas ofertadas no semestre
	* \param pre_requisitos Vector de vectors de booleanos com os pr�-requisitos das disciplinas
	* \param co_requisitos Vector de vectors de booleanos com os co-requisitos das disciplinas
	* \param horarios Vector de strings com os nomes dos hor�rios
	* \param grade Vector de vectors de booleanos com as mat�rias lecionadas em cada hor�rio
	* \param aprovacoes Vector de booleanos representando as mat�rias em que o aluno foi aprovado
	* \param cursadas Vector de booleanos representando as mat�rias que o aluno j� cursou, aprovado ou n�o
	*/
	bool geraArquivoCplex(std::string nomeDoArquivo,
						  std::string autor,
						  const std::vector<std::string>& disciplinas,
						  const std::vector<int>& creditos,
						  const std::vector<bool>& ofertadas,
						  const std::vector<std::vector<bool>>& pre_requisitos,
						  const std::vector<std::vector<bool>>& co_requisitos,
						  const std::vector<std::string>& horarios,
						  const std::vector<std::vector<bool>>& grade,
						  const std::vector<bool>& aprovacoes,
						  const std::vector<bool>& cursadas);
}
