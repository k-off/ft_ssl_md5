/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   set_flags.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/08 12:46:10 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:15:46 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

/**
@brief Set flags
@param flags pointer to flags variable
@param argument_vector address of the pointer to current argument
*/
static void		set_flags_md2(int *flags, char **argument_vector)
{
	argument_vector[0]++;
	while (argument_vector[0][0] == 'p' || argument_vector[0][0] == 'q'
		|| argument_vector[0][0] == 'r' || argument_vector[0][0] == 's')
	{
		*flags |= ((argument_vector[0][0] == 'p') << 3);
		*flags |= ((argument_vector[0][0] == 'q') << 2);
		*flags |= ((argument_vector[0][0] == 'r') << 1);
		*flags |= ((argument_vector[0][0] == 's') << 0);
		argument_vector[0]++;
		if (*flags & 1)
			break ;
	}
}

int				set_flags_md(int *flags, char *argument_vector, int index)
{
	static char		*function_name[] = {"MD5", "SH1", "SH224", "SHA256", "SH384", "SH512"};
	static int		output_size[] = {128, 160, 224, 256, 384, 512};
	static t_data	*(*function[])(int flag, int file_descriptor, char *string) = {md5, sha1, sha224, sha256, sha384, sha512};

	set_flags_md2(flags, &argument_vector);
	if (argument_vector[0] && (*flags & 1) == 0)
		return (ILLEGAL);
	if ((*flags & 8) && !(*flags & 16))
		print_md(*flags, function_name[index], function[index](1, 0, NULL), output_size[index]);
	if (*flags & 8)
		*flags |= 16;
	if (argument_vector[0] && *flags & 1)
	{
		(*flags) &= (31 << 1);
		print_md(*flags, function_name[index], function[index](0, -1, argument_vector), output_size[index]);
	}
	return (0);
}
