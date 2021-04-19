#include <curses.h>
#include <term.h>
#include <termios.h>
#include <sys/ioctl.h>

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
#include <string.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*dest;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!s1 || !s2)
		return (0);
	len = ft_strlen(s1) + ft_strlen(s2);
	i = -1;
	j = -1;
	if (NULL == (dest = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (s1[++i])
		dest[i] = s1[i];
	while (s2[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	free(s1);
	return (dest);
}

char	*ft_remove_last_char(char *strtest)
{
	char	*dest;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(strtest) - 1;
	if (len < 0)
		len = 0;
	dest = malloc(sizeof(char) * len);
	while (i < len)
	{
		dest[i] = strtest[i];
		i++;
	}
	dest[len] = '\0';
	free(strtest);
	return (dest);
}

char	*ft_strcpy(char *str)
{
	int		i;
	char	*ret;

	i = 0;
	while (str[i])
		i++;
	ret = malloc(sizeof(char) * (i + 1));
	i = 0;
	while (str[i])
	{
		printf("str[i] = %d\n", str[i]);
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void	ft_clear_line(int col)
{
	while (col > 0)
	{
		write(1, " ", 1);
		col--;
	}
}

int	set_histo(int id, char line[2000])
{
	if (id == 0)
	{
		line[0] = 'n';
		line[1] = 'e';
		line[2] = 'x';
		line[3] = 't';
		line[4] = '\0';
	}
	else if (id == 1)
	{
		line[0] = 'p';
		line[1] = 'r';
		line[2] = 'e';
		line[3] = 'v';
		line[4] = '\0';
	}
	return (ft_strlen(line));
}

static void	term_get_size(int *cols, int *rows)
{
	struct winsize ws;

	if (ioctl(0, TIOCGWINSZ, &ws) == 0) {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
	} else
		*cols = *rows = -1;
}


int	main(void)
{
	char	str[2000];
	char	line[2000];
	int		i = 0;
	//char	*strtest;
	int		l;
	struct termios	term;
	char	*term_name = "xterm-256color";
	char	*cm_cap = tgetstr("cm", NULL);
	int		col;
	int		prompt_len = 9;

	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);

	tcsetattr(0, TCSANOW, &term);
	tgetent(0, term_name);

	//char	*strbackup;
	char *reset_cmd = tgetstr("me", NULL);
	col = tgetnum("co");

	tputs(tigetstr("cl"), 1, ft_putchar);

	//strtest = malloc(sizeof(char));
	//strtest[0] = '\0';
	line[0] = '\0';
	write (1, "Prompt : ", 9);

	// Si la str est VIDE on ne peut pas continuer à erase. Si la str + le prompt
	// est plus long que le nombre de col du terminal alors il faut remonter
	// d'une ligne (dans le cas d'un backspace ou d'un next / prev)

	while (42)
	{
		tputs(tigetstr("dm"), 1, ft_putchar);
		do
		{
			l = read(0, str, 100);
			if (!strcmp(str, "\e[A"))
			{

				tputs(tigetstr("cr"), 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				tputs(tigetstr("DC"), 1, ft_putchar);
				//tputs(tigetstr("ce"), 1, ft_putchar);
				write (1, "Prompt : ", 9);
				write (1, "prev", 4);//Ecrire la ligne de l'histo
				i = set_histo(1, line);
			}
			else if (!strcmp(str, "\e[B"))
			{
				tputs(tigetstr("cr"), 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				tputs(tigetstr("DC"), 1, ft_putchar);
				//ft_clear_line (col);
				//tputs(tigetstr("cr"), 1, ft_putchar);
				//tputs(tigetstr("ce"), 1, ft_putchar);
				write (1, "Prompt : ", 9);
				write (1, "next", 4);
				i = set_histo(0, line);
			}

			else if (!strcmp(str, "\e[D"))
			{
				tputs(tigetstr("sf"), 1, ft_putchar);
				//tputs(cursor_left, 1, ft_putchar);
			}
/*
			else if (!strcmp(str, "\e[C"))
			{
				//tputs(tigetstr("kI"), 1, ft_putchar);
				tputs(cursor_right, 1, ft_putchar);
			}
*/
			else if (*str == 127)
			{
				if (i > 0)
				{
					int cols;
					int rows;

					term_get_size(&cols, &rows);
					//printf("cols = %d\nrows = %d\n", cols, rows);
					//col = tgetnum("co");
					if (((prompt_len + i) % cols) == 0)
					{
						tputs(tigetstr("bw"), 1, ft_putchar);
						tputs(tigetstr("le"), 1, ft_putchar);
						tputs(cursor_left, 1, ft_putchar);
						ft_putchar(' ');
						//tputs(cursor_left, 1, ft_putchar);
					}
					//strtest = ft_remove_last_char(strtest);
					//tputs(cursor_left, 1, ft_putchar);
					//tputs(tigetstr("ed"), 1, ft_putchar);
					//write (1, " ", 1);
					//ft_putchar(' ');
					else
					{
					tputs(tigetstr("le"), 1, ft_putchar);
					//ft_putchar(' ');
					tputs(cursor_left, 1, ft_putchar);
					//tputs(tigetstr("bw"), 1, ft_putchar);
					ft_putchar(' ');
					//ft_putchar(' ');
					/*
					char cmd_term;
					cmd_term = 0x1b;
					write(1, &cmd_term, 1);
					write (1, "[6n", 3);
					char test[10];
					read(1, test, 10);
					write(2, test, 10);
					//write (1, "\033[6n", 4);
					*/
					tputs(cursor_left, 1, ft_putchar);
					}
					line[i] = '\0';
					i--;
					//tputs(tigetstr("ll"), 1, ft_putchar);
					//write(1, "back", 4);
					//write (1, strtest, ft_strlen(strtest));
				}
				else
					tputs(tigetstr("bl"), 1, ft_putchar);
			}
			else if (*str >= 32 && *str <= 126)
			{
				//strbackup = ft_strcpy(str);
				//strtest = ft_strjoin(strtest, str);
				write(1, str, 1);
				line[i] = str[0];
				i++;
			}
			else if (!strcmp(str, "\4"))
			{
				write (1, "", 0);
				if (i == 0)
				{
					//Exit here
					printf("exit\n");
					exit (0);
				}
				printf ("Ctrl D\n");
			}
			else if (*str == '\n')
			{
				write (1, "\n", 1);
				line[i] = '\0';
				printf("line = %s\n", line);
				//Send line to treat line HERE
				write (1, "Prompt : ", 9);
				i = 0;
				line[i] = '\0';
			}
		}while (strcmp(str,"\n") && strcmp(str, "\4"));
	}
	write (1, "\n", 1);
	tputs(save_cursor, 1 ,ft_putchar);
	tputs(reset_cmd, 1, putchar);
}
