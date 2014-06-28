% esta funcion toma el tablero actual y calcula la probabilidad de que
% cierto jugador sobreviva en el caminando aleatoriamente

% en un futuro, la proyeccion es usar esta informacion para valuar las
% jugadas con base a este numero, buscando la mejor

function prob_vivir = gen_prob_vivir2(quien,fichas,colores,polvora)

% lo primero es "buscar" la ficha del jugador que vamos a estudiar
for i=1:7
    for j=1:7
        if fichas(i,j)=='F' && colores(i,j)==quien
            donde=[i j];
        end
    end
end

% ahora voy a contar los movimientos posibles desde cada casilla hacia
% las casillas que le son colindantes
cont_movs=zeros(7);
for i=1:7
    for j=1:7
        for m=max([i-1 1]):min([i+1 7])
            for n=max([j-1 1]):min([j+1 7])
                if (max([i-m m-i])+max([j-n n-j]))
                    if ~fichas(m,n)
                        cont_movs(i,j)=cont_movs(i,j)+1;
                    else
                        if fichas(m,n)=='F' && colores(m,n)==quien
                            cont_movs(i,j)=cont_movs(i,j)+1;
                        end
                    end
                end
            end
        end
    end
end

% hora de generar la matriz de transicion T
T=0.5*eye(49);

% hago dos supuestos muy ingenuos para construir T
%  -> estando en una casilla, es igual de probable hacer cualquier
%     movimiento posible;
%  -> estando en una casilla amenazada por una bomba que no es propia,
%     hay una probabilidad de 1 de sufrir un ataque
for i=1:7
    for j=1:7
        if cont_movs(i,j)
            aux_prob=1/(2*cont_movs(i,j));
            for m=max([i-1 1]):min([i+1 7])
                for n=max([j-1 1]):min([j+1 7])
                    if (max([i-m m-i])+max([j-n n-j]))
                        if ~fichas(m,n)
                            T((i-1)*7+j,(m-1)*7+n)=aux_prob;
                        else
                            if fichas(m,n)=='F' && colores(m,n)==quien
                                T((i-1)*7+j,(m-1)*7+n)=aux_prob;
                            end
                        end
                    end
                end
            end
        else
            T((i-1)*7+j,(i-1)*7+j)=1;
        end
    end
end

% acto seguido, espero tener la matriz de transicion invariante T_m,
% a la cual se tiende, es decir T^n->T_m cuando n->infinity
% notese que yo tomo T^10,000

% ACTUALIZACION: debido a que la probabilidad de permanencia para
% cualquier casilla es constante, T_m puede expresarse como suma
% infinita de T^n's
T_n=eye(49); T_m=zeros(49); pow2=1;
for i=1:20
    T_n=T_n*T;
    pow2=pow2/2;
    T_m=T_m+T_n*pow2;
end

% teniendo T_m, la probabilidad de morir se puede "recolectar"
% trivialmente, y la de morir se calcula facilmente

fancy_matrix=[];
for i=1:7
	fancy_matrix=[fancy_matrix; T_m((donde(1)-1)*7+donde(2),(i-1)*7+1:i*7)];
end

fancy=fancy_matrix;

prob_vivir=0;
for i=1:7
	for j=1:7
		if polvora(i,j)==0
			prob_vivir=prob_vivir+fancy_matrix(i,j);
		end
	end
end

end

