#ifndef ARKANOID_H
# define ARKANOID_H

# include <unistd.h>
# include <stdlib.h>
# include <GLFW/glfw3.h>
# include <GL/gl.h>
# include <GL/freeglut.h>
# include <fcntl.h>
# include <iostream>
# include <vector>
# include "LeapControl.hpp"

# define TRUE 1
# define FALSE 0

extern GLfloat g_x;
extern int g_mod;
extern bool g_launch;

class Game {

public:
    GLFWwindow	*win;
	int			win_x;
	int			win_y;

    bool		getlvl;
	std::string	lvl;
	char		lvl_next;
	int			len;
	std::vector<std::string>		level;

    double			ball_x;
	double			ball_y;

	bool			directx;
	bool			directy;

    int			lives;
	int			lines;

    float		v1x;
	float		v1y;
	float		v2x;
	float		v2y;
	float		v3x;
	float		v3y;
	float		v4x;
	float		v4y;

    LeapControl leap;

    Game();
    ~Game();

    int play();

    void draw_level();
    void draw_player();
    void draw_ball();

    void init_ball();
    void get_level();

    void put_vertex(int x, int y, int value);
    void put_ball();
    void save_pos(int x, int y, float size);

    void checkMove();
    void check_pad();
    void check_brick();
    void checkBallOnPad();
    int the_return_ofcheck_brick(int x, int y, float size);

    int make_level(std::string lvln);

    void move(double value);
    void move_left(void);
    void move_right(void);

    void victory();
    void next_level();
    // void			move_left(void);
    // void			move_right(void);
    // void			next_level(t_game *g);
};

#endif
