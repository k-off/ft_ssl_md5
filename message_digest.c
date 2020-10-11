/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_digest.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:32:22 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 14:27:43 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

/**
@brief Iterate through file names provided by user and execute selected fun
@param index number of function in the function array
@param argument_counter arguments count
@param argument_vector arguments array
@param flags input and output settings
@return zero on success, error number otherwise
*/
static int		iterate_file_names(int index, int argument_counter, char **argument_vector, int flags)
{
	int				file_descriptor;
	static char		*function_name[] = {"MD5", "SH1", "SH224", "SHA256", "SH384", "SH512"};
	static int		output_size[] = {128, 160, 224, 256, 384, 512};
	static t_data	*(*function[])(int flag, int file_descriptor, char *string) = {md5, sha1, sha224, sha256, sha384, sha512};

	file_descriptor = -2;
	while (argument_counter > 0)
	{
		file_descriptor = open(argument_vector[0], O_RDONLY);
		if (file_descriptor < 0)
			print_error(function_name[index], argument_vector[0], NULL, NO_FILE);
		else if (read(file_descriptor, "", 0) < 0)
			print_error(function_name[index], argument_vector[0], NULL, NO_PERMISSION);
		else
			print_md(flags, function_name[index], function[index](0, file_descriptor, argument_vector[0]), output_size[index]);
		close(file_descriptor);
		argument_counter--;
		argument_vector++;
	}
	return (0);
}

int				message_digest(int index, int argument_counter, char **argument_vector, int flags)
{
	static char		*function_name[] = {"MD5", "SHA1", "SHA224", "SHA256", "SHA384", "SHA512"};
	static int		output_size[] = {128, 160, 224, 256, 384, 512};
	static t_data	*(*function[])(int flag, int file_descriptor, char *string) = {md5, sha1, sha224, sha256, sha384, sha512};

	if (argument_counter < 1)
		return (print_md(flags, function_name[index], function[index](0, 0, NULL), output_size[index]));
	while (argument_counter > 0 && argument_vector[0][0] == '-')
	{
		if (set_flags_md(&flags, argument_vector[0], index) == ILLEGAL)
			return (print_error(function_name[index], NULL, argument_vector[0], ILLEGAL));
		if (flags & 1 && argument_counter < 2)
			return (print_error(function_name[index], NULL, NULL, EMPTY_S));
		else if (flags & 1 && argument_counter > 1)
		{
			flags &= (31 << 1);
			argument_counter--;
			if (argument_vector)
				argument_vector++;
			print_md(flags, function_name[index], function[index](0, -2, argument_vector[0]), output_size[index]);
		}
		flags &= (31 << 1);
		argument_counter--;
		if (argument_vector)
			argument_vector++;
	}
	return (!(argument_counter == 0 && (flags & 14) && (flags - 24 != 0)) ?
			iterate_file_names(index, argument_counter, argument_vector, flags) :
			print_md(flags, function_name[index], function[index](0, 0, NULL), output_size[index]));
}
