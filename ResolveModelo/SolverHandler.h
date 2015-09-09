#pragma once
#include <vector>
#include <Instancia.h>
#include <ilcplex/ilocplexi.h>

class SolverHandler {
	using AlunoPtr = std::unique_ptr<Aluno>;
public:
	//! Recebe referência para um curso e para um aluno, e os uso para construir
	//! o modelo. Instancia o ambiente para o solver (RAII)
	//! \param curso Referência const para um curso
	//! \param aluno Referência const para um aluno
	explicit SolverHandler(Curso const* curso, AlunoPtr aluno);
	//! Resolve o modelo a partir dos dados inseridos
	void solve();
	//! Retorna os números das disciplinas selecionadas, ou vazio se solve()
	//! ainda não foi chamado
	//! \return Um vector de inteiros com os números das disciplinas selecionadas
	std::vector<int> solucao() const;
	//! Retorna os nomes das disciplinas selecionadas, ou vazio se solve()
	//! ainda não foi chamado
	//! \return Um vector de strings com os números das disciplinas selecionadas
	std::vector<std::string> disciplinas() const;
	//! Retorna a soma dos créditos das disciplinas selecionadas, ou 0 se
	//! solve() ainda não foi chamado
	//! \return Um inteiro que representa a soma dos créditos das
	//! disciplinas selecionadas
	int valorFinal() const;
	//! Termina o ambiente do solver
	~SolverHandler();
private:
	//! Ponteiro const para o curso a ser utilizado
	Curso const* curso_;
	//! Referência para o aluno a ser utilizado
	AlunoPtr aluno_;
	//! Ambiente do solver. É inicializado no ctor liberado no dtor
	IloEnv env;
	//! Vector com os números das disciplinas selecionadas
	std::vector<int> solucao_;
	//! Vector com os nomes das disciplinas selecionadas
	std::vector<std::string> disciplinas_;
	//! Soma dos créditos das disciplinas selecionadas
	double valorFinal_;
};
