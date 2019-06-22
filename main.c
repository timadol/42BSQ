#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <io.h>  

#define ERR_EOM -1
#define ERR_EOF -2
#define ERR_OUT_OF_BOUNDS -3

#define BUFFSIZE 4
typedef struct _LIST_
{
    /* data */
    int buff[BUFFSIZE];
    struct _LIST_ * next;
} LIST;
LIST Head[2];
LIST *PrevHead = Head;
LIST *CurrHead = Head + 1;

// Чтение строки из файла в связанный список
// -----------------------------------------
int LIST_readstr(LIST *head, int file )
{
    char c;
    int n = 0;
    int size = 0;

    while( read(file, &c, 1) )
    {
        if( n >= BUFFSIZE )
        {
            if( !head->next ) 
            {
                head->next = malloc( sizeof( LIST ));
                if( !head->next)  
                    return ERR_EOM;
                head->next->next = NULL;
            }
            head = head->next;
            n = 0;
        }
        if (c == '.')
            head->buff[n++] = 1;
        else if (c == 'o')
            head->buff[n++] = 0;
        else
            head->buff[n++] = -1;
        size++;
        if( c == '\n' ) return size;
    }
    head->buff[n] = -1;
    return ERR_EOF;
}



// Чтение символа из связанного списка по его индексу
// --------------------------------------------------
int LIST_get(LIST *head, unsigned int idx )
{
    while( idx >= BUFFSIZE )
    {
        if( head->next )
        {
            head = head->next;
            idx -= BUFFSIZE;
        }
        else return ERR_OUT_OF_BOUNDS;
    }
    return head->buff[idx];    
}
// Запись символа из связанного списка по его индексу
// --------------------------------------------------
int LIST_put(LIST *head, unsigned int idx, int val)
{
    while( idx >= BUFFSIZE )
    {
        if( head->next )
        {
            head = head->next;
            idx -= BUFFSIZE;
        }
        else return (ERR_OUT_OF_BOUNDS);
    }
    head->buff[idx] = val;
    return (0);
}

int min_of(int a, int b, int c)
{
    int res = a;
    if( res > b ) res = b;
    if( res > c ) res = c;
    return res;
}

void    LIST_proc(LIST *prev, LIST *curr)
{
    int i = 1;
    int val;
    int left;
    int leftup;
    int up;
    while ((val = LIST_get(curr, i)) != -1)
    {
        if (val)
        {
            left = LIST_get(curr, i - 1);
            up = LIST_get(prev, i);
            leftup = LIST_get(prev, i - 1);
            LIST_put(curr, i, 1 + min_of(left, up, leftup));
        }
        printf("%d", LIST_get(curr, i));
        i++;
    }
    printf("\n");
}

int    LIST_max(LIST *head)
{
    int max;
    int i;
    int value;

    max = 0;
    i = 0;
    while ((value = LIST_get(head, i++)) != -1)
        if (value > max)
            max = value;
    return (max);
}
// Высвобождение памяти, занятой связанным списком
// -----------------------------------------------
void LIST_destroy(LIST *head)
{
    LIST *next = head->next->next;
    LIST *elem = head->next;
    while (next)
    {
        free(elem);
        elem = next;
        next = next->next;
    }
    free(elem);
}

char *readstr()
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


char ft_getchar()
{
    char c;
    if (read(0, &c, 1))
        return (c);
    return (0);
}

int main(int argc, char *argv[])
{
    LIST * temphead;
    //char *str = ft_getstr();
    char charset[3];
    int file;
    int m,size,max,i = 0;
    char c;
    

    // инициализация списка
    Head[0].next = NULL;
    Head[1].next = NULL;

    file = open("test.txt", _O_RDONLY);
    size = LIST_readstr(PrevHead, file);
    max = LIST_max(PrevHead);

    while(LIST_readstr(CurrHead, file) == size)
    {
        LIST_proc( PrevHead, CurrHead );
        m = LIST_max( CurrHead );
        if( m > max ) max = m;
        temphead = PrevHead;
        PrevHead = CurrHead;
        CurrHead = temphead;
    }
    printf("\n%d", max);
    // while((c = LIST_getchr(&Head[0] ,i++)) != '\n')
    // {
    //     printf("%c", c);
    // }

    // освобождаем ресурсы
    close (file);
    LIST_destroy(PrevHead);
    LIST_destroy(CurrHead);
}