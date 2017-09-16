#include "Game.hpp"
#include "LeapControl.hpp"
#include <fstream>
#include <sstream>
#include <array>

/**
 ** TODO: Repair draw_level
 ** Abstract draw functions
 ** Abstract Move
 ** More levels
 ** Fix Physic (Ball on pad + ball against brick)
 */

Game::Game() {
    // LeapControl leap();
    lives = 5;
    win_x = 800;
    win_y = 600;
    getlvl = false;
    lvl = "1";
    lvl_next = '1';
}

Game::~Game() {

}

int Game::play()
{
    if (win_x <= 200 && win_y <= 200)
        glfwSetWindowSize(win, 800, 600);
    glfwGetFramebufferSize(win, &(win_x), &(win_y));
    glViewport(0, 0, win_x, win_y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    draw_player();
    draw_level();
    checkMove();
    if (::g_launch == true)
        draw_ball();
    else if (::g_launch == false) {
        if (leap.checkLaunch())
            ::g_launch = true;
        init_ball();
    }
    glfwSwapBuffers(win);
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
    if (leap.connected){
        leap.getCoord();
        move(leap.x);
    }
}

// Function Drawing
void Game::draw_level()
{
    int value;
    int x = 0;
    int y = 0;

    if (lvl[0] == lvl_next) {
		get_level();
		init_ball();
	}
    if ((level.empty() == false) && (level[0].empty() == false))
        len = level[0].size();
    for(auto row : level) {
        x = 0;
        while (row[x])
        {
            value = row[x] - '0';
            if (row[x] >= '0')
                put_vertex(x, y, value);
            x++;
        }
        y++;
    }
}

void Game::draw_player(void)
{
	glBegin(GL_QUADS);
	glColor3f(153, 255, 204);
    glVertex2f(-0.1f + ::g_x, 0.01f - 0.9f);
    glVertex2f(0.1f + ::g_x, 0.01f - 0.9f);
    glVertex2f(0.1f + ::g_x, -0.01f - 0.9f);
    glVertex2f(-0.1f + ::g_x, -0.01f - 0.9f);
	glEnd();
}

void Game::draw_ball() {
    if (ball_y < 185 && directy == true)
	{
        if (ball_y + 1 == 185)
            directy = false;
        ball_y += 1;
	}
    else if (ball_y > 0 && directy == false)
	{
        directy = false;
        ball_y--;
	}
    if (ball_x <= 97 && directx == true)
	{
        if (ball_x + 1 > 97)
            directx = false;
        ball_x++;
	}
    else if (ball_x >= -99 && directx == false)
	{
        if (ball_x - 1 < -99)
            directx = true;
        ball_x--;
	}
    checkBallOnPad();
    put_ball();
    check_pad();
}

void Game::checkBallOnPad() {
    // std::cout << "GX = " << ::g_x << "  ball_x = " << ball_x / 100 << std::endl;
    if (ball_x / 100 < ::g_x + 0.03f && ball_y == 0)
        directx = false;
    else if (ball_x / 100 > ::g_x + 0.07f && ball_y == 0)
        directx = true;
}

void Game::init_ball() {
    directx = true;
    directy = true;
    ball_x = ::g_x * 100;
    ball_y = 0;
}

void Game::get_level() {
	std::fstream fs;
	std::string lvl_name;

    lvl_name = "levels/level" + lvl;
	make_level(lvl_name);
}

void Game::put_vertex(int x, int y, int value) {
	float	size;

	glBegin(GL_QUADS);
    size = ((float)win_x / len);
	save_pos(x, y, size);
	glColor3f(1, 1 - 0.1 * value, 1 - 0.1 * value);
    glVertex2f(v1x, v1y);
	glColor3f(0.3f, 0.3f - 0.1 * value, 0.3f - 0.1 * value);
    glVertex2f(v2x, v2y);
    glVertex2f(v3x, v3y);
    glVertex2f(v4x, v4y);
	glEnd();
}

void Game::put_ball() {
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
    glVertex2f(0.0f + ((float)ball_x * 0.01f),
    -0.87f + ((float)ball_y * 0.01f));
    glVertex2f(0.0f + ((float)ball_x * 0.01f),
    -0.9f + ((float)ball_y * 0.01f));
    glVertex2f(0.028f + ((float)ball_x * 0.01f),
    -0.9f + ((float)ball_y * 0.01f));
    glVertex2f(0.028f + ((float)ball_x * 0.01f),
    -0.87f + ((float)ball_y * 0.01f));
	glEnd();
}

void Game::save_pos(int x, int y, float size) {
    v1x = -1.0f + ((float)x * size / (win_x / 2)) + 0.005f;
    v1y = 0.99f - (0.10 * y);
    v2x = -1.0f + ((float)(x + 1) * size / (win_x / 2)) - 0.005f;
    v2y = 0.99f - (0.10 * y);
    v3x = -1.0f + ((float)(x + 1) * size / (win_x / 2)) - 0.005f;
    v3y = 0.90f - (0.10 * y);
    v4x = -1.0f + ((float)x * size / (win_x / 2)) + 0.005f;
    v4y = 0.90f - (0.10 * y);
}

void Game::check_pad() {
    if (ball_y == 0)
	{
        if (((float)ball_x * 0.01) < ::g_x + 0.12 &&
            ((float)ball_x * 0.01) > ::g_x - 0.12)
            directy = 1;
		else
		{
            lives--;
            if (lives == 0)
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
Game::check_brick() {
	float	size;
    int x = 0;
    int y = 0;

    size = 0.f;
    if ((level.empty() == false) && (level[0].empty() == false))
        size = ((float)win_x / len);
    for(auto row : level) {
        x = 0;
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
Game::the_return_ofcheck_brick(int x, int y, float size) {
    if (ball_y <= 185 && ball_y >= 180 - (y * 10))
	{
        if ((float)ball_x * 0.01 >= -1.0 + ((float)x * size /
                    (win_x / 2)) + 0.005 &&
                (float)ball_x * 0.01 <= -1.0 + ((float)(x + 1) *
                    size / (win_x / 2)) - 0.005)
		{
            directy *= -1;
            if (level[y][x] == '0')
                level[y][x] = ' ';
            else if (level[y][x] == '1')
                level[y][x] = '0';
            else if (level[y][x] == '2')
                level[y][x] = '1';
			return (1);
		}
	}
	return (0);
}

int
Game::make_level(std::string lvln)
{
    std::string line;

	std::ifstream file(lvln);
    while (std::getline(file, line))
    {
        level.push_back(line);
    }
    lvl_next++;
	return (0);
}

/*!
 * \brief Game::victory
 */
void Game::victory()
{
	std::cout << "Victory is yours!" << std::endl;
	exit(0);
}

/*!
 * \brief Game::next_level
 */
void Game::next_level()
{
    if (lvl[0] > '0' && lvl[0] <= '3')
	{
        for (auto row : level)
		{
			for (auto brick : row)
			{
				if (brick >= '0' && brick <= '2')
					return ;
			}
		}
        lvl[0]++;
    } else if (lvl[0] > '3') {
        victory();
    } else {
        return;
    }
}
