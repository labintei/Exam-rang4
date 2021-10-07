/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micorshell_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labintei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:37:17 by labintei          #+#    #+#             */
/*   Updated: 2021/10/07 21:05:03 by labintei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct		s_list
{
	char			**cmds;
	char			type;
	int				pipe_fd[2];
	struct s_list	*previous;
	struct s_list	*next;
}					t_list;

int		ft_strlen(char	*s)
{
	int		i;

	i = 0;
	while(s && s[i])
		i++;
	return(i);
}

int		ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	return(fd);
}

void	exit_void_error()
{
	ft_putstr_fd("fatal error",2);
	exit(1);
}

void	restart_t_list(t_list	**list)
{
	while ((*list) && (*list)->previous)
		(*list) = (*list)->previous;
}

void	clear_tab(char	***s)
{
	int	i;

	i = -1;
	while ((*s) && (*s)[(++i)])
		free((*s)[i]);
	if (*s)
		free(*s);
}

void	clear_list(t_list	**list)
{
	t_list	*temp;

	restart_t_list(list);
	while((*list))
	{
		temp = (*list)->next;
		if((*list)->cmds)
			clear_tab(&((*list)->cmds));
		free((*list));
		(*list) = temp;
	}
}

int	count_args(char	**s, int i)
{
	int		j;

	j = i;
	while(s && s[j] && strcmp(s[j], "|") && strcmp(s[j], ";"))
		j++;
	return(j + 1 - i);
}


void	add_arg(t_list *cmds, char **s, int *i)
{
	int	j;

	cmds->cmds = malloc(sizeof(char *) * (count_args(s, (*i)) + 1));
	if(cmds->cmds)
		return(exit_void_error());
	while(s && s[(*i)] && strcmp("|", s[(*i)]) && strcmp(";", s[(*i)]))
	{

		(*i)++;
	}
}


void	add_cmds(t_list **cmds, char **s, int *i)
{
	t_list	*temp;

	temp = malloc(sizeof(t_list));
	if(!temp)
		return(exit_void_error());
	temp->next = NULL;
	temp->previous = NULL;
	temp->type = 'E';
	temp->cmds = 0;
	if((*cmds))
	{
		(*cmds)->next = temp;
		temp->previous = (*cmds);
	}
	(*cmds) = temp;
	return (add_arg((*cmds), s, i));
}
/*
void	parse_arg(t_list **cmds, char	**s)
{
	int		b;

	b = (strcmp(";", s) == 0);
	if(b && !(*cmds))
		return ;
	else if(!b && ((!(*cmds) || ((*cmds)->type != 'E'))))
		return(add_cmds(cmds, s));
	else if(strcmp("|", s) == 0)
		(*cmds)->type = '|';
	else if(b)
		(*cmds)->type = ';';
	else
		return(add_arg(*cmds, s));
}*/

// COMMENT AMELIORER TOUT CA (CHECKER LA LONGUEUR DE CHAQUE COMMAND *cmds et avoir 
// parametre (*word))

int	exec_cmds(t_list **cmds, char	**env)
{

}

void	parse_arg(t_list	**cmds, char **s)
{
	int	i;

	i = 1;
	while(s && s[i])
	{
		if(!(strcmp(";", s[i])) && !(*cmds))
			i++;
		else if(strcmp(";", s[i]) && strcmp("|", s[i]))
			add_cmds(cmds, s, &i);
		else if(!(strcmp("|", s[i])) && (++i))
			(*cmds)->type = '|';
		else if(!(strcmp(";", s[i])) && (++i))
			(*cmds)->type = ';';
	}

}

int	main(int argc, char **argv, char **envp)
{
	t_list	*cmds;
	int		i;
	int		ret;

	ret = 0;
	i = 1;
	cmds= NULL;
	parse_arg(&(cmds), argv);
	if (cmds)
		ret = exec_cmds(&cmds, envp);
	clear_list(&(cmds));
	return (ret);
}
