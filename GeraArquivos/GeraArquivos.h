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
	//! \param creditos Vector de inteiros contentdo um número de créditos semanais de cada disciplina
	//! \param ofertadas Vector de booleanos contendo as disciplinas ofertadas no semestre
	//! \param pre_requisitos Vector de vectors de booleanos com os pré-requisitos das disciplinas
	//! \param co_requisitos Vector de vectors de booleanos com os co-requisitos das disciplinas
	//! \param horarios Vector de strings com os nomes dos horários
	//! \param grade Vector de vectors de booleanos com as matérias lecionadas em cada horário
	//! \param aprovacoes Vector de booleanos representando as matérias em que o aluno foi aprovado
	//! \param cursadas Vector de booleanos representando as matérias que o aluno já cursou, aprovado ou não
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

	//! \brief Recebe o caminho para um arquivo CSS e o estabelece como estilo do horário
	//! \param nomeArquivoCSS endereço para o arquivo css
	//! \return Verdadeiro se a escrita for bem sucedida
	bool escolheCSS(std::string nomeArquivoCSS);

	//! \brief Escreve o horário do aluno numa página HTML
	//! \param horario Matriz de horário do curso
	//! \param nomeDisciplinas Vector com os nomes das disciplinas do curso
	//! \param solucao Referência const para um vector de booleanos, representando as matérias selecionadas
	//! \param caminho Caminho para o arquivo de saída
	//! \param nomeAluno Nome do aluno cuja grade foi gerada
	//! \param numDisciplinas Número de disciplinas da matriz do curso
	//! \param numHorarios Número de horários em uma semana
	//! \param numPeriodos Número de períodos ofertados simultaneamente
	//! \param numDiasLetivos Número de dias letivos por semana
	//! \return Uma cópia do arquivo gerado, em string
	std::string escreveHTML(const std::vector<std::vector<bool>>& horario,
							const std::vector<std::string>& nomeDisciplinas,
							const std::vector<bool>& solucao,
							std::string caminho, std::string nomeAluno,
							int numDisciplinas,
							int numHorarios,
							int numPeriodos,
							int numDiasLetivos);
}
