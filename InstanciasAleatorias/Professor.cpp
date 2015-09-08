#include "Professor.h"

Professor::Professor() : numMinistradas_(), nome_(), disciplinasMinistradas_() {}

Professor::Professor(std::string nome, int numMinistradas)
	: numMinistradas_(numMinistradas),
	  nome_(nome),
	  disciplinasMinistradas_() {}

Professor::Professor(const Professor& outro)
	: numMinistradas_(outro.numMinistradas_),
	  nome_(outro.nome_),
	  disciplinasMinistradas_(outro.disciplinasMinistradas_) {}

Professor& Professor::operator=(const Professor& outro) {
	numMinistradas_ = outro.numMinistradas_;
	nome_ = outro.nome_;
	disciplinasMinistradas_ = outro.disciplinasMinistradas_;

	return *this;
}

Professor::Professor(Professor&& outro)
	: numMinistradas_(outro.numMinistradas_),
	  nome_(std::move(outro.nome_)),
	  disciplinasMinistradas_(std::move(outro.disciplinasMinistradas_)) {}

Professor& Professor::operator=(Professor&& outro) {
	numMinistradas_ = outro.numMinistradas_;
	nome_ = std::move(outro.nome_);
	outro.numMinistradas_ = 0;

	disciplinasMinistradas_ = std::move(outro.disciplinasMinistradas_);

	return *this;
}

void Professor::adicionaDisciplina(int disciplina) {
	//! Insere a disciplina no conjunto. Se ela já existir, será ignorada
	disciplinasMinistradas_.insert(disciplina);
}

const std::set<int>& Professor::disciplinasMinistradas() const {
	return disciplinasMinistradas_;
}

int Professor::numMinistradas() const {
	return numMinistradas_;
}

std::string Professor::nome() const {
	return nome_;
}

bool Professor::ministra(int disciplina) const {
	return disciplinasMinistradas_.find(disciplina)
			!= end(disciplinasMinistradas_);
}
