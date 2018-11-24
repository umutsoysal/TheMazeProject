#include <stdio.h>
#include "fssimplewindow.h"
#include "shoot_class.h"



int main(void)
{
    PRINT
    
    FsOpenWindow(0,0,800,600,1);

	GameMenu menu;
	ShootingGame game(100,100);

	for(;;)
	{
		menu.Run();
		if(FSKEY_ESC==menu.lastKey)
		{
			break;
		}
		else if(FSKEY_S==menu.lastKey)
		{
			game.Run();
		}
	}

	return 0;
}

