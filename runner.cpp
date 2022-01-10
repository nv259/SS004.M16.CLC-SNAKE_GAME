#include    <iostream>
#include    <windows.h>
#include	<stdexcept>
#include    <conio.h>
#include    <cstdlib>
#include	<random>
#include    <ctime>

int base_score=1;
double game_speed=220;
int player_score;

bool cheat = false;
bool End_game = false;
int count_phase = 1;
// Ð?c Nhân
void gotoxy(int column, int line){
	COORD coord{};
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

struct  Point
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

bool is_going_through_portal, first_step;
class SNAKE {
public:
	int snake_length;
	Point snake[100];

	void move(char direct) {
		/* move the head */
		if (!is_going_through_portal || !first_step)
		{
		    snake[0].pre_x = snake[0].x;
            snake[0].pre_y = snake[0].y;
		}

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
		first_step = false;
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

class FOOD{
public:
	int x, y;
    int type;
    // Type 1: Th?c an thu?ng
    // Type 2: x5 di?m nh?n vào
    // Type 3: x2 t?c d? ch?y :)))))))))))
    // type 4: -5*base_score (n?u âm thì set l?i là 0)
	void make_food() {
		/* std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<int> distX(0, 29);
		std::uniform_int_distribution<int> distY(0, 19);
		x = distX(gen);
		y = distY(gen); */
		x = rand() % 30;
            y = rand() % 20;
		if (count_phase % 5 == 0)
        {
            if (!cheat) type = rand() % 4 + 1;
            else type = 2;
            if (type == 2)
                player_score += 4 * base_score;
            if (type == 3)
            {
                if (game_speed >= 20) game_speed /= 2;
                else game_speed = 1;
            }
            if (type == 4)
            {
                player_score = player_score - 2 * base_score;
                if (player_score < 0) player_score = 0;
            }
        }
        else
            if (cheat)
                player_score += 4 * base_score;
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

bool portal_is_opening;
int time_left_for_portal_to_disappear;
class PORTAL {
public:
    int x, y;

    void make_portal() {
        x = rand() % 30;
        y = rand() % 20;
    }

    bool is_able(SNAKE s, FOOD f, PORTAL p) {
		if (x == 0 || x == 30 || y == 0 || y == 20) return false;
		for (int i = 0; i < s.snake_length; i++)
			if (s.snake[i].x == x && s.snake[i].y == y) return false;

        if (x == f.x && y == f.y) return false;
        if (x == p.x && y == p.y) return false;

		return true;
    }

    void init(SNAKE s, FOOD f, PORTAL p) {
        do {
            make_portal();
        } while (!is_able(s, f, p));
    }

    void draw() {
        gotoxy(x, y);
        std::cout << time_left_for_portal_to_disappear;
        gotoxy(0, 23);
    }
};

class BOARD {
public:
	void draw()
	{
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

bool Game_over(SNAKE s, std::string& reason){
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

void eat_food(SNAKE& s, FOOD& fruit){
	if (s.snake[0].x == fruit.x && s.snake[0].y == fruit.y)
    {
		s.snake[s.snake_length].x = s.snake[s.snake_length - 1].x;
		s.snake[s.snake_length].y = s.snake[s.snake_length - 1].y;
		s.snake_length++;
		fruit.init(s);
		player_score += base_score;
		count_phase++;
	}
}

bool able_to_move(char direct, char pre_direct) {
    // snake can move to wasd
	if (direct == 'a' && pre_direct != 'd'
		|| direct == 'd' && pre_direct != 'a'
		|| direct == 's' && pre_direct != 'w'
		|| direct == 'w' && pre_direct != 's') return true;

    // game paused
    if (direct == 'x') return true;
    if (direct == 'r') return true;
	return false;
}

const std::string available_key = "0xasdw";

SNAKE snake;
FOOD fruit;
BOARD board;
PORTAL portal1, portal2;

void Game_level(){
    std::string level;
    int _level;
    std::cout<<"Vui long nhap so tu 1-->5 \n";
    std::cin>>level;
    if (level == "862006")
    {
        //std::cout << "You are a loser";
        End_game = true;
    }
    else
    {
        if (level == "21520378")
            _level = 6;
        else
        {
            while(1)
            {
              if(level.size()==1)
                if(level[0]=='1' || level[0]=='2' || level[0]=='3' || level[0]=='4' || level[0]=='5')
                    break;
               std::cout<<"Vui long nhap so tu 1-->5 \n";
               std::cin>>level;
            }
            _level=level[0]-'0';
            base_score=5*_level;
            game_speed-=40*_level;
        }
        if (_level == 6)
        {
            std::string cheating = "You are cheating.(#_<-)";
            for (int i = 0; i < cheating.size(); i++)
            {
                std::cout << cheating[i];
                Sleep(50);
            }
            Sleep(500);
            cheat = true;
            base_score = 150;
            game_speed = 150;
        }
    }

}

char curr_direct(SNAKE s) {
    if (s.snake[0].x == s.snake[1].x && s.snake[0].y == s.snake[1].y - 1) return 'w';
    if (s.snake[0].x == s.snake[1].x && s.snake[0].y == s.snake[1].y + 1) return 's';
    if (s.snake[0].x == s.snake[1].x + 1 && s.snake[0].y == s.snake[1].y) return 'd';
    return 'a';
}

void try_make_portal() {
    if (rand() % 1 == 0)
    {
        portal1.init(snake, fruit, portal2);
        portal2.init(snake, fruit, portal1);
        time_left_for_portal_to_disappear = 50;
        portal_is_opening = true;
    }
}

bool go_to_portal(PORTAL in, PORTAL out, char direct) {
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
void Restart()
{
    base_score = 1;
    game_speed = 220;
    player_score = 0;

    cheat = false;
    End_game = false;
    count_phase = 1;
    is_going_through_portal, first_step;
    SNAKE snake;
    FOOD fruit;
    BOARD board;
    PORTAL portal1, portal2;
    portal_is_opening;
    time_left_for_portal_to_disappear;
}
int main()
{
    REPLAY:{
    Restart();
    Game_level();

    if (End_game)
    {
        char X = (char)254;
        std::cout << "X   X  XX  X  X    X  XX  XXX    X    X    XX  XXX XXX XX \n";
        std::cout << " X X  X  X X  X   X X X X X     X X   X   X  X X   X   X X \n";
        std::cout << "  X   X  X X  X   XXX XX  XXX   XXX   X   X  X XXX XXX XX \n";
        std::cout << "  X   X  X X  X   X X X X X     X X   X   X  X   X X   X X \n";
        std::cout << "  X    XX   XX    X X X X XXX   X X   XXX  XX  XXX XXX X X \n";
        return 0;
    }
    // Nh?p sai cheatcode
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
			if (pre_direct = 'x' && !able_to_move(direct, curr_direct(snake)))
            {
                direct = 'x';
                pre_direct = curr_direct(snake);
            }

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
		if (time_left_for_portal_to_disappear > 0)
        {
            if (!go_to_portal(portal1, portal2, direct))
                go_to_portal(portal2, portal1, direct);
        }
		snake.move(direct);
		snake.draw();
		eat_food(snake, fruit);
		fruit.draw();

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

		if (cheat)
            if (count_phase % 7 == 0)
                if (game_speed >= 30)
                    game_speed -= 20;
		Sleep(game_speed);
	}
	gotoxy(0, 25);
	std::cout << reason<<std::endl;

        if(direct == 'r'){
          system("cls");
          goto REPLAY;
        }
    }
    std::cout<<"an r de choi lai\n";
    char directs = _getch();
    if(directs == 'r'){
    system("cls");
    goto REPLAY;
    }
	return 0;
}
