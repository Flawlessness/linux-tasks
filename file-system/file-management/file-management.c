#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void makedir(char *dirname)
{
    int res = mkdir(dirname, 0777);
    if (res == -1)
    {
        printf("Error creating directory\n");
        return;
    }
}

void removedir(char *dirname)
{
    DIR *dir = opendir(dirname);

    if (dir == NULL)
    {
        printf("Error opening directory\n");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char path[100];
        sprintf(path, "%s/%s", dirname, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            removedir(path);
        }
        else
        {
            int res = unlink(path);
            if (res == -1)
            {
                printf("Error removing file\n");
                return;
            }
        }
    }

    closedir(dir);

    int res = rmdir(dirname);
    if (res == -1)
    {
        printf("Error removing directory\n");
        return;
    }
}

void lsdir(char *dirname)
{
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        printf("Error opening directory\n");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void makefile(char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error creating file\n");
        return;
    }
    fclose(file);
}

void removefile(char *filename)
{
    int res = unlink(filename);
    if (res == -1)
    {
        perror("Error deleting file\n");
        return;
    }
}

void readfile(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);
}

void create_symlink(char *filename, char *linkname)
{
    int res = symlink(filename, linkname);
    if (res == -1)
    {
        printf("Error creating symbolic link\n");
        return;
    }
}

void read_symlink(char *filename)
{
    char linkname[100];
    int res = readlink(filename, linkname, sizeof(linkname));
    if (res == -1)
    {
        printf("Error reading symbolic link\n");
        return;
    }
    printf("%s\n", linkname);
}

void readfile_symlink(char *filename)
{
    char linkname[100];
    int res = readlink(filename, linkname, sizeof(linkname));
    if (res == -1)
    {
        printf("Error reading symbolic link\n");
        return;
    }

    readfile(linkname);
}

void remove_symlink(char *filename)
{
    int res = unlink(filename);
    if (res == -1)
    {
        printf("Error removing symbolic link\n");
        return;
    }
}

void create_hardlink(char *filename, char *linkname)
{
    int res = link(filename, linkname);
    if (res == -1)
    {
        printf("Error creating hard link\n");
        return;
    }
}

void remove_hardlink(char *filename)
{
    int res = unlink(filename);
    if (res == -1)
    {
        printf("Error removing hard link\n");
        return;
    }
}

void read_permissions(char *filename)
{
    struct stat file_stat;
    int res = stat(filename, &file_stat);
    if (res == -1)
    {
        printf("Error getting file status\n");
        return;
    }
    printf("Permissions: %o\n", file_stat.st_mode & 0777);
    printf("Number of hard links: %ld\n", file_stat.st_nlink);
}

void change_permissions(char *filename, int permission)
{
    int res = chmod(filename, permission);
    if (res == -1)
    {
        printf("Error changing file permissions\n");
        return;
    }
}

int main(int argc, char *argv[])
{
    if ((argc < 2) || (argc > 3))
    {
        printf("Wrong number of arguments\n");
        return 1;
    }

    char *name = argv[1];
    char *progname = strrchr(argv[0], '/');
    if (progname == NULL)
    {
        progname = argv[0];
    }
    else
    {
        progname++;
    }

    if (strcmp(progname, "makedir") == 0)
    {
        makedir(name);
    }
    else if (strcmp(progname, "removedir") == 0)
    {
        removedir(name);
    }
    else if (strcmp(progname, "lsdir") == 0)
    {
        lsdir(name);
    }
    else if (strcmp(progname, "makefile") == 0)
    {
        makefile(name);
    }
    else if (strcmp(progname, "removefile") == 0)
    {
        removefile(name);
    }
    else if (strcmp(progname, "readfile") == 0)
    {
        readfile(name);
    }
    else if (strcmp(progname, "create-symlink") == 0)
    {
        if (argc == 2)
        {
            printf("Wrong number of arguments\n");
            return 1;
        }
        create_symlink(name, argv[2]);
    }
    else if (strcmp(progname, "read-symlink") == 0)
    {
        read_symlink(name);
    }
    else if (strcmp(progname, "readfile-symlink") == 0)
    {
        readfile_symlink(name);
    }
    else if (strcmp(progname, "remove-symlink") == 0)
    {
        remove_symlink(name);
    }
    else if (strcmp(progname, "create-hardlink") == 0)
    {
        if (argc == 2)
        {
            printf("Wrong number of arguments\n");
            return 1;
        }
        create_hardlink(name, argv[2]);
    }
    else if (strcmp(progname, "remove-hardlink") == 0)
    {
        remove_hardlink(name);
    }
    else if (strcmp(progname, "read-permissions") == 0)
    {
        read_permissions(name);
    }
    else if (strcmp(progname, "change-permissions") == 0)
    {
        if (argc == 2)
        {
            printf("Wrong number of arguments\n");
            return 1;
        }
        int len = strlen(argv[2]);
        for (int i = 0; i < len; i++)
        {
            if ((argv[2][i] < 48) || (argv[2][i] > 57))
            {
                printf("The second argument must be a number\n");
                return 1;
            }
        }
        char * tmp;
        change_permissions(name, strtol(argv[2], &tmp, 8));
    }
    else
    {
        printf("Unknown command: %s\n", progname);
        return 1;
    }

    return 0;
}

