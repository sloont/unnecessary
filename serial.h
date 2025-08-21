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
	UNNECESSARY_T_SERIAL_STRUCTURE *serial = malloc(sizeof(struct UNNECESSARY_SERIAL_STRUCTURE));
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
			fwrite(ch_structure->name, strlen(ch_structure->name), 1, file);
			fwrite(" = ", 3, 1, file);
			size_t n_properties = UNNECESSARY_SERIAL_STRUCTURE_propertyCount(ch_structure->structure);
			for (size_t j = 0; j < n_properties; j++)
			{
				const char *string = UNNECESSARY_SERIAL_getString(ch_structure->structure, j);
				if (NULL == strchr(string, ','))
				{
					fwrite("\"", 1, 1, file);
					fwrite(string, strlen(string), 1, file);
					fwrite("\"", 1, 1, file);
				}
				else
				{
					fwrite(string, strlen(string), 1, file);
				}
				if (n_properties-- > 1)
				{
					fwrite(",", 1, 1, file);
				}
			}
			fwrite("\n", 1, 1, file);
		}
		else
		{
			fwrite("\n", 1, 1, file);
			fwrite("\n{", 2, 1, file);
			fwrite(ch_structure->name, strlen(ch_structure->name), 1, file);
			UNNECESSARY_SERIAL_FILE_write_recursive(ch_structure->structure, file);
			fwrite("}\n\n", 3, 1, file);
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
		perror("failed");
	};
	UNNECESSARY_SERIAL_FILE_write_recursive(structure, file);
	fclose(file);
	return 0;
}

#endif //UNNECESSARY_SERIAL_H
