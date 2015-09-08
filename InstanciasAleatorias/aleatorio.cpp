#include "aleatorio.h"
#include <chrono>

//! Alias para o tipo de valor que a engine mt19337 toma como seed
using TipoSeed = std::mt19937::result_type;

//! Alias para a classe estática que representa um relógio de alta precisão
using TipoRelogio = std::chrono::high_resolution_clock;

//! Constante para maior valor a ser gerado. Igual ao RAND_MAX padrão
const int MAX_RANDOM = 32767;

//! Gera uma distribuição uniforme na mesma faixa do rand()
std::uniform_int_distribution<> dist(0, MAX_RANDOM);
//! Número de milissegundos desde The Epoch, serve de seed para o Mersenne-Twister
TipoSeed seed = static_cast<TipoSeed>(
	TipoRelogio::now().time_since_epoch().count());
//! Engine de números aleatórios
std::mt19937 gerador(seed);

namespace aleatorio {
	int randomInt() {
		return dist(gerador);
	}

	std::mt19937 geradorAleatorio() {
		return gerador;
	}
}
