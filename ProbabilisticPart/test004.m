% no explicare nada, escribo esto el 27.03.2014 y estoy enojado



fichas=[
    [ 0   0   0   0   0   0   0 ]
    [ 0  'F'  0   0   0   0   0 ]
    [ 0   0   0   0   0   0   0 ]
    [ 0   0   0   0   0   0   0 ]
    [ 0   0   0   0   0   0   0 ]
    [ 0   0   0   0   0  'F'  0 ]
    [ 0   0   0   0   0   0   0 ]
    ];
colores=[
    [0 0 0 0 0 0 0]
    [0 2 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 1 0]
    [0 0 0 0 0 0 0]
    ];
% botones=[
%     [0 0 0 0 0 0 0]
%     [0 0 0 0 0 0 0]
%     [0 0 0 0 0 0 0]
%     [0 0 0 0 0 0 0]
%     [0 0 0 0 0 0 0]
%     [0 0 0 0 0 0 0]
%     [0 0 0 0 0 0 0]
%     ];

polvora=[
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 1]
    ];

matriz_conteo_movs_posibles=zeros(7);

donde=[2 2];
aqui=zeros(1,50); aqui((donde(1)-1)*7+donde(2))=1;


T=0.5*eye(50); T(50,50)=1;

for i=1:7
    for j=1:7
        for m=max([i-1 1]):min([i+1 7])
            for n=max([j-1 1]):min([j+1 7])
                if (max([i-m m-i])+max([j-n n-j]))
                    if ~fichas(m,n)
                        matriz_conteo_movs_posibles(i,j)=matriz_conteo_movs_posibles(i,j)+1;
                    else
                        if fichas(m,n)=='F' && colores(m,n)==1
                            matriz_conteo_movs_posibles(i,j)=matriz_conteo_movs_posibles(i,j)+1;
                        end
                    end
                end
            end
        end
    end
end

for i=1:7
    for j=1:7
        if polvora(i,j)
            T((i-1)*7+j,50)=T((i-1)*7+j,(i-1)*7+j);
            T((i-1)*7+j,(i-1)*7+j)=0;
        end
        if matriz_conteo_movs_posibles(i,j)
            % aqui hago un iluso supuesto de equiprobabilidad
            aux_prob=1/(2*matriz_conteo_movs_posibles(i,j));
            for m=max([i-1 1]):min([i+1 7])
                for n=max([j-1 1]):min([j+1 7])
                    if (max([i-m m-i])+max([j-n n-j]))
                        if ~fichas(m,n)
                            T((i-1)*7+j,(m-1)*7+n)=aux_prob;
                        else
                            if fichas(m,n)=='F' && colores(m,n)==1
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

T_movil=eye(50);

for i=1:1
T_movil=T_movil*T;   
end

vector_probabilidad=aqui*T_movil;
probabilidad_de_morir=vector_probabilidad(50);
matriz_elegante=[
    vector_probabilidad(1:7);
    vector_probabilidad(8:14);
    vector_probabilidad(15:21);
    vector_probabilidad(22:28);
    vector_probabilidad(29:35);
    vector_probabilidad(36:42);
    vector_probabilidad(43:49);
    ];

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%matriz_elegante

 probabilidad_de_vivir=1-probabilidad_de_morir;
% 
% cuando_morir=zeros(1,7);
% cuando_morir(1)=matriz_elegante((donde(1)-1)*7+donde(2));
% acumulado=cuando_morir(1);
% for step=1:6
%     aux=0;
%     for i=max([donde(1)-step 1]):min([donde(1)+step 7])
%         for j=max([donde(2)-step 1]):min([donde(2)+step 7])
%             aux=aux+matriz_elegante(i,j);
%         end
%     end
%     cuando_morir(step+1)=aux-acumulado;
%     acumulado=acumulado+cuando_morir(step+1);
% end
% 
% que_tan_facil_es_escapar=0;
% for i=2:7
%     que_tan_facil_es_escapar=que_tan_facil_es_escapar+cuando_morir(i)/(2^i);
% end

probabilidad_de_vivir
%cuando_morir
%que_tan_facil_es_escapar