
#include <stdio.h>

static int ft_isalnum(int c)
{
    if (c >= 'a' && c <= 'z')
        return (1);
    if (c >= 'A' && c <= 'Z')
        return (1);
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

static int ft_memcmp(const void *s1, const void *s2, size_t n)
{
    size_t i;

    i = 0;
    if (n <= 0)
        return (0);
    // if (s1 == NULL || s2 == NULL)
    // {
    //     return (0);
    // }
    while (((unsigned char *)s1)[i] == ((unsigned char *)s2)[i] && i < n - 1) // attention enlever le ((unsigned char *)s1)[i] && 
    {
        i++;
    }
    return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
}

void ft_count(int n)
{
    printf("%d. ", n);
}

void ft_display(int result)
{
    if (result == 1)
        printf("OK\n");
    else
        printf("KO :\(\n");
}

int ft_assert_p(void *s, void *t, size_t n) // entrer n = 4 pour un int
{
    if (ft_memcmp(s, t, n) == 0)
        return (1);
    else
        return (0);
}

int ft_assert_int(int i, int j)
{
    if (i == j)
        return (1);
    else
        return (0);
}
