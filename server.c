/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:20:23 by lumugot           #+#    #+#             */
/*   Updated: 2025/01/06 15:47:13 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static char	*join_char(char *str, char character)
{
	int		index;
	int		size;
	char	*f_str;

	if (!str)
		size = 0;
	else
		size = ft_strlen(str);
	f_str = malloc(sizeof(char) * (size + 2));
	if (!f_str)
		return (NULL);
	index = 0;
	while (index < size)
	{
		f_str[index] = str[index];
		index++;
	}
	f_str[index] = character;
	f_str[index + 1] = '\0';
	free(str);
	return (f_str);
}

static void	handle_signal(int signal, siginfo_t *info, void *context)
{
	static int	bit_index = 7;
	static char	character = 0;
	static char	*message = NULL;

	(void)context;
	if (signal == SIGUSR1)
		character |= (1 << bit_index);
	bit_index--;
	if (bit_index < 0)
	{
		if (character == '\0')
		{
			ft_putstr_fd(message, 1);
			ft_putchar_fd('\n', 1);
			kill(info->si_pid, SIGUSR2);
			free(message);
			message = NULL;
		}
		else
			message = join_char(message, character);
		bit_index = 7;
		character = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	ft_putstr_fd("Server is ready, PiD:  ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putchar_fd('\n', 1);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
