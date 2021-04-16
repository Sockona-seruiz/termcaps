#include <curses.h>
#include <term.h>
#include <termios.h>

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

int	main(void)
{
	char	str[2000];
	char	*strtest;
	int		l;
	struct termios	term;
	char	*term_name = "xterm-256color";
	char	*cm_cap = tgetstr("cm", NULL);
	int		col;

	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);

	tcsetattr(0, TCSANOW, &term);
	tgetent(0, term_name);

	char	*strbackup;
	char *reset_cmd = tgetstr("me", NULL);
	col = tgetnum("co");

	strtest = malloc(sizeof(char));
	strtest[0] = '\0';
	while (strcmp(str, "\4"))
	{
		tputs(tigetstr("dm"), 1, ft_putchar);
		do
		{
			l = read(0, str, 100);
			if (!strcmp(str, "\e[A"))
			{

				tputs(tigetstr("cr"), 1, ft_putchar);
				ft_clear_line (col);
				tputs(tigetstr("cr"), 1, ft_putchar);
				//tputs(tigetstr("ce"), 1, ft_putchar);

				write (1, "prev", 4);//Ecrire la ligne de l'histo
			}
			else if (!strcmp(str, "\e[B"))
			{
				tputs(tigetstr("cr"), 1, ft_putchar);
				ft_clear_line (col);
				tputs(tigetstr("cr"), 1, ft_putchar);
				//tputs(tigetstr("ce"), 1, ft_putchar);

				write (1, "next", 4);
			}
			else if (*str == 127)// && !strcmp(str, "\177"))
			{
				//strtest = ft_remove_last_char(strtest);
				tputs(cursor_left, 1, ft_putchar);
				tputs(tigetstr("ed"), 1, ft_putchar);
				//tputs(tigetstr("ll"), 1, ft_putchar);
				//write(1, "back", 4);
				//write (1, strtest, ft_strlen(strtest));
			}
			else if (!strcmp(str, "S"))
				printf("test = %s", strtest);
			else if (*str >= 32 && *str <= 126)
			{
				//strbackup = ft_strcpy(str);
				//strtest = ft_strjoin(strtest, str);
				write(1, str, l);
			}
			else if (*str == '\n')
				write (1, "\n", 1);
		}while (strcmp(str,"\n") && strcmp(str, "\4"));
	}
	write (1, "\n", 1);
	tputs(save_cursor, 1 ,ft_putchar);
	tputs(reset_cmd, 1, putchar);
}
