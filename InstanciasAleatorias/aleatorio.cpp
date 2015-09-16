#include "aleatorio.h"

int Aleatorio::randomInt() {
	return dist(gerador);
}

Aleatorio::Aleatorio()
	: dist{0, MAX_RANDOM}, 
	seed{static_cast<TipoSeed>(
		TipoRelogio::now().time_since_epoch().count())},
	gerador{seed} {
	gerador.discard(gerador.state_size);
}

std::mt19937 Aleatorio::geradorAleatorio() {
	return gerador;
}
