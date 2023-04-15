#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void reverse_string(char *str)
{
    int i = 0;
    int j = strlen(str) - 1;
    while (i < j)
    {
        unsigned char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void copy_directory(const char *src_path, const char *dest_path)
{
    DIR *dir = opendir(src_path);
    if (dir == NULL)
    {
        printf("Failed to open directory %s\n", src_path);
        return;
    }

    mkdir(dest_path, 0000);

    struct stat st;
    stat(src_path, &st);
    chmod(dest_path, st.st_mode);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char src_entry_path[256];
        sprintf(src_entry_path, "%s/%s", src_path, entry->d_name);

        char reversed_name[256];
        strcpy(reversed_name, entry->d_name);
        reverse_string(reversed_name);

        char dest_entry_path[256];
        sprintf(dest_entry_path, "%s/%s", dest_path, reversed_name);


        if (entry->d_type == DT_REG)
        {
            FILE *src_file = fopen(src_entry_path, "rb");
            FILE *dest_file = fopen(dest_entry_path, "wb");

            stat(src_entry_path, &st);
            chmod(dest_entry_path, st.st_mode);

            if (src_file == NULL || dest_file == NULL)
            {
                printf("Failed to copy file %s\n", src_entry_path);
                continue;
            }

            // Get the size of the source file
            fseek(src_file, 0, SEEK_END);
            long file_size = ftell(src_file);
            fseek(src_file, -1, SEEK_CUR);
            while (file_size != 0)
            {
                unsigned char buffer;
                fread(&buffer, 1, 1, src_file);
                fwrite(&buffer, 1, 1, dest_file);
                fseek(src_file, -2, SEEK_CUR);
                file_size--;
            }

            fclose(src_file);
            fclose(dest_file);
        }

        else if (entry->d_type == DT_DIR)
        {
            copy_directory(src_entry_path, dest_entry_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Enter the path to the directory\n");
        return 1;
    }

    char reversed_path[256] = "";
    strcpy(reversed_path, argv[1]);

    int path_len = strlen(reversed_path) - 1;
    int position_last_slah = path_len;
    while (position_last_slah > 0)
    {
        if (reversed_path[position_last_slah] == '/')
        {
            break;
        }
        position_last_slah--;
    }
    if (position_last_slah != 0)
    {
        position_last_slah++;
    }
    while (position_last_slah < path_len)
    {
        unsigned char tmp = reversed_path[position_last_slah];
        reversed_path[position_last_slah] = reversed_path[path_len];
        reversed_path[path_len] = tmp;
        position_last_slah++;
        path_len--;
    }

    copy_directory(argv[1], reversed_path);

    return 0;
}
