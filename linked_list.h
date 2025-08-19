#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct UNNECESSARY_LINKED_NODE
{
	void *thing;
	struct UNNECESSARY_LINKED_NODE *next;
} UNNECESSARY_T_LINKED_NODE;

static UNNECESSARY_T_LINKED_NODE *UNNECESSARY_LINKED_LIST_create()
{
	// we want to guarantee that we have nulled the empty list
	UNNECESSARY_T_LINKED_NODE *list = malloc(sizeof(*list));
	list->thing = NULL;
	list->next = NULL;
	return list;
}

static void UNNECESSARY_LINKED_LIST_push(
	// ** because we are mutating the global
	UNNECESSARY_T_LINKED_NODE **list,
	void *thing
)
{
	// allocate memory for node
	UNNECESSARY_T_LINKED_NODE *node = malloc(sizeof(UNNECESSARY_T_LINKED_NODE));
	// point node to provided thing
	node->thing = thing;
	// the new node will be the tail, so set next to NULL
	node->next = NULL;

	// if the list had no nodes
	if (!*list)
	{
		*list = node;
		return;
	}

	// copy list pointer to function scope for iteration
	UNNECESSARY_T_LINKED_NODE *iter = *list;
	// point to last node in the list
	while (iter->next != NULL)
		iter = iter->next;

	// the global list pointer now points at new node
	iter->next = node;
}

static void UNNECESSARY_LINKED_LIST_free(UNNECESSARY_T_LINKED_NODE **list)
{
	if (!*list)
	{
		return;
	}
	UNNECESSARY_T_LINKED_NODE *node = *list;
	if (!node->next)
	{
		free(*list);
		return;
	}
	while (node->next)
	{
		(*list) = node->next;
		free(node);
		node = (*list);
	}
	free(*list);


}

static void *UNNECESSARY_LINKED_LIST_pop(UNNECESSARY_T_LINKED_NODE **list)
{
	// if the list points to NULL we should error
	if (!*list)
		// return *list;
		return "";

	void* thing;
	// function scope pointer to list
	UNNECESSARY_T_LINKED_NODE *iter = *list;

	// if this is the last node we free it without iteration
	if ((*list)->next == NULL)
	{
		thing = (*list)->thing;
		*list = NULL;
		free(iter);
		return thing;
	}

	// find second to last node
	while (iter->next->next != NULL)
		iter = iter->next;

	// write thing for return
	thing = iter->next->thing;
	// free popped node
	free(iter->next);
	// point the new tail's next to null
	iter->next = NULL;

	return thing;
}
#endif //LINKED_LIST_H
