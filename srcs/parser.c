/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:44:21 by tkafanov          #+#    #+#             */
/*   Updated: 2024/07/30 12:44:19 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



typedef struct s_node
{
	t_tokens		*token;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

t_node *create_node(t_tokens *token)
{
	t_node	*new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node)
	{
		new_node->token = token;
		new_node->left = NULL;
		new_node->right = NULL;
	}
	return new_node;
}



t_node *build_tree(t_tokens *tokens)
{
	if (tokens == NULL)
		return NULL;

	t_node		*root = create_node(tokens);
	t_tokens	*current_token = tokens->next;

	if (current_token != NULL)
	{
		if (current_token->type == T_PIPE)
		{
			root->left = create_node(current_token);
			root->right = NULL;
		}
		else if (current_token->type == T_D_QUOTE || current_token->type == T_S_QUOTE || current_token->type == T_WORD || current_token->type == T_OPTION)
		{
			root->left = create_node(current_token);
			root->right = build_tree(current_token->next);
		}
	}

	return root;
}

void parse_tokens(t_tokens *tokens)
{
	t_node	*root = build_tree(tokens);
}
