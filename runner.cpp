<<<<<<< Updated upstream
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
=======
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <random>

class Color
{
	HANDLE consoleHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD color_;
public:
	explicit Color(const WORD code) : color_(code)
	{
	}

	friend std::ostream& operator <<(std::ostream& ss, const Color& obj)
	{
		SetConsoleTextAttribute(obj.consoleHandle_, obj.color_);
		return ss;
	}
};

int base_score = 1;
double game_speed = 220;
int player_score;

bool cheat = false;
bool end_game = false;
int count_phase = 1;
bool dead_by_multiuniverse = false;
int time_left_for_portal_to_disappear;
// ??c Nh?n
void gotoxy(short column, short line)
{
	const COORD coord{column, line};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
>>>>>>> Stashed changes
}

struct Point
{
	int x;
	int y;
	// using pre_x, pre_y to store pre position of this point
	// so that it is easier to update next point OTS
	int pre_x;
	int pre_y;
};

struct High_score
{
	int score;
<<<<<<< Updated upstream
	char user_name[30];
};

=======
	std::string user_name;
};

bool is_going_through_portal;
bool first_step;

>>>>>>> Stashed changes
class SNAKE
{
public:
	int snake_length;
	Point snake[100];

<<<<<<< Updated upstream
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
=======
	void move(const char direct)
	{
		/* move the head */
		if (!is_going_through_portal || !first_step)
		{
			snake[0].pre_x = snake[0].x;
			snake[0].pre_y = snake[0].y;
		}

		bool temp = false;

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
		default: break;
		}

		/* move the part */
>>>>>>> Stashed changes
		for (int i = 1; i < snake_length; i++)
		{
			snake[i].pre_x = snake[i].x;
			snake[i].pre_y = snake[i].y;

			snake[i].x = snake[i - 1].pre_x;
			snake[i].y = snake[i - 1].pre_y;
<<<<<<< Updated upstream
=======

			if (abs(snake[i - 1].x - snake[i].x) + abs(snake[i - 1].y - snake[i].y) > 1)
				temp = true;
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	}

	void draw()
	{
		for (int i = 0; i < snake_length; i++)
		{
			gotoXY(snake[i].x, snake[i].y);
			if (i == 0) std::cout << '@';
=======

		/*
		// init the snake with length = 20
		for (int i = 3; i < 20; i++)
		{
		    snake[i].x = 14 + i;
		    snake[i].y = 10;
		}
		*/
	}

	void draw() const
	{
		for (int i = 0; i < snake_length; i++)
		{
			gotoxy(static_cast<short>(snake[i].x), static_cast<short>(snake[i].y));
			if (i == 0)
				std::cout << '@';
>>>>>>> Stashed changes
			else std::cout << '#';
		}
	}
};

class FOOD
{
public:
<<<<<<< Updated upstream
	int x, y;

=======
	int x;
	int y;
	int type;
	// Type 1: Th?c an thu?ng
	// Type 2: x5 di?m nh?n v?o
	// Type 3: x2 t?c d? ch?y :)))))))))))
	// type 4: -5*base_score (n?u ?m th? set l?i l? 0)
>>>>>>> Stashed changes
	void make_food()
	{
		std::random_device seed;
		std::mt19937 gen(seed());
<<<<<<< Updated upstream
		std::uniform_int_distribution<int> distX(1, 29);
		std::uniform_int_distribution<int> distY(1, 19);

		x = distX(gen);
		y = distY(gen);
	}

	bool isAble(SNAKE s)
	{
		/*fruit can't be on the edges of the board*/
		if (x == 0 || x == 30 || y == 0 || y == 20) return false;
=======
		const std::uniform_int_distribution<int> dist_x(1, 29);
		const std::uniform_int_distribution<int> dist_y(2, 21);
		x = dist_x(gen);
		y = dist_y(gen);

		if (count_phase % 5 == 0)
		{
			if (!cheat)
				type = rand() % 4 + 1;
			else type = 2;

			if (type == 2)
				player_score += 4 * base_score;

			if (type == 3)
			{
				if (game_speed >= 20)
					game_speed /= 2;
				else game_speed = 1;
			}

			if (type == 4)
			{
				player_score = player_score - 2 * base_score;
				if (player_score < 0)
					player_score = 0;
			}
		}
		else if (cheat)
			player_score += 4 * base_score;
	}

	bool is_able(const SNAKE s) const
	{
		/* fruit can't be on the edges of the board */
		if (x == 0 || x == 30 || y == 1 || y == 22)
			return false;
>>>>>>> Stashed changes

		/*fruit can't be in the snake*/
		for (int i = 0; i < s.snake_length; i++)
			if (s.snake[i].x == x && s.snake[i].y == y)
				return false;

		return true;
	}

<<<<<<< Updated upstream
	void init(SNAKE s)
=======
	void init(const SNAKE s)
>>>>>>> Stashed changes
	{
		do
		{
			make_food();
		}
<<<<<<< Updated upstream
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
=======
		while (!is_able(s));
	}

	void draw() const
	{
		gotoxy(static_cast<short>(x), static_cast<short>(y));
		std::cout << Color(240) << ' ' << Color(7);
	}
};

bool portal_is_opening;

class PORTAL
{
public:
	int x, y;

	void make_portal()
	{
		std::random_device seed;
		std::mt19937 gen(seed());
		const std::uniform_int_distribution<int> dist_x(1, 29);
		const std::uniform_int_distribution<int> dist_y(2, 21);
		x = dist_x(gen);
		y = dist_y(gen);
	}

	bool is_able(const SNAKE s, const FOOD f, const PORTAL p) const
	{
		if (x == 0 || x == 30 || y == 1 || y == 22)
			return false;

		for (int i = 0; i < s.snake_length; i++)
			if (s.snake[i].x == x && s.snake[i].y == y)
				return false;

		if (x == f.x && y == f.y)
			return false;

		if ((x == p.x || x == p.x - 1 || x == p.x + 1) && y == p.y)
			return false;

		return true;
	}

	void init(const SNAKE s, const FOOD f, const PORTAL p)
	{
		do
		{
			make_portal();
		}
		while (!is_able(s, f, p));
	}

	void draw() const
	{
		gotoxy(static_cast<short>(x), static_cast<short>(y));
		std::cout << time_left_for_portal_to_disappear;
	}
};
>>>>>>> Stashed changes

class BOARD
{
public:
	void draw()
	{
<<<<<<< Updated upstream
		std::cout << static_cast<char>(218);
		gotoXY(1, 0);
		for (int i = 1; i < 31; i++)
			std::cout << static_cast<char>(196);
		std::cout << static_cast<char>(191) << '\n';
=======
		gotoxy(0, 1);
		std::cout << static_cast<char>(218);
		for (int i = 1; i < 31; i++)
			std::cout << static_cast<char>(196);
		std::cout << static_cast<char>(191) << '\n';

>>>>>>> Stashed changes
		for (int row = 1; row < 21; row++)
		{
			for (int col = 1; col < 33; col++)
			{
<<<<<<< Updated upstream
				if (col == 1 || col == 32) std::cout << static_cast<char>(179);
				else std::cout << ' ';
=======
				if (col == 1 || col == 32)
					std::cout << static_cast<char>(179);
				else std::cout << " ";
>>>>>>> Stashed changes
			}

			std::cout << '\n';
		}

		std::cout << static_cast<char>(192);
		for (int i = 1; i < 31; i++)
			std::cout << static_cast<char>(196);
<<<<<<< Updated upstream
		std::cout << static_cast<char>(217) << "\n\nScore: " << player_score;
=======
		std::cout << static_cast<char>(217);
>>>>>>> Stashed changes
	}
};

bool game_over(const SNAKE s, std::string& reason)
{
<<<<<<< Updated upstream
	/*snake hit the wall*/
	if (s.snake[0].x > 30 || s.snake[0].x < 1 || s.snake[0].y > 20 || s.snake[0].y < 1)
=======
	/* portal cut the snake */
	if (dead_by_multiuniverse)
	{
		reason = "Oh no! Your tail is teleported to another universe, you're shocked to dead!";
		return true;
	}

	/* snake hit the wall */
	if (s.snake[0].x > 30 || s.snake[0].x < 1 || s.snake[0].y > 21 || s.snake[0].y < 2)
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
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
=======
		player_score += base_score;
		count_phase++;
		fruit.init(s);
		fruit.draw();
	}
}

bool able_to_move(const char direct, const char pre_direct)
{
	// snake can move using w, a, s, d
	if (direct == 'a' && pre_direct != 'd'
		|| direct == 'd' && pre_direct != 'a'
		|| direct == 's' && pre_direct != 'w'
		|| direct == 'w' && pre_direct != 's')
		return true;

	// game paused
	if (direct == 'x')
		return true;
	if (direct == 'r' || direct == 'R')
>>>>>>> Stashed changes
		return true;
	return false;
}

const std::string available_key = "xasdw";

SNAKE snake;
FOOD fruit;
BOARD board;
<<<<<<< Updated upstream

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
=======
PORTAL portal1, portal2;

void game_level()
{
	std::string level;
	int _level;
	std::cout << "Vui long chon cap do tu 1 --> 5 \nPs: cap do la so nguyen lon hon 0 va be hon 6 \n";
	std::cin >> level;
	if (level == "862006")
	{
		//std::cout << "You are a loser";
		end_game = true;
	}
	else
	{
		if (level == "21520378")
			_level = 6;
		else
		{
			while (true)
			{
				if (level.size() == 1)
					if (level[0] == '1' || level[0] == '2' || level[0] == '3' || level[0] == '4' || level[0] == '5')
						break;
				std::cout << "Vui long nhap so tu 1-->5 \n";
				std::cin >> level;
			}
			_level = level[0] - '0';
			base_score = 5 * _level;
			game_speed -= 40 * _level;
		}
		if (_level == 6)
		{
			const std::string cheating = "You are cheating.(#_<-)";
			for (const char c : cheating)
			{
				std::cout << c;
				Sleep(50);
			}
			Sleep(500);
			cheat = true;
			base_score = 150;
			game_speed = 150;
		}
	}
}

char curr_direct(const SNAKE s)
{
	if (s.snake[0].x == s.snake[1].x && s.snake[0].y == s.snake[1].y - 1)
		return 'w';

	if (s.snake[0].x == s.snake[1].x && s.snake[0].y == s.snake[1].y + 1)
		return 's';

	if (s.snake[0].x == s.snake[1].x + 1 && s.snake[0].y == s.snake[1].y)
		return 'd';

	return 'a';
}

void try_make_portal()
{
	portal1.init(snake, fruit, portal2);
	portal2.init(snake, fruit, portal1);
	time_left_for_portal_to_disappear = 50;
	portal_is_opening = true;
}

bool go_to_portal(const PORTAL in, const PORTAL out)
{
	if (snake.snake[0].x == in.x && snake.snake[0].y == in.y)
	{
		snake.snake[0].pre_x = snake.snake[0].x;
		snake.snake[0].pre_y = snake.snake[0].y;
		snake.snake[0].x = out.x;
		snake.snake[0].y = out.y;
		is_going_through_portal = true;
		first_step = true;
		return true;
	}
	return false;
}

void restart()
{
	base_score = 1;
	game_speed = 220;
	player_score = 0;

	cheat = false;
	end_game = false;
	count_phase = 1;
}

TCHAR pressAnyKey(const TCHAR* prompt = nullptr)
{
	TCHAR ch;
	DWORD mode;
	DWORD count;
	const HANDLE console_handle = GetStdHandle(STD_INPUT_HANDLE);

	if (prompt == nullptr) prompt = TEXT("Paused! Press any key to continue playing...");
	WriteConsole(
		GetStdHandle(STD_OUTPUT_HANDLE),
		prompt,
		lstrlen(prompt), &count,
		nullptr
	);

	GetConsoleMode(console_handle, &mode);
	SetConsoleMode(console_handle, 0);

	WaitForSingleObject(console_handle, INFINITE);

	ReadConsole(console_handle, &ch, 1, &count, nullptr);

	SetConsoleMode(console_handle, mode);

>>>>>>> Stashed changes
	return ch;
}

void cls()
{
<<<<<<< Updated upstream
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
=======
	static HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	constexpr COORD topLeft{0, 0};

	std::cout.flush();

	if (!GetConsoleScreenBufferInfo(console_handle, &csbi))
		abort();

	const DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	FillConsoleOutputCharacter(console_handle, TEXT(' '), length, topLeft, &written);

	FillConsoleOutputAttribute(console_handle, csbi.wAttributes, length, topLeft, &written);

	SetConsoleCursorPosition(console_handle, topLeft);
>>>>>>> Stashed changes
}

int main()
{
<<<<<<< Updated upstream
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
=======
	SetConsoleCP(437);
	SetConsoleOutputCP(437);

REPLAY:
	{
		restart();
		game_level();
		cls();

		if (end_game)
		{
			std::cout << "X   X  XX  X  X    X  XX  XXX    X    X    XX  XXX XXX XX \n";
			std::cout << " X X  X  X X  X   X X X X X     X X   X   X  X X   X   X X \n";
			std::cout << "  X   X  X X  X   XXX XX  XXX   XXX   X   X  X XXX XXX XX \n";
			std::cout << "  X   X  X X  X   X X X X X     X X   X   X  X   X X   X X \n";
			std::cout << "  X    XX   XX    X X X X XXX   X X   XXX  XX  XXX XXX X X \n";
			return 0;
		}
		// Nh?p sai cheatcode

		bool start = true;
		char direct = 'x';
		char pre_direct = 'a';
		std::string reason;

		snake.init();
		fruit.init(snake);

		while (!game_over(snake, reason))
		{
			if (_kbhit())
			{
				pre_direct = direct;
				direct = _getch();

				if (!available_key.find(direct))
					direct = pre_direct;

				if (!able_to_move(direct, pre_direct))
					direct = pre_direct;
			}

			if (direct == 'x')
			{
				gotoxy(0, 24);
				if (start)
				{
					board.draw();
					snake.draw();
					fruit.draw();
					gotoxy(0, 24);
					pressAnyKey(TEXT("Use a | w | d | s to move around, x to pause"));
					start = false;
					cls();
				}
				else pressAnyKey();
				direct = pre_direct;
				gotoxy(0, 24);
				std::cout << "                                            ";
				continue;
			}
			gotoxy(0, 0);
			std::cout << "Score: " << player_score << '\n';

			board.draw();
			if (time_left_for_portal_to_disappear > 0 && !go_to_portal(portal1, portal2))
				go_to_portal(portal2, portal1);

>>>>>>> Stashed changes
			snake.move(direct);
			snake.draw();
			eat_food(snake, fruit);
			fruit.draw();
<<<<<<< Updated upstream
			Sleep(80);
=======
			eat_food(snake, fruit);

			time_left_for_portal_to_disappear--;
			if (time_left_for_portal_to_disappear <= 0)
				portal_is_opening = false;
			if (time_left_for_portal_to_disappear > 0)
			{
				portal1.draw();
				portal2.draw();
			}
			if (!portal_is_opening)
				try_make_portal();
			Sleep(100);
>>>>>>> Stashed changes
		}
		gotoxy(0, 25);
		std::cout << reason << std::endl;

<<<<<<< Updated upstream
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
=======
		if (direct == 'r' || direct == 'R')
		{
			cls();
			goto REPLAY;
		}
	}
	std::cout << "Press 'r' to replay or 'Esc' to exit...";

NHAPLAI: const char option = _getch();
	if (option == 'r' || option == 'R')
	{
		cls();
		goto REPLAY;
	}
	if (option == 27)
		return 0;
	goto NHAPLAI;
>>>>>>> Stashed changes
}