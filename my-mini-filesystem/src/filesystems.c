
// Author:
// Date:
// Description:

#include "filesystems.h"

// HELPER FUNCTIONS

// Given an inode ID, return a string of the format the file
// permissions in the format of rwxrwxrwx
char *format_permissions(int inode_id)
{
	uint16_t octal_num = inodes[inode_id].mode;
	char perms[] = "rwx";	  // permission characters
	static char perm_str[10]; // permission string buffer

	for (int i = 0; i < 9; i++)
	{
		perm_str[i] = ((octal_num >> (8 - i)) & 1) ? perms[i % 3] : '-';
	}
	perm_str[9] = '\0'; // null terminate the string

	return perm_str;
}

// Returns the ID of the next available inode ID and
// marks it as "in-use" on the bit vector
int get_inode()
{
	int bit_index = -1;
	uint64_t mask = 1;
	for (int i = 0; i < 64; i++)
	{
		if ((bitvector & mask) == 0)
		{
			bitvector |= mask;
			bit_index = i;
			break;
		}
		mask <<= 1;
	}
	return bit_index;
}

// Given an inode ID, free its data blocks and
// mark it as free in the bit vector
void release_inode(int inode_id)
{
	uint64_t mask = 1 << inode_id;
	bitvector &= ~mask;
	free(inodes[inode_id].block_ptr);
}

// Given an inode ID, return 1 if the current_user has
// permission to read the file. Otherwise 0.
int can_read(int inode_id)
{
	uint16_t file_owner_id = inodes[inode_id].uid;
	uint16_t file_permissions = inodes[inode_id].mode;
	int mask = 04;

	if (current_user == file_owner_id)
	{
		mask = 0400;
	}

	return (file_permissions & mask) != 0;
}

// Given an inode ID, return 1 if the current_user has
// permission to write to the file. Otherwise 0.
int can_write(int inode_id)
{
	uint16_t file_owner_id = inodes[inode_id].uid;
	uint16_t file_permissions = inodes[inode_id].mode;
	int mask = 02;

	if (current_user == file_owner_id)
	{
		mask = 0200;
	}

	return (file_permissions & mask) != 0;
}

// Given an inode ID, return 1 if the current_user has
// permission to execute the file. Otherwise 0.
int can_execute(int inode_id)
{
	uint16_t file_owner_id = inodes[inode_id].uid;
	uint16_t file_permissions = inodes[inode_id].mode;
	int mask = 01;

	if (current_user == file_owner_id)
	{
		mask = 0100;
	}

	return (file_permissions & mask) != 0;
}

// Create new empty directory and return its inode ID
int make_dir_file()
{
	int inode_id = get_inode();
	inodes[inode_id].file_type = DIRECTORY_FILETYPE;
	inodes[inode_id].mode = 0755;
	inodes[inode_id].uid = current_user;
	inodes[inode_id].atime = time(NULL);
	inodes[inode_id].block_ptr = malloc(sizeof(dir_entry_t) * MAX_ENTRIES);
	dir_entry_t *entres = (dir_entry_t *)inodes[inode_id].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		entres[i].inode_num = -1;
	}
	return inode_id;
}

// Create new regular file and return its inode ID.
// File contents is passes as args and copied into
// the file's memory block
int make_regular_file(char *args)
{
	int inode_id = get_inode();
	inodes[inode_id].file_type = REGULAR_FILETYPE;
	inodes[inode_id].mode = 0755;
	inodes[inode_id].uid = current_user;
	inodes[inode_id].atime = time(NULL);
	inodes[inode_id].block_ptr = malloc(sizeof(char) * strlen(args) + 1);

	char *data = (char *)inodes[inode_id].block_ptr;
	strcpy(data, args);
	return inode_id;
}

// Add a new entry into the parent directory
void make_entry(int parent_dir, char *file_name, int inode_id)
{
	int empty_entry = -1;
	dir_entry_t *entres = (dir_entry_t *)inodes[parent_dir].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{

		if (entres[i].inode_num == -1)
		{
			empty_entry = i;
			break;
		}
	}
	entres[empty_entry].inode_num = inode_id;
	strcpy(entres[empty_entry].name, file_name);
}

// Search the given directory for the given file name.
// Returns the position inside the directory
int search_dir(int dir_id, char *file_name)
{
	if (strlen(file_name) == 0)
	{
		return -1;
	}
	dir_entry_t *entres = (dir_entry_t *)inodes[dir_id].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		if (entres[i].inode_num != -1 && strcmp(entres[i].name, file_name) == 0)
		{
			return i;
		}
	}
	return -1;
}

// Search the given directory for the given file name.
// Returns the inode ID
int search_dir_inode(int dir_id, char *file_name)
{
	int dir_entry_id = search_dir(dir_id, file_name);
	if (dir_entry_id >= 0)
	{
		dir_entry_t *entres = (dir_entry_t *)inodes[dir_id].block_ptr;
		return entres[dir_entry_id].inode_num;
	}

	return -1;
}

// Checks if a file name is invalid for the current working
// directory. Meaning the file name is taken or the file name
// is too short.
int invalid_name(int cwd, char *file_name)
{
	return strlen(file_name) == 0 || search_dir(cwd, file_name) >= 0;
}

// COMPLETED COMMANDS

// Make a new directory file in the current working directory
void mkdir(char *file_name)
{
	if (!can_write(cwd) || !can_execute(cwd))
	{
		printf("Invalid file permissions\n");
		return;
	}

	if (invalid_name(cwd, file_name))
	{
		printf("Invalid file name\n");
		return;
	}
	int id = make_dir_file();
	make_entry(cwd, file_name, id);
}

// Make a new regular file current working directory
void mkfile(char *name)
{
	if (!can_write(cwd) || !can_execute(cwd))
	{
		printf("Invalid file permissions\n");
		return;
	}

	if (invalid_name(cwd, name))
	{
		printf("Invalid file name\n");
		return;
	}
	printf("Enter the contents of the file: \n");
	char buffer[MAX_FILE_SIZE];
	fgets(buffer, sizeof(buffer), stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
	int id = make_regular_file(buffer);
	make_entry(cwd, name, id);
}

// Print out the contents of the current working directory
void ls()
{
	if (!can_execute(cwd))
	{
		printf("Invalid file permissions\n");
		return;
	}

	dir_entry_t *entres = (dir_entry_t *)inodes[cwd].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		if (entres[i].inode_num != -1)
		{
			int inode_id = entres[i].inode_num;
			printf("%d \t", inode_id);
			printf("%c", (inodes[inode_id].file_type == DIRECTORY_FILETYPE) ? 'd' : '-');
			printf("%s \t", format_permissions(inode_id));
			printf("%d \t", inodes[inode_id].uid);

			char buf[20];
			strftime(buf, sizeof(buf), "%d %b %H:%M", localtime(&inodes[inode_id].atime));
			printf("%s \t", buf);

			printf("%s \n", entres[i].name);
		}
	}
}

// TO BE COMPLETED

// Change the current user ID to the given ID
void su(char *args)
{
	int userId = atoi(args);
	current_user = userId;
}

// Print the contents of the regular file
void cat(char *name)
{
	int id = search_dir_inode(cwd, name);
	if (id < 0 || !can_read(id))
	{
		printf("Invalid file or file permissions\n");
		return;
	}

	if (inodes[id].file_type == REGULAR_FILETYPE)
	{
		char *content = (char *)inodes[id].block_ptr;
		printf("%s\n", content);
	}
	else
	{
		printf("Not a file\n");
	}
}

// Update the file permissions
void chmod(char *args)
{
	int newPermission;
	char name[MAX_NAME_LEN];
	sscanf(args, "%o %s", &newPermission, name);

	int id = search_dir_inode(cwd, name);
	if (id < 0 || !can_write(id))
	{
		printf("Invalid input\n");
		return;
	}

	inodes[id].mode = newPermission;
}

// Change the cwd and cwd_path.
// Note: new_dir_path must be an absolute path
void cd(int current_dir_id, char *new_dir_path)
{
	int newCWD = search_dir_inode(current_dir_id, new_dir_path);

	if (newCWD < 0 || !can_execute(newCWD) || inodes[newCWD].file_type != DIRECTORY_FILETYPE) // must be directory
	{
		printf("Invalid input\n");
		return;
	}
	cwd = newCWD;
	strcpy(cwd_path, new_dir_path);
}

// Remove the regular file in the cwd
void rm(char *args)
{
	int id = search_dir_inode(cwd, args);
	if (id < 0 || !can_write(id) || inodes[id].file_type != REGULAR_FILETYPE)
	{
		printf("Invalid input\n");
		return;
	}

	release_inode(id);
	int entry = search_dir(cwd, args);
	dir_entry_t *entries = (dir_entry_t *)inodes[cwd].block_ptr;
	entries[entry].inode_num = -1;
}

// helper function for recursive call to delete all file and directory under the specify directory
void rm_recursive_helper(int id)
{
	dir_entry_t *entries = (dir_entry_t *)inodes[id].block_ptr;
	for (int i = 0; i < MAX_ENTRIES; i++) // check all entries
	{
		int entry = entries[i].inode_num;
		if (entry != -1)
		{
			if (inodes[entry].file_type == DIRECTORY_FILETYPE) // if its directory go deeper
			{
				rm_recursive_helper(entry);
			}
			else // if its file, release file
			{
				release_inode(entry);
			}
		}
	}
}

// Remove the directory file in the cwd
void rm_recursive(char *args)
{
	int id = search_dir_inode(cwd, args);
	if (id < 0 || !can_write(id) || inodes[id].file_type != DIRECTORY_FILETYPE)
	{
		printf("Invalid input\n");
		return;
	}

	rm_recursive_helper(id);
	release_inode(id);
	int entry = search_dir(cwd, args);
	dir_entry_t *entries = (dir_entry_t *)inodes[cwd].block_ptr;
	entries[entry].inode_num = -1;
}
