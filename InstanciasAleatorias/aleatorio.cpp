#include "aleatorio.h"

int Aleatorio::randomInt() {
	return dist(gerador);
}

Aleatorio::Aleatorio()
	: dist{0, MAX_RANDOM}, 
	gerador{std::random_device()()} {
	gerador.discard(gerador.state_size);
}

std::mt19937 Aleatorio::geradorAleatorio() {
	return gerador;
}
