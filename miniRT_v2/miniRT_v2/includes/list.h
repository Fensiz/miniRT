#ifndef LIST_H
#define LIST_H
typedef struct		s_list
{
	void			*data;
	struct s_list	*next;
}					t_list;

t_list	*ft_lstaddback(t_list **lst);

#endif
