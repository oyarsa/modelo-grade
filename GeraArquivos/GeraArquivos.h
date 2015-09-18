#pragma once

#include <vector>

class Curso;

namespace geraArquivo {
	//! \brief Recebe estruturas de dados e nomes de arquivo e autor e gera um
	//! arquivo .dat da OPL, para ser lido pelo CPLEX
	//!
	//! \param nomeDoArquivo Nome do arquivo final
	//! \param autor Nome do autor do arquivo
	//! \param nomeDisciplinas Vector de strings contendo os nomes das disciplinas do curso
	//! \param creditos Vector de inteiros contentdo um n�mero de cr�ditos semanais de cada disciplina
	//! \param ofertadas Vector de booleanos contendo as disciplinas ofertadas no semestre
	//! \param pre_requisitos Vector de vectors de booleanos com os pr�-requisitos das disciplinas
	//! \param co_requisitos Vector de vectors de booleanos com os co-requisitos das disciplinas
	//! \param horarios Vector de strings com os nomes dos hor�rios
	//! \param grade Vector de vectors de booleanos com as mat�rias lecionadas em cada hor�rio
	//! \param aprovacoes Vector de booleanos representando as mat�rias em que o aluno foi aprovado
	//! \param cursadas Vector de booleanos representando as mat�rias que o aluno j� cursou, aprovado ou n�o
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

	//! \brief Recebe o caminho para um arquivo CSS e o estabelece como estilo do hor�rio
	//! \param nomeArquivoCSS endere�o para o arquivo css
	//! \return Verdadeiro se a escrita for bem sucedida
	bool escolheCSS(std::string nomeArquivoCSS);

	//! \brief Escreve o hor�rio do aluno numa p�gina HTML
	//! \param horario Matriz de hor�rio do curso
	//! \param nomeDisciplinas Vector com os nomes das disciplinas do curso
	//! \param solucao Refer�ncia const para um vector de booleanos, representando as mat�rias selecionadas
	//! \param caminho Caminho para o arquivo de sa�da
	//! \param nomeAluno Nome do aluno cuja grade foi gerada
	//! \param numDisciplinas N�mero de disciplinas da matriz do curso
	//! \param numHorarios N�mero de hor�rios em uma semana
	//! \param numPeriodos N�mero de per�odos ofertados simultaneamente
	//! \param numDiasLetivos N�mero de dias letivos por semana
	//! \return Uma c�pia do arquivo gerado, em string
	std::string escreveHTML(const std::vector<std::vector<bool>>& horario,
							const std::vector<std::string>& nomeDisciplinas,
							const std::vector<bool>& solucao,
							std::string caminho, std::string nomeAluno,
							int numDisciplinas,
							int numHorarios,
							int numPeriodos,
							int numDiasLetivos);
}
