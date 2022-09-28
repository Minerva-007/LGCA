// ---------------------------------------------------
// As Dr. Zia said, start files with comments lol.
// Anyways, this file yolos a Lattice Gas Automata.
#include"stdio.h"
#include"time.h"
#include"stdlib.h"
#include"conio.h"
#include"string.h"
#include"windows.h"
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
#define BOUNDARY 6
#define XSRC 7
#define YSRC 8
#define SINK 9

class cell{
	private:
		bool particle[10];//Left, right, UL, UR, DL, DR, BOUNDARY, XSRC, YSRC
		coord p;
	public:
		cell(){}
		cell(int x, int y)
		{
			p.x = x;
			p.y = y;
			memset(&particle, 0, 10);//No particles rn
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
		void genXSRC()
		{
			for(int i=0; i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					if(*((cellist+j+i*ww)->getArr()+XSRC))
					{
						//Add particles moving towards RIGHT, UL, or DL to its right
						switch(rand()%3)
						{
							case 0:{
								*((cellist+(j+1)+(i)*ww)->getArr()+RIGHT)=true;
								break;
							}
							case 1:{
								*((cellist+(j+1)+(i)*ww)->getArr()+UR)=true;
								break;
							}
							case 2:{
								*((cellist+(j+1)+(i)*ww)->getArr()+DR)=true;
								break;
							}
						}
					}
				}
			}
		}
		void genYSRC()
		{
			for(int i=0; i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					if(*((cellist+j+i*ww)->getArr()+YSRC))
					{
						//Add particles moving towards either UL or UR above it
						switch(rand()%2)
						{
							case 0:{
								*((cellist+(j)+(i-1)*ww)->getArr()+UL)=true;
								break;
							}
							case 1:{
								*((cellist+(j)+(i-1)*ww)->getArr()+UR)=true;
								break;
							}
						}
					}
				}
			}
		}
		void CheckSinks()
		{
			for(int i=0; i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					if(*((cellist+j+i*ww)->getArr()+SINK))
					{
						//Remove particles from its surroundings that are headed towards it.
						*((cellist+(j-1)+(i)*ww)->getArr()+LEFT)=false;
						*((cellist+(j+1)+(i)*ww)->getArr()+RIGHT)=false;
						*((cellist+(j-1)+(i-1)*ww)->getArr()+UL)=false;
						*((cellist+(j+1)+(i-1)*ww)->getArr()+UR)=false;
						*((cellist+(j-1)+(i+1)*ww)->getArr()+DL)=false;
						*((cellist+(j+1)+(i+1)*ww)->getArr()+DR)=false;
					}
				}
			}
		}
		void CheckBoundaries(int j, int i)
		{
			if(*((cellist+j+i*ww)->getArr()+BOUNDARY)||*((cellist+j+i*ww)->getArr()+XSRC)||*((cellist+j+i*ww)->getArr()+YSRC)||*((cellist+j+i*ww)->getArr()+SINK))return;			
			if(*((cellist+j+i*ww)->getArr()+LEFT))
			{
				if(*((cellist+(j-1)+i*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+LEFT)=false;
					*((cellist+j+i*ww)->getArr()+RIGHT)=true;
				}
			}
			if(*((cellist+j+i*ww)->getArr()+RIGHT))
			{
				if(*((cellist+(j+1)+i*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+RIGHT)=false;
					*((cellist+j+i*ww)->getArr()+LEFT)=true;
				}
			}
			//----------------------------------
			if(*((cellist+j+i*ww)->getArr()+UL))
			{
				if(*((cellist+(j-1)+i*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+UL)=false;
					*((cellist+j+i*ww)->getArr()+UR)=true;
				}
				if(*((cellist+(j)+(i-1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+UL)=false;
					*((cellist+j+i*ww)->getArr()+DL)=true;
				}
				if(*((cellist+(j-1)+(i-1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+UL)=false;
					*((cellist+j+i*ww)->getArr()+DR)=true;
				}
			}
			if(*((cellist+j+i*ww)->getArr()+UR))
			{
				if(*((cellist+(j+1)+i*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+UR)=false;
					*((cellist+j+i*ww)->getArr()+UL)=true;
				}
				if(*((cellist+(j)+(i-1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+UR)=false;
					*((cellist+j+i*ww)->getArr()+DR)=true;
				}
				if(*((cellist+(j+1)+(i-1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+UR)=false;
					*((cellist+j+i*ww)->getArr()+DL)=true;
				}
			}
			if(*((cellist+j+i*ww)->getArr()+DL))
			{
				if(*((cellist+j+(i+1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+DL)=false;
					*((cellist+j+i*ww)->getArr()+UL)=true;
				}
				if(*((cellist+(j-1)+(i)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+DL)=false;
					*((cellist+j+i*ww)->getArr()+DR)=true;
				}
				if(*((cellist+(j-1)+(i+1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+DL)=false;
					*((cellist+j+i*ww)->getArr()+UR)=true;
				}
			}
			if(*((cellist+j+i*ww)->getArr()+DR))
			{
				if(*((cellist+j+(i+1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+DR)=false;
					*((cellist+j+i*ww)->getArr()+UR)=true;
				}
				if(*((cellist+(j+1)+i*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+DR)=false;
					*((cellist+j+i*ww)->getArr()+DL)=true;
				}
				if(*((cellist+(j+1)+(i+1)*ww)->getArr()+BOUNDARY))
				{
					*((cellist+j+i*ww)->getArr()+DR)=false;
					*((cellist+j+i*ww)->getArr()+UL)=true;
				}
			}
		}
		void CheckSideWalls()
		{
			for(int i=1;i<ll-1;i++)//Exclude extreme corners where not only up/down changes but left-right too.
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
			for(int i=1;i<ww-1;i++)//Exclude extreme corners where not only up/down changes but left-right too.
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
			if(*((cellist+j+i*ww)->getArr()+BOUNDARY)||*((cellist+j+i*ww)->getArr()+XSRC)||*((cellist+j+i*ww)->getArr()+YSRC)||*((cellist+j+i*ww)->getArr()+SINK))return;
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
			if(*((cellist+j+i*ww)->getArr()+BOUNDARY)||*((cellist+j+i*ww)->getArr()+XSRC)||*((cellist+j+i*ww)->getArr()+YSRC)||*((cellist+j+i*ww)->getArr()+SINK))return;
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
			if(*((cellist+j+i*ww)->getArr()+BOUNDARY)||*((cellist+j+i*ww)->getArr()+XSRC)||*((cellist+j+i*ww)->getArr()+YSRC)||*((cellist+j+i*ww)->getArr()+SINK))return;
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
			// Check for sinks
			CheckSinks();
			// Check for sources
			genXSRC();
			genYSRC();
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
					CheckBoundaries(j,i);
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
    		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
			GetConsoleScreenBufferInfo(output, &csbiInfo);
    		#endif
    		SetConsoleTextAttribute(output,9);//
			for(int i=0;i<ll;i++)
			{
				for(int j=0;j<ww;j++)
				{
					int ans=0;
					if(*((cellist+j+i*ww)->getArr()+BOUNDARY))
					{
						*(dis+j+i*ww)=219;
						continue;
					}
					if(*((cellist+j+i*ww)->getArr()+XSRC))
					{
						*(dis+j+i*ww)='>';
						continue;
					}
					if(*((cellist+j+i*ww)->getArr()+YSRC))
					{
						*(dis+j+i*ww)='^';
						continue;
					}
					if(*((cellist+j+i*ww)->getArr()+SINK))
					{
						*(dis+j+i*ww)='#';
						continue;
					}
					for(int k=0;k<6;k++)ans=ans+(*((cellist+j+i*ww)->getArr()+k)==true?1:0);
					switch(ans)
					{
						case 0:*(dis+j+i*ww)=' ';break;
						case 1:*(dis+j+i*ww)='.';break;
						case 2:*(dis+j+i*ww)=176;break;
						case 3:*(dis+j+i*ww)=176;break;
						case 4:*(dis+j+i*ww)=177;break;
						case 5:*(dis+j+i*ww)=177;break;
						case 6:*(dis+j+i*ww)=178;break;
					}
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
					if(!(*((cellist+j+i*ww)->getArr()+BOUNDARY)||*((cellist+j+i*ww)->getArr()+XSRC)||*((cellist+j+i*ww)->getArr()+YSRC)||*((cellist+j+i*ww)->getArr()+SINK)))
					{
						*((nex+(j-1)+ i   *ww)->getArr()+LEFT) 	=*((cellist+j+i*ww)->getArr()+LEFT);
						*((nex+(j+1)+ i   *ww)->getArr()+RIGHT)	=*((cellist+j+i*ww)->getArr()+RIGHT);
						*((nex+(j-1)+(i-1)*ww)->getArr()+UL) 	=*((cellist+j+i*ww)->getArr()+UL);
						*((nex+(j+1)+(i-1)*ww)->getArr()+UR)    =*((cellist+j+i*ww)->getArr()+UR);
						*((nex+(j-1)+(i+1)*ww)->getArr()+DL)    =*((cellist+j+i*ww)->getArr()+DL);
						*((nex+(j+1)+(i+1)*ww)->getArr()+DR)   	=*((cellist+j+i*ww)->getArr()+DR);
					}
					*((nex+j+i*ww)->getArr()+BOUNDARY)      =*((cellist+j+i*ww)->getArr()+BOUNDARY);
					*((nex+j+i*ww)->getArr()+XSRC)  	    =*((cellist+j+i*ww)->getArr()+XSRC);
					*((nex+j+i*ww)->getArr()+YSRC)    	    =*((cellist+j+i*ww)->getArr()+YSRC);
					*((nex+j+i*ww)->getArr()+SINK)   	    =*((cellist+j+i*ww)->getArr()+SINK);
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

#define WIDTH 60
#define LENGTH 20
#define PARTICLES 0000
#define SIMTIME 1000

int main()
{
	srand(time(0));
	int w=WIDTH, l=LENGTH;
	simulation s(PARTICLES, w, l);
	for(int i=2;i<50;i++){s.add_particle(i,10,BOUNDARY);}//Central line
	//Top cover
	s.add_particle(1,2,BOUNDARY);
	s.add_particle(2,2,BOUNDARY);
	s.add_particle(3,2,BOUNDARY);
	s.add_particle(4,2,BOUNDARY);
	s.add_particle(5,2,BOUNDARY);
	s.add_particle(6,2,BOUNDARY);
	s.add_particle(7,2,BOUNDARY);
	s.add_particle(8,2,BOUNDARY);
	s.add_particle(9,2,BOUNDARY);
	s.add_particle(10,2,BOUNDARY);
	s.add_particle(11,2,BOUNDARY);
	s.add_particle(12,2,BOUNDARY);
	s.add_particle(13,2,BOUNDARY);
	s.add_particle(14,2,BOUNDARY);
	s.add_particle(15,2,BOUNDARY);
	s.add_particle(16,2,BOUNDARY);
	s.add_particle(17,2,BOUNDARY);
	s.add_particle(18,2,BOUNDARY);
	s.add_particle(19,2,BOUNDARY);
	s.add_particle(20,2,BOUNDARY);
	s.add_particle(21,2,BOUNDARY);
	s.add_particle(22,2,BOUNDARY);
	s.add_particle(23,2,BOUNDARY);
	s.add_particle(24,2,BOUNDARY);
	s.add_particle(25,2,BOUNDARY);
	s.add_particle(26,2,BOUNDARY);
	s.add_particle(27,2,BOUNDARY);
	s.add_particle(28,2,BOUNDARY);
	s.add_particle(29,2,BOUNDARY);
	s.add_particle(30,2,BOUNDARY);
	s.add_particle(31,2,BOUNDARY);
	s.add_particle(32,2,BOUNDARY);
	s.add_particle(33,2,BOUNDARY);
	s.add_particle(34,2,BOUNDARY);
	s.add_particle(35,2,BOUNDARY);
	s.add_particle(36,2,BOUNDARY);
	s.add_particle(37,2,BOUNDARY);
	s.add_particle(38,2,BOUNDARY);
	s.add_particle(39,2,BOUNDARY);
	s.add_particle(40,2,BOUNDARY);
	s.add_particle(41,2,BOUNDARY);
	s.add_particle(42,2,BOUNDARY);
	s.add_particle(43,2,BOUNDARY);
	s.add_particle(44,2,BOUNDARY);
	s.add_particle(45,2,BOUNDARY);
	s.add_particle(46,2,BOUNDARY);
	s.add_particle(47,2,BOUNDARY);
	s.add_particle(48,2,BOUNDARY);
	s.add_particle(49,2,BOUNDARY);
	s.add_particle(50,2,BOUNDARY);
	s.add_particle(51,2,BOUNDARY);
	s.add_particle(52,2,BOUNDARY);
	s.add_particle(53,2,BOUNDARY);
	s.add_particle(54,2,BOUNDARY);
	s.add_particle(55,2,BOUNDARY);
	s.add_particle(56,2,BOUNDARY);
	s.add_particle(57,2,BOUNDARY);
	s.add_particle(58,2,BOUNDARY);
	//Bottom cover
	s.add_particle(1,18,BOUNDARY);
	s.add_particle(2,18,BOUNDARY);
	s.add_particle(3,18,BOUNDARY);
	s.add_particle(4,18,BOUNDARY);
	s.add_particle(5,18,BOUNDARY);
	s.add_particle(6,18,BOUNDARY);
	s.add_particle(7,18,BOUNDARY);
	s.add_particle(8,18,BOUNDARY);
	s.add_particle(9,18,BOUNDARY);
	s.add_particle(10,18,BOUNDARY);
	s.add_particle(11,18,BOUNDARY);
	s.add_particle(12,18,BOUNDARY);
	s.add_particle(13,18,BOUNDARY);
	s.add_particle(14,18,BOUNDARY);
	s.add_particle(15,18,BOUNDARY);
	s.add_particle(16,18,BOUNDARY);
	s.add_particle(17,18,BOUNDARY);
	s.add_particle(18,18,BOUNDARY);
	s.add_particle(19,18,BOUNDARY);
	s.add_particle(20,18,BOUNDARY);
	s.add_particle(21,18,BOUNDARY);
	s.add_particle(22,18,BOUNDARY);
	s.add_particle(23,18,BOUNDARY);
	s.add_particle(24,18,BOUNDARY);
	s.add_particle(25,18,BOUNDARY);
	s.add_particle(26,18,BOUNDARY);
	s.add_particle(27,18,BOUNDARY);
	s.add_particle(28,18,BOUNDARY);
	s.add_particle(29,18,BOUNDARY);
	s.add_particle(30,18,BOUNDARY);
	s.add_particle(31,18,BOUNDARY);
	s.add_particle(32,18,BOUNDARY);
	s.add_particle(33,18,BOUNDARY);
	s.add_particle(34,18,BOUNDARY);
	s.add_particle(35,18,BOUNDARY);
	s.add_particle(36,18,BOUNDARY);
	s.add_particle(37,18,BOUNDARY);
	s.add_particle(38,18,BOUNDARY);
	s.add_particle(39,18,BOUNDARY);
	s.add_particle(40,18,BOUNDARY);
	s.add_particle(41,18,BOUNDARY);
	s.add_particle(42,18,BOUNDARY);
	s.add_particle(43,18,BOUNDARY);
	s.add_particle(44,18,BOUNDARY);
	s.add_particle(45,18,BOUNDARY);
	s.add_particle(46,18,BOUNDARY);
	s.add_particle(47,18,BOUNDARY);
	s.add_particle(48,18,BOUNDARY);
	s.add_particle(49,18,BOUNDARY);
	s.add_particle(50,18,BOUNDARY);
	s.add_particle(51,18,BOUNDARY);
	s.add_particle(52,18,BOUNDARY);
	s.add_particle(53,18,BOUNDARY);
	s.add_particle(54,18,BOUNDARY);
	s.add_particle(55,18,BOUNDARY);
	s.add_particle(56,18,BOUNDARY);
	s.add_particle(57,18,BOUNDARY);
	s.add_particle(58,18,BOUNDARY);
	//End cap
	s.add_particle(58,2,BOUNDARY);
	s.add_particle(58,3,BOUNDARY);
	s.add_particle(58,4,BOUNDARY);
	s.add_particle(58,5,BOUNDARY);
	s.add_particle(58,6,BOUNDARY);
	s.add_particle(58,7,BOUNDARY);
	s.add_particle(58,8,BOUNDARY);
	s.add_particle(58,9,BOUNDARY);
	s.add_particle(58,10,BOUNDARY);
	s.add_particle(58,11,BOUNDARY);
	s.add_particle(58,12,BOUNDARY);
	s.add_particle(58,13,BOUNDARY);
	s.add_particle(58,14,BOUNDARY);
	s.add_particle(58,15,BOUNDARY);
	s.add_particle(58,16,BOUNDARY);
	s.add_particle(58,17,BOUNDARY);
	s.add_particle(58,18,BOUNDARY);
	//Wall in the path
	s.add_particle(30,7,BOUNDARY);
	s.add_particle(30,6,BOUNDARY);
	s.add_particle(30,5,BOUNDARY);
	//Wall in the path
	s.add_particle(40,2,BOUNDARY);
	s.add_particle(40,3,BOUNDARY);
	s.add_particle(40,4,BOUNDARY);
	s.add_particle(40,8,BOUNDARY);
	s.add_particle(40,9,BOUNDARY);
	//Wall in the path
	s.add_particle(30,11,BOUNDARY);
	s.add_particle(30,12,BOUNDARY);
	s.add_particle(30,13,BOUNDARY);
	s.add_particle(30,14,BOUNDARY);
	//Source
	s.add_particle(3,3,XSRC);
	s.add_particle(3,4,XSRC);
	s.add_particle(3,5,XSRC);
	s.add_particle(3,6,XSRC);
	s.add_particle(3,7,XSRC);
	s.add_particle(3,8,XSRC);
	s.add_particle(3,9,XSRC);
	//Sink
	s.add_particle(3,11,SINK);
	s.add_particle(3,12,SINK);
	s.add_particle(3,13,SINK);
	s.add_particle(3,14,SINK);
	s.add_particle(3,15,SINK);
	s.add_particle(3,16,SINK);
	s.add_particle(3,17,SINK);
	system("pause");
	for(int i=0;i<SIMTIME;i++)
	{
		#ifndef DEBUG
		Sleep(1500/(i+15)*2);
		#endif
		s.display_lattice();
		s.next_gen();
		#ifdef DEBUG
		system("pause");
		#endif
	}
	return 0;
}