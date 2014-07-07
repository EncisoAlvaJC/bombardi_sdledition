% esta funcion toma el tablero actual y calcula la probabilidad de que
% cierto jugador sobreviva en el caminando aleatoriamente

% en un futuro, la proyeccion es usar esta informacion para valuar las
% jugadas con base a este numero, buscando la mejor

function prob = gen(quien,fichas,colores,polvora)

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
T=0.5*eye(50); T(50,50)=1;

% hago dos supuestos muy ingenuos para construir T
%  -> estando en una casilla, es igual de probable hacer cualquier
%     movimiento posible;
%  -> estando en una casilla amenazada por una bomba que no es propia,
%     hay una probabilidad de 1 de sufrir un ataque
for i=1:7
    for j=1:7
        if polvora(i,j)
            T((i-1)*7+j,50)=T((i-1)*7+j,(i-1)*7+j);
            T((i-1)*7+j,(i-1)*7+j)=0;
        end
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
T_m=eye(50);
for i=1:10000
    T_m=T_m*T;
end

% teniendo T_m, la probabilidad de morir se puede "recolectar"
% trivialmente, y la de morir se calcula facilmente
prob=1-T_m((donde(1)-1)*7+donde(2),50);

end

