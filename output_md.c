/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   output_md.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/25 21:34:08 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 14:41:01 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

int			print_message(t_message *messages_head)
{
	t_message *chunk;

	chunk = messages_head;
	while (chunk)
	{
		write(1, chunk->string, chunk->len);
		chunk = chunk->next;
	}
	return (chunk == NULL);
}

/**
@brief convert digest message into a string
@param hash digest message
@param size size of digest message in bits
@return string to be printed
*/
static char		*hex(uint32_t *hash, int size)
{
	static char	final_output[128];
	static char *hex_charset = "0123456789abcdef";
	int			bytes;
	int			i;
	uint64_t	*hash64;

	ft_memset(final_output, 0, 128);
	bytes = size > 256 ? 16 : 8;
	size /= size > 256 ? 64 : 32;
	hash64 = (uint64_t*)hash;
	while (size)
	{
		i = bytes;
		size--;
		while (i > 0)
		{
			i--;
			final_output[size * bytes + i] =
				hex_charset[bytes == 8 ? hash[size] % 16 : hash64[size] % 16];
			bytes == 8 ? (hash[size] /= 16) : (hash64[size] /= 16);
		}
	}
	return (final_output);
}

/**
@brief Print if -r flag is not set
@param fun_name function name
@param data all data
@param hex_s output
*/
static void	print_forward(char *fun_name, t_data *data, char *hex_s)
{
	if (data->is_file)
	{
		write(1, fun_name, ft_strlen(fun_name));
		write(1, " (", 2);
		write(1, data->filename, ft_strlen(data->filename));
		write(1, ") = ", 4);
		write(1, hex_s, ft_strlen(hex_s));
	}
	else if (data->is_string)
	{
		write(1, fun_name, ft_strlen(fun_name));
		write(1, " (\"", 3);
		write(1, data->string, ft_strlen(data->string));
		write(1, "\") = ", 5);
		write(1, hex_s, ft_strlen(hex_s));
	}
	else
		write(1, hex_s, ft_strlen(hex_s));
}

/**
@brief Print if -r flag is set
@param data all data
@param hex_s output
*/
static void	print_reverse(t_data *data, char *hex_s)
{
	if (data->is_file)
	{
		write(1, hex_s, ft_strlen(hex_s));
		write(1, " ", 1);
		write(1, data->filename, ft_strlen(data->filename));
	}
	else if (data->is_string)
	{
		write(1, hex_s, ft_strlen(hex_s));
		write(1, " \"", 2);
		write(1, data->string, ft_strlen(data->string));
		write(1, "\"", 1);
	}
	else
		write(1, hex_s, ft_strlen(hex_s));
}

int			print_md(int flags, char *fun_name, t_data *data, int size)
{
	char	*hex_s;

	hex_s = hex((uint32_t*)data->hash, size);
	if (flags & 4)
		write(1, hex_s, ft_strlen(hex_s));
	else if ((flags & 2) == 0)
		print_forward(fun_name, data, hex_s);
	else
		print_reverse(data, hex_s);
	write(1, "\n", 1);
	return (0);
}
