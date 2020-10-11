/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   md5.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:32:52 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 14:40:30 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

uint32_t g_primes_md5[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t g_shift_bits_md5[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/**
@brief Main compression function for md5 algorithm
@param current_values temporary working array
@param current_chunk current message chunk casted to uint_64
@param cycle number of current cycle
@return zero on success
*/
static int		compress(uint32_t *current_values, uint32_t *current_chunk, int cycle)
{
	uint32_t f;
	uint32_t g;

	g = cycle;
	if (cycle < 16)
		f = (current_values[1] & current_values[2]) | ((~current_values[1]) & current_values[3]);
	else if (cycle < 32)
	{
		f = (current_values[3] & current_values[1]) | ((~current_values[3]) & current_values[2]);
		g = (5 * cycle + 1) % 16;
	}
	else if (cycle < 48)
	{
		f = current_values[1] ^ current_values[2] ^ current_values[3];
		g = (3 * cycle + 5) % 16;
	}
	else
	{
		f = current_values[2] ^ (current_values[1] | (~current_values[3]));
		g = (7 * cycle) % 16;
	}
	f += current_values[0] + g_primes_md5[cycle] + current_chunk[g];
	f = (f << g_shift_bits_md5[cycle]) | (f >> (32 - g_shift_bits_md5[cycle]));
	current_values[0] = current_values[3];
	current_values[3] = current_values[2];
	current_values[2] = current_values[1];
	current_values[1] += f;
	return (0);
}

/**
@brief Main loop of MD5 algorithm
@param hash array of 64-bit hash words
@param head beginning of linked list of message chunks
@return zero on success
*/
static int		main_loop(uint32_t *hash, t_message *head)
{
	uint32_t			current_values[4];
	int					i;

	while (head)
	{
		set_vars32(current_values, hash, 4, !ADD);
		i = 0;
		while (i < 64)
		{
			if (compress(current_values, (uint32_t*)head->string, i))
				print_error("md5", "", NULL, COMPRESSION_ERROR);
			i++;
		}
		set_vars32(hash, current_values, 4, ADD);
		head = head->next;
	}
	return (0);
}

t_data			*md5(int message_flag, int file_descriptor, char *string)
{
	static t_data		data;

	ft_memset(&data, 0, sizeof(data));
	if (file_descriptor >= 0 ? read_fd(&data, file_descriptor) : read_str(&data, string))
		print_error("md5", "", NULL, NO_PERMISSION);
	data.is_file = file_descriptor > 0;
	data.filename = file_descriptor > 0 ? string : NULL;
	data.is_string = file_descriptor < 0;
	data.string = file_descriptor < 0 ? string : NULL;
	set_vars32((uint32_t*)data.hash, (uint32_t[]){0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}, 4, !ADD);
	if (main_loop((uint32_t*)data.hash, data.chunk))
		print_error("md5", "", NULL, MAIN_LOOP_ERROR);
	endian_rev32((uint32_t*)data.hash, 4);
	if (message_flag)
		print_message(data.chunk);
	free_data(data.chunk);
	return (&data);
}
