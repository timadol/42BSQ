#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ft_getstr()
{
    char *res;
    char *save;
    char c;
    int size;
    int i;

    size =  0;
    res = malloc(size + 1);
    *res = 0;
    while(read(0, &c, 1) && c != '\n')
    {
        save = res;
        size++;
        i = 0;
        res = (char*)malloc(size + 1);
        res[size] = 0;
        res[size - 1] = c;
        while (save && save[i])
            res[i] = save[i++];
        free(save);
    }
    return (res);
}

char    ft_getchar()
{
    char c;
    if (read(0, &c, 1))
        return (c);
    return (0);
}

int main(int argc, char *argv[])
{
    char *str = ft_getstr();
    printf("%s", str);
    printf(", strlen = %d", strlen(str));
}