#pragma once
#define OLC_PGE_APPLICATION
#define MINIAUDIO_IMPLEMENTATION
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <deque>
#include <vector>
#include <random>
#include <math.h>
#include <lib/olcPixelGameEngine.hpp>
#include <lib/miniaudio.hpp>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

class SettingMenu
{
	class GameMenu
	{
	public:
		std::string gameName;
		bool isgameMenu = false;
		int index = 0;
		int subIndex = 0;
		int afterEnter = 0;

		GameMenu() {};
		~GameMenu() {};
	};

public:
	int index = 0;
	GameMenu gameMenu;
	bool debugStatus;
	std::deque<std::string> menuList {"Resume","DEBUG MODE:","Game:"};
	std::vector<std::vector<std::string>> subList{ 
		std::vector<std::string>{"ON ", "OFF"}};

	std::string getSubList(int index) {
		if (index == 1) {
			std::string ret;
			debugStatus ? ret = subList.at(0).at(0) : ret = subList.at(0).at(1);
			return ret;
		}
		if (index == 2) {
			std::string ret;
			ret = gameMenu.gameName;
			return ret;
		}
		return "";
	}

	SettingMenu() {};
	~SettingMenu() {};

private:

};

class EngineSetting
{
public:
	bool isPause = false;
	int clock_speed;
	bool debugMode = false;
	std::deque<uint16_t> pc;
	std::deque<uint16_t> addr;

	EngineSetting() {};
	~EngineSetting() {};

private:

};

class Chip8;
class Engine : public olc::PixelGameEngine
{
	friend class Chip8;
	Chip8* interpreter;
	EngineSetting engine_setting;
	SettingMenu setting;
	std::vector<std::string> romList;
	float last_speed_clock;
	float last_time_clock;
	float clock_time;
	float clock_speed;
	void setKey();

	ma_result result_ma;
	ma_engine engine_ma;
	
	uint8_t keypad[16]{};
	uint32_t video[64 * 32]{};

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

public:
	Engine(const char* filename, float clock_speed, float clock_time);
	~Engine();

private:
	std::string intToHex(int i, int size = 2) {
		std::ostringstream ss;
		ss << std::hex << i;
		std::string result = ss.str();

		std::string hex;
		for (int i = 0; i < size - (int)result.length(); i++) {
			hex.append("0");
		}
		hex.append(result);

		return hex;
	}

	std::string toLowerCase(std::string input) {
		std::string str = input;
		int i = 0;
		while (i < str.length())
		{
			if (isupper(str[i]))
			{
				str[i] = tolower(str[i]);
			}
			i++;
		}
		return str;
	}
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
	friend class Engine;
	Engine* engine;
	uint8_t memory[4096];
	uint16_t pc;
	uint8_t regis[16];
	uint16_t index = 0x0;
	uint16_t stack[16];
	uint8_t sp = 0;
	uint16_t opcode = 0x0;

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

	uint8_t delayTimer = 0x0;
	uint8_t soundTimer = 0x0;

	Chip8(Engine* eng);
	~Chip8();
	void Cycle();
	bool LoadROM();
	bool LoadROM(std::string input);

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

bool Chip8::LoadROM()
{
	std::string pathGame = ".\\ROM\\";
	pathGame.append(engine->setting.gameMenu.gameName);
	std::ifstream file(pathGame, std::ios::binary | std::ios::ate);
	
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

bool Chip8::LoadROM(std::string input)
{
	std::string pathGame = ".\\ROM\\";
	pathGame.append(input);
	std::ifstream file(pathGame, std::ios::binary | std::ios::ate);

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

Engine::Engine(const char* filename = "TICTAC",float clock_speed = 500, float clock_time = 60) : interpreter(new Chip8(this))
{
	sAppName = "Chip-8 Emu";

	engine_setting.clock_speed = (int)clock_speed;
	setting.gameMenu.gameName = filename;
	setting.debugStatus = engine_setting.debugMode;

	this->clock_speed = 1.0f / clock_speed;
	this->clock_time = 1.0f / clock_time;

	namespace fs = std::experimental::filesystem;

	std::string path = ".\\ROM\\";

	for (const auto& file : fs::directory_iterator(path)) {
		romList.push_back(file.path().filename().string());
	}

	if (interpreter->LoadROM()) {
		std::cout << "ROM OK\n";
	}

	//set Keypad 0
	for (int i = 0; i < 16; i++) {
		keypad[i] = 0;
	}

	result_ma = ma_engine_init(NULL, &engine_ma);

	if (result_ma != MA_SUCCESS) {
		printf("Failed to initialize audio engine.");
	}

}

bool Engine::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::DARK_BLUE);

	setting.debugStatus = engine_setting.debugMode;

	if (GetKey(olc::Key::BACK).bPressed) {
		engine_setting.isPause ? engine_setting.isPause = false : engine_setting.isPause = true;
		setting.index = 0;
		setting.gameMenu.isgameMenu = false;
		setting.gameMenu.afterEnter = 0;
	}

	if (!engine_setting.isPause && !setting.gameMenu.isgameMenu) {

		last_time_clock += fElapsedTime;
		last_speed_clock += fElapsedTime;

		setKey();

		if (last_speed_clock > this->clock_speed) {
			/*std::cout << "speed_clock = " << last_speed_clock << "\n";*/
			if (engine_setting.addr.size() <= 4) {
				engine_setting.pc.push_front(interpreter->pc);
				engine_setting.addr.push_front(interpreter->opcode);
			}
			else {
				engine_setting.pc.pop_back();
				engine_setting.addr.pop_back();
			}
			interpreter->Cycle();
			last_speed_clock = 0;
		}

		if (last_time_clock > this->clock_time) {
			/*std::cout << "time_clock = " << last_time_clock << "\n";*/
			last_time_clock = 0;
			if (interpreter->delayTimer > 0) {
				--interpreter->delayTimer;
			}
			if (interpreter->soundTimer > 0) {
				--interpreter->soundTimer;
				ma_engine_play_sound(&engine_ma, "C:\\Users\\ACER\\source\\repos\\CHIP-8\\chip8\\Debug\\SOUND\\beep.mp3", NULL);
			}
		}

		int scale = engine_setting.debugMode ? 1 : 2;

		if (setting.debugStatus && !setting.gameMenu.isgameMenu) {
			//debug mode
			const float padx = 2;
			const float pady = 2;
			DrawStringDecal(olc::vd2d((VIDEO_WIDTH * scale) + padx, (0 * scale) + pady), "CLOCK SPEED:" + std::to_string(engine_setting.clock_speed), olc::WHITE, olc::vd2d(0.4f, 0.4f));

#pragma region REGISTER
			for (int i = 0; i < 16; i++) {
				std::ostringstream ss;
				ss << std::hex << i;
				std::string result = ss.str();
				std::string str = "R-";
				str.append(result);
				if (i < 8) {
					DrawStringDecal(olc::vd2d((VIDEO_WIDTH * scale) + padx, ((0 * scale) + pady) + (4 * i + 4)), str.append(":") + intToHex(interpreter->regis[i]), olc::WHITE, olc::vd2d(0.3f, 0.3f));
				}
				else {
					DrawStringDecal(olc::vd2d(((VIDEO_WIDTH * scale) + padx) + 30, ((0 * scale) + pady) + (4 * i + 4) - (4 * 8)), str.append(":") + intToHex(interpreter->regis[i]), olc::WHITE, olc::vd2d(0.3f, 0.3f));
				}
			}
			DrawStringDecal(olc::vd2d((VIDEO_WIDTH * scale) + padx, ((0 * scale) + pady) + (4 * 8 + 4)), "DT:" + std::to_string(interpreter->delayTimer), olc::WHITE, olc::vd2d(0.3f, 0.3f));
			DrawStringDecal(olc::vd2d((VIDEO_WIDTH * scale) + padx + 30, ((0 * scale) + pady) + (4 * 8 + 4)), "ST:" + std::to_string(interpreter->soundTimer), olc::WHITE, olc::vd2d(0.3f, 0.3f));
#pragma endregion

			for (int i = 0; i < (int)engine_setting.addr.size(); i++) {
				std::string out = intToHex(engine_setting.pc.at(i) - 0x200, 3).append(":").append(intToHex(engine_setting.addr.at(i), 4));
				olc::vd2d lenght = GetTextSize(out);

				FillRect(olc::vd2d((VIDEO_WIDTH * scale) + padx - 1, ((0 * scale) + pady) + (4 * (0 + 9) + 5) - 1), olc::vd2d(lenght.x * 0.3f + 1, lenght.y * 0.3f + 2), olc::WHITE);
				DrawStringDecal(olc::vd2d((VIDEO_WIDTH * scale) + padx, ((0 * scale) + pady) + (4 * (i + 9) + 5)), out, (i == 0) ? olc::DARK_BLUE : olc::WHITE, olc::vd2d(0.3f, 0.3f));
			}
		}
		

		//render to screen

		//resize
		int* temp = new int[(VIDEO_WIDTH * scale) * (VIDEO_HEIGHT * scale)];
		double x_ratio = VIDEO_WIDTH / (double)(VIDEO_WIDTH * scale);
		double y_ratio = VIDEO_HEIGHT / (double)(VIDEO_HEIGHT * scale);
		double px, py;
		for (int i = 0; i < (int)(VIDEO_HEIGHT * scale); i++) {
			for (int j = 0; j < (int)(VIDEO_WIDTH * scale); j++) {
				px = std::floor(j * x_ratio);
				py = std::floor(i * y_ratio);
				temp[(i * (VIDEO_WIDTH * scale)) + j] = video[(int)((py * VIDEO_WIDTH) + px)];
			}
		}

		//draw
		for (int y = 0; y < (int)(VIDEO_HEIGHT * scale); y++) {
			for (int x = 0; x < (int)(VIDEO_WIDTH * scale); x++) {

				int index = y * VIDEO_WIDTH * scale + x;
				uint32_t pixel = temp[index];
				Draw(olc::vd2d(x, y), pixel == 0x0 ? olc::BLACK : olc::WHITE);

			}
		}

		delete[] temp;
	}

	//Setting Menu
	if(engine_setting.isPause) {
		int item = setting.menuList.size();
		if (GetKey(olc::Key::UP).bPressed) {
			if (setting.index > 0) {
				setting.index--;
			}
		}
		if (GetKey(olc::Key::DOWN).bPressed) {
			if (setting.index < item - 1) {
				setting.index++;
			}
		}
		if (GetKey(olc::Key::ENTER).bPressed) {
			switch (setting.index)
			{
			case 0:
				engine_setting.isPause ? engine_setting.isPause = false : engine_setting.isPause = true;
				break;

			case 1:
				engine_setting.debugMode ? engine_setting.debugMode = false : engine_setting.debugMode = true;
				break;
			
			case 2:
				engine_setting.isPause ? engine_setting.isPause = false : engine_setting.isPause = true;
				setting.gameMenu.isgameMenu ? setting.gameMenu.isgameMenu = false : setting.gameMenu.isgameMenu = true;
				setting.gameMenu.index = 0;
				setting.gameMenu.subIndex = 0;
				break;

			default:
				break;
			}
		}


		Clear(olc::DARK_BLUE);

		float longX = 0;
		float longY = 0;
		for (int i = 0; i < (int)setting.menuList.size(); i++) {
			std::string out = setting.menuList.at(i) + setting.getSubList(i);
			if (GetTextSize(out).x > longX) {
				longX = (float)GetTextSize(out).x;
			}
			longY += GetTextSize(out).y;
		}

		int index = setting.index;
		std::string out = setting.menuList.at(index) + setting.getSubList(index);
		float def = (longX - GetTextSize(out).x) / 4.0f;
		FillRect(olc::vd2d(((ScreenWidth() / 2 - longX / 2 + 28) + def) - 0.5f, (ScreenHeight() / 2 + (0 + 5 * index) - longY / 4) - 0.5f), olc::vd2d((GetTextSize(out).x - (0.5f * GetTextSize(out).x)) + 1, (GetTextSize(out).y - 3.8f) + 1),olc::WHITE);

		for (int i = 0; i < (int)setting.menuList.size(); i++) {
			std::string out = setting.menuList.at(i) + setting.getSubList(i);
			float def = (longX - GetTextSize(out).x)/4.0f;
			DrawStringDecal(olc::vd2d((ScreenWidth() / 2 - longX / 2 + 28) + def, (ScreenHeight() / 2 + (0 + 5 * i)) - longY/4), out, i == index ? olc::DARK_BLUE : olc::WHITE, olc::vd2d(0.5f, 0.5f));
		}
	}

	if (setting.gameMenu.isgameMenu) {
		Clear(olc::DARK_BLUE);

		int maxIndex = std::ceil((float)romList.size() / 5.0f) - 1;
		int maxSubIndex;

		if ((romList.size() % 5 == 0)) {
			maxSubIndex = 5 - 1;
		}
		else {
			maxSubIndex = 5 - 1;
			if (setting.gameMenu.index == maxIndex) {
				maxSubIndex = (romList.size() % 5) - 1;
			}
		}

		if (GetKey(olc::Key::UP).bPressed) {
			if (setting.gameMenu.subIndex > 0) {
				setting.gameMenu.subIndex--;
			}
			else {
				if (setting.gameMenu.index > 0) {
					setting.gameMenu.index--;
					setting.gameMenu.subIndex = 5 - 1;
				}
			}
			/*std::cout << "INDEX" << setting.gameMenu.index << std::endl;
			std::cout << "SUBINDEX" << setting.gameMenu.subIndex << std::endl;*/
		}

		if (GetKey(olc::Key::DOWN).bPressed) {
			if (setting.gameMenu.subIndex < maxSubIndex) {
				setting.gameMenu.subIndex++;
			}
			else {
				if (setting.gameMenu.index < maxIndex) {
					setting.gameMenu.index++;
					setting.gameMenu.subIndex = 0;
				}
			}
			/*std::cout << "INDEX" << setting.gameMenu.index << std::endl;
			std::cout << "SUBINDEX" << setting.gameMenu.subIndex << std::endl;*/
		}

		float longX = 0;
		float longY = 0;
		{
			for (int i = 0; i < 5; i++) {
				
				if (i == 0) {
					for (int x = 0; x < 5; x++) {
						int index = (setting.gameMenu.index * maxIndex) + x;
						std::string out = (index < romList.size()) ? romList.at(index) : "";
						olc::vd2d size = GetTextSize(out);
						if ((float)size.x > longX) longX = (float)size.x;
					}
					for (int y = 0; y < 5; y++) {
						int index = (setting.gameMenu.index * maxIndex) + y;
						std::string out = (index < romList.size()) ? romList.at(index) : "";
						olc::vd2d size = GetTextSize(out);
						longY += (float)size.y;
					}
				}

				int index = (setting.gameMenu.index * maxIndex) + i;
				std::string out = (index < romList.size()) ? romList.at(index) : "";
				float center = (longX - GetTextSize(out).x)/4.0f;
				float offsetx = 13;
				if (i == 5 - 1) {
					FillRect(olc::vd2d(ScreenWidth() / 2.0f - longX / 2.0f + offsetx - 0.5f, (ScreenHeight() / 2 + (5 * setting.gameMenu.subIndex) - longY / 4.0f) - 0.5f), olc::vd2d(longX - (longX * 0.5f) + 1, 4.f + 1),olc::WHITE);
				}
				DrawStringDecal(olc::vd2d(ScreenWidth()/2.0f - longX/2.0f + center + offsetx,ScreenHeight()/2 + (5 * i) - longY/4.0f), out , toLowerCase(out) == toLowerCase(setting.gameMenu.gameName) ? olc::RED : setting.gameMenu.subIndex == i ? olc::DARK_BLUE : olc::WHITE, olc::vd2d(0.5f, 0.5f));
			}
		}

		float scale = 4;
		if (setting.gameMenu.index > 0) {
			olc::vd2d top(ScreenWidth() / 2.0f + longX / 2.0f - 5, ScreenHeight() / 2 - longY / 4.0f);
			FillTriangle((olc::vd2d(0.5, 0) *= scale) += top, (olc::vd2d(1, 1) *= scale) += top, (olc::vd2d(0, 1) *= scale) += top, olc::RED);
		}
		if (setting.gameMenu.index < maxIndex) {
			olc::vd2d buttom(ScreenWidth() / 2.0f + longX / 2.0f - 5, ScreenHeight()/2 + longY / 4.0f);
			FillTriangle((olc::vd2d(0, 0) *= scale) += buttom, (olc::vd2d(1, 0) *= scale) += buttom, (olc::vd2d(0.5f, 1) *= scale) += buttom, olc::RED);
		}

		if (GetKey(olc::Key::ENTER).bReleased) {
			setting.gameMenu.afterEnter = 1;
		}

		if (GetKey(olc::Key::ENTER).bPressed) {
			int index = (setting.gameMenu.index * maxIndex) + setting.gameMenu.subIndex;
			if (!(setting.gameMenu.gameName == romList.at(index)) && setting.gameMenu.afterEnter == 1) {
				if (interpreter->LoadROM(romList.at(index))) {
					memset(video, 0, sizeof(video));
					memset(interpreter->regis, 0, sizeof(interpreter->regis));
					memset(keypad, 0, sizeof(keypad));
					setting.gameMenu.gameName = romList.at(index);
					interpreter->pc = START_ADDRESS;
					interpreter->delayTimer = 0;
					interpreter->soundTimer = 0;
					/*std::cout << "Change\n" << std::endl;*/
				};
			}
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
				if(!((yPos + row) * VIDEO_WIDTH + (xPos + col) > ((64 * 32) - 1) && (yPos + row) * VIDEO_WIDTH + (xPos + col) < 0))
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