#define HORIZONTAL 300
#define VERTICAL 301
#define SIGNO 30  //distancia necesaria para mostrar el signo
#define TRUE 1
#define FALSE 0

//#ifdef APIWIN32
//---funciones para mostrar datos a pantalla---

//---Dibuja el signo dependiendo si es positivo o negativo---
void drawSig(HDC hdc, char sig, int x, int y){
	HPEN pen;
    if(sig == '+'){
		pen = CreatePen(PS_SOLID, 2, RGB(0,153,255));
		SelectObject(hdc, pen);
        MoveToEx(hdc, x + 20, y - 5, NULL);
		LineTo(hdc, x + 20, y + 5);
        MoveToEx(hdc, x + 15, y, NULL);
		LineTo(hdc, x + 25, y);
    }else if(sig == '-'){
        pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
        SelectObject(hdc, pen);
        MoveToEx(hdc, x + 15, y, NULL);
		LineTo(hdc, x + 25, y);
    }
}

//---dibuja los corchetes dependiendo de los digitos dentro de la matriz y de las filas en ella--- 
void traza_Corchete(HDC hdc, POS pos, int fil , int pixeles ){
	int y = 20;
    HPEN pen =  CreatePen(PS_SOLID, 2, RGB(0,180,252));
	SelectObject(hdc, pen);

		MoveToEx(hdc, pos.x + 10, pos.y, NULL);
		LineTo(hdc, pos.x + 5, pos.y);
		LineTo(hdc, pos.x + 5, pos.y + y * fil);
		LineTo(hdc, pos.x + 10, pos.y + y * fil);

		MoveToEx(hdc, pos.x + pixeles, pos.y, NULL);
		LineTo(hdc, pos.x + 5 + pixeles, pos.y);
		LineTo(hdc, pos.x + 5 + pixeles, pos.y + y * fil);
		LineTo(hdc, pos.x + pixeles, pos.y + y * fil);

}
//---Se encarga de imprimir una matriz dependiendo de sus filas y columnas---
void printfMatriz(HDC hdc, MATRIZ ma, POS pos, int dig){
    int i, j, e;
    TCHAR text[100];

    int pixel[ma.col], piexeT = 0;
    for(e = 0; e < ma.col; e++){
        pixel[e] = (getDigit(setMaxInCol(ma, e), dig) * 8) + SIGNO;
        piexeT += pixel[e];
	}
	
    for(i = 0; i < ma.fil; i++){
        for(j = 0; j < ma.col; j++){
        	drawSig(hdc, ma.sig[i][j], pos.x + j * pixel[j - 1], (pos.y + i * 20) + 8);
            TextOut(hdc, pos.x + j * pixel[j - 1] + SIGNO, pos.y + i * 20, text, sprintf(text, "%.*f", dig,  ma.datos[i][j]));
            
        }
    }
    traza_Corchete(hdc, pos, ma.fil, piexeT);
}

///-------------------Pasos para llegar a la determinante--------
MATRIZ paso1(HDC hdc, POS *pos, MATRIZ ma, POS boton, int i, int j, int true, int dig){
	TCHAR text[100];
	MATRIZ ma1;

	ma1 = getMatriz2x2(ma, boton.x + j, boton.y + i);
    
    if(true){      //si es verdadero imprime procedimiento
        drawSig(hdc, ma.sig[boton.x + j][boton.y + i], pos->x, pos->y + 18);
        pos->x += SIGNO;
        TextOut(hdc, pos->x, pos->y + 10, text, sprintf(text, "%.*f", dig,  ma.datos[boton.x + j][boton.y + i]));
        pos->x += (strlen(text) *8);
        printfMatriz(hdc, ma1, *pos, dig);
        //lo multiplico por dos al digito por que son 2 columnas y le sumo 2 por los dos puntos de las dos columnas
        pos->x += ((ma1.digCol + 2 + dig * 2)*8) + 2 * SIGNO;
    }
    return ma1;
}
DATOS paso2(HDC hdc, POS *pos, MATRIZ ma, MATRIZ ma1, POS boton, int i, int j, int true, int dig){
	TCHAR text[100];
    DATOS dat;
    dat = multiplicaX(ma1);

    if(true){
        drawSig(hdc, ma.sig[boton.x + j][boton.y + i], pos->x, pos->y + 8);
        pos->x += SIGNO;
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, "%.*f", dig,  ma.datos[boton.x + j][boton.y + i]));
        pos->x += (strlen(text) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, " ("));
        drawSig(hdc, dat.sigD1, pos->x, pos->y + 8);
        pos->x += SIGNO + (strlen(text-1) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, "%.*f", dig, dat.dato1));
        pos->x += (strlen(text) *8);
        drawSig(hdc, '-', pos->x, pos->y + 8);
        pos->x += SIGNO;
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, " ("));
        drawSig(hdc, dat.sigD2, pos->x, pos->y + 8);
        pos->x += SIGNO + (strlen(text-1) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, "%.*f", dig, dat.dato2));
        pos->x += (strlen(text) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, " ) )"));
        pos->x += (8);
    }

	return dat;
}
void paso3(HDC hdc, POS *pos, MATRIZ ma, DATOS dat, POS boton, int i, int j, int true, int dig){
	TCHAR text[100];

    float res;
    char sig = '+';

    res = (dat.sigD1 == '-' ? dat.dato1 * -1: dat.dato1) - (dat.sigD2 == '-' ? dat.dato2 * -1: dat.dato2);
    sig = (isPositivo(res) ? '+' : '-');
    res = (isPositivo(res) ? res : res*-1);

    if(true){
        drawSig(hdc, ma.sig[boton.x + j][boton.y + i], pos->x, pos->y + 8);
        pos->x += SIGNO;
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, "%.*f", dig,  ma.datos[boton.x + j][boton.y + i]));
        pos->x += (strlen(text) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, " ("));
        drawSig(hdc, sig , pos->x, pos->y + 8);
        pos->x += SIGNO + (strlen(text-1) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, "%.*f", dig, res));
        pos->x += (strlen(text) *8);
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, " )"));
        pos->x += (8);
    }
}
float paso4(HDC hdc, POS *pos, MATRIZ ma, DATOS dat, POS boton, int i, int j, int true, int dig){
	TCHAR text[100];
    float res, resT;
    char sig = '+';

    res = (dat.sigD1 == '-' ? dat.dato1 * -1: dat.dato1) - (dat.sigD2 == '-' ? dat.dato2 * -1: dat.dato2);
    resT = res * (ma.sig[boton.x + j][boton.y + i] == '-' ? ma.datos[boton.x + j][boton.y + i] * -1: ma.datos[boton.x + j][boton.y + i]);
    sig = (isPositivo(res) ? '+' : '-');
    res = (isPositivo(res) ? res : res*-1);

    if(true){
        drawSig(hdc, (sig == ma.sig[boton.x + j][boton.y + i] ? '+' : '-') , pos->x, pos->y + 8);
        pos->x += SIGNO;
        TextOut(hdc, pos->x, pos->y, text, sprintf(text, "%.*f", dig,  res * ma.datos[boton.x + j][boton.y + i]));
        pos->x += (strlen(text) *8);
    }

    return resT;
}
//-----------------------------------------------------------------------------

float trazaDeterminante(HDC hdc, POS pos, MATRIZ ma, POS boton, int direccion, int true, int dig){
    TCHAR text[100];
    float resDet = 0.0; //resultado de la determinante

    MATRIZ ma1;
    DATOS dat;
    dat = creaDatos();
    POS posP1;  posP1 = creaPos(pos.x, pos.y);
    POS posP2;  posP2 = creaPos(pos.x, pos.y + 60);
    POS posP3;  posP3 = creaPos(pos.x, pos.y + 120);
    POS posP4;  posP4 = creaPos(pos.x, pos.y + 180);
    float sig[3][3] = {{'+', '-', '+'},
                       {'-', '+', '-'},
                       {'+', '-', '+'}};

	int i, j;
	int e, o;
    if(direccion == HORIZONTAL){
        for(e = 0; e < 3; e++){
            ma.sig[boton.x][boton.y + e] = (ma.sig[boton.x][boton.y + e] == sig[boton.x][boton.y + e] ? '+' : '-'); 
        }
     	for(i = 0; i < 3; i++){
     		ma1 = paso1(hdc, &posP1, ma, boton, i, 0, true, dig);
     		dat = paso2(hdc, &posP2, ma, ma1, boton, i, 0, true, dig );
     		paso3(hdc, &posP3, ma, dat, boton, i, 0, true, dig);
     		resDet += paso4(hdc, &posP4, ma, dat, boton, i, 0, true, dig);
		}
        return resDet;
    }else if(direccion == VERTICAL ){
    	for(o = 0; o < 3; o++){
            ma.sig[boton.x + o][boton.y] = (ma.sig[boton.x + o][boton.y] == sig[boton.x + o][boton.y] ? '+' : '-'); 
        }
    	for(j = 0; j < 3; j++){
            ma1 = paso1(hdc, &posP1, ma, boton, 0, j, true, dig);
     		dat = paso2(hdc, &posP2, ma, ma1, boton, 0, j, true, dig);
     		paso3(hdc, &posP3, ma, dat, boton, 0, j, true, dig);
     		resDet += paso4(hdc, &posP4, ma, dat, boton, 0, j, true, dig);
        }
        return resDet;
    }
}  ///prueva

void regresaMatriz(HWND hwnd, MATRIZ ma){
    TCHAR text[50];
    int dato = INPUT_X1;

    int i,j;
    for(i = 0; i < 3; i++){
        for(j = 0; j < 4; j++){
            sprintf(text,"%.3f", (ma.sig[i][j] == '-' ? ma.datos[i][j] * -1 : ma.datos[i][j]));
            SetDlgItemText(hwnd,dato,text);
            dato++;
        }
    }
}
void Procedimientos(HWND hwnd, HDC hdc, HPEN pen, POS pos, MATRIZ ma, POS boton, int direccion, int mostrar[], int dig){

    float res[4];
    double Vx[3];
    int x = 500, y = 50, disx = 400, disy = 110, l = 0;
    MATRIZ prueva;
    POS pos1;   pos1= creaPos( pos.x, pos.y + 250);
    TCHAR text[100];

        //-----------caja de resultados--------------------------------
		TextOut(hdc, x + 15, y - 9, text, wsprintf(text,"Resultados"));
        MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x + 10 , y);
		MoveToEx(hdc, x + (strlen(text) * 8) + 10, y, NULL);
		LineTo(hdc, x +  disx - (strlen(text) * 8) + 10, y);
		LineTo(hdc, x + disx - (strlen(text) * 8) + 10, y + disy);
		LineTo(hdc, x , y + disy);
		LineTo(hdc, x , y);
    
    int i,j;
    for(i = 0; i < 4; i++){
        prueva = sustituyeColumna(ma,i);
        if(mostrar[i]){
            regresaMatriz(hwnd, prueva);
        }
        res[i] = trazaDeterminante(hdc, pos, prueva, boton,direccion, mostrar[i], dig);
        l += 20;
        TextOut(hdc, x + 15, y + l, text, sprintf(text,"Det: %.*f", dig, res[i]));
    }

    if(res[0] != 0.0 && res[1] != 0.0 && res[2] != 0.0 && res[3] != 0.0){
        TextOut(hdc, x + 15 + 150, y + 20, text, sprintf(text,"x1: %lf", res[0] / res[3]));
        TextOut(hdc, x + 15 + 150, y + 40, text, sprintf(text,"x2: %lf", res[1] / res[3]));
        TextOut(hdc, x + 15 + 150, y + 60, text, sprintf(text,"x3: %lf", res[2] / res[3]));
    }
}
//#endif









