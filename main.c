#include "sand.h"
#include <string.h>
#include <stdlib.h>
void
	print_canvas(char *canvas, struct winsize *win)
{
	write(1, "\033[H", 3);
	usleep(20000);
	write(1, canvas, win->ws_col * win->ws_row);
}

void
	ft_simulate(char *canvas, char *old_canvas, struct winsize *win)
{
	int x = 0;
	int y = 0;

	while (y < win->ws_row)
	{
		x = 0;
		while (x < win->ws_col)
		{
			if (canvas[y * win->ws_col + x] == '#')
			{
				if (y < win->ws_row - 1)
				{
					if (canvas[(y + 1) * win->ws_col + x] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[(y + 1) * win->ws_col + x] = '#';
					}
					else if (x > 0 && canvas[(y + 1) * win->ws_col + (x - 1)] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[(y + 1) * win->ws_col + (x - 1)] = '#';
					}
					else if (x < win->ws_col - 1 && canvas[(y + 1) * win->ws_col + (x + 1)] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[(y + 1) * win->ws_col + (x + 1)] = '#';
					}
				}
			}
			else if (canvas[y * win->ws_col + x] == 'o')
			{
				if (y < win->ws_row - 1)
				{
					if (canvas[(y + 1) * win->ws_col + x] == ' ')
					{
							old_canvas[y * win->ws_col + x] = ' ';
							old_canvas[(y + 1) * win->ws_col + x] = 'o';
					}
					else if (x < win->ws_col - 1 && canvas[y * win->ws_col + (x + 1)] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[y * win->ws_col + (x + 1)] = 'o';
					}
					else if (x > 0 && canvas[y * win->ws_col + (x - 1)] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[y * win->ws_col + (x - 1)] = 'o';
					}
					else if (x > 0 && canvas[(y + 1) * win->ws_col + (x - 1)] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[(y + 1) * win->ws_col + (x - 1)] = 'o';
					}
					else if (x < win->ws_col - 1 && canvas[(y + 1) * win->ws_col + (x + 1)] == ' ')
					{
						old_canvas[y * win->ws_col + x] = ' ';
						old_canvas[(y + 1) * win->ws_col + (x + 1)] = 'o';
					}
				}
			}
			else if (canvas[y * win->ws_col + x] == 'S')
			{
				if (y < win->ws_row - 1)
				{
					old_canvas[(y + 1) * win->ws_col + x] = '#';
				}
			}
			else if (canvas[y * win->ws_col + x] == 'W')
			{
				if (y < win->ws_row - 1)
				{
					old_canvas[(y + 1) * win->ws_col + x] = 'o';
				}
			}
			x++;
		}
		y++;
	}
}

int
	main(int ac, char **av)
{
	struct winsize win;
	struct termios term;
	struct termios oldterm;
	int c_x = 0;
	int c_y = 0;
	int sim_state = 0;

	tcgetattr(0, &oldterm);
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, 0, &term);
	ioctl(0, TIOCGWINSZ, &win);
	char	canvas[win.ws_col * win.ws_row + 1];
	char	old_canvas[win.ws_col * win.ws_row + 1];
	memset(canvas, 32, win.ws_col * win.ws_row);
	memcpy(old_canvas, canvas, win.ws_col * win.ws_row);
	print_canvas(canvas, &win);
	while ("non")
	{
		if (sim_state == 0)
		{
			memcpy(canvas, old_canvas, win.ws_col * win.ws_row);
			char c;
			if (read(0, &c, 1))
			{
				if (c == 32)
				{
					printf("\033[%d;%df", 0, 0);
					fflush(0);
					write(1, "EDIT_MODE", 9);
					printf("\033[%d;%df", c_y + 1, c_x + 1);
					fflush(0);
					sim_state = 1;
				}
				if (c == 104)
					sim_state = 3;
			}
			if (sim_state == 0)
			{
				ft_simulate(canvas, old_canvas, &win);
				print_canvas(canvas, &win);
			}
		}
		else if (sim_state == 2)
		{
			int c;
			if (read(0, &c, 1))
			{
				if (c == 27)
					sim_state = 0;
			}
		}
		else if (sim_state == 3)
		{
			printf("\033[%d;%df", 20, 20);
			printf("==========================================================================");
			printf("\033[%d;%df", 21, 20);
			printf("||                     Welcome to sandsim by lejulien                   ||");
			printf("\033[%d;%df", 22, 20);
			printf("==========================================================================");
			printf("\033[%d;%df", 23, 20);
			printf("|| space : edit Mode                ||  escape : go back to sim         ||");
			printf("\033[%d;%df", 24, 20);
			printf("==========================================================================");
			printf("\033[%d;%df", 25, 20);
			printf("|| S : Create sand source           ||  s : create sand                 ||");
			printf("\033[%d;%df", 26, 20);
			printf("==========================================================================");
			printf("\033[%d;%df", 27, 20);
			printf("|| W : create water source          ||  w : create water                ||");
			printf("\033[%d;%df", 28, 20);
			printf("==========================================================================");
			printf("\033[%d;%df", 29, 20);
			printf("|| b : create blocks                ||  return : remove things          ||");
			printf("\033[%d;%df", 30, 20);
			printf("==========================================================================");
			printf("\033[%d;%df", 31, 20);
			printf("||                         q : to quit the sim                          ||");
			printf("\033[%d;%df", 32, 20);
			printf("==========================================================================");
			fflush(0);
			sim_state = 2;
		}
		else if (sim_state == 1)
		{
			int c;
			if (read(0, &c, 4))
			{
				if (c == 27) // escape
				{
					sim_state = 0;
					continue ;
				}
				if (c == 4283163 && c_y > 0) // up
				{
					c_y -= 1;
					write(1, &c, 3);
				}
				if (c == 4348699 && c_y < win.ws_row - 1) // down
				{
					c_y += 1;
					write(1, &c, 3);
				}
				if (c == 4479771 && c_x > 0) // left
				{
					c_x -= 1;
					write(1, &c, 3);
				}
				if (c == 4414235 && c_x < win.ws_col - 1) // right
				{
					c_x += 1;
					write(1, &c, 3);
				}
				if (c == 115) // sand
				{
					old_canvas[c_y * win.ws_col + c_x] = '#';
					write(1, "#", 1);
					write(1, "\033[D", 3);
				}
				if (c == 83) // sand generator
				{
					old_canvas[c_y * win.ws_col + c_x] = 'S';
					write(1, &c, 1);
					write(1, "\033[D", 3);
				}
				if (c == 119) // water
				{
					old_canvas[c_y * win.ws_col + c_x] = 'o';
					write(1, "o", 1);
					write(1, "\033[D", 3);
				}
				if (c == 87) // water generator
				{
					old_canvas[c_y * win.ws_col + c_x] = 'W';
					write(1, "W", 1);
					write(1, "\033[D", 3);
				}
				if (c == 98) // Block
				{
					old_canvas[c_y * win.ws_col + c_x] = '=';
					write(1, "=", 1);
					c_x += 1;
				}
				if (c == 127) // delete
				{
					old_canvas[c_y * win.ws_col + c_x] = ' ';
					write(1, " ", 1);
					write(1, "\033[2D", 4);
					c_x -= 1;
				}
				if (c == 113) // quit
					exit(0);
				if (c == 99)
				{
					memset(old_canvas, ' ', win.ws_col * win.ws_row);
					memcpy(canvas, old_canvas, win.ws_col * win.ws_row);
					print_canvas(canvas, &win);
					printf("\033[%d;%df", 0, 0);
					fflush(0);
					write(1, "EDIT_MODE", 9);
					printf("\033[%d;%df", c_y + 1, c_x + 1);
					fflush(0);
				}

				c = 0;
			}
		}
	}
	tcsetattr(0, 0, &oldterm);
	return (0);
}
