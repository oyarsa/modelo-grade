#pragma once
#include <memory>

namespace fagoc {

class Solver
{
public:
    Solver(const Curso& curso, const Aluno& aluno) : curso_(curso), aluno_(aluno) {};

    virtual void solve() = 0;
    virtual std::shared_ptr<Solucao> solucao() const = 0;
protected:
    std::shared_ptr<Solucao> solucao_ = nullptr;
    const Curso& curso_;
    const Aluno& aluno_;
};

template <class Solver>
double soluciona_alunos(Curso& curso, const std::vector<Aluno>& alunos,
                        std::vector<std::shared_ptr<fagoc::Solucao>>& solucoes)
{
    double sum = 0;
    for (std::size_t i = 0; i < alunos.size(); i++) {
        Solver solver(curso, alunos[i]);
        solver.solve();
        solucoes[i] = solver.solucao();
        curso.atualiza_ofertadas(solucoes[i]->solucao_bool);
        sum += solucoes[i]->funcao_objetivo;
    }

    return sum;
}

}

