/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:25:24 by lumugot           #+#    #+#             */
/*   Updated: 2025/04/11 01:07:14 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk_bonus.h"

int	g_wait_conf = 0;

static void	handle_confirm(int signal)
{
	if (signal == SIGUSR1)
		g_wait_conf = 1;
	else
		ft_putstr_fd("Server received your message ☺️", 1);
}

static void	send_signal(int pid, char character)
{
	int	bit_index;

	bit_index = 7;
	while (bit_index >= 0)
	{
		if ((character >> bit_index) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (g_wait_conf == 0)
			;
		g_wait_conf = 0;
		bit_index--;
	}
}

int	check_arg(int argc, char **argv)
{
	if (argc != 3 || !ft_strlen(argv[2]))
	{
		ft_putstr_fd("Arguments or Message Invalid !", 2);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int					pid;
	int					message;
	struct sigaction	sa;

	if (argc == 1)
	{
		ft_putstr_fd("Write message and PID !\n", 2);
		return (0);
	}
	check_arg(argc, argv);
	pid = ft_atoi(argv[1]);
	message = -1;
	if (pid <= 0 || kill(pid, 0))
	{
		ft_putstr_fd("PID Invalid !", 2);
		return (1);
	}
	sa.sa_handler = handle_confirm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (argv[2][++message])
		send_signal(pid, argv[2][message]);
	send_signal(pid, '\0');
}
