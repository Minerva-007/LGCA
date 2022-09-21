// ---------------------------------------------------
// As Dr. Zia said, start files with comments lol.
// Anyways, this file yolos a Lattice Gas Automata.
#include"stdio.h"
#include"time.h"
#include"stdlib.h"
#include"string.h"
#include"windows.h"
#include"vector"
//#define DEBUG  //Uncomment to see debug info in runtime

struct coord{
	int x;
	int y;	
};

enum direction{LEFT=0, RIGHT, UL, UR, DL, DR};

struct cell{
	bool arr[6];//Left, right, UL, UR, DL, DR
};

class particle{
	private:
		int ID;
		coord p;
		cell* grid;
		int ww,ll;
		direction d;
	public:
		particle(int D, cell* g, direction dd, int x, int y, int w, int l)
		{
			ID=D;
			#ifdef DEBUG
				printf("Initializing particle number %d\n\t Coordinates: %d %d\n",ID,x,y);
			#endif
			p.x = x;
			p.y = y;
			grid = g;
			d=dd;
			ww=w; ll=l;
		}
		coord getCoord()
		{
			return p;
		}
		direction getDir()
		{
			return d;
		}
		void updateDir(direction dd)
		{
			//Remain in same coord, change direction
			(grid+p.x+p.y*ww)->arr[d]=false;
			d=dd;
			(grid+p.x+p.y*ww)->arr[d]=true;
		}
		void updatePos()
		{
			#ifdef DEBUG
				printf("Particle number %d at %d, %d, velocity: %d\n", ID, p.x, p.y, d);
			#endif
			(grid+p.x+p.y*ww)->arr[d]=false;
			//Move in lattice according to direction
			switch(d)
			{
				case LEFT:
					p.x=p.x-1;
					break;
				case RIGHT:
					p.x=p.x+1;
					break;
				case UL:
					p.x=p.x-1;
					p.y=p.y-1;
					break;
				case UR:
					p.x=p.x+1;
					p.y=p.y-1;
					break;
				case DL:
					p.x=p.x-1;
					p.y=p.y+1;
					break;
				case DR:
					p.x=p.x+1;
					p.y=p.y+1;
					break;
			}
			//
			(grid+p.x+p.y*ww)->arr[d]=true;
		}
};

class simulation{
	private:
		std::vector<particle> particlelist;
		cell* g;
		int ww,ll;
		void do_collisions()
		{
			for(int i=0; i< particlelist.size();i++)
			{
				//Boundary collisions
				coord p=particlelist[i].getCoord();//Get coord
				if(p.x<=1)
					particlelist[i].updateDir(particlelist[i].getDir()==UL?UR:(particlelist[i].getDir()==DL?DR:RIGHT));
				if(p.x>=ww-1)
					particlelist[i].updateDir(particlelist[i].getDir()==UR?UL:(particlelist[i].getDir()==DR?DL:LEFT));
				if(p.y<=1)
					particlelist[i].updateDir(particlelist[i].getDir()==UR?DR:(particlelist[i].getDir()==UL?DL:particlelist[i].getDir()));
				if(p.y>=ll-1)
					particlelist[i].updateDir(particlelist[i].getDir()==DR?UR:(particlelist[i].getDir()==DL?UL:particlelist[i].getDir()));
				//Neighbor collisions
				int ans=0;
				for(int k=0;k<6;k++)ans=ans+(((g+p.x+p.y*ww)->arr[k]==true?1:0)<<k);
			}
		}
	public:
		simulation(int n, cell* grid, int w, int l)
		{
			for(int i=0;i<n;)
			{
				//Idea: Assign a random number of particles at each cell
				int x = rand()%(w-1)+1, y =rand()%(l-1)+1;
				for(int j=0;j<6;j++)
				{
					(grid+x + y*w)->arr[j]=rand()%2==0?false:true;
					if((grid+x + y*w)->arr[j]==true)
					{
						particle p(i+1, grid, static_cast<direction>(j), x, y, w, l);
						i++;
						particlelist.push_back(p);
					}
				}
			}
			ww=w;
			ll=l;
			g=grid;
		}
		void display_lattice()
		{
			#ifdef DEBUG
				system("cls");
			#else
			COORD pos = {0, 0};
			HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    		SetConsoleCursorPosition(output, pos);
    		#endif
    		char* dis = (char*)malloc((ww+1)*ll);
    		memset(dis, 0, (ww+1)*ll);
			for(int i=0;i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					int ans=0;
					for(int k=0;k<6;k++)ans=ans+((g+j+i*ww)->arr[k]==true?1:0);
					*(dis+j+i*ww)=(ans==0)?' ':ans+0x30;
				}
				*(dis+ww-1+i*ww)='\n';
			}
			puts(dis);
			free(dis);
		}
		void next_gen()
		{
			do_collisions();
			for(int i=0;i<particlelist.size();i++)
				particlelist[i].updatePos();
		}
		void add_particle(int x, int y, direction d)
		{
			particle p(particlelist.size(), g, d, x, y, ww, ll);
			particlelist.push_back(p);
		}
};

#define WIDTH 40
#define LENGTH 20
#define PARTICLES 50
#define SIMTIME 60

int main()
{
	srand(time(0));
	cell grid[WIDTH][LENGTH];		//Grid size
	memset(grid, 0, sizeof(grid));
	int w=WIDTH, l=LENGTH;
	simulation s(PARTICLES, (cell*)grid, w, l);
	s.add_particle(6,10,UR);
	s.add_particle(10,3,UL);
	system("pause");
	for(int i=0;i<SIMTIME;i++)
	{
		#ifndef DEBUG
		Sleep(100);
		#endif
		s.display_lattice();
		s.next_gen();
		#ifdef DEBUG
		system("pause");
		#endif
	}
	return 0;
}