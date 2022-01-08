#include "list.h"
#include "minirt.h"

static t_list	*ft_lstnew(void)
{
	t_list	*new;

	new = memalloc(sizeof(t_list));
	new->data = NULL;
	new->next = NULL;
	return (new);
}

t_list	*ft_lstaddback(t_list **lst)
{
	t_list	*curr;

	curr = *lst;
	if (curr)
	{
		*lst = ft_lstnew();
		curr = *lst;
	}
	else
	{
		while (curr->next)
			curr = curr->next;
		curr->next = ft_lstnew();
		curr = curr->next;
	}
	return (curr);
}

