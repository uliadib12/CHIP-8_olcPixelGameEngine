#include <engine.h>

int main()
{	
	Engine chip_8("C:\\Users\\ACER\\source\\repos\\CHIP-8\\chip8\\Debug\\ROM\\TICTAC");
	if (chip_8.Construct(64*2, 32*2, 10, 10))
		chip_8.Start();

    return 0;
}