#include "Game.hpp"
#include "LeapControl.hpp"
#include <fstream>
#include <sstream>
#include <array>

Game::Game() {
    // LeapControl this->leap();
    this->lives = 5;
	this->win_x = 800;
	this->win_y = 600;
	this->getlvl = false;
	this->lvl = "1";
	this->lvl_next = '1';
}

Game::~Game() {

}

int
Game::play()
{
	if (this->win_x <= 200 && this->win_y <= 200)
		glfwSetWindowSize(this->win, 800, 600);
	glfwGetFramebufferSize(this->win, &(this->win_x), &(this->win_y));
	glViewport(0, 0, this->win_x, this->win_y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_player();
	draw_level();
    checkMove();
	if (::g_launch == true)
		draw_ball();
	else if (::g_launch == false)
		init_ball();
	glfwSwapBuffers(this->win);
	glfwPollEvents();
	return (0);
}

void	Game::move_left(void)
{
    if (::g_x > -0.9)
    ::g_x -= 0.01 * ::g_mod;
}

void	Game::move_right(void)
{
    if (::g_x < 0.9)
    ::g_x += 0.01 * ::g_mod;
}

void	Game::move(double value)
{
    ::g_x = value;// * ::g_mod;
}

void Game::checkMove() {
    this->leap.getCoord();
    // if (this->leap.x < 0)
    //     move_left(this->leap.x);
    // else if (this->leap.x > 0)
    //     move_right(this->leap.x);
    // else
    //     return;
    move(this->leap.x);
}

// Function Drawing
void
Game::draw_level()
{
	int		value;
    int x = 0;
    int y = 0;

	if (this->lvl[0] == this->lvl_next)
	{
		get_level();
		init_ball();
	}
	if ((this->level.empty() == false) && (this->level[0].empty() == false))
		this->len = this->level[0].size();
    for(auto row : this->level) {
        while (row[x])
        {
            value = row[x] - '0';
            if (row[x] >= '0')
                put_vertex(x, y, value);
            x++;
        }
        // std::cout << "Looooooop" << std::endl;
        y++;
    }
}

void
Game::draw_player(void)
{
	glBegin(GL_QUADS);
	glColor3f(153, 255, 204);
	glVertex2f(-0.1f + ::g_x, 0.01f - 0.9f);
	glVertex2f(0.1f + ::g_x, 0.01f - 0.9f);
	glVertex2f(0.1f + ::g_x, -0.01f - 0.9f);
	glVertex2f(-0.1f + ::g_x, -0.01f - 0.9f);
	glEnd();
}

void
Game::draw_ball()
{
	if (this->ball_y < 185 && this->directy == true)
	{
		if (this->ball_y + 1 == 185)
			this->directy = false;
		this->ball_y += 1;
	}
	else if (this->ball_y > 0 && this->directy == false)
	{
		this->directy = false;
		this->ball_y--;
	}
	if (this->ball_x <= 97 && this->directx == true)
	{
		if (this->ball_x + 1 > 97)
			this->directx = false;
		this->ball_x++;
	}
	else if (this->ball_x >= -99 && this->directx == false)
	{
		if (this->ball_x - 1 < -99)
			this->directx = true;
		this->ball_x--;
	}
    checkBallOnPad();
	put_ball();
	check_pad();
}

void Game::checkBallOnPad() {
    // std::cout << "GX = " << ::g_x << "  ball_x = " << this->ball_x / 100 << std::endl;
    if (this->ball_x / 100 < ::g_x + 0.03f && this->ball_y == 0)
        this->directx = false;
    else if (this->ball_x / 100 > ::g_x + 0.07f && this->ball_y == 0)
        this->directx = true;
}

void
Game::init_ball()
{
	this->directx = true;
	this->directy = true;
	this->ball_x = ::g_x * 100;
	this->ball_y = 0;
}

void
Game::get_level()
{
	std::fstream fs;
	std::string lvl_name;

    lvl_name = "../levels/level" + this->lvl;
	make_level(lvl_name);
}

void
Game::put_vertex(int x, int y, int value)
{
	float	size;

	glBegin(GL_QUADS);
	size = ((float)this->win_x / this->len);
	save_pos(x, y, size);
	glColor3f(1, 1 - 0.1 * value, 1 - 0.1 * value);
	glVertex2f(this->v1x, this->v1y);
	glColor3f(0.3f, 0.3f - 0.1 * value, 0.3f - 0.1 * value);
	glVertex2f(this->v2x, this->v2y);
	glVertex2f(this->v3x, this->v3y);
	glVertex2f(this->v4x, this->v4y);
	glEnd();
}

void
Game::put_ball()
{
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex2f(0.0f + ((float)this->ball_x * 0.01f),
	-0.87f + ((float)this->ball_y * 0.01f));
	glVertex2f(0.0f + ((float)this->ball_x * 0.01f),
	-0.9f + ((float)this->ball_y * 0.01f));
	glVertex2f(0.028f + ((float)this->ball_x * 0.01f),
	-0.9f + ((float)this->ball_y * 0.01f));
	glVertex2f(0.028f + ((float)this->ball_x * 0.01f),
	-0.87f + ((float)this->ball_y * 0.01f));
	glEnd();
}

void
Game::save_pos(int x, int y, float size)
{
	this->v1x = -1.0f + ((float)x * size / (this->win_x / 2)) + 0.005f;
	this->v1y = 0.99f - (0.10 * y);
	this->v2x = -1.0f + ((float)(x + 1) * size / (this->win_x / 2)) - 0.005f;
	this->v2y = 0.99f - (0.10 * y);
	this->v3x = -1.0f + ((float)(x + 1) * size / (this->win_x / 2)) - 0.005f;
	this->v3y = 0.90f - (0.10 * y);
	this->v4x = -1.0f + ((float)x * size / (this->win_x / 2)) + 0.005f;
	this->v4y = 0.90f - (0.10 * y);
}

void
Game::check_pad()
{
	if (this->ball_y == 0)
	{
		if (((float)this->ball_x * 0.01) < ::g_x + 0.12 &&
			((float)this->ball_x * 0.01) > ::g_x - 0.12)
			this->directy = 1;
		else
		{
			this->lives--;
			if (this->lives == 0)
			{
				std::cout << "You lost freak." << std::endl;
				exit(0);
			}
			::g_launch = false;
		}
	}
	check_brick();
}

void
Game::check_brick()
{
	float	size;
    int x = 0;
    int y = 0;

    size = 0.f;
	if ((this->level.empty() == false) && (this->level[0].empty() == false))
		size = ((float)this->win_x / this->len);
    for(auto row : this->level) {
        while (row[x])
        {
            if (row[x] != ' ') {
                if (the_return_ofcheck_brick(x, y, size) == 1)
                {
                    next_level();
                    return ;
                }
            }
            x++;
        }
        // std::cout << "Looooooop" << std::endl;
        y++;
    }
}

int
Game::the_return_ofcheck_brick(int x, int y, float size)
{
	if (this->ball_y <= 185 && this->ball_y >= 180 - (y * 10))
	{
		if ((float)this->ball_x * 0.01 >= -1.0 + ((float)x * size /
					(this->win_x / 2)) + 0.005 &&
				(float)this->ball_x * 0.01 <= -1.0 + ((float)(x + 1) *
					size / (this->win_x / 2)) - 0.005)
		{
			this->directy *= -1;
			if (this->level[y][x] == '0')
				this->level[y][x] = ' ';
			else if (this->level[y][x] == '1')
				this->level[y][x] = '0';
			else if (this->level[y][x] == '2')
				this->level[y][x] = '1';
			return (1);
		}
	}
	return (0);
}

int
Game::make_level(std::string lvln)
{
	// int		x;
	// int		y;
	std::string line;

	std::ifstream file(lvln);
	// x = 0;
    while (std::getline(file, line))
    {
        this->level.push_back(line);
    }
	// while (ft_gnl(fd, &line) > 0)
	// {
    //     std::string line;
	// 	this->level[x] = ft_strdup(line);
	// 	x++;
	// 	free(line);
	// }
	// this->level[x] = NULL;
	this->lvl_next++;
	return (0);
}

void
Game::victory()
{
	std::cout << "Victory is yours!" << std::endl;
	exit(0);
}

void
Game::next_level()
{
    if (this->lvl[0] > '0' && this->lvl[0] <= '3')
	{
        for (auto row : this->level)
		{
			for (auto brick : row)
			{
				if (brick >= '0' && brick <= '2')
					return ;
			}
		}
		this->lvl[0]++;
	}
	else if (this->lvl[0] > '3') {
        victory();
    }
    else {
        return;
    }
}
