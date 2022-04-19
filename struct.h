#define MAX 4

/*---Estructura matriz para cada matriz que necesitemos en la aplicacion---------
  ---Con sus datos y signos corespondientes, fila y coluna, ademas de informar---
  ---la cantidad de digitos totales en todas las columnas y filas----------------*/
struct Matriz{
    float datos[MAX][MAX];
    char sig[MAX][MAX];
    int fil;
    int col;
    int digFil;
    int digCol;
};
typedef struct Matriz MATRIZ;

//---funcion encargada de devolverme los digitos del parametro recibido---
int getDigit(float dato, int dig){
    char text[100];
    sprintf(text, "%.*f", dig, dato);
    return strlen(text);
}
/*---funcion encargada de devolver el dato mayor de todas las filas de una misma columna---*/
float setMaxInCol(MATRIZ ma, int col){
    int i;
    int max = ma.datos[0][col];
    for(i = 0; i < ma.fil; i++){
        if(max < ma.datos[i][col]){
            max = ma.datos[i][col];
        }
    }
    return max;
}
//--- funcion encargada de crearme una nueva estructura matriz ---
MATRIZ creaMatriz(int fil, int col){
    MATRIZ nuevo;
    nuevo.digFil = 0;
    nuevo.digCol = 0;
    nuevo.fil = fil;
    nuevo.col = col;
    int i, j, e;
    for(i = 0; i < fil; i++){
        for(j = 0; j < col; j++){
            nuevo.datos[i][j] = 0.0;
            nuevo.sig[i][j] = '+';
        }
    }
    return nuevo;
}
//---funcion encargada de indicarme si el parametro es un positivo---
int isPositivo(float num){
    if(num > 0){ ///positivo
        return 1;
    }else if(num == 0){
        return 1;
    }else{        //negativo
        return 0;
    }
}
//---funcion encargada de rellenar la matriz con los datos de otra matriz externa---
void pushMat(MATRIZ *ma, float dat[][4]){
    int i,j,e;
    for(i = 0; i < ma->fil; i++){
        for(j = 0; j < ma->col; j++){
            if(isPositivo(dat[i][j])){
                ma->datos[i][j] = dat[i][j];
                ma->sig[i][j] = '+';
            }else{
                ma->datos[i][j] = dat[i][j] * -1;
                ma->sig[i][j] = '-';
            }
        }
    }
    for(e = 0; e < ma->col; e++){
        //por defecto tendra un digito depues del punto
    	ma->digCol += getDigit(setMaxInCol(*ma, e), 0); 
	}
	ma->digFil = ma->fil;
}
//---estructura posicion para inticarme las corenadas de algun elemento---
struct pos{
    int x;
    int y;
};
typedef struct pos POS;
//---funcion encargada de crearme una nueva estructura posicion----
POS creaPos(int x, int y){
    POS nuevo;
    nuevo.x = x;
    nuevo.y = y;
    return nuevo;
}
//---funcion que modifica una posicion---
void modPos(POS *pos, int x, int y){
	pos->x = x;
    pos->y= y;
}
//---estructura datos, esto es solo para los datos que obtendremos en el paso 2 para obtener la determinante
struct datos{
	float dato1;
	float dato2;
    char sigD1;
    char sigD2;
};
typedef struct datos DATOS;
//---funcion que me crea los datos
DATOS creaDatos(){
    DATOS nuevo;
	nuevo.dato1 = 0.0;
	nuevo.dato2 = 0.0;
    nuevo.sigD1 = '+';
    nuevo.sigD2 = '+';
    return nuevo;
}
//----funcion que multiplica una matriz de 2x2 y nos devuelve los datos con su respectivo signo
DATOS multiplicaX(MATRIZ ma){
	DATOS nuevo;
	nuevo.dato1 = ma.datos[0][0] * ma.datos[1][1];
    nuevo.sigD1 = (ma.sig[0][0] == ma.sig[1][1] ? '+' : '-');
	nuevo.dato2 = ma.datos[0][1] * ma.datos[1][0];
    nuevo.sigD2 = (ma.sig[0][1] == ma.sig[1][0] ? '+' : '-');
	return nuevo;
}
//---funcion encargada de dar la cantidad de digitos dentro de una matriz ---
void getLengMat(MATRIZ *ma, int fil,  int col){
	int j;
    ma->digFil = fil;
    for(j = 0; j < col; j++){
        ma->digCol += getDigit(setMaxInCol(*ma, j), 0); //---------------
    }
}
//---funcion encargada de eliminar una fila y una columna de una matriz----
MATRIZ getMatriz2x2(MATRIZ ma, int fil, int col){
	MATRIZ nuevo;
	nuevo = creaMatriz( 2,2);
	int i, e, d;
	int i1 = -1, e1 = 0;
	for(i = 0; i < 3; i++){
		if(i != fil && e != col){
			e1=0;
			i1++;
		}
		for(e = 0; e < 3; e++){
			if(i != fil && e != col){
				nuevo.datos[i1][e1] = ma.datos[i][e];
				nuevo.sig[i1][e1] = ma.sig[i][e];
				e1++; 
			}
		}
	}
    getLengMat(&nuevo, 2, 2);
	return nuevo;
}

//---funcion encargada de sustituir una columna por otra------
MATRIZ sustituyeColumna(MATRIZ ma, int col){
	MATRIZ nuevo; 
	nuevo = creaMatriz(3,4); 
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 4; j++){
            if(j == 3){
                nuevo.datos[i][j] = ma.datos[i][col];
                nuevo.sig[i][j] = ma.sig[i][col];
            }else if(j != col){
                nuevo.datos[i][j] = ma.datos[i][j];
                nuevo.sig[i][j] = ma.sig[i][j];
            }else{
                nuevo.datos[i][j] = ma.datos[i][3];
                nuevo.sig[i][j] = ma.sig[i][3];
            }
		}
	}
    getLengMat(&nuevo, 3 , 4);
    return nuevo; 
}
