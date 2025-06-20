/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteme.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:45:49 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/20 18:50:07 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	free_tokens(t_token *tokens, int token_count)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < token_count)
	{
		if (tokens[i].word)
		{
			free(tokens[i].word);
			tokens[i].word = NULL;
		}
		i++;
	}
	free(tokens);
}


void	free_tokens(t_token *tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i].word != NULL)
	{
		free(tokens[i].word);
		i++;
	}
	free(tokens);
}