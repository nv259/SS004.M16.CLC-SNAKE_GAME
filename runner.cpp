#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <random>
#pragma comment(lib, "Winmm.lib")
#include <ctime>
#include <fstream>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <mmsystem.h>

class Color
{
	HANDLE console_handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD color_;
public:
	explicit Color(const WORD code) : color_(code)
	{
	}

	friend std::ostream& operator <<(std::ostream& ss, const Color& obj)
	{
		SetConsoleTextAttribute(obj.console_handle_, obj.color_);
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
	const COORD coord{ column, line };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
	std::string user_name;
};

bool is_going_through_portal;
bool first_step;
Point prevTail;

class SNAKE
{
public:
	int snake_length;
	Point snake[100];

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
		for (int i = 1; i < snake_length; i++)
		{
			snake[i].pre_x = snake[i].x;
			snake[i].pre_y = snake[i].y;

			snake[i].x = snake[i - 1].pre_x;
			snake[i].y = snake[i - 1].pre_y;

			if (abs(snake[i - 1].x - snake[i].x) + abs(snake[i - 1].y - snake[i].y) > 1)
				temp = true;
		}
		first_step = false;
		is_going_through_portal = temp;
		if (time_left_for_portal_to_disappear <= 0 && is_going_through_portal)
			dead_by_multiuniverse = true;
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

		/*
		// init the snake with length = 20
		for (int i = 3; i < 20; i++)
		{
			snake[i].x = 14 + i;
			snake[i].y = 10;
		}
		*/
	}

	void draw(char d, char prevD, bool first_draw) const
	{
	    if (!first_draw)
        {
            if (d == 'w' || d == 's')
            {
                if (prevD == 'a') gotoxy(static_cast<short>(snake[0].pre_x - 1), static_cast<short>(snake[0].pre_y));
                if (prevD == 'd') gotoxy(static_cast<short>(snake[0].pre_x + 2), static_cast<short>(snake[0].pre_y));
                std::cout << Color(0) << " ";
            }

            gotoxy(static_cast<short>(snake[0].x), static_cast<short>(snake[0].y));
            if (d == 'a') gotoxy(static_cast<short>(snake[0].x - 1), static_cast<short>(snake[0].y));
            std::cout << Color(225) << "  ";
            if (d == 'd') std::cout << " ";

            gotoxy(static_cast<short>(snake[snake_length-1].pre_x), static_cast<short>(snake[snake_length-1].pre_y));
            std::cout << Color(0) << "  ";
        }

		for (int i = 0; i < snake_length; i++)
		{
			gotoxy(static_cast<short>(snake[i].x), static_cast<short>(snake[i].y));
			if (i == 0)
            {
                if (d == 'a') gotoxy(static_cast<short>(snake[i].x - 1), static_cast<short>(snake[i].y));
                std::cout << Color(225) << "  ";
				if (d == 'd') std::cout << " ";
            }
			else std::cout << Color(165) << "  ";
		}
        gotoxy(snake[snake_length-1].pre_x, snake[snake_length-1].pre_y);
		std::cout << Color(7);
	}
};

class FOOD
{
public:
	int x;
	int y;
	int type;

	void make_food()
	{
        srand(unsigned (time(NULL)));
        x = rand() % 78 + 2;
        y = rand() % 20 + 1;

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
		if (x == 0 || x == 80 || y == 1 || y == 22)
			return false;

		/* fruit can't be in the snake */
		for (int i = 0; i < s.snake_length; i++)
			if (s.snake[i].x == x && s.snake[i].y == y)
				return false;

		return true;
	}

	void init(const SNAKE s)
	{
		do
		{
			make_food();
		} while (!is_able(s));
	}

	void draw() const
	{
		gotoxy(static_cast<short>(x), static_cast<short>(y));
		std::cout << Color(195) << "  " << Color(7);
		gotoxy(0, 0);
	}
};

bool portal_is_opening;

class PORTAL
{
public:
	int x, y;

	void make_portal()
	{
        x = rand() % 79 + 1;
        y = rand() % 20 + 1;
	}

	bool is_able(const SNAKE s, const FOOD f, const PORTAL p) const
	{
		if (x == 0 || x == 80 || y == 1 || y == 22)
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
		} while (!is_able(s, f, p));
	}

	void draw() const
	{
		gotoxy(static_cast<short>(x), static_cast<short>(y));
		//std::cout << time_left_for_portal_to_disappear;
		std::cout << Color(5) << '@' << Color(7);
		gotoxy(0, 0);
	}
};

class BOARD
{
public:
	void draw()
	{
		gotoxy(0, 1);
		for (int i = 0; i <= 81; i++)
			std::cout << Color(225) << ' ';
		std::cout << '\n';

		for (int row = 1; row <= 21; row++)
		{
			gotoxy(0, row);
			std::cout << Color(225) << "  ";

			gotoxy(80, row);
			std::cout << Color(225) << "  ";
		}

		gotoxy(0, 22);
		for (int i = 0; i <= 81; i++)
			std::cout << Color(225) << ' ';
	}
};

bool game_over(const SNAKE s, std::string& reason)
{
	/* portal cut the snake */
	if (dead_by_multiuniverse)
	{
		reason = "Oh no! Your tail is teleported to another universe, you're shocked to dead!";
		dead_by_multiuniverse = false;
		is_going_through_portal = false;
        first_step = false;
		return true;
	}

	/* snake hit the wall */
	if (s.snake[0].x >= 79 || s.snake[0].x < 1 || s.snake[0].y >= 21 || s.snake[0].y < 1)
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
void delete_fruit(FOOD f) {
    gotoxy(static_cast<short>(f.x), static_cast<short>(f.y));
    std::cout << Color(0) << " ";
}

int length(int x) {
    int result = 0;
    while (x)
    {
        result++;
        x /= 10;
    }
    return result;
}

void eat_food(SNAKE& s, FOOD& fruit)
{
    if ((abs(s.snake[0].x - fruit.x) <= 1) && s.snake[0].y == fruit.y)
	{
        PlaySound(TEXT("EATING.wav"),NULL, SND_FILENAME | SND_ASYNC);
		s.snake[s.snake_length].x = s.snake[s.snake_length - 1].x;
		s.snake[s.snake_length].y = s.snake[s.snake_length - 1].y;
		s.snake_length++;
		player_score += base_score;
		count_phase++;
        delete_fruit(fruit);
		fruit.init(s);
		fruit.draw();
		gotoxy(82, 6);
		std::cout << Color(63);
        std::cout << "        SCORE:        " << player_score;
        for (int i = 0; i < 29 - length(player_score); i++)
            std::cout << " ";
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
		return true;
	return false;
}

const std::string available_key = "0xasdw";

SNAKE snake;
FOOD fruit;
BOARD board;
PORTAL portal1, portal2;
int _level;

void game_level()
{
	std::string level;

	std::cout << "Vui long chon cap do tu 1 --> 5 \nPs: cap do la so nguyen lon hon 0 va be hon 6\n>> ";
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
				std::cout << "Vui long nhap so tu 1-->5\n>> ";
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

void delete_portal(PORTAL p) {
    gotoxy(static_cast<short>(p.x), static_cast<short>(p.y));
    std::cout << Color(0) << " ";
}

void try_make_portal()
{
    delete_portal(portal1);
    delete_portal(portal2);
	portal1.init(snake, fruit, portal2);
	portal2.init(snake, fruit, portal1);
	time_left_for_portal_to_disappear = 100;
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

	if (prompt == nullptr) prompt = TEXT("Paused! Press any key to continue playing... But rest for a while maybe really good for your eyes");
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

	return ch;
}

void resizeConsole(int width, int height)
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, true);
}

void cls()
{
	static HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	constexpr COORD topLeft{ 0, 0 };

	std::cout.flush();

	if (!GetConsoleScreenBufferInfo(console_handle, &csbi))
		abort();

	const DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	FillConsoleOutputCharacter(console_handle, TEXT(' '), length, topLeft, &written);

	FillConsoleOutputAttribute(console_handle, csbi.wAttributes, length, topLeft, &written);

	SetConsoleCursorPosition(console_handle, topLeft);
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

std::string convert_to_string(int x) {
    std::string temp = "";
    while (x)
    {
        temp += " ";
        x /= 10;
    }
    int len = 0;
    return temp;
}

void print_line(std::string rank, int color, High_score x) {
    std::cout << Color(color);
    std::string temp = "| " + rank + ". " + x.user_name;
    std::cout << temp;
    while (int(temp.size()) < 15)
    {
        temp += " ";
        std::cout << " ";
    }
    temp += "| ";
    if (x.score != 0) std::cout << "| " << x.score;
    else std::cout << "|" << x.score;

    temp += convert_to_string(x.score);
    while (int(temp.size()) < 22)
    {
        temp += " ";
        std::cout << " ";
    }
    temp += "|";
    std::cout << "|";
}

void print_player(High_score x, std::string rank, int colour) {
    std::cout << Color(colour);
    std::cout << " " << rank << " " << x.user_name << "(" << x.score << ")";
}

void draw_score_board(High_score scores[], int len) {
    gotoxy(82, 0);
    std::cout << Color(240) << "        TUTORIAL                                   ";
    gotoxy(82, 1);
    std::cout << " 1. Use a | w | d | s to move around.              ";
    gotoxy(82, 2);
    std::cout << " 2. Press X to pause, R to restart.                ";
    gotoxy(82, 3);
    std::cout << " 3. Portals will be helpful if be used it cleverly!";
    gotoxy(82, 4);
    std::cout << " 4. The longer you are, the more score you'll get. ";

    std::cout << Color(240);
    gotoxy(82, 5);
    std::cout << "                                                   ";
    std::cout << Color(112);
    gotoxy(82, 6);
    std::cout << "        SCORE:        " << player_score;
    for (int i = 0; i < 29 - length(player_score); i++)
        std::cout << " ";
    gotoxy(82, 7);
    std::cout << Color(240);
    std::cout << "                                                   ";

    std::cout << Color(112);
    gotoxy(82, 8);
    std::cout << "        TOP PLAYERS                                ";
    std::cout << Color(44);
    gotoxy(82, 9);
    std::cout << "                                                   ";
    gotoxy(82, 9);
    print_player(scores[0], "1st.", 44);
    gotoxy(82, 10);
    std::cout << "                                                   ";
    gotoxy(82, 10);
    print_player(scores[1], "2nd.", 45);
    gotoxy(82, 11);
    std::cout << "                                                   ";
    gotoxy(82, 11);
    print_player(scores[2], "3rd.", 46);
    gotoxy(82, 12);
    std::cout << "                                                   ";
    gotoxy(82, 12);
    print_player(scores[3], "4th.", 47);
    gotoxy(82, 13);
    std::cout << "                                                   ";
    gotoxy(82, 13);
    print_player(scores[4], "5th.", 47);

    int x = 1, colour;
    for (int j = 0; j < 50; j += 2)
    {
        x *= -1;
        for (int i = 14; i < 22; i++)
        {
            x *= -1;
            if (x == 1) colour = 240;
            else colour = 112;

            gotoxy(82 + j, i);
            std::cout << Color(colour) << "  ";
        }
    }
    for (int i = 14; i < 22; i++)
    {
        x *= -1;
        if (x == 1) colour = 112;
        else colour = 240;

        gotoxy(82 + 50, i);
        std::cout << Color(colour) << " ";
    }

    gotoxy(0, 21);
    std::cout << Color(45);
    for (int i = 0; i < 133; i++)
        std::cout << " ";

    for (int i = 22; i < 26; i++)
    {
        gotoxy(0, i);
        std::cout << "  ";
    }

    gotoxy(0, 26);
    std::cout << Color(170);
    for (int i = 0; i < 133; i++)
        std::cout << " ";
}

bool cmp_score(High_score a, High_score b) {
    return a.score > b.score || a.score == b.score && a.user_name < b.user_name;
}

void move_through_portal_check(SNAKE s, char prevD) {
    if (is_going_through_portal)
            {
                if (prevD == 'a')
                {
                    gotoxy(static_cast<short>(portal1.x - 2), static_cast<short>(portal1.y));
                    std::cout << Color(0) << "   ";
                    gotoxy(static_cast<short>(portal2.x - 2), static_cast<short>(portal2.y));
                    std::cout << Color(0) << "   ";
                }
                if (prevD == 'd')
                {
                    gotoxy(static_cast<short>(portal1.x + 1), static_cast<short>(portal1.y));
                    std::cout << Color(0) << "   ";
                    gotoxy(static_cast<short>(portal2.x + 1), static_cast<short>(portal2.y));
                    std::cout << Color(0) << "   ";
                }
            }
}

int main()
{
	SetConsoleCP(437);
	SetConsoleOutputCP(437);
    resizeConsole(990, 470);

    std::ifstream input("best_score.txt");
    High_score scores[1000];
    int len = 0;
    while (1)
    {
        input >> scores[len].user_name >> scores[len++].score;
        if (input.eof()) break;
    }

    std::ofstream output("best_score.txt");
    //PlaySound(L"theme.wav", NULL, SND_ASYNC);
	//PlaySound(TEXT(L"theme.wav"), GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
	//sndPlaySound(TEXT("theme.wav"), SND_FILENAME | SND_ASYNC | SND_LOOP);
    bool start = true;
REPLAY:
	{

        std::sort(scores, scores + len, cmp_score);
        for (int i = 0; i < len; i++)
            output << scores[i].user_name << " " << scores[i].score << "\n";
        PlaySound(TEXT("START_GAME.wav"),NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
		restart();
		game_level();
		cls();
        draw_score_board(scores, len);

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

		char direct = 'x';
		char pre_direct = 'a';
		std::string reason;

		snake.init();
		fruit.init(snake);
        snake.draw(direct, pre_direct, 1);

		while (!game_over(snake, reason))
		{
			if (_kbhit())
			{
				pre_direct = direct;
				direct = _getch();
                PlaySound(TEXT("NONE.wav"),NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
				if (!available_key.find(direct))
					direct = pre_direct;

				if (!able_to_move(direct, pre_direct))
					direct = pre_direct;
			}

			if (direct == 'r' || direct == 'R')
			{
				gotoxy(0, 24);
				std::cout << "You press 'r' while playing, do you wish to restart? [Y/N]";
				RESTART: const char opt = _getch();
				if (opt == 'y' || opt == 'Y')
				{
					cls();
					goto REPLAY;
				}
				if (opt == 'n' || opt == 'N')
				{
					direct = pre_direct;
					continue;
				}

				goto RESTART;
			}

			if (direct == 'x')
			{
				if (start)
				{
					const std::string welcome = "WELCOME TO SNAKE GAME!";
					std::cout << "\t\t\t\t\t" << Color(10);
					for (const char c : welcome)
					{
						std::cout << c;
						Sleep(80);
					}

					std::cout << Color(7);
					Sleep(100);

					// cls();
					snake.draw(direct, pre_direct, 0);
					fruit.draw();
					gotoxy(0, 24);
					pressAnyKey(TEXT("Use a | w | d | s to move around, x to pause"));
					start = false;
					// cls();
				}
				else
				{
					gotoxy(0, 24);
					pressAnyKey();
				}
				direct = pre_direct;
				gotoxy(0, 24);
				std::cout << "                                            ";
				continue;
			}
			// cls();
			gotoxy(0, 0);
			std::cout << Color(240) << "GAME LEVEL: " << _level;
            std::cout << Color(45);
            for (int i = 10; i < 79 ; i++) std::cout << ' ';
			snake.move(direct);
			snake.draw(direct, pre_direct, 0);
			move_through_portal_check(snake, direct);
			//board.draw();
			fruit.draw();
			eat_food(snake, fruit);

            std::string whisper = "";
            if (player_score == 0) whisper = "Let's start!                                                                                              ";
            else if (player_score < 100) whisper = "Wooh! You are getting well!                                                                                                        ";
            else if (player_score < 150) whisper = "You are doing really well! Keep going my friend!                                                    ";
            else if (player_score < 300) whisper = "Haha! New high score is comminggg!                                                                  ";
            else if (player_score < 400) whisper = "Damn! Really ?!                                                                                     ";
            else if (player_score < 500) whisper = "Are you still hungry ?                                                                              ";
            else if (player_score < 530) whisper = "Stop! You have eaten too much!                                                                      ";
            else if (player_score < 600) whisper = "No, god, please, no,                                                                                ";
            else if (player_score < 700) whisper = "Ha~ I really care about your eyes :(                                                                ";
            else if (player_score < 800) whisper = "Fine. You win, let's stop.                                                                          ";
            else                         whisper = "Will you stop for me :(?                                                                            ";

            gotoxy(0, 24);
            std::cout << Color(45) << "  " <<  Color(10) << whisper;

			if (time_left_for_portal_to_disappear > 0 && !go_to_portal(portal1, portal2))
				go_to_portal(portal2, portal1);

			time_left_for_portal_to_disappear--;

			if (time_left_for_portal_to_disappear <= 0)
				portal_is_opening = false;

			if (time_left_for_portal_to_disappear > 0)
			{
				portal1.draw();
				portal2.draw();
			}

			if (!portal_is_opening && rand() % 25 == 1)
				try_make_portal();
			Sleep(100);
		}
		gotoxy(0, 24);
		std::cout << reason << '\n';
	}
     PlaySound(TEXT("END_GAME.wav"),NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    if (player_score > scores[5].score)
    {
        std::string name;
        std::cout << "Congrats! You got a new high score! Enter your name to record this score: ";
        std::cin >> name;
        scores[len].score = player_score;
        scores[len++].user_name = name;
    }
    std::sort(scores, scores + len, cmp_score);
    for (int i = 0; i < len; i++)
        output << scores[i].user_name << " " << scores[i].score << "\n";

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
}
