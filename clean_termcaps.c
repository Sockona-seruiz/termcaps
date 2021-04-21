#include "termcaps.h"

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

int	ft_delete_character(t_termcaps s)
{
	int	cols;
	int	rows;

	if (s.i > 0)
	{
		term_get_size(&cols, &rows);
		if (((s.prompt_len + s.i) % cols) == 0)
		{
			tputs(tigetstr("bw"), 1, ft_putchar);
			tputs(cursor_left, 1, ft_putchar);
			ft_putchar(' ');
		}
		else
		{
			tputs(tigetstr("le"), 1, ft_putchar);
			tputs(cursor_left, 1, ft_putchar);
			ft_putchar(' ');
			tputs(cursor_left, 1, ft_putchar);
		}
		s.line[s.i] = '\0';
		s.i--;
		return (s.i);
	}
		else
			tputs(tigetstr("bl"), 1, ft_putchar);
	return (0);
}

int	ft_manage_history(t_termcaps s, int id)
{
	tputs(tigetstr("cr"), 1, ft_putchar);
	tputs(tigetstr("ed"), 1, ft_putchar);
	tputs(tigetstr("DC"), 1, ft_putchar);
	write (1, "Prompt : ", 9);
	write (1, "prev", 4);//Ecrire la ligne de l'histo
	return (set_histo(id, s.line));
}

void printf_term(struct termios term)
{
	printf("%s\n", term.c_cc);
	printf("%lu\n", term.c_cflag);
	printf("%lu\n", term.c_iflag);
	printf("%lu\n", term.c_ispeed);
	printf("%lu\n", term.c_lflag);
	printf("%lu\n", term.c_oflag);
	printf("%lu\n", term.c_ospeed);
}

int	main (void)
{
	t_termcaps	s;
	char		str[2000];

	memset(&s, 0, sizeof(t_termcaps));
	s.term_name = getenv("TERM");
	tcgetattr(0, &(s.term));
	(s.term).c_lflag &= ~(ECHO);
	(s.term).c_lflag &= ~(ICANON);
	s.prompt_len = 9;
	tcsetattr(0, TCSANOW, &(s.term));
	tgetent(0, s.term_name);
	s.line[0] = '\0';
	s.i = 0;
	printf_term(s.term);
	write (1, "Prompt : ", 9);
	while (42)
	{
		tputs(tigetstr("dm"), 1, ft_putchar);
		do
		{
			s.l = read(0, str, 100);
			if (!strcmp(str, "\e[A"))
			{
				printf("wesh\n");
				s.i = ft_manage_history(s, 1);
			}
			else if (!strcmp(str, "\e[B"))
			{
				//s.i = ft_manage_history(s, 0);
				tputs(tigetstr("cr"), 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				tputs(tigetstr("DC"), 1, ft_putchar);
				//ft_clear_line (col);
				//tputs(tigetstr("cr"), 1, ft_putchar);
				//tputs(tigetstr("ce"), 1, ft_putchar);
				write (1, "Prompt : ", 9);
				write (1, "next", 4);
				s.i = set_histo(0, s.line);
			}

			else if (*str == 127)
			{
				s.i = ft_delete_character(s);
			}
			else if (*str >= 32 && *str <= 126)
			{
				//strbackup = ft_strcpy(str);
				//strtest = ft_strjoin(strtest, str);
				write(1, str, 1);
				s.line[s.i] = str[0];
				s.i++;
			}
			else if (!strcmp(str, "\4"))
			{
				write (1, "", 0);
				if (s.i == 0)
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
				s.line[s.i] = '\0';
				printf("line = %s\n", s.line);
				//Send line to treat line HERE
				write (1, "Prompt : ", 9);
				s.i = 0;
				s.line[s.i] = '\0';
			}
		}while (strcmp(str,"\n") && strcmp(str, "\4"));
	}
	write (1, "\n", 1);
	tputs(save_cursor, 1 ,ft_putchar);
	tputs(tigetstr("me"), 1, putchar);
}
