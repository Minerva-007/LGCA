#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"windows.h"
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

// Lookup table for particle count
char PARTCOUNT[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

// Lookup table for next states
char NEXTSTATE[] = {0, 1, 2, 3, 4, 5, 9, 7, 8, 6, 10, 11, 12, 13, 14, 15, 
16, 24, 20, 28, 18, 26, 22, 30, 17, 25, 21, 29, 19, 27, 23, 31};

#define WIDTH 640
#define HEIGHT 480
char Grid[2][WIDTH][HEIGHT]={0};

class SimulationManager{
	private:
		int turn;
	public:
		SimulationManager()
		{
			memset(Grid, 0, WIDTH*HEIGHT*2);
		}
		~SimulationManager()
		{
		}
		void AddParticle(int x, int y, int ID)
		{
			Grid[0][x-1][y-1] = ID;
		}
		
		void Collide()
		{
			// Update each cell with the LUT
			int disturn = turn%2;
			for(int x=0;x<WIDTH;x++)
				for(int y=0;y<HEIGHT;y++)
					Grid[disturn][x][y] = NEXTSTATE[ Grid[disturn][x][y] ];
		}

		void Propagate()
		{
			// Propagate particles between cells
			int disturn = turn%2;
			int nexturn = (turn+1)%2;
			for(int x=1;x<WIDTH-1;x++)
			{
				for(int y=1;y<HEIGHT-1;y++)
				{
					Grid[nexturn][x-1][y] = Grid[nexturn][x-1][y] | (Grid[disturn][x][y] & 2);// Up
					Grid[nexturn][x+1][y] = Grid[nexturn][x+1][y] | (Grid[disturn][x][y] & 4);// Down
					Grid[nexturn][x][y-1] = Grid[nexturn][x][y-1] | (Grid[disturn][x][y] & 8);// Left
					Grid[nexturn][x][y+1] = Grid[nexturn][x][y+1] | (Grid[disturn][x][y] & 1);// Right
					Grid[nexturn][x][y] = Grid[nexturn][x][y] | (Grid[disturn][x][y] & 16); // Wall
				}
			}
			// Copy nexturn to disturn
			//memcpy(Grid[disturn],Grid[nexturn],HEIGHT*WIDTH);
			// Clear nexturn
			memset(Grid[disturn],0,HEIGHT*WIDTH);
		}

		void Visualize()
		{
			system("cls");
			int curturn = (turn+1)%2;
			for(int x=0;x<WIDTH;x++)
			{
				for(int y=0;y<HEIGHT;y++)
					{
						printf("%2d ",Grid[curturn][x][y]);
					}
				printf("\n");
			}
		}

		void incTick()
		{
			turn++;
		}
};


#include"conio.h"

int main()
{
	SimulationManager sm;
	printf("%d",sizeof(Grid));
	//sm.Visualize();
	//system("pause");
	sm.AddParticle(3, 5, 4);
	sm.AddParticle(7, 5, 2);
	for(int i=1;i<11;i++)
	{
		sm.AddParticle(i,2,16);
		sm.AddParticle(i,9,16);
		sm.AddParticle(2,i,16);
		sm.AddParticle(9,i,16);
	}
	//sm.Visualize();
	//system("pause");
	for(int i=0;i<600;i++)
	{
		sm.Collide();
		sm.Propagate();
		//sm.Visualize();
		sm.incTick();
		//Sleep(100);
		//getch();
	}
	
	return 0;
}
