#include <curses.h>
#include <term.h>
#include <termios.h>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
#include <string.h>

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

int	main(int argc, char const **argv, char const **envp)
{
	char	str[2000];
	int		l;
	struct termios	term;
	char	*term_name = "xterm-256color";

	(void)argc;
	(void)argv;
	(void)envp;
	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSANOW, &term);
	tgetent(0, term_name);

	while (strcmp(str, "\4"))
	{
		tputs(save_cursor, 1, ft_putchar);
		do
		{
			l = read(0, str, 100);
			if (!strcmp(str, "\e[A"))
			{
				tputs(restore_cursor, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				write (1, "prev", 4);
			}
			else if (!strcmp(str, "\e[B"))
			{
				tputs(restore_cursor, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				write (1, "next", 4);
			}
			else if (!strcmp(str, key_backspace) && !strcmp(str, "\177"))
			{
				tputs(cursor_left, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
			}
			else
			{
				write(1, str, l);
				//tputs(save_cursor, 1 ,ft_putchar);
			}
		}while (strcmp(str,"\n") && strcmp(str, "\4"));
	}
	write (1, "\n", 1);
	tputs(save_cursor, 1 ,ft_putchar);
}