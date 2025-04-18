/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:34:09 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 22:12:11 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	print_error(const char *msg, const char *key, const char *value)
{
	size_t	len;

	len = 0;
	while (msg[len] != ':')
		len++;
	len += 2;
	if (key && value)
	{
		write(2, msg, len);
		write(2, "`", 1);
		write(2, (char *)key, ft_strlen(key));
		write(2, "=", 1);
		write(2, (char *)value, ft_strlen(value));
		write(2, "': ", 3);
		while (msg[len])
			write(2, &msg[len++], 1);
	}
	else
		write(2, msg, len);
	write(2, "\n", 1);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*original_dest;

	original_dest = dest;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*ft_strtok(char *str, const char *delim)
{
	char		*start;
	char		*end;
	static char	*next_token;

	if (str != NULL)
		next_token = str;
	if (next_token == NULL)
		return (NULL);
	start = next_token;
	while (*start && ft_strchr(delim, *start))
		start++;
	if (*start == '\0')
		return (next_token = NULL, NULL);
	end = start;
	while (*end && !ft_strchr(delim, *end))
		end++;
	if (*end)
	{
		*end = '\0';
		next_token = end + 1;
	}
	else
		next_token = NULL;
	return (start);
}
