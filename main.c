/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 12:26:15 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 13:51:56 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ssl.h"

size_t			ft_strlen(const char *string)
{
	size_t	length;

	length = 0;
	if (!string)
		return (length);
	while (string[length] != '\0')
		length++;
	return (length);
}

/**
@brief Dispatcher function for ft_ssl projects
@param argument_counter amount of command arguments
@param argument_vector list of command arguments
@return zero on success, error number otherwise
*/
static int		run_command(int argument_counter, char **argument_vector)
{
	static char	*command_names[] = {"md5", "sha1", "sha224", "sha256", "sha384", "sha512"};
	static int	(*functions[])(int fun_index, int argument_counter, char **argument_vector, int f) = {message_digest};
	int			length;

	length = 0;
	while (length < 6)
	{
		if (!ft_strcmp(command_names[length], argument_vector[0]))
			return (functions[0](length, argument_counter - 1, &argument_vector[1], 0));
		length++;
	}
	return (1);
}

/**
@brief Print message if an unsupported command provided in arguments
@param command string
@return error number
*/
static int		print_wrong_command(char *command)
{
	write(2, "ft_ssl:Error: '", 15);
	if (command)
		write(2, command, ft_strlen(command));
	write(2, "' is an invalid command\n", 24);
	write(2, "\nStandard commands:\n", 20);
	write(2, "\nMessage Digest commands:\nsha1\t\tsha256\t\tsha512\n", 47);
	write(2, "md5\t\tsha224\t\tsha384\n\nCypher commands:\n\n", 39);
	return (1);
}

int				main(int argument_counter, char **argument_vector)
{
	int			errno;

	if (argument_counter < 2)
	{
		errno = 1;
		write(2, "usage: ft_ssl command [command opts] [command args]\n", 52);
		exit(errno);
	}
	errno = run_command(argument_counter - 1, &argument_vector[1]);
	if (errno == 1)
		exit(print_wrong_command(argument_vector[1]));
	exit(errno);
}
