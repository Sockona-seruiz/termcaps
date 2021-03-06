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

int	ft_delete_character(t_termcaps *s)
{
	int	cols;
	int	rows;

	if (s->i > 0)
	{
		term_get_size(&cols, &rows);
		if (((s->prompt_len + s->i) % cols) == 0)
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
		s->line[s->i] = '\0';
		s->i--;
		return (s->i);
	}
		else
			tputs(tigetstr("bl"), 1, ft_putchar);
	return (0);
}

int	ft_manage_history(t_termcaps *s, int id)
{
	tputs(tigetstr("cr"), 1, ft_putchar);
	tputs(tigetstr("ed"), 1, ft_putchar);
	tputs(tigetstr("DC"), 1, ft_putchar);
	write (1, "Prompt : ", 9);
	if (id == 0)
		write (1, "next", 4);//Ecrire la ligne de l'histo
	else
		write (1, "prev", 4);
	return (set_histo(id, s->line));
}

void	ft_get_line(t_termcaps *s)
{
	while (42)
	{
		s->l = read(0, s->str, 16);
		if (!strcmp(s->str, "\e[A"))
			s->i = ft_manage_history(s, 1);
		else if (!strcmp(s->str, "\e[B"))
			s->i = ft_manage_history(s, 0);
		else if (*(s->str) == 127)
		{
			s->i = ft_delete_character(s);
		}
		else if (*(s->str) >= 32 && *(s->str) <= 126)
		{
			write(1, s->str, 1);
			s->line[s->i] = s->str[0];
			s->i++;
		}
		else if (*(s->str) == 4)
		{
			if (s->i == 0)
			{
				//Exit here
				printf("exit\n");
				exit (0);
			}
		}
		else if (*(s->str) == '\n')
		{
			write (1, "\n", 1);
			s->line[s->i] = '\0';
			printf("line = %s\n", s->line);
			//Send line to treat line HERE
			write (1, "Prompt : ", 9);
			//s->i = 0;
			//s->line[s->i] = '\0';
			break;
		}
	}
}

char	*ft_copy_line(t_termcaps *s)
{
	int		len;
	int		i;
	char	*result;

	i = 0;
	len = s->i;
	if (len == 0)
		return (NULL);
	result = malloc(sizeof(char) * len + 1);
	result[len] = '\0';
	while (i < len)
	{
		result[i] = s->line[i];
		i++;
	}
	return (result);
}

int	main (void)
{
	t_termcaps	*s;
	char		*line;

	s = malloc(sizeof(t_termcaps));
	memset(s, 0, sizeof(t_termcaps)); // FT_MEMESET
	s->term_name = getenv("TERM");
	tcgetattr(0, &(s->term));
	(s->term).c_lflag &= ~(ECHO);
	(s->term).c_lflag &= ~(ICANON);
	s->prompt_len = 9;
	tcsetattr(0, TCSANOW, &(s->term));
	tgetent(0, s->term_name);
	s->line[0] = '\0';
	s->i = 0;
	write (1, "Prompt : ", 9);
	tputs(tigetstr("dm"), 1, ft_putchar);
	ft_get_line(s);
	line = ft_copy_line(s);
	printf ("final line = %s\n", line);
}
