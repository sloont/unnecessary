#ifndef UNNECESSARY_SERIAL_H
#define UNNECESSARY_SERIAL_H

#include "unnecessary.h"

typedef struct UNNECESSARY_SERIAL_STRUCTURE
{
	UNNECESSARY_T_DYNAMIC_ARRAY *properties;
	UNNECESSARY_T_DYNAMIC_ARRAY *children;
	UNNECESSARY_T_HASH_TABLE *map;
} UNNECESSARY_T_SERIAL_STRUCTURE;

typedef struct UNNECESSARY_SERIAL_STRUCTURE_LINK
{
	const char *name;
	UNNECESSARY_T_SERIAL_STRUCTURE *structure;
} UNNECESSARY_T_SERIAL_LINK;

static UNNECESSARY_T_SERIAL_STRUCTURE *UNNECESSARY_SERIAL_STRUCTURE_create()
{
	UNNECESSARY_T_SERIAL_STRUCTURE *serial = malloc(sizeof(*serial));
	serial->properties = UNNECESSARY_DYNAMIC_ARRAY_create();
	serial->children = UNNECESSARY_DYNAMIC_ARRAY_create();
	serial->map = UNNECESSARY_HASH_TABLE_create();
	return serial;
}

static size_t UNNECESSARY_SERIAL_STRUCTURE_propertyCount(UNNECESSARY_T_SERIAL_STRUCTURE *structure)
{
	return UNNECESSARY_DYNAMIC_ARRAY_size(&structure->properties);
}

static int UNNECESSARY_SERIAL_STRUCTURE_hasProperty(UNNECESSARY_T_SERIAL_STRUCTURE*structure, const char *name)
{
	return UNNECESSARY_HASH_TABLE_get(&structure->map, name) ? 1 : 0;
}

static void UNNECESSARY_SERIAL_setString(UNNECESSARY_T_SERIAL_STRUCTURE *structure, const char *str, const size_t index)
{
	UNNECESSARY_DYNAMIC_ARRAY_set(&structure->properties, index, &str);
}

static const char *UNNECESSARY_SERIAL_getString(UNNECESSARY_T_SERIAL_STRUCTURE *structure, const size_t index)
{
	if (index >= UNNECESSARY_DYNAMIC_ARRAY_size(&structure->properties))
		return "";

	return UNNECESSARY_DYNAMIC_ARRAY_get(&structure->properties, index);
}

static void UNNECESSARY_SERIAL_setReal(UNNECESSARY_T_SERIAL_STRUCTURE *structure, const double n, const size_t index)
{
	const size_t bytes = snprintf(NULL, 0, "%.2f", n);

	if (bytes <= 0)
	{
		printf("UNNECESSARY_SERIAL_setReal: bytes < 0");
		return;
	}
	char buf[64];

	sprintf_s(buf, bytes, "%.2f", n);

	UNNECESSARY_SERIAL_setString(structure, buf, index);
}

static double UNNECESSARY_SERIAL_getReal(UNNECESSARY_T_SERIAL_STRUCTURE *structure, const size_t index)
{
	return atof(UNNECESSARY_SERIAL_getString(structure, index));
}

static void UNNECESSARY_SERIAL_setInt(UNNECESSARY_T_SERIAL_STRUCTURE *structure, const int n, const size_t index)
{
	char buf[sizeof(int) + 1];
	_itoa_s(n, buf, sizeof(int), 10);

	UNNECESSARY_SERIAL_setString(structure, buf, index);
}

static int UNNECESSARY_SERIAL_getInt(UNNECESSARY_T_SERIAL_STRUCTURE *structure, const size_t index)
{
	return atoi(UNNECESSARY_SERIAL_getString(structure, index));
}

static void UNNECESSARY_SERIAL_FILE_write_recursive(
	UNNECESSARY_T_SERIAL_STRUCTURE *structure,
	FILE *file
)
{
	for (size_t i = 0; i < UNNECESSARY_DYNAMIC_ARRAY_size(&structure->children); i++)
	{
		const UNNECESSARY_T_SERIAL_LINK *ch_structure = UNNECESSARY_DYNAMIC_ARRAY_get(&structure->children, i);
		if (0 == UNNECESSARY_DYNAMIC_ARRAY_size(&ch_structure->structure->children))
		{
			fprintf_s(file, ch_structure->name);
			fprintf_s(file, " = ");
			size_t n_properties = UNNECESSARY_SERIAL_STRUCTURE_propertyCount(ch_structure->structure);
			for (size_t j = 0; j < n_properties; j++)
			{
				const char *string = UNNECESSARY_SERIAL_getString(ch_structure->structure, j);
				if (NULL == strchr(string, ','))
				{
					fprintf_s(file, "\"%s\"", string);
				}
				else
				{
					fprintf_s(file, "%s%s", string, n_properties-- > 1 ? ", " : "");
				}
			}
			fprintf_s(file, "\n");
		}
		else
		{
			fprintf_s(file, "\n\n{%s", ch_structure->name);
			UNNECESSARY_SERIAL_FILE_write_recursive(ch_structure->structure, file);
			fprintf_s(file, "}\n\n");
		}
	}
}

static int UNNECESSARY_SERIAL_FILE_write(
	UNNECESSARY_T_SERIAL_STRUCTURE *structure,
	const char *filename
)
{
	FILE *file;
	const errno_t err = fopen_s(&file, filename, "w+");
	if (err != 0)
	{
		perror("write failed");
	};
	UNNECESSARY_SERIAL_FILE_write_recursive(structure, file);
	fclose(file);
	return 0;
}

static int UNNECESSARY_SERIAL_FILE_read(
	UNNECESSARY_T_SERIAL_STRUCTURE **structure,
	const char *filename
)
{
	FILE *file;
	errno_t err = fopen_s(&file, filename, "r");
	if (0 != err)
	{
		perror("read failed");
		return 1;
	}
	char *g_key = "";
	char g_value[64] = "";
	char buf[64];
	UNNECESSARY_T_LINKED_NODE *stack = UNNECESSARY_STACK_create();
	UNNECESSARY_STACK_push(&stack, *structure);
	while (fgets(buf, 64, file))
	{
		const char *found = strchr(buf, '=');
		if (NULL == found)
		{
			if ('{' == buf[0])
			{
				UNNECESSARY_T_SERIAL_STRUCTURE *top = UNNECESSARY_STACK_peek(&stack);
				if (UNNECESSARY_SERIAL_STRUCTURE_hasProperty(*structure, g_key))
				{
					UNNECESSARY_HASH_TABLE_set(&top->map, g_key, (void *)UNNECESSARY_DYNAMIC_ARRAY_size(&top->children));
					UNNECESSARY_T_SERIAL_LINK *link = malloc(sizeof(*link));
					if (NULL == link)
					{
						perror("new link malloc failed");
						return 7;
					}
					link->name = g_key;
					link->structure = UNNECESSARY_SERIAL_STRUCTURE_create();
					UNNECESSARY_DYNAMIC_ARRAY_add(&(*structure)->children, &link);
					UNNECESSARY_T_SERIAL_LINK *child = UNNECESSARY_DYNAMIC_ARRAY_get(&(*structure)->children, (int)UNNECESSARY_HASH_TABLE_get(&(*structure)->map, g_key));
					if (child == NULL)
					{
						return 4;
					}
					UNNECESSARY_STACK_push(&stack, child);
				}
			}
			else if ('}' == buf[0])
			{
				free(UNNECESSARY_STACK_pop(&stack));
			}
			else
			{
				err = strcpy_s(g_key, strlen(buf), buf);
				if (0 != err)
				{
					perror("g_key strcpy failed");
					return 5;
				}
			}
		}
		else
		{
			const ptrdiff_t x = found - buf;
			g_key = malloc(x);				// cut the '=' out
			const void *e_ptr = memcpy(g_key, found + 1, x);
			g_key[x] = '\0';
			if (NULL == e_ptr)
			{
				perror("key memcpy failed");
				return 2;
			}
			const char * value = _strdup(++found);
			if (NULL == value)
			{
				perror("value _strdup failed");
				return 3;
			}
			size_t quote = 0;
			size_t n_tokens = 0;
			size_t l_token = 0;
			char token[64];
			for (size_t i = 0; i < strlen(value); i++)
			{
				const char c = value[i];
				if (c == '\"')
				{
					quote ? quote = 0 : quote = 1;
				}
				else
				{
					if (quote)
					{
						token[l_token++] = c;
					}
					else
					{
						if (',' == c)
						{
							if (UNNECESSARY_SERIAL_STRUCTURE_hasProperty(*structure, g_key))
							{
								UNNECESSARY_T_SERIAL_STRUCTURE *top = UNNECESSARY_STACK_peek(&stack);
								UNNECESSARY_HASH_TABLE_set(&top->map, g_key, (void *)UNNECESSARY_DYNAMIC_ARRAY_size(&top->children));
								UNNECESSARY_T_SERIAL_LINK *link = malloc(sizeof(*link));
								link->name = g_key;
								link->structure = UNNECESSARY_SERIAL_STRUCTURE_create();
								UNNECESSARY_DYNAMIC_ARRAY_add(&(*structure)->children, link);
								const UNNECESSARY_T_SERIAL_LINK *child = UNNECESSARY_DYNAMIC_ARRAY_get(&(*structure)->children, (int)UNNECESSARY_HASH_TABLE_get(&(*structure)->map, g_key));
								if (child == NULL)
								{
									return 6;
								}
								token[l_token] = '\0';
								UNNECESSARY_SERIAL_setString(child->structure, token, n_tokens);
								token[0] = '\0';
								l_token = 0;
								n_tokens++;
							}
						}
						else
						{
							token[l_token++] = c;
						}
					}
				}
			}
			if (0 != l_token)
			{
				if (UNNECESSARY_SERIAL_STRUCTURE_hasProperty(*structure, g_key))
				{
					UNNECESSARY_T_SERIAL_STRUCTURE *top = UNNECESSARY_STACK_peek(&stack);
					UNNECESSARY_HASH_TABLE_set(&top->map, g_key, (void *)UNNECESSARY_DYNAMIC_ARRAY_size(&top->children));
					UNNECESSARY_T_SERIAL_LINK *link = malloc(sizeof(*link));
					link->name = g_key;
					link->structure = UNNECESSARY_SERIAL_STRUCTURE_create();
					UNNECESSARY_DYNAMIC_ARRAY_add(&(*structure)->children, link);
					UNNECESSARY_T_SERIAL_LINK *child = UNNECESSARY_DYNAMIC_ARRAY_get(&(*structure)->children, (int)UNNECESSARY_HASH_TABLE_get(&(*structure)->map, g_key));
					if (child == NULL)
					{
						return 6;
					}
					token[l_token] = '\0';
					UNNECESSARY_SERIAL_setString(child->structure, token, n_tokens);
					token[0] = '\0';
					l_token = 0;
					n_tokens++;
				}
			}
		}
	}
	fclose(file);
	return 0;
}

static int UNNECESSARY_SERIAL_STRUCTURE_free(UNNECESSARY_T_SERIAL_STRUCTURE **structure)
{
	for (int i = 0; i < UNNECESSARY_DYNAMIC_ARRAY_size(&(*structure)->children); i++)
	{	UNNECESSARY_T_SERIAL_LINK *link = UNNECESSARY_DYNAMIC_ARRAY_get(&(*structure)->children, i);
		UNNECESSARY_SERIAL_STRUCTURE_free(&link->structure);
		free(link);
	}
	UNNECESSARY_DYNAMIC_ARRAY_free(&(*structure)->properties);
	UNNECESSARY_HASH_TABLE_free(&(*structure)->map);
	free(*structure);
	return 0;
}

#endif //UNNECESSARY_SERIAL_H
