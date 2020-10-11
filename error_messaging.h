/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_messaging.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/25 16:41:31 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 13:36:37 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERROR_MESSAGING_H
# define __ERROR_MESSAGING_H

# include <unistd.h>
# include "utils.h"

# define NO_FILE 2
# define NO_PERMISSION 3
# define IS_DIRECTORY 4
# define ILLEGAL 5
# define EMPTY_S 6
# define NO_SUPPORT 7
# define COMPRESSION_ERROR 8
# define MAIN_LOOP_ERROR 9

/**
@brief Print custom error message
@param function_name function name
@param file_name file name
@param custom_message custom message
@param error_code error code
** return error code
*/
int				print_error(char *function_name, char *file_name, char *custom_message, int error_code);

/**
@brief Print array of strings
@param file_descriptor output file descriptor
@param arr array of strings
@return zero on success
*/
int				print_strings(int fd, char **arr);

#endif
