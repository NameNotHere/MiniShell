/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:01:43 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 11:48:51 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc == 2 && (ft_strncmp(argv[1], "-i", 2) == 0))
		return (run_pipex_interactive(argv[0], envp));
	else
		return (run_pipex_once(argc, argv, envp));
}
