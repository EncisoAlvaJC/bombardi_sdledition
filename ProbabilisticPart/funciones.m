% esta funcion toma el tablero actual y calcula la probabilidad de que
% cierto jugador sobreviva en el caminando aleatoriamente

% en un futuro, la proyeccion es usar esta informacion para valuar las
% jugadas con base a este numero, buscando la mejor

function [ prob ] = gen_prob_vivir( quien,fichas[],colores,polvora)

cont_movs=zeros(7);

% lo primero es "buscar" la ficha del jugador que vamos a estudiar
donde=[-1 -1];
for i=1:7
    for j=1:7
        if fichas(i,j)=='F' && colores(i,j)==quien
            donde=[i j];
        end
    end
end



end

