/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_messaging.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/25 16:41:22 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:56:10 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "error_messaging.h"
#include <stdio.h>

/**
@brief Store and return a static array of error messages
@param error_code error code
@return error standard_message string
*/
static char	*error_message(int error_code)
{
	static char *standard_message[] = { "", "",
		"File couldn't be open", "File couldn't be read", "Is a directory",
		"illegal option", "Option requires an argument",
		"This function is not yet implemented",
		"Compression failed to complete", "Main loop failed to complete"
	};
	return (standard_message[(error_code < 0 ? -error_code : error_code) % 10]);
}

int			print_strings(int file_descriptor, char **arr)
{
	if (!arr)
		return (1);
	while (arr)
	{
		write(file_descriptor, arr[0], ft_strlen(arr[0]));
		arr++;
	}
	return (0);
}

int			print_error(char *function_name, char *file_name, char *custom_message, int error_code)
{
	static char *ssl_prefix = "ft_ssl:Error: ";
	static char *column = ": ";
	static char *unsupported = "Unsupported error\n";
	char		*standard_message;

	write(2, ssl_prefix, ft_strlen(ssl_prefix));
	if (error_code < 2)
		return ((int)write(2, unsupported, ft_strlen(unsupported)) * error_code);
	function_name ? write(2, function_name, ft_strlen(function_name)) : 0;
	function_name ? write(2, column, ft_strlen(column)) : 0;
	file_name ? write(2, file_name, ft_strlen(file_name)) : 0;
	file_name ? write(2, column, ft_strlen(column)) : 0;
	custom_message ? write(2, custom_message, ft_strlen(custom_message)) : 0;
	custom_message ? write(2, column, ft_strlen(column)) : 0;
	standard_message = error_message(error_code);
	write(2, standard_message, ft_strlen(standard_message));
	write(2, "\n", 1);
	return (error_code);
}
