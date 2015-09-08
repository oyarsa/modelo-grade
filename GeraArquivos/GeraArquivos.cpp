#include "GeraArquivos.h"
#include <fstream>
#include <ctime>
#include <string>

namespace
{
	/*!
	* \brief Imprime um vector em notação de conjunto da OPL, com "{}"
	*
	* \param nome Nome do autor
	* \param conjunto Conjunto a ser impresso
	* \param saida Stream de saída para o arquivo
	*/
	template<class T>
	void imprimeConjunto(std::string nome,
						 const std::vector<T>& conjunto,
						 std::ofstream &saida)
	{
		saida << " " << nome << " = {";
		for (std::size_t i = 0; i < conjunto.size(); i++)
		{
			if (i > 0)
			{
				saida << ", ";
			}
			saida << "\"" << conjunto[i] << "\"";
		}
		saida << "};\n";
	}

	/*!
	* \brief Imprime um vector em notação de vetor da OPL, com "[]"
	*
	* \param nome Nome da variável no arquivo
	* \param vetor Vetor a ser impresso
	* \param saida Stream de saída para o arquivo
	*/
	template<class T>
	void imprimeVetor(std::string nome,
					  const std::vector<T>& vetor,
					  std::ofstream &saida)
	{
		saida << " " << nome << " = [";
		for (std::size_t i = 0; i < vetor.size(); i++)
		{
			if (i > 0)
			{
				saida << ", ";
			}
			saida << vetor[i];
		}
		saida << "];\n";
	}

	/*!
	* \brief Imprime um vector de vectors em notação de matriz da OPL
	*
	* \param nome Nome da variável no arquivo
	* \param matriz Matriz a ser impressa
	* \param saida Stream de saída para o arquivo
	*/
	template<class T>
	void imprimeMatriz(std::string nome,
					   const std::vector<std::vector<T>>& matriz,
					   std::ofstream &saida)
	{
		saida << " " << nome << " = [\n";
		for (size_t i = 0; i < matriz.size(); i++)
		{
			saida << std::string(12, ' ');

			for (size_t j = 0; j < matriz[i].size(); j++)
			{
				if (j == 0)
					saida << '[';

				saida << matriz[i][j];

				if (j < matriz[i].size() - 1)
					saida << ',';
				else
					saida << ']';
			}
			saida << "\n";
		}
		saida << std::string(12, ' ') + "];\n";
	}

	/*!
	* \brief Imprime um cabeçalho com a data, o nome do autor e a versão da OPL
	*
	* \param autor Nome do autor do arquivo
	* \param saida Stream de saída para o arquivo
	*/
	void imprimeCabecalho(std::string autor, std::ofstream &saida)
	{
		auto t = std::time(nullptr);
		struct tm timeinfo;
		localtime_s(&timeinfo, &t);
		char buffer[101];
		asctime_s(buffer, &timeinfo);
		saida << "/" << std::string(45, '*') << "\n";
		saida << " * OPL 12.6.2.0 Data\n";
		saida << " * Author: " << autor << "\n";
		saida << " * Creation Date: " << std::string(buffer);
		saida << " " << std::string(45, '*') << "/\n\n";
	}

} // namespace interno

namespace geraArquivo
{

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
						  const std::vector<bool>& cursadas)
	{
		//! Instancia uma stream de saída com o nome do arquivo de saída dado como argumento
		//! Ela será repassada para os funções de escrita
		std::ofstream saida(nomeDoArquivo);

		//! Testa se o arquivo foi aberto corretamente
		if (!saida.is_open())
			return false;

		//! Repassa as estruturas para as funções de escrita
		imprimeCabecalho(autor, saida);
		imprimeConjunto("disciplinas", disciplinas, saida);
		imprimeVetor("creditos", creditos, saida);
		imprimeVetor("ofertadas", ofertadas, saida);
		imprimeMatriz("prerequisitos", pre_requisitos, saida);
		imprimeMatriz("corequisitos", co_requisitos, saida);
		imprimeConjunto("horarios", horarios, saida);
		imprimeMatriz("grade", grade, saida);
		imprimeVetor("aprovacoes", aprovacoes, saida);
		imprimeVetor("cursadas", cursadas, saida);

		return true;
	}

} // namespace público
