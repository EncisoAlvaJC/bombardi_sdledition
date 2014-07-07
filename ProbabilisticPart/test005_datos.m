% Este script funciona unicamente para introducir datos

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
    [0 1 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 2 0]
    [0 0 0 0 0 0 0]
    ];
polvora1=[
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    ];
    
    polvora2=[
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    [0 0 0 0 0 0 0]
    ];
    
    % pero ahora tambien ejecuta una jugada en particular
    
	[A B]=gen_prob_vivir2(1,fichas,colores,polvora1);
	A
	B
    % ejecutar solo despues de haber establecido "original"