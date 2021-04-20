#ifndef TERMCAPS_H
# define TERMCAPS_H

#include <curses.h>
#include <term.h>
#include <termios.h>
#include <sys/ioctl.h>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
#include <string.h>

typedef struct		s_termcaps
{
	char			str[2000];
	char			line[2000];
	int				i;
	int				l;
	int				col;
	int				prompt_len;
	struct termios	term;
	char			*term_name;
}					t_termcaps;

#endif