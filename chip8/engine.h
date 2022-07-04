#pragma once
#define OLC_PGE_APPLICATION
#define MINIAUDIO_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <math.h> 
#include <lib/olcPixelGameEngine.h>
#include <lib/miniaudio.h>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

class Chip8;
class Engine : public olc::PixelGameEngine
{
	Chip8* interpreter;
	float last_speed_clock;
	float last_time_clock;
	float clock_time;
	float clock_speed;
	void setKey();

	ma_result result;
	ma_engine engine;

public:
	uint8_t keypad[16];
	uint32_t video[64 * 32];

	Engine(const char* filename, float clock_speed, float clock_time);
	~Engine();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};

Engine::~Engine()
{
}

void Engine::setKey() {
	GetKey(olc::Key::K1).bHeld ? keypad[1] = 1 : keypad[1] = 0;
	GetKey(olc::Key::K2).bHeld ? keypad[2] = 1 : keypad[2] = 0;
	GetKey(olc::Key::K3).bHeld ? keypad[3] = 1 : keypad[3] = 0;
	GetKey(olc::Key::K4).bHeld ? keypad[0xC] = 1 : keypad[0xC] = 0;
	GetKey(olc::Key::Q).bHeld ? keypad[4] = 1 : keypad[4] = 0;
	GetKey(olc::Key::W).bHeld ? keypad[5] = 1 : keypad[5] = 0;
	GetKey(olc::Key::E).bHeld ? keypad[6] = 1 : keypad[6] = 0;
	GetKey(olc::Key::R).bHeld ? keypad[0xD] = 1 : keypad[0xD] = 0;
	GetKey(olc::Key::A).bHeld ? keypad[7] = 1 : keypad[7] = 0;
	GetKey(olc::Key::S).bHeld ? keypad[8] = 1 : keypad[8] = 0;
	GetKey(olc::Key::D).bHeld ? keypad[9] = 1 : keypad[9] = 0;
	GetKey(olc::Key::F).bHeld ? keypad[0xE] = 1 : keypad[0xE] = 0;
	GetKey(olc::Key::Z).bHeld ? keypad[0xA] = 1 : keypad[0xA] = 0;
	GetKey(olc::Key::X).bHeld ? keypad[0] = 1 : keypad[0] = 0;
	GetKey(olc::Key::C).bHeld ? keypad[0xB] = 1 : keypad[0xB] = 0;
	GetKey(olc::Key::V).bHeld ? keypad[0xF] = 1 : keypad[0xF] = 0;
}

bool Engine::OnUserCreate()
{
	last_speed_clock = 0;
	last_time_clock = 0;
	return true;
}

class Engine;
class Chip8
{
	Engine* engine;
	uint8_t memory[4096];
	uint16_t pc;
	uint8_t regis[16];
	uint16_t index = 0x0;
	uint16_t stack[16];
	uint8_t sp = 0;
	uint16_t opcode = 0x0;

private:
#pragma region INSTC
	void OP_NULL();
	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();
	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_00E0();
	void OP_00EE();
	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();
	void OP_ExA1();
	void OP_Ex9E();
	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();

	typedef void (Chip8::* Chip8Func)();
	Chip8Func table[0xF + 1]{ &Chip8::OP_NULL };
	Chip8Func table0[0xE + 1]{ &Chip8::OP_NULL };
	Chip8Func table8[0xE + 1]{ &Chip8::OP_NULL };
	Chip8Func tableE[0xE + 1]{ &Chip8::OP_NULL };
	Chip8Func tableF[0x65 + 1]{ &Chip8::OP_NULL };

	void Table0()
	{
		((*this).*(table0[opcode & 0x000Fu]))();
	}

	void Table8()
	{
		((*this).*(table8[opcode & 0x000Fu]))();
	}

	void TableE()
	{
		((*this).*(tableE[opcode & 0x000Fu]))();
	}

	void TableF()
	{
		((*this).*(tableF[opcode & 0x00FFu]))();
	}

#pragma endregion

	

public:
	uint8_t delayTimer = 0x0;
	uint8_t soundTimer = 0x0;

public:
	Chip8(Engine* eng);
	~Chip8();
	void Cycle();
	bool LoadROM(const char* filename);

	std::default_random_engine randGen;
	std::uniform_int_distribution<unsigned int> randByte;
};

Chip8::Chip8(Engine* eng) 
	: engine(eng), 
	  randGen((unsigned int)std::chrono::system_clock::now().time_since_epoch().count())
{

#pragma region PLOT INSTRUCTION TABLE

	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;

#pragma endregion

	pc = START_ADDRESS;

	randByte = std::uniform_int_distribution<unsigned int>(0, 255U);

	const unsigned int FONTSET_SIZE = 80;
	uint8_t fontset[FONTSET_SIZE] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	//load font
	for (int i = 0; i < FONTSET_SIZE; i++) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}


}

Chip8::~Chip8()
{
}

bool Chip8::LoadROM(const char* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	
	if (file.is_open()) {
		file.seekg(0, file.end);
		std::streamoff size = file.tellg();
		char* buffer = new char[(int)size];

		file.seekg(0, file.beg);
		file.read(buffer, size);
		file.close();

		for (int i = 0; i < size; ++i) {
			memory[START_ADDRESS + i] = buffer[i];
		}


		delete[] buffer;
		return true;
	}
	return false;
}

Engine::Engine(const char* filename,float clock_speed = 500, float clock_time = 60) : interpreter(new Chip8(this))
{
	sAppName = "Chip-8 Emu";

	this->clock_speed = 1.0f / clock_speed;
	this->clock_time = 1.0f / clock_time;

	if (interpreter->LoadROM(filename)) {
		std::cout << "ROM OK\n";
	}

	//set Keypad 0
	for (int i = 0; i < 16; i++) {
		keypad[i] = 0;
	}

	result = ma_engine_init(NULL, &engine);

	if (result != MA_SUCCESS) {
		printf("Failed to initialize audio engine.");
	}

}

bool Engine::OnUserUpdate(float fElapsedTime)
{
	last_time_clock += fElapsedTime;
	last_speed_clock += fElapsedTime;

	setKey();

	Clear(olc::DARK_BLUE);

	if (last_speed_clock > this->clock_speed) {
		/*std::cout << "speed_clock = " << last_speed_clock << "\n";*/
		last_speed_clock = 0;
		interpreter->Cycle();
	}

	if (last_time_clock > this->clock_time) {
		/*std::cout << "time_clock = " << last_time_clock << "\n";*/
		last_time_clock = 0;
		if (interpreter->delayTimer > 0) {
			--interpreter->delayTimer;
		}
		if (interpreter->soundTimer > 0) {
			--interpreter->soundTimer;
			ma_engine_play_sound(&engine, "C:\\Users\\ACER\\source\\repos\\CHIP-8\\chip8\\Debug\\SOUND\\beep.mp3",NULL);
		}
	}

	//render to screen
	for (int y = 0; y < VIDEO_HEIGHT; y++) {
		for (int x = 0; x < VIDEO_WIDTH; x++) {

			int index = y * VIDEO_WIDTH + x;
			uint32_t pixel = video[index];
			Draw(olc::vd2d(x, y), pixel == 0x0 ? olc::BLACK : olc::WHITE);

		}
	}

	return true;
}

#pragma region INSTRUCTIONS

//NULL OP
void Chip8::OP_NULL() {}

//Clear the display
void Chip8::OP_00E0() {
	memset(engine->video, 0, sizeof(engine->video));
}

//Return from a subroutine
void Chip8::OP_00EE() {
	--sp;
	pc = stack[sp];
}

//Jump to address nnn
void Chip8::OP_1nnn() {
	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}

//Call subroutine at nnn
void Chip8::OP_2nnn() {
	uint16_t address = opcode & 0x0FFFu;
	stack[sp] = pc;

	sp++;
	pc = address;
}

//Skip next instruction if Vx = kk
void Chip8::OP_3xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (regis[Vx] == byte) {
		pc += 2;
	}
}

//Skip next instruction if Vx != kk
void Chip8::OP_4xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (regis[Vx] != byte) {
		pc += 2;
	}
}

//Skip next instruction if Vx = Vy
void Chip8::OP_5xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (regis[Vx] == regis[Vy]) {
		pc += 2;
	}
}

//Set Vx = kk
void Chip8::OP_6xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	regis[Vx] = byte;
}

//Set Vx = Vx + kk
void Chip8::OP_7xkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	regis[Vx] += byte;
}

//Set Vx = Vy
void Chip8::OP_8xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	regis[Vx] = regis[Vy];
}

//Set Vx = Vx OR Vy
void Chip8::OP_8xy1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	regis[Vx] = regis[Vx] | regis[Vy];
}

//Set Vx = Vx AND Vy
void Chip8::OP_8xy2() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	regis[Vx] = regis[Vx] & regis[Vy];
}

//Set Vx = Vx XOR Vy
void Chip8::OP_8xy3() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	regis[Vx] = regis[Vx] ^ regis[Vy];
}

//Set Vx = Vx + Vy, set VF = carry
void Chip8::OP_8xy4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = regis[Vx] + regis[Vy];

	if (sum > 255u) {
		regis[0xF] = 1;
	}
	else {
		regis[0xF] = 0;
	}

	regis[Vx] = sum & 0xFFu;
}

//Set Vx = Vx - Vy, set VF = NOT borrow
void Chip8::OP_8xy5() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (regis[Vx] > regis[Vy]) {
		regis[0xF] = 1;
	}
	else {
		regis[0xF] = 0;
	}

	regis[Vx] = regis[Vx] - regis[Vy];
}

//Set Vx = Vx SHR 1
void Chip8::OP_8xy6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	regis[0xF] = regis[Vx] & 0x1u;

	regis[Vx] = regis[Vx] >> 1;
}

//Set Vx = Vy - Vx, set VF = NOT borrow
void Chip8::OP_8xy7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (regis[Vy] > regis[Vx])
	{
		regis[0xF] = 1;
	}
	else
	{
		regis[0xF] = 0;
	}

	regis[Vx] = regis[Vy] - regis[Vx];
}

//Set Vx = Vx SHL 1
void Chip8::OP_8xyE() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	//0x80u = 1000 0000
	regis[0xF] = (regis[Vx] & 0x80u) >> 7u;

	regis[Vx] <<= 1;
}

//Skip next instruction if Vx != Vy
void Chip8::OP_9xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (regis[Vx] != regis[Vy]) {
		pc += 2;
	}
}

//Set I = nnn
void Chip8::OP_Annn() {
	uint16_t addr = opcode & 0x0FFFu;

	index = addr;
}

//Jump to location nnn + V0
void Chip8::OP_Bnnn() {
	uint16_t addr = opcode & 0x0FFFu;

	pc = addr + regis[0x0];
}

//Set Vx = random byte AND kk
void Chip8::OP_Cxkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	regis[Vx] = randByte(randGen) & byte;
}

//Draw Function
//Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
void Chip8::OP_Dxyn() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	//wrap if beyond boundries
	uint8_t xPos = regis[Vx] % VIDEO_WIDTH;
	uint8_t yPos = regis[Vy] % VIDEO_HEIGHT;

	regis[0xF] = 0;

	for (int row = 0; row < height; row++) {
		uint8_t spriteByte = memory[index + row];
		for (int col = 0; col < 8; col++) {
			//0x80u = 1000 0000
			uint8_t spritePixel = spriteByte & (0x80 >> col);
			uint32_t* screenPixel = &engine->video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			if (spritePixel) {

				if (*screenPixel == 0xFFFFFFFF) {
					regis[0xF] = 1;
				}

				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

//Skip next instruction if key with the value of Vx is pressed
void Chip8::OP_Ex9E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = regis[Vx];

	if (engine->keypad[key]) {
		pc += 2;
	}
}

//Skip next instruction if key with the value of Vx is not pressed
void Chip8::OP_ExA1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = regis[Vx];

	if (!(engine->keypad[key])) {
		pc += 2;
	}
}

//Set Vx = delay timer value
void Chip8::OP_Fx07() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	regis[Vx] = delayTimer;
}

//Wait for a key press, store the value of the key in Vx
void Chip8::OP_Fx0A() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	bool setKey = FALSE;
	for (int i = 0; i < 16; i++) {
		if (engine->keypad[i])
		{
			regis[Vx] = i;
			setKey = TRUE;
		}
	}

	if (!setKey) {
		pc -= 2;
	}
}

//Set delay timer = Vx
void Chip8::OP_Fx15() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = regis[Vx];
}

//Set sound timer = Vx
void Chip8::OP_Fx18() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = regis[Vx];
}

//Set I = I + Vx
void Chip8::OP_Fx1E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index = index + regis[Vx];
}

//Set I = location of sprite for digit Vx
void Chip8::OP_Fx29() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = regis[Vx];

	index = FONTSET_START_ADDRESS + (5 * digit);
}

//Hex numbers to Base-10 for display
//Store BCD representation of Vx in memory locations I, I+1, and I+2
void Chip8::OP_Fx33() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = regis[Vx];

	memory[index + 2] = value % 10;
	value /= 10;

	memory[index + 1] = value % 10;
	value /= 10;

	memory[index] = value % 10;
}

//Store registers V0 through Vx in memory starting at location I
void Chip8::OP_Fx55() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (int i = 0; i <= Vx; i++) {
		memory[index + i] = regis[i];
	}
}

//Read registers V0 through Vx from memory starting at location I
void Chip8::OP_Fx65() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (int i = 0; i <= Vx; i++) {
		regis[i] = memory[index + i];
	}
}
#pragma endregion

void Chip8::Cycle() {
	//Fetch
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	// Increment PC
	pc += 2;

	//Decode and Execute
	(this->*(table[(opcode & 0xF000u) >> 12u]))();
}