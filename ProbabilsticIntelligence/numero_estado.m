% esta funcion genera un vector con 5 datos en los cuales baso mi algoritmo de inteligencia probabilistica en
% su  fase 1:
%
%                    p_vivir  : probabilidad de no terminar en una casilla amenazada por otros 
%                gradiente :  de los otros jugadres, quien tiene la p_vivir mas alta con respecto al PI, salvo empates
% potencia_explosiva : el valor de las casillas amenazadas, con mayor peso del jugador gradiente
%                  distancia : la suma de las distancias, en casillas, a los jugadores, con mayor peso del jugador gradiente
%                    numero : finalmente, una valuacion (experimental) del estado actual, utilizable en comparaciones

de_quien=1;

%function [p_vivir gradiente potencia_explosiva distancia numero] = numero_estado[de_quien,sobrevivientes,tablero,colores,polvora_propia,polvora_ajena]
function [p_vivir, gradiente, potencia_explosiva, distancia, numero] = numero_estado[de_quien,sobrevivientes,tablero,colores,polvora_propia,polvora_ajena]
	
% parecera trampa, pero ya tengo una funcion que calcula el primer numero
p_vivir=gen_prob_vivir2(de_quien,tablero,colores,polvora_ajena);

% el gradiente lo hallare usado la funcion anterior; guardare las p_vivir para calcular la potencia explosiva
gradiente=0; test_prob=0; temp=0;
p_vivirs=[];
for i=1:5
	if sobrevivientes(i)==1
		if i~=de_quien
			temp=gen_prob_vivir2(i,tablero,colores,polvora_propia);
			p_vivirs=[p_vivirs temp];
			if temp>test_prob
				test_prob=temp;
				gradiente=i;
			end
		else
			p_vivirs=[p_vivirs 1]; % no es importante para calcular el daño ajeno
		end
	else
		p_vivirs=[p_vivirs 1];
	end
end

% la potencia explosiva, po cada jugador, es 1-p_vivir; simplemente las sumare y sumare un extra por el gradiente
potencia_explosiva=0; n_sobrevivientes=0;
for i=1:5
	if sobrevivientes(i)
		n_sobrevivientes=n_sobrevivientes+1;
	end
	potencia_explosiva=potencia_explosiva+1-p_vivirs(i);
end
potencia_explosiva=potencia_explosiva+(n_sobrevivientes-2)*p_vivirs(gradiente);
% notese que el peso extra del gradiente solo se agrega para mas de 2 jugadores y SIEMPRE le da mucho peso

% para calcular las distancias, antes debo hallar la ficha del jugador que se me pide y de todos sus enemigos
% otra cosa importante es que usare una distancia discreta medida en "movimientos"
distancia=0; lugares=[0 0;0 0;0 0;0 0;0 0];
for i=1:7
	for j=1:7
		if tablero(i,j)=='F'
			lugares(colores(i,j),:)=[i,j];
		end
	end
end

lugar_propio=lugares(de_quien,:);
for i=1:5
	if lugares(i,1)~=0
		distancia = distancia+max([abs(lugar_propio(1)-lugares(i,1)),abs(lugar_propio(2)-lugares(i,2))]);
	end
end
distancia = distancia+ (n_sobrevivientes-2)*max([abs(lugar_propio(1)-lugares(gradiente,1)),abs(lugar_propio(2)-lugares(gradiente,2))]);

% finalmente el numero, que viene siendoo multiplicar las tres cantidades importanes (el gradiente no)

numero=p_vivir*(potencia_explosiva+1)*distancia;

end