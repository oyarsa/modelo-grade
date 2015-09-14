#pragma once

#include <set>

//! Professor de um curso, que pode ministrar um determinad número disciplinas
class Professor {
public:
	//! Construtor vazio. Inicializa os membros com os valores padrões
	Professor();
	/*!
	* \brief Inicializa o professor com um nome e o número total de disciplinas ministradas
	*
	* \param nome Nome do professor
	* \param numMinistrada número total de disciplinas ministradas
	*/
	Professor(std::string nome, int numMinistradas);
	//! Construtor de cópia
	Professor(const Professor& outro);
	//! Operador de cópia
	Professor& operator=(const Professor& outro);
	//! Construtor de move
	Professor(Professor&& outro);
	//! Operador de move
	Professor& operator=(Professor&& outro);

	/*!
	* \brief Verifica se o professor ministra tal disciplina
	*
	* \param disciplina Número da disciplina
	* \return Verdadeiro se a disciplina está no conjunto de disciplinas ministradas
	*/
	bool ministra(int disciplina) const;
	//! \brief Adiciona uma disciplina ao conjunto de ministradas
	//! \param disciplina Número da disciplina a ser adicionada
	void adicionaDisciplina(int disciplina);
	//! Retorna o número de disciplinas ministradas
	//! \return Número de disciplinas ministradas
	int numMinistradas() const;
	//! Retorna uma referência const para o conjunto de disciplinas ministradas
	//! \return Const reference para um set de ints
	const std::set<int>& disciplinasMinistradas() const;
	//! Retorna uma cópia do nome do professor
	//! \return String, nome do professor
	std::string nome() const;
private:
	//! Número total de disciplinas ministradas pelo professor
	int numMinistradas_;
	//! Nome do professor
	std::string nome_;
	//! Conjunto de disciplinas ministradas
	std::set<int> disciplinasMinistradas_;
};
