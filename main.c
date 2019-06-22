#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EOM -1
#define ERR_EOF -2
#define ERR_OUT_OF_BOUNDS -3

#define BUFFSIZE 512
typedef struct _LIST_
{
    /* data */
    int buff[BUFFSIZE];
    struct _LIST_ * next;
} LIST;
LIST Head;

// Чтение строки из файла в связанный список
// -----------------------------------------
int LIST_readstr( int file )
{
    LIST *plist = &Head;
    char c;
    int n = 0;
    int size = 0;

    while( read(file, &c, 1) )
    {
        if( n < BUFFSIZE )
            plist->buff[n++] = c;
        else
        {
            if( !plist->next ) 
            {
                plist->next = malloc( sizeof( LIST ));
                if( !plist->next)  
                    return ERR_EOM;
                plist->next->next = NULL;
            }
            plist = plist->next;
            n = 0;
        }
        size++;
        if( c == '\n' ) return size;
    }
    return ERR_EOF;
}

// Получение символа из связанного списка по его индексу
// -----------------------------------------------------
int LIST_getchr( unsigned int idx )
{
    LIST *plist = &Head;

    while( idx >= BUFFSIZE )
    {
        if( plist->next )
        {
            plist = plist->next;
            idx -= BUFFSIZE;
        }
        else return ERR_OUT_OF_BOUNDS;
    }
    return plist->buff[idx];    
}

// Высвобождение памяти, занятой связанным списком
// -----------------------------------------------
void LIST_destroy()
{
    LIST *next = Head.next->next;
    LIST *elem = Head.next;
    while (next)
    {
        free(elem);
        elem = next;
        next = next->next;
    }
    free(elem);
}
/*
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
*/

char ft_getchar()
{
    char c;
    if (read(0, &c, 1))
        return (c);
    return (0);
}

int main(int argc, char *argv[])
{
    char *str = ft_getstr();
    char charset[3];
    int len = strlen(str);

    // инициализация списка
    Head.next = NULL;

    printf("%s", str);
    printf(", strlen = %d", len);
    charset[2] = str[len - 1];
    charset[1] = str[len - 2];
    charset[0] = str[len - 3];
    printf("\nEmpty - \"%c\"", charset[0]);
    printf("\nObsticle - \"%c\"", charset[1]);
    printf("\nFull - \"%c\"", charset[2]);

    // освобождаем память

}