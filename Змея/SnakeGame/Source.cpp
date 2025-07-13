#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <time.h>
#include <Windows.h>

//#define DevInfo // Подключить для дисплея разработчика

// Размер поля и символы
const int HEIGHT = 10, WIDTH = 10;
const char fieldSymbol = '.';
const char snakeHeadSymbol = '@';
const char snakeSymbol = '#';
const char appleSymbol = 'O';

// Важные переменные
int snakeSize = 0;
int applePosition[2];
char snakeDirection = 'u';
char snakeTailDirection = 'u';

// Координаты сегментов змеи
struct Segment {
	int x;
	int y;
};
// Прототип для корректной работы
bool snakeCreate(char field[HEIGHT][WIDTH], Segment snake, Segment snakeArr[]); 

// Инициализация поля 
void fieldInitialize(char field[HEIGHT][WIDTH]) {

	for (int i = 0; i < HEIGHT; i++) { // Заполнение в столбец (y)
		for (int j = 0; j < WIDTH; j++) { // Заполнение в строку (x)
			field[i][j] = fieldSymbol;
		}
	}
}

// Инициализация змейки
void initializeSnake(char field[HEIGHT][WIDTH], Segment snake[]) {

	// Начальная позиция змейки
	snake[0].x = WIDTH / 2;
	snake[0].y = HEIGHT / 2;

	// Создание символа змеи
	field[snake[0].y][snake[0].x] = snakeSymbol;
}

// Отображение игрового поля 
void fieldDisplay(char field[HEIGHT][WIDTH], Segment snake[]) {

	// Инфо разработчика
	#ifdef DevInfo
		std::cout << "Snake size: " << snakeSize << std::endl;
		std::cout << "Apple position: " << applePosition[1] << ' ' << applePosition[0] << std::endl;
		std::cout << "Snake direction: " << snakeDirection << std::endl;
		std::cout << "Snake last segment direction: " << snakeTailDirection << std::endl;
		std::cout << "Snake last segment position: " << snake[snakeSize].x << ' ' << snake[snakeSize].y << std::endl;
	#endif // DevInfo

	// Счетчик очков
	std::cout << "Score: " << snakeSize + 1 << std::endl;

	field[applePosition[0]][applePosition[1]] = appleSymbol; // Отображение яблока

	// Отображение игрового поля
	for (int i = 0; i < HEIGHT; i++) { // Движение по ординате

		for (int j = 0; j < WIDTH; j++) { // Движение по абциссе
			std::cout << field[i][j];
		}
		std::cout << std::endl; 
	}
	std::cout << std::endl;
}

// Определение направления хвоста змеи
void setSnakeTailDirection(const Segment snake[], int tempLastY, int tempLastX) {

	if (snake[snakeSize].y == tempLastY - 1) snakeTailDirection = 'u';
	if (snake[snakeSize].y == tempLastY + 1) snakeTailDirection = 'd';
	if (snake[snakeSize].x == tempLastX - 1) snakeTailDirection = 'l';
	if (snake[snakeSize].x == tempLastX + 1) snakeTailDirection = 'r';
}

// Увеличения размера змейки
bool snakeGrow(char field[HEIGHT][WIDTH], Segment snake[]) {
	// Проверка на максимальный размер
	if (snakeSize < (HEIGHT * WIDTH) - 2) {
		snakeSize++; // Увеличение размера
		snake[snakeSize] = snake[snakeSize - 1]; // Создание нового сегмента змеи путем дублирования

		// Ориентир на направление хвоста если змея длиннее двух сегментов
		if (snakeSize > 1) {
			switch (snakeTailDirection) {
			case 'u': snake[snakeSize].y++; break;
			case 'd': snake[snakeSize].y--; break;
			case 'l': snake[snakeSize].x++; break;
			case 'r': snake[snakeSize].x--; break;
			}
		}
		// Ориентир на направление головы для короткой змеи
		else {
			switch (snakeDirection) {
			case 'u': snake[snakeSize].y++; break;
			case 'd': snake[snakeSize].y--; break;
			case 'l': snake[snakeSize].x++; break;
			case 'r': snake[snakeSize].x--; break;
			}
		}
		snakeCreate(field, snake[snakeSize], snake); // Отображение змеи на поле

		return true;
	}
	else return false;
}

// Генерация яблок
void appleGeneration(char field[HEIGHT][WIDTH], const Segment snake[]) {

	// Генерация яблока в рандомной точке
	for (int i = 0; i < 15; i ++) {
		bool segmentExist = false; // Проверка на наличие змеи в точке генерации яблока

		// Генерация яблока по X и Y
		int appleTempPositionX = rand() % (WIDTH - 1);
		int appleTempPositionY = rand() & (HEIGHT - 1);

		for (int i = 0; i < snakeSize; i++) {
			// Повторная генерация, если любой сегмент змеи находится в точке генерации яблока 
			if (snake[i].y == appleTempPositionY && snake[i].x == appleTempPositionX) {
				segmentExist = true;
				break;
			}
		}

		// Проверка на существование змеи в координатах яблока
		if (!segmentExist) {
			applePosition[0] = appleTempPositionY;
			applePosition[1] = appleTempPositionX;
			break;
		}

		// Ручная генерация яблока в крайнем случае 
		if (i == 14) {

			// Перебор поля в поисках пустого значения
			for (int j = 0; j < HEIGHT; j++) { 
				for (int k = 0; k < WIDTH; k++) {
					// Проверка на наличие змеи в точке массива
					if (field[j][k] != snakeSymbol) {
						field[j][k] = appleSymbol;
						// Создание яблока
						applePosition[0] = j;
						applePosition[1] = k;
					
						j = HEIGHT; // Выход их циклов
						break;
					}
				}
			}
		}
	}
}

// Создание символа змеи, взаимодействие с яблоками
bool snakeCreate(char field[HEIGHT][WIDTH], Segment snake, Segment snakeArr[]) {

	field[snake.y][snake.x] = snakeSymbol; // Создание символа змеи
	
	// Проверка на яблоко в точке создания змейки
	if (snake.y == applePosition[0] && snake.x == applePosition[1]) {

		applePosition[0] = -1;
		applePosition[1] = -1;
		if (!snakeGrow(field, snakeArr)) return false; // Увеличение змейки, проверка на максимальный размер
		appleGeneration(field, snakeArr); // Генерация нового яблока
	}
	return true;

}

// Удаление символа змеи
void snakeDelete(char field[HEIGHT][WIDTH], Segment snake) {
	field[snake.y][snake.x] = fieldSymbol;
}

// Движение вверх
bool moveUp(char field[HEIGHT][WIDTH], Segment snake[]) {

	// Проверка выхода за поле
	if (snake[0].y > 0) {

		snakeDelete(field, snake[snakeSize]); // Удаление последнего сегмета

		if (field[snake[0].y - 1][snake[0].x] == snakeSymbol) return false; // Проверка на столкновение при движении вверх

		// Копирование новых координат головы
		int headX = snake[0].x;
		int headY = snake[0].y - 1;

		// Перетаскивание координат каждого сегмента на место предыдущего
		for (int i = snakeSize; i > 0; i--) {
			// Запоминание координат хвоста змеи
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;
			
			snake[i] = snake[i - 1];
			
			// Cмена направления конца змеи
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}

		// Обновление позиции головы
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // Создание головы змеи в новой позициию, проверка на максимальный размер

		snakeDirection = 'u'; // Смена направления головы
		return true;
	}
	else return false;
}

// Движение вниз
bool moveDown(char field[HEIGHT][WIDTH], Segment snake[]) {

	// Проверка выхода за поле
	if (snake[0].y < HEIGHT - 1) {

		snakeDelete(field, snake[snakeSize]); // Удаление последнего сегмета

		if (field[snake[0].y + 1][snake[0].x] == snakeSymbol) return false; // Проверка на столкновение при движении вниз

		// Копирование новых координат головы
		int headX = snake[0].x;
		int headY = snake[0].y + 1;

		// Перетаскивание координат каждого сегмента на место предыдущего
		for (int i = snakeSize; i > 0; i--) {
			// Запоминание координат хвоста змеи
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;

			snake[i] = snake[i - 1];

			// Cмена направления конца змеи
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}
		// Обновление позиции головы
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // Создание головы змеи в новой позициию, проверка на максимальный размер

		snakeDirection = 'd'; // Смена направления головы
		return true;
	}
	else return false;
}

// Движение влево
bool moveLeft(char field[HEIGHT][WIDTH], Segment snake[]) {

	// Проверка выхода за поле
	if (snake[0].x > 0) {
		
		snakeDelete(field, snake[snakeSize]); // Удаление последнего сегмета

		if (field[snake[0].y][snake[0].x - 1] == snakeSymbol) return false; // Проверка на столкновение при движении влево

		// Копирование новых координат головы

		int headX = snake[0].x - 1;
		int headY = snake[0].y;

		// Перетаскивание координат каждого сегмента на место предыдущего
		for (int i = snakeSize; i > 0; i--) {
			// Запоминание координат хвоста змеи
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;

			snake[i] = snake[i - 1];

			// Cмена направления конца змеи
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}
		// Обновление позиции головы
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // Создание головы змеи в новой позициию, проверка на максимальный размер

		snakeDirection = 'l'; // Смена направления головы
		return true;
	}
	else return false;
}

// Движение вправо
bool moveRight(char field[HEIGHT][WIDTH], Segment snake[]) {

	// Проверка выхода за поле
	if (snake[0].x < WIDTH - 1) {

		snakeDelete(field, snake[snakeSize]); // Удаление последнего сегмета
		
		if (field[snake[0].y][snake[0].x + 1] == snakeSymbol) return false; // Проверка на столкновение при движении вправо

		// Копирование новых координат головы
		int headX = snake[0].x + 1;
		int headY = snake[0].y;

		// Перетаскивание координат каждого сегмента на место предыдущего
		for (int i = snakeSize; i > 0; i--) {
			// Запоминание координат хвоста змеи
			int tempLastY = snake[snakeSize].y;
			int tempLastX = snake[snakeSize].x;

			snake[i] = snake[i - 1];

			// Cмена направления конца змеи
			if (i == snakeSize) {
				setSnakeTailDirection(snake, tempLastY, tempLastX);
			}
		}
		// Обновление позиции головы
		snake[0].x = headX;
		snake[0].y = headY;

		if (!snakeCreate(field, snake[0], snake)) return false; // Создание головы змеи в новой позициию, проверка на максимальный размер

		snakeDirection = 'r'; // Смена направления головы
		return true;

	}
	else return false;
}

int main() {
	srand(time(NULL)); // Отключение привязки генерации случайных чисел ко времени

	char field0[HEIGHT][WIDTH];
	bool playing = true; 
	int gameSpeedMilliseconds = 400; // Скорость игры

	Segment snake[HEIGHT * WIDTH + 1];

	fieldInitialize(field0); 
	initializeSnake(field0, snake);

	char userKey = 'w'; // Ввод пользователя по умолчанию

	appleGeneration(field0, snake); // Первоначальная генерация яблока
	fieldDisplay(field0, snake); // Отображение поля
	
	// Игровой цикл
	do {

		system("cls"); // Очистка экрана
		
		// Проверка зажатия клавиш движения
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

		// Движение взависимости от направлении змейки
		switch (snakeDirection) {
		case 'u': if (!moveUp(field0, snake)) playing = false; break;
		case 'd': if (!moveDown(field0, snake)) playing = false; break;
		case 'l': if (!moveLeft(field0, snake)) playing = false; break;
		case 'r': if (!moveRight(field0, snake)) playing = false; break;
		}

		if (playing) {
			fieldDisplay(field0, snake); // Отображение поля
			std::this_thread::sleep_for(std::chrono::milliseconds(gameSpeedMilliseconds)); // Скорость игры и считывания клавиш
		}

	} while (playing);

	// Выводы конечных надписей
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Небольшая задержки перед конечной заставкой

	if (snakeSize >= HEIGHT * WIDTH - 2) std::cout << "You won!\nScore: " << snakeSize + 1 << std::endl;
	else std::cout << "Game is over!\nScore: " << snakeSize + 1 << std::endl;
	system("pause");
	return 0;
}
