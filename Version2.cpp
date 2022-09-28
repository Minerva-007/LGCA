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

// Macros because why not?
#define LEFT  0
#define RIGHT 1
#define UL	  2
#define DR    3
#define UR	  4
#define DL    5

class cell{
	private:
		bool particle[6];//Left, right, UL, UR, DL, DR
		coord p;
	public:
		cell(){}
		cell(int x, int y)
		{
			p.x = x;
			p.y = y;
			memset(&particle, 0, 6);//No particles rn
		}
		coord getCoord()
		{
			return p;
		}
		bool* getArr()
		{
			return (bool*)particle;
		}
};

class simulation{
	private:
		cell* cellist;
		int ww,ll;
		char* dis;
		cell* nex;
		void CheckSideWalls()
		{
			for(int i=2;i<ll-2;i++)//Exclude extreme corners where not only up/down changes but left-right too.
			{
				if(*((cellist+1+i*ww)->getArr()+UL)==true)
				{
					*((cellist+1+i*ww)->getArr()+UL)=false;
					*((cellist+1+i*ww)->getArr()+UR)=true;
				}
				if(*((cellist+1+i*ww)->getArr()+LEFT)==true)
				{
					*((cellist+1+i*ww)->getArr()+LEFT)=false;
					*((cellist+1+i*ww)->getArr()+RIGHT)=true;
				}
				if(*((cellist+1+i*ww)->getArr()+DL)==true)
				{
					*((cellist+1+i*ww)->getArr()+DL)=false;
					*((cellist+1+i*ww)->getArr()+DR)=true;
				}
				//----------------------------------------------------
				if(*((cellist+(ww-2)+i*ww)->getArr()+UR)==true)
				{
					*((cellist+(ww-2)+i*ww)->getArr()+UR)=false;
					*((cellist+(ww-2)+i*ww)->getArr()+UL)=true;
				}
				if(*((cellist+(ww-2)+i*ww)->getArr()+RIGHT)==true)
				{
					*((cellist+(ww-2)+i*ww)->getArr()+RIGHT)=false;
					*((cellist+(ww-2)+i*ww)->getArr()+LEFT)=true;
				}
				if(*((cellist+(ww-2)+i*ww)->getArr()+DR)==true)
				{
					*((cellist+(ww-2)+i*ww)->getArr()+DR)=false;
					*((cellist+(ww-2)+i*ww)->getArr()+DL)=true;
				}
				
			}
		}
		void Checkfloorceil()
		{
			for(int i=2;i<ww-2;i++)//Exclude extreme corners where not only up/down changes but left-right too.
			{
				if(*((cellist+i+1*ww)->getArr()+UL)==true)
				{
					*((cellist+i+1*ww)->getArr()+UL)=false;
					*((cellist+i+1*ww)->getArr()+DL)=true;
				}
				if(*((cellist+i+1*ww)->getArr()+UR)==true)
				{
					*((cellist+i+1*ww)->getArr()+UR)=false;
					*((cellist+i+1*ww)->getArr()+DR)=true;
				}
				if(*((cellist+i+(ll-2)*ww)->getArr()+DL)==true)
				{
					*((cellist+i+(ll-2)*ww)->getArr()+DL)=false;
					*((cellist+i+(ll-2)*ww)->getArr()+UL)=true;
				}
				if(*((cellist+i+(ll-2)*ww)->getArr()+DR)==true)
				{
					*((cellist+i+(ll-2)*ww)->getArr()+DR)=false;
					*((cellist+i+(ll-2)*ww)->getArr()+UR)=true;
				}
			}
		}
		void rule2(int j, int i)
		{
			// Case 1: Left and Right exist.
			if(*((cellist+j+i*ww)->getArr()+LEFT)==true){
				if(*((cellist+j+i*ww)->getArr()+RIGHT)==true)
				{
					*((cellist+j+i*ww)->getArr()+LEFT)=false;
					*((cellist+j+i*ww)->getArr()+RIGHT)=false;
					//Pick one
					switch(rand()%2)
					{
						case 0:{
							*((cellist+j+i*ww)->getArr()+UL)=true;
							*((cellist+j+i*ww)->getArr()+DR)=true;
							break;
						}
						case 1:{
							*((cellist+j+i*ww)->getArr()+UR)=true;
							*((cellist+j+i*ww)->getArr()+DL)=true;
							break;
						}
					}
					return;
				}}
			// Case 2: UL and DR exist
			if(*((cellist+j+i*ww)->getArr()+UL)==true){
				if(*((cellist+j+i*ww)->getArr()+DR)==true)
				{
					*((cellist+j+i*ww)->getArr()+UL)=false;
					*((cellist+j+i*ww)->getArr()+DR)=false;
					//Pick one
					switch(rand()%2)
					{
						case 0:{
							*((cellist+j+i*ww)->getArr()+UR)=true;
							*((cellist+j+i*ww)->getArr()+DL)=true;
							break;
						}
						case 1:{
							*((cellist+j+i*ww)->getArr()+LEFT)=true;
							*((cellist+j+i*ww)->getArr()+RIGHT)=true;
							break;
						}
					}
					return;
				}}
			//case 3: DL and UR exist
			if(*((cellist+j+i*ww)->getArr()+DL)==true){
				if(*((cellist+j+i*ww)->getArr()+UR)==true)
				{
					*((cellist+j+i*ww)->getArr()+DL)=false;
					*((cellist+j+i*ww)->getArr()+UR)=false;
					//Pick one
					switch(rand()%2)
					{
						case 0:{
							*((cellist+j+i*ww)->getArr()+UL)=true;
							*((cellist+j+i*ww)->getArr()+DR)=true;
							break;
						}
						case 1:{
							*((cellist+j+i*ww)->getArr()+LEFT)=true;
							*((cellist+j+i*ww)->getArr()+RIGHT)=true;
							return;
						}
					}
				}}
		}
		void rule3(int j, int i)
		{
			// Case 1: the three are a space apart.
			// i.e. LEFT, DR, UR <-> RIGHT, UL, DL
			if(*((cellist+j+i*ww)->getArr()+LEFT)==true&&*((cellist+j+i*ww)->getArr()+DR)==true&&*((cellist+j+i*ww)->getArr()+UR)==true){
				*((cellist+j+i*ww)->getArr()+LEFT)=false;
				*((cellist+j+i*ww)->getArr()+DR)=false;
				*((cellist+j+i*ww)->getArr()+UR)=false;
				*((cellist+j+i*ww)->getArr()+RIGHT)=true;
				*((cellist+j+i*ww)->getArr()+DL)=true;
				*((cellist+j+i*ww)->getArr()+UL)=true;
				return;
			}
			if(*((cellist+j+i*ww)->getArr()+RIGHT)==true&&*((cellist+j+i*ww)->getArr()+UL)==true&&*((cellist+j+i*ww)->getArr()+DL)==true){
				*((cellist+j+i*ww)->getArr()+LEFT)=true;
				*((cellist+j+i*ww)->getArr()+DR)=true;
				*((cellist+j+i*ww)->getArr()+UR)=true;
				*((cellist+j+i*ww)->getArr()+RIGHT)=false;
				*((cellist+j+i*ww)->getArr()+DL)=false;
				*((cellist+j+i*ww)->getArr()+UL)=false;
				return;
			}
			// Case 2: Two have an head-on collision, one is on another axis.
			// The head-on collision ones change path to free axis, the third one continues its path.
			if(*((cellist+j+i*ww)->getArr()+RIGHT)==true&&*((cellist+j+i*ww)->getArr()+LEFT)==true)
			{
				*((cellist+j+i*ww)->getArr()+RIGHT)=false;
				*((cellist+j+i*ww)->getArr()+LEFT)=false;
				//Find the third one
				if(*((cellist+j+i*ww)->getArr()+UR)||*((cellist+j+i*ww)->getArr()+DL))
				{
					*((cellist+j+i*ww)->getArr()+UL)=true;
					*((cellist+j+i*ww)->getArr()+DR)=true;
				}
				else
				{
					*((cellist+j+i*ww)->getArr()+UR)=true;
					*((cellist+j+i*ww)->getArr()+DL)=true;
				}
				return;
			}
			if(*((cellist+j+i*ww)->getArr()+UL)==true&&*((cellist+j+i*ww)->getArr()+DR)==true)
			{
				*((cellist+j+i*ww)->getArr()+UL)=false;
				*((cellist+j+i*ww)->getArr()+DR)=false;
				//Find the third one
				if(*((cellist+j+i*ww)->getArr()+LEFT)||*((cellist+j+i*ww)->getArr()+RIGHT))
				{
					*((cellist+j+i*ww)->getArr()+UR)=true;
					*((cellist+j+i*ww)->getArr()+DL)=true;
				}
				else
				{
					*((cellist+j+i*ww)->getArr()+LEFT)=true;
					*((cellist+j+i*ww)->getArr()+RIGHT)=true;
				}
				return;
			}
			if(*((cellist+j+i*ww)->getArr()+UR)==true&&*((cellist+j+i*ww)->getArr()+DL)==true)
			{
				*((cellist+j+i*ww)->getArr()+UR)=false;
				*((cellist+j+i*ww)->getArr()+DL)=false;
				//Find the third one
				if(*((cellist+j+i*ww)->getArr()+LEFT)||*((cellist+j+i*ww)->getArr()+RIGHT))
				{
					*((cellist+j+i*ww)->getArr()+UL)=true;
					*((cellist+j+i*ww)->getArr()+DR)=true;
				}
				else
				{
					*((cellist+j+i*ww)->getArr()+LEFT)=true;
					*((cellist+j+i*ww)->getArr()+RIGHT)=true;
				}
				return;
			}
		}
		void rule4(int j, int i)
		{
			// Figure out which axis is free
			if(!(*((cellist+j+i*ww)->getArr()+LEFT)||*((cellist+j+i*ww)->getArr()+RIGHT)))
			{
				*((cellist+j+i*ww)->getArr()+LEFT)=true;
				*((cellist+j+i*ww)->getArr()+RIGHT)=true;
				switch(rand()%2)
				{
					case 0:{
						*((cellist+j+i*ww)->getArr()+UL)=false;
						*((cellist+j+i*ww)->getArr()+DR)=false;
						break;
					}
					case 1:{
						*((cellist+j+i*ww)->getArr()+DL)=false;
						*((cellist+j+i*ww)->getArr()+UR)=false;
						break;
					}
				}
				return;
			}
			if(!(*((cellist+j+i*ww)->getArr()+UL)||*((cellist+j+i*ww)->getArr()+DR)))
			{
				*((cellist+j+i*ww)->getArr()+UL)=true;
				*((cellist+j+i*ww)->getArr()+DR)=true;
				switch(rand()%2)
				{
					case 0:{
						*((cellist+j+i*ww)->getArr()+UR)=false;
						*((cellist+j+i*ww)->getArr()+DL)=false;
						break;
					}
					case 1:{
						*((cellist+j+i*ww)->getArr()+LEFT)=false;
						*((cellist+j+i*ww)->getArr()+RIGHT)=false;
						break;
					}
				}
				return;
			}
			if(!(*((cellist+j+i*ww)->getArr()+DL)||*((cellist+j+i*ww)->getArr()+UR)))
			{
				*((cellist+j+i*ww)->getArr()+UR)=true;
				*((cellist+j+i*ww)->getArr()+DL)=true;
				switch(rand()%2)
				{
					case 0:{
						*((cellist+j+i*ww)->getArr()+UL)=true;
						*((cellist+j+i*ww)->getArr()+DR)=true;
						break;
					}
					case 1:{
						*((cellist+j+i*ww)->getArr()+LEFT)=true;
						*((cellist+j+i*ww)->getArr()+RIGHT)=true;
						break;
					}
				}
				return;
			}
		}
		//-------------------------------------------------------------------------
		void do_collisions()
		{
			// Boundary collisions
			Checkfloorceil();// Check for upper and lower walls
			CheckSideWalls();// Check for side walls
			//
			int ans=0;
			for(int i=0; i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					//Find number of cells
					ans=0;
					for(int k=0;k<6;k++)ans=ans+(*((cellist+j+i*ww)->getArr()+k)==true?1:0);
					switch(ans)
					{
						case 2:
							rule2(j,i);
							break;
						case 3:
							rule3(j,i);
							break;
						case 4:
							rule4(j,i);
							break;
						
						default:
							//rule2(i,j);
							break;
					}
				}
			}
			
		}
		
	public:
		simulation(int n, int w, int l)
		{
			//Setup display screen
			dis = (char*)malloc((ww+1)*(ll+1));
			memset(dis, 0, sizeof(dis));
			
			ww=w;
			ll=l;
			
			cellist = new cell[w*l];//Create original cell array
			nex = new cell[w*l];//Create temporary cell array
			for(int i=0;i<l;i++)
			{
				for(int j=0; j<w; j++)
				{
					*(cellist+j+i*w)=cell(j,i);
					*(nex+j+i*w)=*(cellist+j+i*w);
				}
			}
			
			for(int i=0;i<n;)
			{
				//Idea: Assign a random number of particles at each cell
				int x = rand()%(w-1)+1, y =rand()%(l-1)+1;
				for(int j=0;j<6;j++)
				{
					bool* curcell=(cellist+x + y*w)->getArr();
					*(curcell+j)=(rand()%2==0)?false:true;
					if(*(curcell+j)==true)i++;
				}
			}
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
    		
			for(int i=0;i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					int ans=0;
					for(int k=0;k<6;k++)ans=ans+(*((cellist+j+i*ww)->getArr()+k)==true?1:0);
					*(dis+j+i*ww)=(ans==0)?' ':ans+0x30;
				}
				*(dis+ww-1+i*ww)='\n';
			}
			puts(dis);
		}
		void next_gen()
		{
			do_collisions();//Handle boundary collisions and intra-cell collisions
			for(int i=1;i<ll-1;i++)
			{
				for(int j=1;j<ww-1;j++)
				{
					//Update nex according to what is in cellist
					*((nex+(j-1)+ i   *ww)->getArr()+LEFT) =*((cellist+j+i*ww)->getArr()+LEFT);
					*((nex+(j+1)+ i   *ww)->getArr()+RIGHT)=*((cellist+j+i*ww)->getArr()+RIGHT);
					*((nex+(j-1)+(i-1)*ww)->getArr()+UL)   =*((cellist+j+i*ww)->getArr()+UL);
					*((nex+(j+1)+(i-1)*ww)->getArr()+UR)   =*((cellist+j+i*ww)->getArr()+UR);
					*((nex+(j-1)+(i+1)*ww)->getArr()+DL)   =*((cellist+j+i*ww)->getArr()+DL);
					*((nex+(j+1)+(i+1)*ww)->getArr()+DR)   =*((cellist+j+i*ww)->getArr()+DR);
				}
			}
			//Copy nex to cellist
			for(int i=0;i<ll;i++)
			{
				for(int j=0; j<ww; j++)
				{
					*(cellist+j+i*ww)=*(nex+j+i*ww);
				}
			}
			
		}
		void add_particle(int x, int y, int d)
		{
			*((cellist+x+y*ww)->getArr()+d)=true;
		}
};

#define WIDTH 80
#define LENGTH 30
#define PARTICLES 10000
#define SIMTIME 100

int main()
{
	srand(time(0));
	int w=WIDTH, l=LENGTH;
	simulation s(PARTICLES, w, l);
	s.add_particle(14,14,UL);
	s.add_particle(14,6,DL);
	s.add_particle(6,6,DR);
	s.add_particle(6,14,UR);
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