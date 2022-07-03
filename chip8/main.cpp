#include <engine.h>

int main()
{	
	Engine chip_8("C:\\Users\\ACER\\source\\repos\\CHIP-8\\chip8\\Debug\\ROM\\PONG2");
	if (chip_8.Construct(64, 32, 15, 15))
		chip_8.Start();

    return 0;
}