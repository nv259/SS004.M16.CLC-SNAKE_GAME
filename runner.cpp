// OTS : of the snake
#include    <iostream>
#include    <windows.h>
#include	<stdexcept>
#include    <conio.h>
#include    <cstdlib>
#include	<random>
#include    <ctime>

void gotoxy(int column, int line)
{
	COORD coord{};
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

struct  Point {
	int x, y;
	// using pre_x, pre_y to store pre position of this point
	// so that it is easier to update next point OTS
	int pre_x, pre_y;
};

struct High_score
{
	int score;
	char user_name[30];
};

class SNAKE {
public:
	int snake_length;
	Point snake[100];

	void move(char direct) {
		/* move the head */
		snake[0].pre_x = snake[0].x;
		snake[0].pre_y = snake[0].y;
		switch (direct) {
		case 'a':
			snake[0].x--;
			break;
		case 's':
			snake[0].y++;
			break;
		case 'd':
			snake[0].x++;
			break;
		case 'w':
			snake[0].y--;
			break;
		}

		/* move the part */
		for (int i = 1; i < snake_length; i++)
		{
			snake[i].pre_x = snake[i].x;
			snake[i].pre_y = snake[i].y;
			snake[i].x = snake[i - 1].pre_x;
			snake[i].y = snake[i - 1].pre_y;
		}
	}

	void init() {
		snake_length = 3;
		snake[0].x = 14;
		snake[0].y = 10;
		snake[1].x = 15;
		snake[1].y = 10;
		snake[2].x = 16;
		snake[2].y = 10;
	}

	void draw() {
		for (int i = 0; i < snake_length; i++)
		{
			gotoxy(snake[i].x, snake[i].y);
			if (i == 0) std::cout << 'O';
			else std::cout << 'o';
		}
	}
};

class FOOD {
public:
	int x, y;

	void make_food() {
		std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<int> distX(0, 29);
		std::uniform_int_distribution<int> distY(0, 19);

		x = distX(gen);
		y = distY(gen);
	}

	bool isAble(SNAKE s) {
		/* fruit can't be on the edges of the board */
		if (x == 0 || x == 30 || y == 0 || y == 20) return false;

		/* fruit can't be in the snake */
		for (int i = 0; i < s.snake_length; i++)
			if (s.snake[i].x == x && s.snake[i].y == y) return false;
		return true;
	}

	void init(SNAKE s) {
		do {
			make_food();
		} while (!isAble(s));
	}

	void draw() {
		gotoxy(x, y);
		std::cout << (char)254;
		gotoxy(0, 23);
	}
};

int player_score;

class BOARD {
public:
	void    draw() {
		std::cout << (char)218;
		gotoxy(1, 0);
		for (int i = 1; i < 31; i++)
			std::cout << (char)196;
		std::cout << (char)191 << '\n';
		for (int row = 1; row < 21; row++)
		{
			for (int col = 1; col < 33; col++)
			{
				if (col == 1 || col == 32) std::cout << (char)179;
				else std::cout << " ";
			}
			std::cout << '\n';
		}
		std::cout << (char)192;
		for (int i = 1; i < 31; i++)
			std::cout << (char)196;
		std::cout << (char)217 << "\n\nSCORE: " << player_score;
	}
};

bool Game_over(SNAKE s, std::string& reason) {
	/* snake hit the wall */
	if (s.snake[0].x > 30 || s.snake[0].x < 1 || s.snake[0].y > 20 || s.snake[0].y < 1)
	{
		reason = "You hit the wall! Game over!";
		return true;
	}

	/* snake bite itself */
	for (int i = 1; i < s.snake_length; i++)
		if (s.snake[0].x == s.snake[i].x && s.snake[0].y == s.snake[i].y)
		{
			reason = "You bit yourself off! Game over!";
			return true;
		}

	return false;
}

void eat_food(SNAKE& s, FOOD& fruit) {
	if (s.snake[0].x == fruit.x && s.snake[0].y == fruit.y) {
		s.snake[s.snake_length].x = s.snake[s.snake_length - 1].x;
		s.snake[s.snake_length].y = s.snake[s.snake_length - 1].y;
		s.snake_length++;
		fruit.init(s);
		player_score += 10;
	}
}

bool able_to_move(char direct, char pre_direct) {
	if (direct == 'a' && pre_direct != 'd'
		|| direct == 'd' && pre_direct != 'a'
		|| direct == 's' && pre_direct != 'w'
		|| direct == 'w' && pre_direct != 's') return true;
	return false;
}

const std::string available_key = "xasdw";

SNAKE snake;
FOOD fruit;
BOARD board;

int main() {
	// ios_base::sync_with_stdio(false);
	// cin.tie(NULL);
	// srand(time(NULL));

	/* variables */

	SetConsoleCP(437);
	SetConsoleOutputCP(437);

	char direct = 'x', pre_direct = 'a';

	snake.init();
	fruit.init(snake);

	std::string reason;

	while (!Game_over(snake, reason))
	{
		if (_kbhit())
		{
			pre_direct = direct;
			direct = _getch();
			if (!available_key.find(direct)) direct = pre_direct;
			if (!able_to_move(direct, pre_direct)) direct = pre_direct;
		}
		system("cls");

		if (direct == 'x')
		{
			board.draw();
			snake.draw();
			fruit.draw();
			Sleep(100);
			continue;
		}

		board.draw();
		snake.move(direct);
		snake.draw();
		eat_food(snake, fruit);
		fruit.draw();
		Sleep(100);
	}
	gotoxy(0, 25);
	std::cout << reason;

	return 0;
}