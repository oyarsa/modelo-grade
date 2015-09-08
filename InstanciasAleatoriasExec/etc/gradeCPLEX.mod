/*********************************************
 * OPL 12.6.2.0 Model
 * Author: Italo
 * Creation Date: 20/08/2015 at 14:03:03
 *********************************************/
 float temp;
 execute{
	var before = new Date();
	temp = before.getTime();
 }
 
 {string} disciplinas = ...;
 {string} horarios = ...;
 int creditos[disciplinas] = ...;
 dvar boolean y[disciplinas];
 
 int prerequisitos[disciplinas] [disciplinas] = ...;
 int corequisitos[disciplinas] [disciplinas] = ...;
 int grade[horarios][disciplinas] = ...;
 int aprovacoes[disciplinas] = ...;
 int cursadas[disciplinas] = ...;
 int ofertadas[disciplinas] = ...;
 int n_prerequisitos[D in disciplinas] = sum (d in disciplinas) prerequisitos[D][d];
 
 maximize
 	sum (d in disciplinas) creditos[d] * y[d];
 	
 subject to {
	forall (d in disciplinas) 		  
		n_prerequisitos[d] * y[d] <= sum (x in disciplinas) aprovacoes[x] * prerequisitos[d][x];
		
	forall (d in disciplinas)
		forall (x in disciplinas)
			(corequisitos[d][x] == 1) ? y[d] <= (y[x] + cursadas[x]) : y[d] <= 1;
	
	forall (h in horarios)
		sum (d in disciplinas) 2 * grade[h][d] * y[d] <= 2;
	
	forall (d in disciplinas)
		y[d] <= (1 - aprovacoes[d]);
	
	forall (d in disciplinas)
		y[d] <= ofertadas[d];
 }
 
 {string} Disc = {d | d in disciplinas : y[d] == 1};
 
 execute DISPLAY {
 	writeln("Discplinas = ", Disc);
 	var after = new Date();
	writeln("Tempo de execucao ~= ",after.getTime()-temp);
 }