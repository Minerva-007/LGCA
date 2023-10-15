#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"windows.h"
#include"windowsx.h"

/*

*/


// Define cell types
// Cell types are based on their counting, from 0 to 15
// in the following notation (based on vim):
// 0 0000 empty
// 1 0001 right
// 2 0010 up
// 3 0011 up and right
// 4 0100 down
// 5 0101 down and right
// 6 0110 up and down
// 7 0111 up down right
// 8 1000 left
// 9 1001 left and right
// 10 1010 left and up
// 11 1011 left up right
// 12 1100 left down
// 13 1101 left down right
// 14 1110 left down up
// 15 1111 left down up right
// Cell types 16 is reserved for walls, and 17 through 31 define particle-wall collision

// Lookup table for cell color
char CELLCOLOR[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5};

// Raw colormap assigned to particle count
unsigned int RAWCOLORMAP[] = {
RGB(255, 255, 255), // Empty Space
RGB(192, 192, 192), // 1 particle
RGB(128, 128, 128), // 2 particles
RGB(64, 64, 64),    // 3 particles
RGB(0, 0, 0),       // 4 particles
RGB(255, 0, 0)		//walls
};

// Actual colormap for each cell type
unsigned int COLORMAP[32] = {0};

// Lookup table for next states
char NEXTSTATE[] = {0, 1, 2, 3, 4, 5, 9, 7, 8, 6, 10, 11, 12, 13, 14, 15, 
16, 24, 20, 28, 18, 26, 22, 30, 17, 25, 21, 29, 19, 27, 23, 31};

#define WIDTH 800
#define HEIGHT 600

// Double buffer for cell grid
char Grid[2][WIDTH][HEIGHT]={0};

// Class duh
class SimulationManager{
	private:
		// Which buffer to use
		int turn;
		// Rendering Related Data
		HDC _hdc,_hmemdc;
		HWND _hwnd;
		// Screen Buffer
		DWORD* _point;
		HBITMAP _hbmp;
		HPEN hpen;
	public:
		SimulationManager(){}
		SimulationManager(HWND hwnd)
		{
			turn=0;
			// Screen buffer prep code here
			_hdc=GetDC(hwnd);
			if(_hdc==0)printf("screen DC error\n");
			_hwnd=hwnd;
			BITMAPINFO bmi={0};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = WIDTH;
			bmi.bmiHeader.biHeight = -HEIGHT; // top-down
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			_hbmp=CreateDIBSection(_hdc,&bmi,DIB_RGB_COLORS,(void**)&_point,0,0);
			if(_hbmp==0)printf("Bitmap error\n");
			memset(_point,RGB(255,255,255),WIDTH*HEIGHT*4);
			_hmemdc=CreateCompatibleDC(_hdc);
			SelectObject(_hmemdc,_hbmp);
			BitBlt(_hdc,0,0,WIDTH,HEIGHT,_hmemdc,0,0,SRCCOPY);
			// Pen creation for drawmode here
			hpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
			SelectObject(_hmemdc, hpen);
			// Preprocess the color map
			for(int i=0;i<32;i++)
			{
				COLORMAP[i] = RAWCOLORMAP[CELLCOLOR[i]];
			}
			// Initialize grid
			for(int x=0;x<WIDTH;x++)
			{
				for(int y=0;y<HEIGHT;y++)
					{
						Grid[turn%2][x][y] = 0;
					}
			}
		}
		
		~SimulationManager()
		{
			// Release screen buffer resources
			ReleaseDC(_hwnd,_hdc);
			DeleteObject(hpen);
			DeleteObject(_hbmp);
			DeleteDC(_hmemdc);
			printf("Frames generated: %d\n",turn);
		}
		
		void AddParticle(int x, int y, int ID)
		{
			//printf("Particle added\n");
			// Add particle lmao
			Grid[turn%2][x-1][y-1] = Grid[turn%2][x-1][y-1] | ID;
		}
		
		void AddLine(int x1, int y1, int x2, int y2)
		{
			MoveToEx(_hmemdc, x1, y1, 0);
			LineTo(_hmemdc, x2, y2);
			Render();
		}
		
		void CollidePropagate()
		{
			int disturn = turn%2;
			int nexturn = (turn+1)%2;
			for(int x=1;x<WIDTH-1;x++)
			{
				for(int y=1;y<HEIGHT-1;y++)
				{
					// Update each cell with the LUT
					Grid[disturn][x][y] = NEXTSTATE[ Grid[disturn][x][y] ];
					// Propagate particles from the cell
					Grid[nexturn][x-1][y] = Grid[nexturn][x-1][y] | (Grid[disturn][x][y] & 2);// Up
					Grid[nexturn][x+1][y] = Grid[nexturn][x+1][y] | (Grid[disturn][x][y] & 4);// Down
					Grid[nexturn][x][y-1] = Grid[nexturn][x][y-1] | (Grid[disturn][x][y] & 8);// Left
					Grid[nexturn][x][y+1] = Grid[nexturn][x][y+1] | (Grid[disturn][x][y] & 1);// Right
					Grid[nexturn][x][y] = Grid[nexturn][x][y] | (Grid[disturn][x][y] & 16); // Wall
				}
			}
			// Reset the grid for next time use
			memset(Grid[disturn],0,HEIGHT*WIDTH);
		}

		void Visualize()
		{
			int curturn = (turn+1)%2;
			// Set pixels on screen according to particle count
			for(int x=0;x<WIDTH;x++)
			{
				for(int y=0;y<HEIGHT;y++)
					{
						*(_point+y*WIDTH+x) = COLORMAP[Grid[curturn][x][y]];
					}
			}
			// Render
			Render();
		}
		
		void RefreshWalls()
		{
			int curturn=turn%2;
			// Set pixels in grid according to screen drawing
			for(int x=0;x<WIDTH;x++)
			{
				for(int y=0;y<HEIGHT;y++)
					{
						if(*(_point+y*WIDTH+x)==RGB(255,0,0))
							Grid[curturn][x][y] = 16;
					}
			}
		}
		
		inline void incTick()
		{
			turn++;
		}
		
		inline void Render()
		{
			BitBlt(_hdc,0,0,WIDTH,HEIGHT,_hmemdc,0,0,SRCCOPY);
		}
		
		inline void runIter()
		{
			CollidePropagate();
			Visualize();
			incTick();
		}
};



#include"conio.h"

SimulationManager *sm;
// Flow in a pipe
void CreateScenario1()
{
	// Fill all edges with walls
	for(int i=100;i<400;i++)
	{
		sm->AddParticle(i,10,16);
		sm->AddParticle(i-50,90,16);
	}
	for(int i=10;i<90;i++)
	{
		sm->AddParticle(100,i,16);
	}
	for(int i=10;i<300;i++)
	{
		sm->AddParticle(400,i,16);
		sm->AddParticle(350,i+80,16);
	}
	for(int i=400;i<550;i++)
	{
		sm->AddParticle(i,300,16);
		sm->AddParticle(i,350,16);
	}
	for(int i=300;i<350;i++)
	{
		sm->AddParticle(550,i,16);
	}
	for(int i=350;i<470;i++)
	{
		sm->AddParticle(400,i,16);
		sm->AddParticle(350,i,16);
	}
	for(int i=350;i<400;i++)
	{
		sm->AddParticle(i,470,16);
	}
	// Fill inside with some particles
		for(int i=101;i<399;i++)
	{
		for(int j=11;j<89;j++)
		{
			sm->AddParticle(i,j,15);
		}
	}
	
}

// Flow over a block
void CreateScenario2()
{
	// Create the wind tunnel
	for(int i=2;i<790;i++)
	{
		sm->AddParticle(i, 200, 16);
		sm->AddParticle(i, 300, 16);
	}
	// Create an obstacle
	for(int i=225;i<275;i++)
	{
		sm->AddParticle(400, i, 16);
		sm->AddParticle(450,i,16);
	}
	// Add a wall
	for(int i=200;i<225;i++)
	{
		sm->AddParticle(700, i, 16);
	}
	for(int i=275;i<300;i++)
	{
		sm->AddParticle(700, i, 16);
	}
}

void CreateBoundary()
{
	// Add Boundary
	for(int i=10;i<790;i++)
	{
		sm->AddParticle(i, 10, 16);
		sm->AddParticle(i, 590, 16);
	}
	for(int i=10; i<590; i++)
	{
		sm->AddParticle(10, i, 16);
		sm->AddParticle(790, ((i<180||i>420)?i:0), 16);// Perforated outlet
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static bool flag=false;
	static bool drawmode=true;
	switch(Message) {
		case WM_CREATE: {
			sm = new SimulationManager(hwnd);
			CreateBoundary();
			SetTimer(hwnd, 1, 1, 0);
			break;
		}
		case WM_TIMER: {
			if(flag==true)
			{
				// Add particles to the inlet
				for(int i=11; i<590;i++)
					sm->AddParticle(11, i, 4);
				sm->runIter();
			}
			break;
		}
		case WM_MOUSEMOVE:{
			static int prx, pry;
			if(drawmode)
			{
				if(wParam&MK_LBUTTON)
				{
					sm->AddLine(prx, pry, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				}
				prx = GET_X_LPARAM(lParam);
				pry = GET_Y_LPARAM(lParam);
			}
			break;
		}
		case WM_LBUTTONDOWN:{
			if(!drawmode)
			{
				flag=true;
			}
			break;
		}
		case WM_RBUTTONDOWN:{
			if(drawmode==true)
			{
				sm->RefreshWalls();
				drawmode=false;
			}
			flag=false;
			break;
		}
		case WM_DESTROY: {
			delete sm;
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = &WndProc; /* insert window procedure function here */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* use "A" as icon name when you want to use the project icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* as above */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","R-LGCA by Minerva!",WS_VISIBLE|WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,WIDTH+100,HEIGHT+100,NULL,NULL,hInstance,NULL);
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

