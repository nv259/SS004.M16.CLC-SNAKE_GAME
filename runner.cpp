// OTS : of the snake
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <random>

void gotoXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

struct Point
{
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

class SNAKE
{
public:
	int snake_length;
	Point snake[100];

	void move(char direct)
	{
		/*move the head*/
		snake[0].pre_x = snake[0].x;
		snake[0].pre_y = snake[0].y;
		switch (direct)
		{
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

		/*move the part*/
		for (int i = 1; i < snake_length; i++)
		{
			snake[i].pre_x = snake[i].x;
			snake[i].pre_y = snake[i].y;
			snake[i].x = snake[i - 1].pre_x;
			snake[i].y = snake[i - 1].pre_y;
		}
	}

	void init()
	{
		snake_length = 3;
		snake[0].x = 14;
		snake[0].y = 10;
		snake[1].x = 15;
		snake[1].y = 10;
		snake[2].x = 16;
		snake[2].y = 10;
	}

	void draw()
	{
		for (int i = 0; i < snake_length; i++)
		{
			gotoXY(snake[i].x, snake[i].y);
			if (i == 0) std::cout << '@';
			else std::cout << '#';
		}
	}
};

class FOOD
{
public:
	int x, y;

	void make_food()
	{
		std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<int> distX(1, 29);
		std::uniform_int_distribution<int> distY(1, 19);

		x = distX(gen);
		y = distY(gen);
	}

	bool isAble(SNAKE s)
	{
		/*fruit can't be on the edges of the board*/
		if (x == 0 || x == 30 || y == 0 || y == 20) return false;

		/*fruit can't be in the snake*/
		for (int i = 0; i < s.snake_length; i++)
			if (s.snake[i].x == x && s.snake[i].y == y) return false;
		return true;
	}

	void init(SNAKE s)
	{
		do
		{
			make_food();
		}
		while (!isAble(s));
	}

	void draw()
	{
		gotoXY(x, y);
		std::cout << static_cast<char>(254);
		gotoXY(0, 0);
	}
};

int player_score;

class BOARD
{
public:
	void draw()
	{
		std::cout << static_cast<char>(218);
		gotoXY(1, 0);
		for (int i = 1; i < 31; i++)
			std::cout << static_cast<char>(196);
		std::cout << static_cast<char>(191) << '\n';
		for (int row = 1; row < 21; row++)
		{
			for (int col = 1; col < 33; col++)
			{
				if (col == 1 || col == 32) std::cout << static_cast<char>(179);
				else std::cout << ' ';
			}

			std::cout << '\n';
		}

		std::cout << static_cast<char>(192);
		for (int i = 1; i < 31; i++)
			std::cout << static_cast<char>(196);
		std::cout << static_cast<char>(217) << "\n\nScore: " << player_score;
	}
};

bool Game_over(SNAKE s, std::string& reason)
{
	/*snake hit the wall*/
	if (s.snake[0].x > 30 || s.snake[0].x < 1 || s.snake[0].y > 20 || s.snake[0].y < 1)
	{
		reason = "You hit the wall! Game over!";
		return true;
	}

	/*snake bite itself*/
	for (int i = 1; i < s.snake_length; i++)
		if (s.snake[0].x == s.snake[i].x && s.snake[0].y == s.snake[i].y)
		{
			reason = "You bit yourself off! Game over!";
			return true;
		}

	return false;
}

void eat_food(SNAKE& s, FOOD& fruit)
{
	if (s.snake[0].x == fruit.x && s.snake[0].y == fruit.y)
	{
		s.snake[s.snake_length].x = s.snake[s.snake_length - 1].x;
		s.snake[s.snake_length].y = s.snake[s.snake_length - 1].y;
		s.snake_length++;
		fruit.init(s);
		player_score += 10;
	}
}

bool able_to_move(char direct, char pre_direct)
{
	if (direct == 'a' && pre_direct != 'd' ||
		direct == 'd' && pre_direct != 'a' ||
		direct == 's' && pre_direct != 'w' ||
		direct == 'w' && pre_direct != 's')
		return true;
	return false;
}

const std::string available_key = "xasdw";

SNAKE snake;
FOOD fruit;
BOARD board;

TCHAR pressAnyKey(const TCHAR* prompt = NULL)
{
	TCHAR ch;
	DWORD mode;
	DWORD count;
	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

	// Prompt the user
	if (prompt == NULL) prompt = TEXT("Paused! Press any key to continue...");
	WriteConsole(
		GetStdHandle(STD_OUTPUT_HANDLE),
		prompt,
		lstrlen(prompt),
		&count,
		NULL
	);

	// Switch to raw mode
	GetConsoleMode(hstdin, &mode);
	SetConsoleMode(hstdin, 0);

	// Wait for the user's response
	WaitForSingleObject(hstdin, INFINITE);

	// Read the (single) key pressed
	ReadConsole(hstdin, &ch, 1, &count, NULL);

	// Restore the console to its previous state
	SetConsoleMode(hstdin, mode);

	// Return the key code
	return ch;
}

void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = {0, 0};

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background color formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

int main()
{
	// ios_base::sync_with_stdio(false);
	// cin.tie(NULL);
	// srand(time(NULL));

	// variables

	SetConsoleCP(437);
	SetConsoleOutputCP(437);

	bool flag = false;
	do
	{
		std::string reason;
		char direct = 'x', pre_direct = 'a';

		snake.init();
		fruit.init(snake);

		while (!Game_over(snake, reason))
		{
			if (_kbhit())
			{
				pre_direct = direct;
				direct = _getch();
				if (direct == 'x')
				{
					gotoXY(0, 24);
					pressAnyKey();
					cls();
				}
				if (!available_key.find(direct)) direct = pre_direct;
				if (!able_to_move(direct, pre_direct)) direct = pre_direct;
			}

			if (direct == 'x')
			{
				board.draw();
				snake.draw();
				fruit.draw();
				Sleep(80);
				continue;
			}

			board.draw();
			snake.move(direct);
			snake.draw();
			eat_food(snake, fruit);
			fruit.draw();
			Sleep(80);
		}

		short tempY = 25;
		char opt = '\0';
		do
		{
			gotoXY(0, tempY);
			std::cout << reason << " Replay? [Y/N] => ";
			std::cin >> opt;

			if (opt == 'Y' || opt == 'y')
			{
				player_score = 0;
				cls();
			}
			else if (opt == 'N' || opt == 'n')
				flag = true;
			else
			{
				tempY++;
				gotoXY(0, tempY);
				std::cout << "Replay? [Y/N] = > ";
			}
		}
		while (opt != 'Y' && opt != 'y' && opt != 'N' && opt != 'n');
	}
	while (!flag);

	return 0;
}