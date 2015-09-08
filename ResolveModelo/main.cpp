#pragma warning(disable: 4996)

#include <iostream>
#include <chrono>
#include <ilcplex/ilocplexi.h>
#include <Instancia.h>
#include <CursoAleatorio.h>
#include <memory>
#include <ilcp/cp.h>
#include <string>
#include <tuple>
#include <CompFagoc.h>
#include <windows.h>
#include <future>

ILOSTLBEGIN

using CursoPtr = unique_ptr<Curso>;

tuple<CursoPtr, string, int> menu() {
	int numDisciplinas{}, numPreRequisitos{}, numCoRequisitos{}, numHorarios{},
			numOfertadas{}, numAlunos{}, numProfessores{}, maxMinistradas{};
	int opcao;
	string pasta;
	CursoPtr curso;

	cout << "\nSOLUCIONADOR DE INSTANCIAS DO MODELO DE GRADE\n";
	cout << string(60, '*') << "\n\n";
	cout << "Selecione a opcao desejada:\n\n";
	cout << "1 - Valores aleatorios padrao\n";
	cout << "2 - Valores aleatorios customizados\n";
	cout << "3 - Modelo FAGOC\n";
	cout << "\nOpcao: ";

	while (true) {
		cin >> opcao;
		switch (opcao) {
		case 1:
			numDisciplinas = 50;
			numPreRequisitos = 10;
			numCoRequisitos = 5;
			numHorarios = 20;
			numOfertadas = numDisciplinas / 2;
			numProfessores = 20;
			maxMinistradas = 10;

			cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			cin >> pasta;
			cout << "Digite o numero de alunos: ";
			cin >> numAlunos;


			curso = CursoPtr{new CursoAleatorio(numDisciplinas, numPreRequisitos,
			                                    numCoRequisitos, numHorarios,
			                                    numOfertadas, numProfessores,
			                                    maxMinistradas)};
			return make_tuple(move(curso), pasta, numAlunos);

		case 2:
			cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			cin >> pasta;
			cout << "\nNumero de disciplinas: ";
			cin >> numDisciplinas;
			cout << "Numero de pre-requisitos: ";
			cin >> numPreRequisitos;
			cout << "Numero de co-requisitos: ";
			cin >> numCoRequisitos;
			cout << "Numero de horarios: ";
			cin >> numHorarios;
			cout << "Numero de disciplinas ofertadas no perido: ";
			cin >> numOfertadas;
			cout << "Numero de alunos: ";
			cin >> numAlunos;
			cout << "Numero de professores: ";
			cin >> numProfessores;
			cout << "Numero maximo de disciplinas que um professor pode ministrar: ";
			cin >> maxMinistradas;

			curso = CursoPtr{new CursoAleatorio(numDisciplinas, numPreRequisitos,
			                                    numCoRequisitos, numHorarios,
			                                    numOfertadas, numProfessores,
			                                    maxMinistradas)};

			return make_tuple(move(curso), pasta, numAlunos);

		case 3:
			cout << "\nDigite o nome da pasta de destino dos arquivos: ";
			cin >> pasta;
			cout << "Numero de alunos: ";
			cin >> numAlunos;

			curso = CursoPtr{new CompFagoc()};

			return make_tuple(move(curso), pasta, numAlunos);

		default:
			cout << "\nOpcao invalida. Tente de novo: ";
		}
	}
}

string resolveAluno(Curso const* curso, string aln) {
	// Stream de saída, uma string que será retornada.
	// Como vários modelos são resolvidos em paralelo, não é possível
	// escrever diretamente no arquivo
	ostringstream saida{};

	// Inicialização das variáveis do curso
	const auto& nomeDisciplinas = curso->nomeDisciplinas();
	const auto& creditos = curso->creditos();
	const auto& preRequisitos = curso->preRequisitos();
	const auto& coRequisitos = curso->coRequisitos();
	const auto& horarios = curso->horarios();
	const auto& ofertadas = curso->ofertadas();
	const auto numDisciplinas = creditos.size();
	const auto numHorarios = horarios.size();

	// Gera o novo aluno
	Aluno aluno(preRequisitos, coRequisitos, aln);
	const auto& aprovacoes = aluno.aprovacoes();
	const auto& cursadas = aluno.cursadas();

	IloEnv env{};
	try {
		// ------ Elaboração do modelo ----------

		IloModel mod(env);

		// Variáveis de decisão
		IloBoolVarArray y(env, numDisciplinas);

		// Função objetivo
		IloExpr obj(env);
		for (size_t d = 0; d < numDisciplinas; d++) {
			obj += creditos[d] * y[d];
		}
		mod.add(IloMaximize(env, obj));
		obj.end();

		// ------ Restrições --------------------

		// Pré-requisitos
		for (size_t d = 0; d < numDisciplinas; d++) {
			auto numPreRequisitos = 0;
			auto preReqAprovados = 0;
			for (size_t p = 0; p < numDisciplinas; p++) {
				if (preRequisitos[d][p]) {
					numPreRequisitos++;
					if (aprovacoes[p]) {
						preReqAprovados++;
					}
				}
			}
			mod.add(numPreRequisitos * y[d] <= preReqAprovados);
		}

		// Co-requisitos
		for (size_t d = 0; d < numDisciplinas; d++) {
			for (size_t c = 0; c < numDisciplinas; c++) {
				if (coRequisitos[d][c]) {
					mod.add(y[d] <= y[c] + cursadas[c]);
				}
			}
		}

		// Horários, cria as restrições dois a dois
		for (size_t h = 0; h < numHorarios - 1; h += 2) {
			IloExpr disciplinasConcorrentes(env);
			for (size_t d = 0; d < numDisciplinas; d++) {
				disciplinasConcorrentes += (horarios[h][d] + horarios[h + 1][d])
					* y[d];
			}
			mod.add(disciplinasConcorrentes <= 2);
			disciplinasConcorrentes.end();
		}

		// Disciplinas já aprovadas
		for (size_t d = 0; d < numDisciplinas; d++) {
			mod.add(y[d] <= 1 - aprovacoes[d]);
		}

		// Disciplinas ofertadas
		for (size_t d = 0; d < numDisciplinas; d++) {
			mod.add(y[d] <= ofertadas[d]);
		}

		// ----- Resolve o modelo -------

		// Se o número de disciplinas for menor que 30, utiliza
		// o CPLEX. Se for maior, a versão community não resolve, então
		// utiliza-se o CP Optimizer
		IloNumArray solucao(env);
		IloNum valor;
		if (numDisciplinas <= 30) {
			IloCplex cplex(mod);
			cplex.exportModel("aluno.lp");
			cplex.setOut(env.getNullStream());
			cplex.solve();

			valor = cplex.getObjValue();
			cplex.getValues(solucao, y);
		}
		else {
			IloCP cp(mod);
			cp.exportModel("aluno.cpo");
			cp.setOut(env.getNullStream());
			cp.solve();

			valor = cp.getObjValue();
			cp.getValues(y, solucao);
		}

		// Imprime detalhes do aluno
		saida << "Aluno: " << aln << "\n";

		saida << "Materias aprovadas:\n";
		for (size_t d = 0; d < numDisciplinas; d++) {
			if (aprovacoes[d]) {
				saida << nomeDisciplinas[d] << " ";
			}
		}

		saida << "\nMaterias cursadas:\n";
		for (size_t d = 0; d < numDisciplinas; d++) {
			if (cursadas[d]) {
				saida << nomeDisciplinas[d] << " ";
			}
		}
		// Imprime resposta
		saida << "\nSolucao:\n";
		for (size_t d = 0; d < numDisciplinas; d++) {
			if (solucao[d]) {
				saida << nomeDisciplinas[d] << " ";
			}
		}
		saida << "\nValor final: " << valor << "\n\n\n";
	}
	catch (IloException& e) {
		cerr << "Erro: " << e << "\n";
		return "";
	}
	catch (...) {
		cerr << "Excecao desconhecida" << "\n";
		return "";
	}
	env.end();

	return saida.str();
}

bool geraAlunos(string caminho, CursoPtr curso, int numAlunos) {

	// Cria o diretório, e encerra a função se for malsucedido
	if (!CreateDirectory(caminho.c_str(), nullptr))
		return false;

	// Inicializa vector de futures que retornam strings
	vector<future<string>> futures{};
	
	// Inicializa uma instância que guarda o curso passado como argumento
	Instancia instancia(move(curso));

	// Nome padrão do aluno
	string nome = "aln";
	ostringstream saida{};
	for (auto i = 0; i < numAlunos; i++) {
		auto aln = nome + to_string(i + 1);
		// Chama a função de resolução de forma assíncrona e insere o future no vector
		futures.push_back(async(resolveAluno, instancia.curso(), aln));
		//saida << resolveAluno(instancia.curso(), aln);
	}

	// String stream que irá receber cada retorno
	for (auto& solucao : futures) {
		saida << solucao.get();
	}

	// Cria a stream de escrita do arquivo
	ofstream arquivoSaida(caminho + "\\" + "resultado.txt");
	arquivoSaida << saida.str() << endl;

	return true;
}

int main() {
	auto opcoes = menu();
	auto curso = get<0>(move(opcoes));
	auto pasta = get<1>(opcoes);
	auto numAlunos = get<2>(opcoes);

	string dir = "C:\\Users\\Italo\\Google Drive\\Testes\\";
	auto begin = chrono::system_clock::now();
	if (!geraAlunos(dir + pasta, move(curso), numAlunos)) {
		cout << "Nao foi possivel resolver os modelos.\n";
	}
	else {
		cout << "Modelos resolvidos com sucesso.\n\n";
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> diferenca = end - begin;
	cout << "Tempo total: " << diferenca.count() << "s\n\n";
}
