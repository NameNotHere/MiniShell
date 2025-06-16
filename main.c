/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/12 16:02:47 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

// everything is a heredoc but properly split in size but not properly displayed
int	main()
{
	int i = 0;
	int	token_count;
	// char	*string = "cat << uuu 99 a EOF | unset cd grep \"pattern\" > ./mom.txt output.txt";
	char	*string = "cat << 99 a EOF | grep \"pattern\" > output.txt ./mimi";
/*	char *string = "(export PATH=\\$PATH:/custom/bin && cd ~/projects && (echo \\\"Building project...\\\" && make all | tee build.log) && grep -i \\\"error\\\" build.log || echo \\\"No errors found\\\" > errors.txt && cat <<EOF > report.txt\n\
Build Report - $(date)\n User: $USER\n\
Hostname: $(hostname)\n EOF\n\
&& cat build.log >> report.txt && sort report.txt | uniq > final_report.txt && ((echo \\\"Report created\\\" && ls -lh final_report.txt) > /dev/null & ) && rm -f temp* && echo \\\"Done ✅\\\")";
*/
	t_token *output = tokenize(string, &token_count);
	while (token_count > i)
	{
		printf("%d %s \n", output[i].ty, output[i].word);
		i++;
	}
	return (0);
}
