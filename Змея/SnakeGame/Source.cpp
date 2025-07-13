#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>
#include <Windows.h>

//#define DevInfo // ���������� ��� ������� ������������

// ������ ���� � �������
const int HEIGHT = 10, WIDTH = 10;
const char fieldSymbol = '.';
const char snakeHeadSymbol = '@';
const char snakeSymbol = '#';
const char appleSymbol = 'O';

// ������ ����������
int snakeSize = 0;
int applePosition[2];
char snakeDirection = 'u';
char snakeTailDirection = 'u';

// ���������� ��������� ����
struct Segment {
	int x;
	int y;
};
// �������� ��� ���������� ������
bool snakeCreate(char field[HEIGHT][WIDTH], Segment snake, Segment snakeArr[]); 

// ������������� ���� 
void fieldInitialize(char field[HEIGHT][WIDTH]) {

	for (int i = 0; i < HEIGHT; i++) { // ���������� � ������� (y)
		for (int j = 0; j < WIDTH; j++) { // ���������� � ������ (x)
			field[i][j] = fieldSymbol;
		}
	}
}

// ������������� ������
void initializeSnake(char field[HEIGHT][WIDTH], Segment snake[]) {

	// ��������� ������� ������
	snake[0].x = WIDTH / 2;
	snake[0].y = HEIGHT / 2;

	// �������� ������� ����
	field[snake[0].y][snake[0].x] = snakeSymbol;
}

// ����������� �������� ���� 
void fieldDisplay(char field[HEIGHT][WIDTH], Segment snake[]) {

	// ���� ������������
	#ifdef DevInfo
		std::cout << "Snake size: " << snakeSize << std::endl;
		std::cout << "Apple position: " << applePosition[1] << ' ' << applePosition[0] << std::endl;
		std::cout << "Snake direction: " << snakeDirection << std::endl;
		std::cout << "Snake last segment direction: " << snakeTailDirection << std::endl;
		std::cout << "Snake last segment position: " << snake[snakeSize].x << ' ' << snake[snakeSize].y << std::endl;
	#endif // DevInfo

	// ������� �����
	std::cout << "Score: " << snakeSize + 1 << std::endl;

	field[applePosition[0]][applePosition[1]] = appleSymbol; // ����������� ������

	// ����������� �������� ����
	for (int i = 0; i < HEIGHT; i++) { // �������� �� ��������

		for (int j = 0; j < WIDTH; j++) { // �������� �� �������
			std::cout << field[i][j];
		}
		std::cout << std::endl; 
	}
	std::cout << std::endl;
}

// ����������� ����������� ������ ����
void setSnakeTailDirection(const Segment snake[], int tempLastY, int tempLastX) {

	if (snake[snakeSize].y == tempLastY - 1) snakeTailDirection = 'u';
	if (snake[snakeSize].y == tempLastY + 1) snakeTailDirection = 'd';
	if (snake[snakeSize].x == tempLastX - 1) snakeTailDirection = 'l';
	if (snake[snakeSize].x == tempLastX + 1) snakeTailDirection = 'r';
}

// ���������� ������� ������
bool snakeGrow(char field[HEIGHT][WIDTH], Segment snake[]) {
	// �������� �� ������������ ������
	if (snakeSize < (HEIGHT * WIDTH) - 2) {
		snakeSize++; // ���������� �������
		snake[snakeSize] = snake[snakeSize - 1]; // �������� ������ �������� ���� ����� ������������

		// �������� �� ����������� ������ ���� ���� ������� ���� ���������
		if (snakeSize > 1) {
			switch (snakeTailDirection) {
			case 'u': snake[snakeSize].y++; break;
			case 'd': snake[snakeSize].y--; break;
			case 'l': snake[snakeSize].x++; break;
			case 'r': snake[snakeSize].x--; break;
			}
		}
		// �������� �� ����������� ������ ��� �������� ����
		else {
			switch (snakeDirection) {
			case 'u': snake[snakeSize].y++; break;
			case 'd': snake[snakeSize].y--; break;
			case 'l': snake[snakeSize].x++; break;
			case 'r': snake[snakeSize].x--; break;
			}
		}
		snakeCreate(field, snake[snakeSize], snake); // ����������� ���� �� ����

		return true;
	}
	else return false;
}

// ��������� �����
void appleGeneration(char field[HEIGHT][WIDTH], const Segment snake[]) {

	// ��������� ������ � ��������� �����
	for (int i = 0; i < 15; i ++) {
		bool segmentExist = false; // �������� �� ������� ���� � ����� ��������� ������

		// ��������� ������ �� X � Y
		int appleTempPositionX = rand() % (WIDTH - 1);
		int appleTempPositionY = rand() & (HEIGHT - 1);

		for (int i = 0; i < snakeSize; i++) {
			// ��������� ���������, ���� ����� ������� ���� ��������� � ����� ��������� ������ 
			if (snake[i].y == appleTempPositionY && snake[i].x == appleTempPositionX) {
				segmentExist = true;
				break;
			}
		}

		// �������� �� ������������� ���� � ����������� ������
		if (!segmentExist) {
			applePosition[0] = appleTempPositionY;
			applePosition[1] = appleTempPositionX;
			break;
		}

		// ������ ��������� ������ � ������� ������ 
		if (i == 14) {

			// ������� ���� � ������� ������� ��������
			for (int j = 0; j < HEIGHT; j++) { 
				for (int k = 0; k < WIDTH; k++) {
					// �������� �� ������� ���� � ����� �������
					if (field[j][k] != snakeSymbol) {
						field[j][k] = appleSymbol;
						// �������� ������
						applePosition[0] = j;
						applePosition[1] = k;
					
						j = HEIGHT; // ����� �� ������
						break;
					}
				}
			}
		}
	}
}

// �������� ������� ����, �������������� � ��������
bool snakeCreate(char field[HEIGHT][WIDTH], Segment snake, Segment snakeArr[]) {

	field[snake.y][snake.x] = snakeSymbol; // �������� ������� ����
	
	// �������� �� ������ � ����� �������� ������
	if (snake.y == applePosition[0] && snake.x == applePosition[1]) {

		applePosition[0] = -1;
		applePosition[1] = -1;
		if (!snakeGrow(field, snakeArr)) return false; // ���������� ������, �������� �� ������������ ������
		appleGeneration(field, snakeArr); // ��������� ������ ������
	}
	return true;

}

// �������� ������� ����
void snakeDelete(char field[HEIGHT][WIDTH], Segment snake) {
	field[snake.y][snake.x] = fieldSymbol;
}

// �������� �����
bool moveUp(char field[HEIGHT][WIDTH], Segment snake[]) {

	// �������� ������ �� ����
	if (snake[0].y > 0) {

		snakeDelete(field, snake[snakeSize]); // �������� ���������� �������

		if (field[snake[0].y - 1][snake[0].x] == snakeSymbol) return false; // �������� �� ������������ ��� �������� �����

		// ����������� ����� ��������� ������
		int headX = snake[0].x;
		int headY = snake[0].y - 1;

		// �������������� ��������� ������� �������� �� ����� �����������
		for (int i = snakeSize; i > 0; i--) {
			// ����������� ��������� ������ ����
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;
			
			snake[i] = snake[i - 1];
			
			// C���� ����������� ����� ����
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}

		// ���������� ������� ������
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // �������� ������ ���� � ����� ��������, �������� �� ������������ ������

		snakeDirection = 'u'; // ����� ����������� ������
		return true;
	}
	else return false;
}

// �������� ����
bool moveDown(char field[HEIGHT][WIDTH], Segment snake[]) {

	// �������� ������ �� ����
	if (snake[0].y < HEIGHT - 1) {

		snakeDelete(field, snake[snakeSize]); // �������� ���������� �������

		if (field[snake[0].y + 1][snake[0].x] == snakeSymbol) return false; // �������� �� ������������ ��� �������� ����

		// ����������� ����� ��������� ������
		int headX = snake[0].x;
		int headY = snake[0].y + 1;

		// �������������� ��������� ������� �������� �� ����� �����������
		for (int i = snakeSize; i > 0; i--) {
			// ����������� ��������� ������ ����
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;

			snake[i] = snake[i - 1];

			// C���� ����������� ����� ����
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}
		// ���������� ������� ������
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // �������� ������ ���� � ����� ��������, �������� �� ������������ ������

		snakeDirection = 'd'; // ����� ����������� ������
		return true;
	}
	else return false;
}

// �������� �����
bool moveLeft(char field[HEIGHT][WIDTH], Segment snake[]) {

	// �������� ������ �� ����
	if (snake[0].x > 0) {
		
		snakeDelete(field, snake[snakeSize]); // �������� ���������� �������

		if (field[snake[0].y][snake[0].x - 1] == snakeSymbol) return false; // �������� �� ������������ ��� �������� �����

		// ����������� ����� ��������� ������

		int headX = snake[0].x - 1;
		int headY = snake[0].y;

		// �������������� ��������� ������� �������� �� ����� �����������
		for (int i = snakeSize; i > 0; i--) {
			// ����������� ��������� ������ ����
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;

			snake[i] = snake[i - 1];

			// C���� ����������� ����� ����
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}
		// ���������� ������� ������
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // �������� ������ ���� � ����� ��������, �������� �� ������������ ������

		snakeDirection = 'l'; // ����� ����������� ������
		return true;
	}
	else return false;
}

// �������� ������
bool moveRight(char field[HEIGHT][WIDTH], Segment snake[]) {

	// �������� ������ �� ����
	if (snake[0].x < WIDTH - 1) {

		snakeDelete(field, snake[snakeSize]); // �������� ���������� �������
		
		if (field[snake[0].y][snake[0].x + 1] == snakeSymbol) return false; // �������� �� ������������ ��� �������� ������

		// ����������� ����� ��������� ������
		int headX = snake[0].x + 1;
		int headY = snake[0].y;

		// �������������� ��������� ������� �������� �� ����� �����������
		for (int i = snakeSize; i > 0; i--) {
			// ����������� ��������� ������ ����
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;

			snake[i] = snake[i - 1];

			// C���� ����������� ����� ����
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}
		// ���������� ������� ������
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // �������� ������ ���� � ����� ��������, �������� �� ������������ ������

		snakeDirection = 'r'; // ����� ����������� ������
		return true;

	}
	else return false;
}

int main() {
	srand(time(NULL)); // ���������� �������� ��������� ��������� ����� �� �������

	char field0[HEIGHT][WIDTH];
	bool playing = true; 
	int gameSpeedMilliseconds = 400; // �������� ����

	Segment snake[HEIGHT * WIDTH + 1];

	fieldInitialize(field0); 
	initializeSnake(field0, snake);

	char userKey = 'w'; // ���� ������������ �� ���������

	appleGeneration(field0, snake); // �������������� ��������� ������
	fieldDisplay(field0, snake); // ����������� ����
	
	// ������� ����
	do {

		system("cls"); // ������� ������
		
		// �������� ������� ������ ��������
		if (GetAsyncKeyState('W') & 0x8000) {
			if (snakeDirection != 'd') {
				snakeDirection = 'u';
			}
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			if (snakeDirection != 'u') {
				snakeDirection = 'd';
			}
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			if (snakeDirection != 'r') {
				snakeDirection = 'l';
			}
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			if (snakeDirection != 'l') {
				snakeDirection = 'r';
			}
		}
		if (GetAsyncKeyState('E') & 0x8000) {
			appleGeneration(field0, snake);
		}

		// �������� ������������ �� ����������� ������
		switch (snakeDirection) {
		case 'u': if (!moveUp(field0, snake)) playing = false; break;
		case 'd': if (!moveDown(field0, snake)) playing = false; break;
		case 'l': if (!moveLeft(field0, snake)) playing = false; break;
		case 'r': if (!moveRight(field0, snake)) playing = false; break;
		}

		if (playing) {
			fieldDisplay(field0, snake); // ����������� ����
			std::this_thread::sleep_for(std::chrono::milliseconds(gameSpeedMilliseconds)); // �������� ���� � ���������� ������
		}

	} while (playing);

	// ������ �������� ��������
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ��������� �������� ����� �������� ���������

	if (snakeSize >= HEIGHT * WIDTH - 2) std::cout << "You won!\nScore: " << snakeSize + 1 << std::endl;
	else std::cout << "Game is over!\nScore: " << snakeSize + 1 << std::endl;
	return 0;
}