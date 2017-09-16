#include <GLFW/glfw3.h>
#include "Game.hpp"

GLfloat g_x = 0;
int g_mod = 7;
bool g_launch = false;

static void	move_left(void)
{
    if (::g_x > -0.9)
    ::g_x -= 0.01 * ::g_mod;
}

static void	move_right(void)
{
    if (::g_x < 0.9)
    ::g_x += 0.01 * ::g_mod;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        move_left();
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        move_right();
    if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS ||
        action == GLFW_REPEAT))
        ::g_launch = true;
}


int main(void)
{
	Game *arkanoid = new Game();

    glfwInit();
    arkanoid->win = glfwCreateWindow(arkanoid->win_x, arkanoid->win_y, "Arkanoid", nullptr, nullptr);
    glfwMakeContextCurrent(arkanoid->win);
    glfwSwapInterval(1);

    glfwSetKeyCallback(arkanoid->win, key_callback);

    while (!glfwWindowShouldClose(arkanoid->win))
        arkanoid->play();

    glfwDestroyWindow(arkanoid->win);
	return (0);
}
