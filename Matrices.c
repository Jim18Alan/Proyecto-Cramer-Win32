#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#define APIWIN32
#include "struct.h"
#include "Controles.h"
#include "Matrices.h"
#define POSX 150
#define POSY 20

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps; 
	HDC hdc;
	static HPEN PincelA;  
	static HBRUSH BrochaA;
	static HINSTANCE hInst;
	static HWND inpX[3][4], calcularD, calcularA, calcularB, calcularC, opcionX[3], opcionY[3], borrar, inpDig, okDMas, okDMen;	
	TCHAR Dato1[10];   		//cadena de texto para guardar los resultados de los inputs
	static MATRIZ ma;   	//matriz base para realizar operaciones
	float Datos[3][4];		//datos para rellenar la matriz base
	static int Opcion = 0;  //variable encargada de indicar que opcion mostrar a pantalla dependiendo de los botones 
	static int direccion;	//variable encargada de indicar la direccion de como se obtiene la determininate
	POS posicionDet;  		//posicion para mostrar los datos de las determinantes
	static POS posBoton, pos; 	//posicicion donde se comensara el procedieminteo para ontener la determinante 
	static int mostrar[4];  //variable encargada de indicar que determindates mostrar dependiendo del boton escogido 
	static int estado;      //controla el inicio de la ejecucion 
	static int digitosPunto;  //cantidad de digitos despues del punto
	switch(Message) {
		
		case WM_CREATE:{
			hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		    PincelA = CreatePen(PS_SOLID, 2, RGB(0,180,252));
		    BrochaA  = CreateSolidBrush(RGB(0,247,255));
		
			int i, j;
			int n, m; 
			int inputX = INPUT_X1;
			int opcion = OPCION1;

			pos = creaPos(POSX + 55, POSY);
			for(m = 0; m < 3; m++){
				opcionY[m] = CreateWindow("button", "opc", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x, pos.y, 40, 20, hwnd, (HMENU)opcion, hInst, NULL);	
											opcion++; pos.x += 60;
			}
			pos = creaPos(POSX, POSY + 30);
			for(n = 0; n < 3; n++){
				opcionX[n] = CreateWindow("button", "opc", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x, pos.y, 40, 20, hwnd, (HMENU)opcion, hInst, NULL);
											opcion++; pos.y+= 30;
			}
			pos = creaPos(POSX + 50, POSY + 30);
			for(i = 0; i < 3; i++){
				for(j = 0; j < 4; j++){
					inpX[i][j] = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_TABSTOP,
													pos.x, pos.y, 50, 20, hwnd, (HMENU)inputX, hInst, NULL);
					pos.x += 60;
					inputX++;
				}
				pos.x = POSX + 50;
				pos.y += 30;
			}
			pos = creaPos(POSX + 55, POSY + 140);
			
			calcularA = CreateWindow("button", "A", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x, pos.y, 40, 20,hwnd, (HMENU)CALCULARA, hInst, NULL);
											pos.x += 60;
			calcularB = CreateWindow("button", "B", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x, pos.y, 40, 20, hwnd, (HMENU)CALCULARB, hInst, NULL);
											pos.x += 60;
			calcularC = CreateWindow("button", "C", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x, pos.y, 40, 20, hwnd, (HMENU)CALCULARC, hInst, NULL);
											pos.x += 60;
			calcularD = CreateWindow("button", "D", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x, pos.y, 40, 20, hwnd, (HMENU)CALCULARD, hInst, NULL);
											
			inpDig = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_TABSTOP,
										 	50, 50, 50, 20, hwnd, (HMENU)INPUTDIG, hInst, NULL);
			okDMas = CreateWindow("button", "+", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											30, 50, 20, 20, hwnd, (HMENU)OKDIGMAS, hInst, NULL);
			okDMen = CreateWindow("button", "-", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											100, 50, 20, 20, hwnd, (HMENU)OKDIGMENOS, hInst, NULL);
			
			borrar = CreateWindow("button", "Borrar", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
											pos.x - 185, pos.y + 30, 240, 25, hwnd, (HMENU)BORRAR, hInst, NULL);
			
			ma = creaMatriz(3, 4);
			posBoton = creaPos(0,0);
			direccion = HORIZONTAL;
			mostrar[0] = FALSE; 
			mostrar[1] = FALSE; 
			mostrar[2] = FALSE; 
			mostrar[3] = FALSE; 
			estado = INICIO;
			digitosPunto = 1;
			wsprintf(Dato1, "%d", digitosPunto);
			SetDlgItemText(hwnd,INPUTDIG,Dato1);
			break;
		}
		case WM_COMMAND:{
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			switch (wmId)
			{
				case BORRAR: {
					int i, j;
						int INPUT_X = INPUT_X1;
						for(i = 0; i < 3; i++){
							for(j = 0; j < 4; j++){
								SetDlgItemText(hwnd,INPUT_X," ");
								//GetDlgItemText(hwnd,INPUT_X,Dato1,10); 
								INPUT_X++;
							}
						}
					mostrar[0] = FALSE; 
					mostrar[1] = FALSE; 
					mostrar[2] = FALSE; 
					mostrar[3] = FALSE; 	
					estado = INICIO;
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				case OKDIGMENOS:{
					digitosPunto += (digitosPunto <= 1 ? 0 : -1);
					wsprintf(Dato1, "%d", digitosPunto);
					SetDlgItemText(hwnd,INPUTDIG,Dato1);
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				case OKDIGMAS:{
					digitosPunto += (digitosPunto >= 6 ? 0 : 1);
					wsprintf(Dato1, "%d", digitosPunto);
					SetDlgItemText(hwnd,INPUTDIG,Dato1);
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				case INPUTDIG:{
					GetDlgItemText(hwnd,INPUTDIG,Dato1,10);
					digitosPunto = atoi(Dato1); 
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				case CALCULARA:{
					if(estado == INICIO){
						int i, j;
						int INPUT_X = INPUT_X1;
						for(i = 0; i < 3; i++){
							for(j = 0; j < 4; j++){
								GetDlgItemText(hwnd,INPUT_X,Dato1,10); 
								Datos[i][j] = atof(Dato1); 
								INPUT_X++;
							}
						}
						pushMat(&ma, Datos);
					}
					estado = ACTIVO;
					Opcion = OPCION4;
					mostrar[0] = TRUE; 
					mostrar[1] = FALSE; 
					mostrar[2] = FALSE; 
					mostrar[3] = FALSE; 	
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				case CALCULARB:{
					if(estado == INICIO){
						int i, j;
						int INPUT_X = INPUT_X1;
						for(i = 0; i < 3; i++){
							for(j = 0; j < 4; j++){
								GetDlgItemText(hwnd,INPUT_X,Dato1,10); 
								Datos[i][j] = atof(Dato1); 
								INPUT_X++;
							}
						}
						pushMat(&ma, Datos);
					}
					estado = ACTIVO;
					Opcion = OPCION4;
					mostrar[0] = FALSE; 
					mostrar[1] = TRUE; 
					mostrar[2] = FALSE; 
					mostrar[3] = FALSE; 	
					InvalidateRect(hwnd,NULL,TRUE);
					break; 
				}
				case CALCULARC:{
					if(estado == INICIO){
						int i, j;
						int INPUT_X = INPUT_X1;
						for(i = 0; i < 3; i++){
							for(j = 0; j < 4; j++){
								GetDlgItemText(hwnd,INPUT_X,Dato1,10); 
								Datos[i][j] = atof(Dato1); 
								INPUT_X++;
							}
						}
						pushMat(&ma, Datos);
					}
					estado = ACTIVO;
					Opcion = OPCION4;
					mostrar[0] = FALSE; 
					mostrar[1] = FALSE; 
					mostrar[2] = TRUE; 
					mostrar[3] = FALSE; 	
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				case CALCULARD:{
					if(estado == INICIO){
						int i, j;
						int INPUT_X = INPUT_X1;
						for(i = 0; i < 3; i++){
							for(j = 0; j < 4; j++){
								GetDlgItemText(hwnd,INPUT_X,Dato1,10); 
								Datos[i][j] = atof(Dato1); 
								INPUT_X++;
							}
						}
						pushMat(&ma, Datos);
					}
					estado = ACTIVO;
					Opcion = OPCION4;
					mostrar[0] = FALSE; 
					mostrar[1] = FALSE; 
					mostrar[2] = FALSE; 
					mostrar[3] = TRUE; 	
					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}
				
				case OPCION1: { Opcion = OPCION1; modPos(&posBoton, 0,0); direccion = VERTICAL; InvalidateRect(hwnd,NULL,TRUE); break;}
				case OPCION2: { Opcion = OPCION2; modPos(&posBoton, 0,1); direccion = VERTICAL; InvalidateRect(hwnd,NULL,TRUE); break;}
				case OPCION3: { Opcion = OPCION3; modPos(&posBoton, 0,2); direccion = VERTICAL; InvalidateRect(hwnd,NULL,TRUE); break;}
				case OPCION4: { Opcion = OPCION4; modPos(&posBoton, 0,0); direccion = HORIZONTAL; InvalidateRect(hwnd,NULL,TRUE); break;}
				case OPCION5: { Opcion = OPCION5; modPos(&posBoton, 1,0); direccion = HORIZONTAL; InvalidateRect(hwnd,NULL,TRUE); break;}
				case OPCION6: { Opcion = OPCION6; modPos(&posBoton, 2,0); direccion = HORIZONTAL; InvalidateRect(hwnd,NULL,TRUE); break;}
				default:
				return DefWindowProc(hwnd, Message, wParam, lParam);
			}
			break;
		}

		case WM_PAINT: {
			TCHAR text[100];
			hdc = BeginPaint(hwnd, &ps);
 			dibuja(hdc, mostrar,pos);
			posicionDet = creaPos(100, 250);
			seleciona(hdc, POSX + 50, POSY + 30, Opcion);
			Procedimientos(hwnd, hdc, PincelA, posicionDet,ma, posBoton, direccion, mostrar, digitosPunto);
			EndPaint(hwnd, &ps);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			DeleteObject(BrochaA);
			DeleteObject(PincelA);
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Metodo de Cramer",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		1000, /* width */
		600, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}

