/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sha512.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:33:05 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:29:28 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

uint64_t g_primes_sha512[] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
	0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
	0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
	0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
	0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
	0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
	0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
	0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
	0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
	0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
	0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
	0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
	0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
	0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
	0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
	0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
	0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
	0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
	0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
	0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
	0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
	0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

/**
@brief Main compression function for 64-bit version of SHA2 algorithm
@param temporary_hash temporary working array
@param message current message chunk casted to uint_64
@param cycle number of current cycle
@return zero on success
*/
static int		compress512(uint64_t *temporary_hash, uint64_t *message, int cycle)
{
	uint64_t	s[2];
	uint64_t	tmp[2];
	uint64_t	ch;
	uint64_t	maj;

	s[1] = ((temporary_hash[4] >> 14) | (temporary_hash[4] << (64 - 14))) ^
			((temporary_hash[4] >> 18) | (temporary_hash[4] << (64 - 18))) ^
			((temporary_hash[4] >> 41) | (temporary_hash[4] << (64 - 41)));
	ch = (temporary_hash[4] & temporary_hash[5]) ^ ((~temporary_hash[4]) & temporary_hash[6]);
	tmp[0] = temporary_hash[7] + s[1] + ch + g_primes_sha512[cycle] + message[cycle];
	s[0] = ((temporary_hash[0] >> 28) | (temporary_hash[0] << (64 - 28))) ^
			((temporary_hash[0] >> 34) | (temporary_hash[0] << (64 - 34))) ^
			((temporary_hash[0] >> 39) | (temporary_hash[0] << (64 - 39)));
	maj = (temporary_hash[0] & temporary_hash[1]) ^ (temporary_hash[0] & temporary_hash[2]) ^ (temporary_hash[1] & temporary_hash[2]);
	tmp[1] = s[0] + maj;
	temporary_hash[7] = temporary_hash[6];
	temporary_hash[6] = temporary_hash[5];
	temporary_hash[5] = temporary_hash[4];
	temporary_hash[4] = temporary_hash[3] + (uint64_t)tmp[0];
	temporary_hash[3] = temporary_hash[2];
	temporary_hash[2] = temporary_hash[1];
	temporary_hash[1] = temporary_hash[0];
	temporary_hash[0] = (uint64_t)(tmp[0] + tmp[1]);
	return (0);
}


/**
@brief Set 128-byte long message from 2x 64-byte chunks for SHA512
@param message message string
@param chunk address of current head of chunks linked list
*/
static void		set_message(uint64_t *message, t_message **chunk)
{
	int		part;

	part = 0;
	ft_memset(message, 0, 128);
	while (part < 2)
	{
		ft_memcpy(&message[part * 8], chunk[0]->string, 64);
		part++;
		if (part < 2 && chunk[0]->next == NULL)
		{
			message[15] = message[7];
			message[7] = 0;
			break ;
		}
		if (part < 2)
			chunk[0] = chunk[0]->next;
	}
}

/**
@brief Set temporary working array of 64-bit words
@param w temporary working array
@param message current message chunk casted to uint_64
@return temporary working array
*/
static uint64_t	*set_work(uint64_t *w, uint64_t *message)
{
	int			cycle;
	uint64_t	s0;
	uint64_t	s1;

	ft_memset(w, 0, sizeof(uint64_t) * 16);
	set_vars64(w, (uint64_t*)message, 16, !ADD);
	cycle = 16;
	while (cycle < 80)
	{
		s0 = ((w[cycle - 15] >> 1) | (w[cycle - 15] << (64 - 1))) ^
			((w[cycle - 15] >> 8) | (w[cycle - 15] << (64 - 8))) ^ (w[cycle - 15] >> 7);
		s1 = ((w[cycle - 2] >> 19) | (w[cycle - 2] << (64 - 19))) ^
			((w[cycle - 2] >> 61) | (w[cycle - 2] << (64 - 61))) ^ (w[cycle - 2] >> 6);
		w[cycle] = w[cycle - 16] + s0 + w[cycle - 7] + s1;
		cycle++;
	}
	return (w);
}

/**
@brief Main loop of 64-bit version of SHA2 algorithm
@param function_name function name
@param hash array of 64-bit hash words
@param head beginning of linked list of message chunks
@return zero on success, error number otherwise
*/

static int		main_loop(char *function_name, uint64_t *hash, t_message *head)
{
	uint64_t		temporary_hash[8];
	uint64_t		work[80];
	int				cycle;
	unsigned char	message[128];

	while (head)
	{
		set_message((uint64_t*)message, &head);
		change_endian64((uint64_t*)message, head->next == NULL);
		set_work(work, (uint64_t*)message);
		set_vars64(temporary_hash, hash, 8, !ADD);
		cycle = 0;
		while (cycle < 80)
		{
			if (compress512(temporary_hash, work, cycle))
				print_error(function_name, "", NULL, COMPRESSION_ERROR);
			cycle++;
		}
		set_vars64(hash, temporary_hash, 8, ADD);
		head = head->next;
	}
	return (0);
}

t_data			*sha512(int message_flag, int file_descr, char *string)
{
	static t_data		data;

	ft_memset(&data, 0, sizeof(data));
	if (file_descr >= 0 ? read_fd(&data, file_descr) : read_str(&data, string))
		print_error("sha512", "", NULL, NO_PERMISSION);
	data.is_file = file_descr > 0;
	data.filename = file_descr > 0 ? string : NULL;
	data.is_string = file_descr < 0;
	data.string = file_descr < 0 ? string : NULL;
	set_vars64(data.hash, (uint64_t[]){0x6a09e667f3bcc908,
		0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
		0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b,
		0x5be0cd19137e2179}, 8, !ADD);
	if (main_loop("sha512", (uint64_t*)data.hash, data.chunk))
		print_error("sha512", "", NULL, MAIN_LOOP_ERROR);
	if (message_flag)
		print_message(data.chunk);
	free_data(data.chunk);
	return (&data);
}

t_data			*sha384(int message_flag, int file_descr, char *string)
{
	static t_data		data;

	ft_memset(&data, 0, sizeof(data));
	if (file_descr >= 0 ? read_fd(&data, file_descr) : read_str(&data, string))
		print_error("sha384", "", NULL, NO_PERMISSION);
	data.is_file = file_descr > 0;
	data.filename = file_descr > 0 ? string : NULL;
	data.is_string = file_descr < 0;
	data.string = file_descr < 0 ? string : NULL;
	set_vars64(data.hash, (uint64_t[]){0xcbbb9d5dc1059ed8,
		0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
		0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7,
		0x47b5481dbefa4fa4}, 8, !ADD);
	if (main_loop("sha384", (uint64_t*)data.hash, data.chunk))
		print_error("sha384", "", NULL, MAIN_LOOP_ERROR);
	if (message_flag)
		print_message(data.chunk);
	free_data(data.chunk);
	return (&data);
}
