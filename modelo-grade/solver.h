#pragma once
#include <memory>

namespace fagoc {

class Solver
{
public:
    Solver(const Curso& curso, const Aluno& aluno) 
		: solucao_{}, curso_{curso}, aluno_{aluno}, horario_(curso_.horario()) {};
    Solver(const Curso& curso, const Aluno& aluno, 
		   const std::vector<std::vector<char>>& horario) 
		: solucao_{}, curso_{curso}, aluno_{aluno}, horario_(horario) {};
	Solver& operator=(const Solver&) = delete;
    virtual ~Solver() = default;

    // Soluciona a instância do problema e guarda a solução no atributo 'solucao'
    virtual void solve() = 0;
    virtual const Solucao& solucao() const = 0;
	const Curso& curso() const;
	const Aluno& aluno() const;
	const std::vector<std::vector<char>>& horario() const;
protected:
	Solucao solucao_;
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

inline const std::vector<std::vector<char>>& Solver::horario() const 
{
	return horario_;
}
template <typename Solver>
double soluciona_alunos(Curso& curso, const std::vector<Aluno>& alunos,
                        std::vector<Solucao>& solucoes)
{
	return soluciona_alunos<Solver>(curso, alunos, solucoes, curso.horario());
}

template <typename Solver>
double soluciona_alunos(Curso& curso, const std::vector<Aluno>& alunos,
                        std::vector<Solucao>& solucoes,
						const std::vector<std::vector<char>>& horario)
{
	static_assert(std::is_base_of<fagoc::Solver, Solver>::value, 
				  "Parametro nao e um solver");
    auto sum = 0.0;
    for (const auto& aluno : alunos) {
        Solver solver{curso, aluno, horario};
        solver.solve();
		auto solucao = solver.solucao();
        curso.atualiza_ofertadas(solucao.solucao_bool);
        sum += solucao.funcao_objetivo;
		solucoes.emplace_back(std::move(solucao));
    }

    return sum;
}

}

