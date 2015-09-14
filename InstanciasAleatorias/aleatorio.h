#include <random>
#include <chrono>

//! Classe de geração de números aleatórios usando Mersenne Twister (mt19937)
class Aleatorio {
	//! Alias para o tipo de valor que a engine mt19337 toma como seed
	using TipoSeed = std::mt19937::result_type;
	//! Alias para a classe que representa um relógio de alta precisão
	using TipoRelogio = std::chrono::high_resolution_clock;
public:
	Aleatorio();
	//! \brief Gera um número aleatório de 0 a 32767 a partir de uma distribuição uniforme
	//! \return Int na faixa 0 <= x <= 32767
	int randomInt();

	//!brief Retorna uma instância do Mersenne-Twister apropriadamente seedada
	//! com um relógio de alta resolução
	//! \return Instância do mt19937 seedado
	std::mt19937 geradorAleatorio();
private:
	//! Constante para maior valor a ser gerado. Igual ao RAND_MAX padrão
	const int MAX_RANDOM = 32767;
	//! Gera uma distribuição uniforme na mesma faixa do rand()
	std::uniform_int_distribution<> dist;
	//! Seed do mersenne twister. Número de milissegundos, em alta precisão,
	//! desde Epoch
	TipoSeed seed;
	//! Engine de números aleatórios
	std::mt19937 gerador;
};
