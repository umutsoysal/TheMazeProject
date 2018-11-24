#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "fssimplewindow.h"

void DrawShip(int x,int y)
{
	glColor3ub(0,255,0);

	glBegin(GL_QUADS);

	glVertex2i(x-15,y);
	glVertex2i(x+15,y);
	glVertex2i(x+15,y-10);
	glVertex2i(x-15,y-10);

	glVertex2i(x-5,y);
	glVertex2i(x-5,y-20);
	glVertex2i(x+5,y-20);
	glVertex2i(x+5,y);

	glEnd();
}

class Target;

class Missile
{
public:
	int x,y,state;
	void Initialize(void);
	bool Launch(int x,int y);
	void Move(int mv);
	void Draw(void);
	void SpecialEffect(void);
	bool CheckCollision(Target &t);
};
void Missile::Initialize(void)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	state=0;
}
bool Missile::Launch(int x,int y)
{
	if(0==state)
	{
		this->x=x;
		this->y=y;
		this->state=1;
		return true;
	}
	return false;
}
void Missile::Move(int mv)
{
	y-=mv;
	if(y<0)
	{
		state=0;
	}
}
void Missile::Draw(void)
{
	glColor3ub(255,0,0);
	glBegin(GL_LINES);
	glVertex2i(x,y);
	glVertex2i(x,y+10);
	glEnd();
}



class Target
{
public:
	int v,state,x,y,w,h;
	void Initialize(void);
	void Move(void);
	void Draw(void);
	void Reverse(void);
};
bool Missile::CheckCollision(Target &t)
{
	int dx=x-t.x;
	int dy=y-t.y;
	if(0<=dx && dx<=t.w && 0<=dy && dy<=t.h)
	{
		return true;
	}
	return false;
}

void Target::Initialize(void)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	v=15+rand()%5;
	state=1;
	x=0;
	y=100+rand()%20;
	w=60+rand()%40;
	h=20;
}
void Target::Move(void)
{
	x+=v;
	if((800<x+w && 0<v) || (x<0 && v<0))
	{
		Reverse();
	}
}
void Target::Reverse(void)
{
	v=-v;
}
void Target::Draw(void)
{
	glColor3ub(0,255,255);
	glBegin(GL_QUADS);

	glVertex2i(x  ,y);
	glVertex2i(x+w,y);
	glVertex2i(x+w,y+h);
	glVertex2i(x  ,y+h);

	glEnd();
}



int sx,sy;
int mv=10;
const int nMissile=3;
Missile missile[nMissile];

const int nTarget=5;
Target target[nTarget];

void Render(void *)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	DrawShip(sx,sy);
	for(auto m : missile)
	{
		if(0!=m.state)
		{
			m.Draw();
		}
	}
	for(auto t : target)
	{
		if(0!=t.state)
		{
			t.Draw();
		}
	}
	FsSwapBuffers();
}

int main(void)
{
	FsOpenWindow(16,16,800,600,1);

	srand((int)time(nullptr));

	bool terminate=false;
	sx=400;
	sy=550;

	int nShot=0;

	FsRegisterOnPaintCallBack(Render,nullptr);
	for(auto &t : target)
	{
		t.Initialize();
	}
	for(auto &m : missile)
	{
		m.Initialize();
	}

	while(true!=terminate)
	{
		FsPollDevice();
		auto key=FsInkey();

		if(FSKEY_ESC==key)
		{
			terminate=true;
		}
		else if(FSKEY_LEFT==key)
		{
			sx-=20;
		}
		else if(FSKEY_RIGHT==key)
		{
			sx+=20;
		}
		else if(FSKEY_SPACE==key)
		{
			for(auto &m : missile)
			{
				if(true==m.Launch(sx,sy))
				{
					++nShot;
					break;
				}
			}
		}

		for(auto &m : missile)
		{
			if(0!=m.state)
			{
				m.Move(mv);
			}
		}
		for(auto &t : target)
		{
			if(0!=t.state)
			{
				t.Move();
			}
		}

		for(auto &m : missile)
		{
			if(0!=m.state)
			{
				for(auto &t : target)
				{
					if(0!=t.state && true==m.CheckCollision(t))
					{
						printf("Hit!\n");
						m.state=0;
						t.state=0;
						break;
					}
				}
			}
		}


		{
			int nLiveTarget=0;
			for(auto t : target)
			{
				if(0!=t.state)
				{
					++nLiveTarget;
				}
			}
			if(0==nLiveTarget)
			{
				printf("You've shot %d missiles.\n",nShot);
				break;
			}
		}


		FsPushOnPaintEvent();
		FsSleep(10);
	}

	return 0;
}

