#include <random>

namespace aleatorio {
	/*!
	* \brief Gera um número aleatório de 0 a 32767 a partir de uma distribuição uniforme
	*
	* \return Int na faixa 0 <= x <= 32767
	*/
	int randomInt();

	/*!
	* \brief Retorna uma instância do Mersenne-Twister apropriadamente seedada
	* com um relógio de alta resolução
	*
	* \return Instância do mt19937 seedado
	*/
	std::mt19937 geradorAleatorio();
}
