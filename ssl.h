/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ssl.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 12:34:20 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:36:39 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SSL_H
# define __SSL_H

# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "error_messaging.h"

/**
@brief Dispatcher function for hash functions
@param function_index number of function in the function array
@param argument_counter arguments count
@param argument_vector arguments array
@return zero on success, error number otherwise
*/
int				message_digest(int function_index, int argument_counter, char **argument_vector, int flags);

/**
@brief Iterate through arguments and store selected options
@param flags pointer to store selected options
@param argument_vector argument vector
@param function_name function name
@param function pointer
@return zero on success
*/
int				set_flags_md(int *flags, char *argument_vector, char *function_name, int *(*function)(int, char*));

/**
@brief Print result of the hashing function
@param flags selected options
@param function_name function name
@param output result to be printed
@param size_of_output in bytes
@return zero on success
*/
int				print_result_md(int flags, char *function_name, int *output, int size_of_output);

#endif
