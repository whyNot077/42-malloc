/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkim3 <minkim3@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 15:47:28 by minkim3           #+#    #+#             */
/*   Updated: 2022/12/04 21:51:27 by minkim3          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr(char const *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
}

void	ft_putstr_fd(int fd, char const *s)
{
	if (fd < 0 || !s)
		return ;
	int i;

	i = 0;
	while (s[i])
		i++;
	write(fd, s, i);
}
