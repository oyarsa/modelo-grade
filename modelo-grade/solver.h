#pragma once
#include <memory>

namespace fagoc {

class Solver
{
public:
    Solver(const Curso& curso, const Aluno& aluno) : curso_(curso), 
		aluno_(aluno), horario_(curso_.horario()) {};
    Solver(const Curso& curso, const Aluno& aluno, 
		   const std::vector<std::vector<char>>& horario) 
		: curso_(curso), aluno_(aluno), horario_(horario) {};
    virtual ~Solver() = default;

    // Soluciona a instância do problema e guarda a solução no atributo 'solucao'
    virtual void solve() = 0;
    virtual std::shared_ptr<Solucao> solucao() const = 0;
	const Curso& curso() const;
	const Aluno& aluno() const;
protected:
    std::shared_ptr<Solucao> solucao_ = nullptr;
    const Curso& curso_;
    const Aluno& aluno_;
	const std::vector<std::vector<char>>& horario_;
};

inline const Curso& Solver::curso() const
{
	return curso_;
}

inline const Aluno& Solver::aluno() const
{
	return aluno_;
}


template <class Solver>
double soluciona_alunos(Curso& curso, const std::vector<Aluno>& alunos,
                        std::vector<std::shared_ptr<Solucao>>& solucoes)
{
    double sum = 0;
    for (size_t i = 0; i < alunos.size(); i++) {
        Solver solver(curso, alunos[i]);
        solver.solve();
        solucoes[i] = solver.solucao();
        curso.atualiza_ofertadas(solucoes[i]->solucao_bool);
        sum += solucoes[i]->funcao_objetivo;
    }

    return sum;
}

}

