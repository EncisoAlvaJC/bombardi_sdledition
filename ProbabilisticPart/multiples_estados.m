
TABLERO=[
[ 0  0  0  0  0  0  0 ]
[ 0 'F' 0  0  0  0  0 ]
[ 0  0  0  0  0  0  0 ]
[ 0  0  0  0  0  0  0 ]
[ 0  0  0  0  0  0  0 ]
[ 0  0  0  0  0 'F' 0 ]
[ 0  0  0  0  0  0  0 ]
];

COLORES=[
[ 0 0 0 0 0 0 0 ]
[ 0 0 0 0 0 0 0 ]
[ 0 0 0 0 0 0 0 ]
[ 0 0 0 0 0 0 0 ]
[ 0 0 0 0 0 0 0 ]
[ 0 0 0 0 0 0 0 ]
[ 0 0 0 0 0 0 0 ]
];

tablero_aux=zeros(9);
for i=1:7
	tablero_aux(i,1)='P';
	tablero_aux(i,9)='P';
	tablero_aux(1,i)='P';
	tablero_aux(9,i)='P';
end

Tr=eye(98);
for i=1:49
	Tr()