﻿#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <Instancia.h>
#include <CursoAleatorio.h>
#include <string>
#include <tuple>
#include <CompFagoc.h>
#include <windows.h>
#include <AlunoAleatorio.h>
#include "SolverHandler.h"

using CursoPtr = std::unique_ptr<Curso>;

std::tuple<CursoPtr, std::string, int> menu() {
	int numDisciplinas{}, numPreRequisitos{}, numCoRequisitos{}, numHorarios{},
			numOfertadas{}, numAlunos{}, numProfessores{}, maxMinistradas{};
	int opcao;
	std::string pasta;

	std::cout << "\nSOLUCIONADOR DE INSTANCIAS DO MODELO DE GRADE\n";
	std::cout << std::string(60, '*') << "\n\n";
	std::cout << "Selecione a opcao desejada:\n\n";
	std::cout << "1 - Valores aleatorios padrao\n";
	std::cout << "2 - Valores aleatorios customizados\n";
	std::cout << "3 - Modelo FAGOC\n";
	std::cout << "\nOpcao: ";

	CursoPtr curso;

	while (true) {
		std::cin >> opcao;
		switch (opcao) {
		case 1:
			numDisciplinas = 50;
			numPreRequisitos = 10;
			numCoRequisitos = 5;
			numHorarios = 20;
			numOfertadas = numDisciplinas / 2;
			numProfessores = 20;
			maxMinistradas = 10;

			std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			std::cin >> pasta;
			std::cout << "Digite o numero de alunos: ";
			std::cin >> numAlunos;


			curso = std::unique_ptr<Curso>{new CursoAleatorio{numDisciplinas, numPreRequisitos,
				numCoRequisitos, numHorarios,
				numOfertadas, numProfessores,
				maxMinistradas}};
			return make_tuple(move(curso), pasta, numAlunos);

		case 2:
			std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			std::cin >> pasta;
			std::cout << "\nNumero de disciplinas: ";
			std::cin >> numDisciplinas;
			std::cout << "Numero de pre-requisitos: ";
			std::cin >> numPreRequisitos;
			std::cout << "Numero de co-requisitos: ";
			std::cin >> numCoRequisitos;
			std::cout << "Numero de horarios: ";
			std::cin >> numHorarios;
			std::cout << "Numero de disciplinas ofertadas no perido: ";
			std::cin >> numOfertadas;
			std::cout << "Numero de alunos: ";
			std::cin >> numAlunos;
			std::cout << "Numero de professores: ";
			std::cin >> numProfessores;
			std::cout << "Numero maximo de disciplinas que um professor pode ministrar: ";
			std::cin >> maxMinistradas;

			curso = std::unique_ptr<Curso>{new CursoAleatorio{numDisciplinas, numPreRequisitos,
				numCoRequisitos, numHorarios,
				numOfertadas, numProfessores,
				maxMinistradas}};

			return make_tuple(move(curso), pasta, numAlunos);

		case 3:
			std::cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			std::cin >> pasta;
			std::cout << "Numero de alunos: ";
			std::cin >> numAlunos;

			curso = std::unique_ptr<Curso>{new CompFagoc{}};

			return make_tuple(move(curso), pasta, numAlunos);

		default:
			std::cout << "\nOpcao invalida. Tente de novo: ";
		}
	}
}


bool escreveHTML(Curso const* curso, const std::vector<bool>& solucao,
				 std::string caminho, std::string nomeAluno) {
	std::ofstream saida(caminho + "\\" + nomeAluno + ".html");

	if (!saida.is_open())
		return false;

	const auto& horario = curso->horarios();
	const auto& nomeDisciplinas = curso->nomeDisciplinas();
	const auto numDisciplinas = curso->creditos().size();
	const auto numHorarios = horario.size();

	saida << "<!DOCTYPE html>\n"
		<< "<html>\n"
		<< "<body>\n"
		<< "<table style=\"width:100%\">\n";

	saida << "<tr>\n"
		<< "<th>Segunda</th>\n"
		<< "<th>Terça</th>\n"
		<< "<th>Quarta</th>\n"
		<< "<th>Quinta</th>\n"
		<< "<th>Sexta</th>\n"
		<< "</tr>\n";

	for (auto i = 0; i < numHorarios / 5; i++) {
		saida << "<tr>\n";
		for (auto j = 0; j <= numHorarios - 4; j += 4) {
			auto encontrou = false;
			for (auto k = 0; k < numDisciplinas; k++) {
				if (horario[j + i][k] && solucao[k]) {
					saida << "<td>" << nomeDisciplinas[k] << "</td>\n";
					encontrou = true;
					break;
				}
			}
			if (!encontrou) {
				saida << "<td> ----- </td>\n";
			}
		}
		saida << "</tr>\n";
	}

	saida << "</table>\n"
		<< "</body>\n"
		<< "</html>\n";

	return true;
}


bool geraAlunos(std::string caminho, CursoPtr curso, int numAlunos) {
	// Cria o diretório, e encerra a função se for malsucedido
	if (!CreateDirectory(caminho.c_str(), nullptr))
		return false;

	// Nome padrão do aluno
	std::string nome = "aln";
	std::ostringstream saida{};
	saida << std::nounitbuf;

	for (auto i = 0; i < numAlunos; i++) {
		auto aln = nome + std::to_string(i + 1);
		SolverHandler solver{curso.get(), std::move(std::unique_ptr<Aluno>{
			new AlunoAleatorio{curso->preRequisitos(), curso->coRequisitos(), aln}})};
		solver.solve();
		saida << aln << "\n";
		const auto& nomeDisciplinas = solver.disciplinas();
		copy(begin(nomeDisciplinas), end(nomeDisciplinas),
		     std::ostream_iterator<std::string>(saida, " "));
		saida << solver.valorFinal() << "\n\n";
		escreveHTML(curso.get(), solver.solucao(), caminho, aln);
	}

	// Cria a stream de escrita do arquivo
	std::ofstream arquivoSaida(caminho + "\\" + "resultado.txt");
	arquivoSaida << std::nounitbuf << saida.str() << std::flush;

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	auto opcoes = menu();
	auto curso = std::get<0>(move(opcoes));
	auto pasta = std::get<1>(opcoes);
	auto numAlunos = std::get<2>(opcoes);

	std::string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	auto begin = std::chrono::system_clock::now();
	std::cout << "\n";
	if (!geraAlunos(dir + pasta, move(curso), numAlunos)) {
		std::cout << "Nao foi possivel resolver os modelos.\n\n";
	}
	else {
		std::cout << "Modelos resolvidos com sucesso.\n\n";
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diferenca = end - begin;
	std::cout << "Tempo total: " << diferenca.count() << "s\n\n";
}
