import os
import subprocess
import time

comeco = time.clock()

with open('resultado.txt', 'w') as meuArquivo:
	for arquivo in os.listdir('.'):
		if arquivo.endswith('.dat'):
			print("Resolvendo", arquivo)
			command = 'oplrun grade.opl '
			print(subprocess.check_output(command + arquivo, shell=True, universal_newlines=True),
					file=meuArquivo)
					
with open('tempo.txt', 'w') as saida:
	print('Tempo total: {0} segundos'.format(time.clock() - comeco), file=saida)