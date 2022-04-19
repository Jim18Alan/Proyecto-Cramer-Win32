#define INPUT_X1   200
#define INPUT_X2   201
#define INPUT_X3   202
#define INPUT_X4   203
#define INPUT_X5   204
#define INPUT_X6   205
#define INPUT_X7   206 
#define INPUT_X8   207
#define INPUT_X9   208
#define INPUT_X10  209
#define INPUT_X11  210
#define INPUT_X12  211

#define OPCION1  212
#define OPCION2  213
#define OPCION3  214
#define OPCION4  215
#define OPCION5  216
#define OPCION6  217

#define CALCULARA   230
#define CALCULARB   231
#define CALCULARC   232
#define CALCULARD   233

#define INICIO 400
#define ACTIVO 401

#define BORRAR 410
#define INPUTDIG 411
#define OKDIGMAS 412
#define OKDIGMENOS 413


void seleciona(HDC hdc, int x, int y, int opcion){
	int i,j;
    int lengX = 50, lengY = 20; 
    HBRUSH brush;
    HPEN pen;
    pen = CreatePen(PS_SOLID, 2, RGB(0,180,252)); 
	SelectObject(hdc, pen);
    brush = CreateSolidBrush(RGB(0,247,255));
    SelectObject(hdc, brush); 

    switch (opcion)
    {
    case OPCION1: lengY = 80;
        break;
    case OPCION2: x += 60; lengY = 80;
        break;
    case OPCION3: x += 120; lengY = 80;
        break;
    case OPCION4: lengX = 170;
        break;
    case OPCION5: y += 30; lengX = 170;
        break;
    case OPCION6: y += 60; lengX = 170;
        break;
    default: opcion = 0;
        break;
    }
    x += -4; y += -4;
    lengY += 8, lengX += 8;
    if(opcion != 0){
		RoundRect(hdc, x, y, x + lengX, y + lengY, 5, 5);
    }
}

void dibuja(HDC hdc, int mov[], POS pos){
	
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,180,252)); 
	SelectObject(hdc, pen);
    pos = creaPos(pos.x - 175, pos.y - 18);
    POS posA[4];
        posA[0] = creaPos(pos.x, pos.y);
        posA[1] = creaPos(pos.x + 60, pos.y);
        posA[2] = creaPos(pos.x + 120, pos.y);
        posA[3] = creaPos(pos.x + 180 , pos.y);
                                                
	int i;
	for(i = 0; i < 4; i++){
		if(mov[i]){
            MoveToEx(hdc, posA[i].x, posA[i].y + 15, NULL);
            LineTo(hdc, posA[i].x + 30,posA[i].y + 15);
            LineTo(hdc, posA[i].x + 15,posA[i].y);
            LineTo(hdc, posA[i].x,posA[i].y + 15);
		}
    }
}
