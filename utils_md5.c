/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_md5.chunk                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:32:52 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:39:46 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

void		set_vars64(uint64_t *destination, uint64_t *source, int amount, int add)
{
	if (add)
		while (amount)
		{
			amount--;
			destination[amount] += source[amount];
		}
	else
		while (amount)
		{
			amount--;
			destination[amount] = source[amount];
		}
}

void		set_vars32(unsigned *destination, unsigned *source, int amount, int add)
{
	if (add)
		while (amount)
		{
			amount--;
			destination[amount] += source[amount];
		}
	else
		while (amount)
		{
			amount--;
			destination[amount] = source[amount];
		}
}

void		endian_rev32(unsigned *chunk, int amount)
{
	unsigned	*current_word;
	unsigned	num;
	int			i;

	while (amount)
	{
		amount--;
		current_word = &chunk[amount];
		num = 0;
		i = 0;
		num += *current_word & 0xff;
		num = num << 8;
		while (++i < 4)
		{
			num += *current_word >> (i * 8) & 0xff;
			if ((i * 8) == 24)
				break ;
			num = num << 8;
		}
		*current_word = num;
	}
}

void		change_endian32(unsigned *chunk, int is_last)
{
	int		i;

	i = 0;
	while (i < (is_last ? 14 : 16))
	{
		chunk[i] = (((chunk[i] & 0x000000FF) << 24) | ((chunk[i] & 0x0000FF00) << 8) |
				((chunk[i] & 0x00FF0000) >> 8) | ((chunk[i] & 0xFF000000) >> 24));
		i++;
	}
	if (is_last && !chunk[15])
	{
		chunk[15] = chunk[14];
		chunk[14] = 0;
	}
}

void		change_endian64(uint64_t *chunk, int is_last)
{
	int		i;

	i = 0;
	while (i < (is_last ? 14 : 16))
	{
		chunk[i] = ((((chunk[i]) & 0xff00000000000000ull) >> 56) |
				(((chunk[i]) & 0x00ff000000000000ull) >> 40) |
				(((chunk[i]) & 0x0000ff0000000000ull) >> 24) |
				(((chunk[i]) & 0x000000ff00000000ull) >> 8) |
				(((chunk[i]) & 0x00000000ff000000ull) << 8) |
				(((chunk[i]) & 0x0000000000ff0000ull) << 24) |
				(((chunk[i]) & 0x000000000000ff00ull) << 40) |
				(((chunk[i]) & 0x00000000000000ffull) << 56));
		i++;
	}
	if (is_last && !chunk[15])
	{
		chunk[15] = chunk[14];
		chunk[14] = 0;
	}
}
