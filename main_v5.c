/* ================================================================== */
/* libft-tester.c                                                     */
/*                                                                    */
/* Testeur libft qui compare les fonctions de la libft aux vraies     */
/* fonctions de la libc (<ctype.h>, <string.h>, <stdlib.h>).          */
/* Pour chaque test : la fonction ft_ et la fonction officielle       */
/* sont appelées sur les mêmes entrées, puis les résultats sont       */
/* comparés entre eux (et non à une valeur codée en dur).             */
/*                                                                    */
/* Compilation :                                                      */
/*   cc -Wall -Wextra -Werror libft-tester.c -L. -lft -o libft-tester */
/*   ./libft-tester                                                   */
/* (ou : cc libft-tester.c libft.a -o libft-tester)                   */
/*                                                                    */
/* Retour : 0 si tout est OK, 1 sinon.                                */
/* ================================================================== */

#include <stdio.h>    /* printf, snprintf */
#include <stdlib.h>   /* malloc, free, exit, atoi */
#include <string.h>   /* strlen, strchr, strrchr, strncmp, memcmp,
                         memset, memcpy, memmove, memchr, strdup */
#include <limits.h>   /* INT_MAX, INT_MIN, SIZE_MAX */
#include <ctype.h>    /* isalpha, isdigit, ... toupper, tolower */
#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>
#include "libft.h"

/* ------------------------------------------------------------------ */
/* Boîte à outils de test                                              */
/* ------------------------------------------------------------------ */

static int  g_total = 0;
static int  g_ok = 0;
static int  g_failed[2048];
static int  g_failed_count = 0;

static void ft_test(char *name, int result)
{
    g_total++;
    printf("%3d. %-44s ", g_total, name);
    if (result == 1)
    {
        g_ok++;
        printf("OK\n");
    }
    else
    {
        if (g_failed_count < 2048)
            g_failed[g_failed_count++] = g_total;
        printf("KO :(\n");
    }
}

/* Variante qui affiche attendu/obtenu quand c'est KO */
static void ft_test2(char *name, int result, long expected, long got)
{
    g_total++;
    printf("%3d. %-44s ", g_total, name);
    if (result == 1)
    {
        g_ok++;
        printf("OK\n");
    }
    else
    {
        if (g_failed_count < 2048)
            g_failed[g_failed_count++] = g_total;
        printf("KO  attendu=%ld obtenu=%ld\n", expected, got);
    }
}

static int assert_int(long i, long j)
{
    return (i == j);
}

/* Les fonctions ctype de la libc peuvent renvoyer n'importe quelle
   valeur non nulle : on compare en booléen. */
static int assert_bool(int ft_val, int libc_val)
{
    return ((!!ft_val) == (!!libc_val));
}

static int assert_str(const char *a, const char *b)
{
    if (!a || !b)
        return (0);
    return (strcmp(a, b) == 0);
}

static int assert_mem(const void *a, const void *b, size_t n)
{
    return (memcmp(a, b, n) == 0);
}

/* ------------------------------------------------------------------ */
/* Auxiliaires pour les tests bonus / allouants                        */
/* ------------------------------------------------------------------ */

static void inc_content(void *content)
{
    (*(int *)content)++;
}

static void *dup_times10(void *content)
{
    int *copy;

    copy = malloc(sizeof(int));
    if (copy)
        *copy = *(int *)content * 10;
    return (copy);
}

static void free_content(void *content)
{
    free(content);
}

static char upper_idx(unsigned int i, char c)
{
    if (c >= 'a' && c <= 'z')
        return (char)(c - 32);
    (void)i;
    return (c);
}

static void upper_inplace(unsigned int i, char *c)
{
    (void)i;
    if (*c >= 'a' && *c <= 'z')
        *c = (char)(*c - 32);
}

/* marqueur de suppression */
static int g_del_called = 0;
static void mark_del(void *content)
{
    g_del_called++;
    (void)content;
}

/* Libère un tableau terminé par NULL renvoyé par ft_split */
static void free_words(char **words)
{
    size_t i;

    if (!words)
        return;
    i = 0;
    while (words[i])
        free(words[i++]);
    free(words);
}

/* ------------------------------------------------------------------ */
/* PARTIE 1 : fonctions ctype (comparées à <ctype.h>)                  */
/* Balayage de chaque octet de -128 à 255, comme la moulinette         */
/* ------------------------------------------------------------------ */

static int test_ctype_all(void)
{
    int c;
    int failed;

    failed = 0;
    c = -128;
    while (c <= 255)
    {
        if (!assert_bool(ft_isalpha(c), isalpha(c)))
            { printf("   isalpha(%d) diverge\n", c); failed = 1; }
        if (!assert_bool(ft_isdigit(c), isdigit(c)))
            { printf("   isdigit(%d) diverge\n", c); failed = 1; }
        if (!assert_bool(ft_isalnum(c), isalnum(c)))
            { printf("   isalnum(%d) diverge\n", c); failed = 1; }
        if (!assert_bool(ft_isascii(c), isascii(c)))
            { printf("   isascii(%d) diverge\n", c); failed = 1; }
        if (!assert_bool(ft_isprint(c), isprint(c)))
            { printf("   isprint(%d) diverge\n", c); failed = 1; }
        c++;
    }
    return (!failed);
}

static int test_toupper_tolower_all(void)
{
    int c;
    int failed;

    failed = 0;
    c = 0;
    while (c <= 255)
    {
        if (ft_toupper(c) != toupper(c))
            { printf("   toupper(%d) diverge\n", c); failed = 1; }
        if (ft_tolower(c) != tolower(c))
            { printf("   tolower(%d) diverge\n", c); failed = 1; }
        c++;
    }
    return (!failed);
}

/* ------------------------------------------------------------------ */
/* PARTIE 2 : strlen, strchr, strrchr, strnstr, strncmp                */
/* ------------------------------------------------------------------ */

static void part_string_search(void)
{
    char big[64];
    const char *hit;

    /* ---- strlen : comparé au vrai strlen ---- */
    ft_test("strlen basics", assert_int((int)ft_strlen("bonjour"), (int)strlen("bonjour"))
            && assert_int((int)ft_strlen(""), (int)strlen(""))
            && assert_int((int)ft_strlen("a"), (int)strlen("a")));

    /* ---- strchr ---- */
    ft_test("strchr trouve (== libc)", assert_mem(ft_strchr("bonjour", 'j'),
                strchr("bonjour", 'j'), 4));
    ft_test("strchr cherche nul", ft_strchr("bonjour", 0) == strchr("bonjour", 0));
    ft_test("strchr absent (== libc)", (ft_strchr("azerty", 'z' + 10) == NULL)
            && (strchr("azerty", 'z' + 10) == NULL));
    ft_test("strchr premiere occurrence",
            *(ft_strchr("lala", 'l')) == *(strchr("lala", 'l')));

    /* ---- strrchr ---- */
    ft_test("strrchr dernier (== libc)", *(ft_strrchr("lala", 'l'))
            == *(strrchr("lala", 'l')));
    ft_test("strrchr absent (== libc)", (ft_strrchr("azerty", 'q') == NULL)
            && (strrchr("azerty", 'q') == NULL));
    ft_test("strrchr nul retour fin",
            ft_strrchr("lala", 0) == strrchr("lala", 0));

    /* ---- strnstr : pas de libc, valeurs de référence ---- */
    ft_test("strnstr trouve", ft_strnstr("bonjour", "jour", 10) != NULL);
    ft_test("strnstr hors len", ft_strnstr("bonjour", "jour", 4) == NULL);
    ft_test("strnstr little vide",
            ft_strnstr("abc", "", 2) != NULL && strcmp(ft_strnstr("abc", "", 2), "abc") == 0);
    ft_test("strnstr len 0", ft_strnstr("abc", "abc", 0) == NULL);
    ft_test("strnstr little plus grand", ft_strnstr("hi", "hello", 10) == NULL);
    snprintf(big, sizeof(big), "abacadabra");
    hit = ft_strnstr(big, "adab", 10);
    ft_test("strnstr chevauchement", hit != NULL && strcmp(hit, "adabra") == 0);

    /* ---- strncmp : comparé au vrai strncmp ---- */
    ft_test("strncmp egal (== libc)", ft_strncmp("abc", "abc", 3) == strncmp("abc", "abc", 3));
    ft_test("strncmp diff (== libc)", (ft_strncmp("abc", "abd", 3) < 0)
            == (strncmp("abc", "abd", 3) < 0));
    ft_test("strncmp borne par n", ft_strncmp("abc", "abx", 2) == strncmp("abc", "abx", 2));
    ft_test("strncmp n=0", ft_strncmp("abc", "xyz", 0) == strncmp("abc", "xyz", 0));
    ft_test("strncmp n plus grand", ft_strncmp("ab", "abc", 10) == strncmp("ab", "abc", 10));
    /* comparaison UNSIGNED : 0x80 (128) doit etre > 0x01 (1) */
    ft_test("strncmp comparaison unsigned", ft_strncmp("a\200", "a\001", 2) > 0);
}

/* ------------------------------------------------------------------ */
/* PARTIE 3 : memset, bzero, memcpy, memmove, memchr                   */
/* ------------------------------------------------------------------ */

static void part_memory(void)
{
    char b1[64], b2[64];

    /* ---- memset ---- */
    memset(b1, 'x', 8);
    ft_memset(b2, 'x', 8);
    ft_test("memset remplissage (== libc)", memcmp(b1, b2, 8) == 0);
    ft_test("memset retourne dst", ft_memset(b2, 'y', 2) == b2);
    memset(b1, 0, 4);
    ft_memset(b2, 0, 4);
    ft_test("memset zero", memcmp(b1, b2, 4) == 0);

    /* ---- bzero ---- */
    memset(b1, 'a', 6); bzero(b1, 6);
    ft_memset(b2, 'a', 6); ft_bzero(b2, 6);
    ft_test("bzero (== libc bzero)", memcmp(b1, b2, 6) == 0);
    ft_bzero(b2, 0); /* ne doit rien casser */
    ft_test("bzero n=0", 1);

    /* ---- memcpy ---- */
    ft_memcpy(b1, "hello", 6);
    ft_memcpy(b2, "hello", 6);
    ft_test("memcpy chaine + nul (== libc)", memcmp(b1, b2, 6) == 0
            && strcmp(b1, "hello") == 0);
    ft_test("memcpy n=0 : rien ne bouge", ft_memcpy(b1, "ZZZ", 0) == b1
            && strcmp(b1, "hello") == 0);

    /* ---- memmove : les 3 chevauchements ---- */
    memcpy(b1, "abcdef", 7);
    memmove(b1 + 2, b1, 4);              /* libc : dst apres src    */
    memcpy(b2, "abcdef", 7);
    ft_memmove(b2 + 2, b2, 4);           /* ft   : dst apres src    */
    ft_test("memmove av -> arriere", memcmp(b1, b2, 7) == 0
            && memcmp(b1, "ababcd", 7) == 0);

    memcpy(b1, "abcdef", 7);
    memmove(b1, b1 + 2, 4);              /* libc : dst avant src    */
    memcpy(b2, "abcdef", 7);
    ft_memmove(b2, b2 + 2, 4);           /* ft   : dst avant src    */
    ft_test("memmove ar -> avant", memcmp(b1, b2, 7) == 0
            && memcmp(b1, "cdefef", 7) == 0);

    memcpy(b1, "sameplace", 10);
    memmove(b1, b1, 10);
    memcpy(b2, "sameplace", 10);
    ft_memmove(b2, b2, 10);
    ft_test("memmove sur lui-meme", memcmp(b1, b2, 10) == 0);

    memcpy(b1, "QQQQQQQQ", 9);
    memmove(b1 + 1, b1, 8);
    memcpy(b2, "QQQQQQQQ", 9);
    ft_memmove(b2 + 1, b2, 8);
    ft_test("memmove bloc 8 octets", memcmp(b1, b2, 9) == 0);

    /* ---- memchr ---- */
    ft_test("memchr trouve (== libc)",
            memcmp(ft_memchr("abcdef", 'c', 6), memchr("abcdef", 'c', 6), 1) == 0);
    ft_test("memchr cherche apres nul",
            memcmp(ft_memchr("abc\0x", 'x', 6), memchr("abc\0x", 'x', 6), 1) == 0
            && ft_memchr("abc\0x", 'x', 6) != NULL);
    ft_test("memchr hors zone (== libc)", (ft_memchr("abcdef", 'c', 2) == NULL)
            && (memchr("abcdef", 'c', 2) == NULL));
    ft_test("memchr trouve le 0", ft_memchr("abc", 0, 4) != NULL
            && ft_memchr("abc", 0, 4) == memchr("abc", 0, 4));
}

/* ------------------------------------------------------------------ */
/* PARTIE 4 : strlcpy, strlcat                                         */
/* Pas dans la libc standard : valeurs de reference + cas limites      */
/* du sujet.                                                           */
/* ------------------------------------------------------------------ */

static void part_strl(void)
{
    char d1[16];

    /* ---- strlcpy ---- */
    ft_memset(d1, '-', 16);
    ft_test2("strlcpy retour = strlen(src)",
            (size_t)ft_strlcpy(d1, "test", 16) == strlen("test"), 4,
            (long)ft_strlen(d1));
    ft_test("strlcpy copie", assert_str(d1, "test"));

    ft_memset(d1, '-', 16);
    ft_test("strlcpy tronque securise", (size_t)ft_strlcpy(d1, "testlong", 5) == 8
            && assert_str(d1, "test"));
    ft_memset(d1, '-', 16);
    ft_test("strlcpy size=0 : dst intact", (size_t)ft_strlcpy(d1, "abc", 0) == 3
            && d1[0] == '-');
    ft_memset(d1, '-', 16);
    ft_test("strlcpy src vide", (size_t)ft_strlcpy(d1, "", 16) == 0
            && d1[0] == '\0');

    /* ---- strlcat ---- */
    ft_strlcpy(d1, "bon", 16);
    ft_test("strlcat retour = total", (size_t)ft_strlcat(d1, "jour", 16) == 7
            && assert_str(d1, "bonjour"));
    ft_strlcpy(d1, "bonjour", 16);
    ft_test("strlcat insuffisant : dst non touche",
            (size_t)ft_strlcat(d1, "xyz", 4) == 7 && assert_str(d1, "bonjour"));
    ft_strlcpy(d1, "", 16);
    ft_test("strlcat sur vide", (size_t)ft_strlcat(d1, "abc", 16) == 3
            && assert_str(d1, "abc"));
    ft_strlcpy(d1, "abc", 16);
    ft_test("strlcat size<strlen(dst)", (size_t)ft_strlcat(d1, "def", 2) == 5
            && assert_str(d1, "abc"));
    ft_strlcpy(d1, "ab", 16);
    ft_test("strlcat remplit pile", (size_t)ft_strlcat(d1, "cdef", 6) == 6
            && assert_str(d1, "abcde"));
}

/* ------------------------------------------------------------------ */
/* PARTIE 5 : conversions                                              */
/* ------------------------------------------------------------------ */

static void part_conversions(void)
{
    char tmp[32];

    /* ---- atoi : comparé au vrai atoi ---- */
    ft_test2("atoi '123'", assert_int(ft_atoi("123"), atoi("123")),
            atoi("123"), ft_atoi("123"));
    ft_test2("atoi ' \\t -45'", assert_int(ft_atoi("  \t -45"), atoi("  \t -45")),
            atoi("  \t -45"), ft_atoi("  \t -45"));
    ft_test2("atoi '+7choucroute'", assert_int(ft_atoi("+7choucroute"), atoi("+7choucroute")),
            atoi("+7choucroute"), ft_atoi("+7choucroute"));
    ft_test2("atoi 0", assert_int(ft_atoi("0"), atoi("0")), 0, ft_atoi("0"));
    ft_test2("atoi '-0'", assert_int(ft_atoi("-0"), atoi("-0")), 0, ft_atoi("-0"));
    ft_test2("atoi double signe", assert_int(ft_atoi("--5"), atoi("--5")),
            atoi("--5"), ft_atoi("--5"));
    ft_test2("atoi signe seul", assert_int(ft_atoi("-"), atoi("-")),
            atoi("-"), ft_atoi("-"));
    ft_test2("atoi tout espaces", assert_int(ft_atoi("   "), atoi("   ")),
            atoi("   "), ft_atoi("   "));
    snprintf(tmp, sizeof(tmp), "%d", INT_MAX);
    ft_test2("atoi INT_MAX", assert_int(ft_atoi(tmp), atoi(tmp)),
            (long)INT_MAX, ft_atoi(tmp));
    ft_test2("atoi INT_MAX+1 overflow", assert_int(ft_atoi("2147483648"),
            atoi("2147483648")), atoi("2147483648"), ft_atoi("2147483648"));
    ft_test2("atoi INT_MIN", assert_int(ft_atoi("-2147483648"), atoi("-2147483648")),
            (long)INT_MIN, ft_atoi("-2147483648"));
    ft_test2("atoi espaces autorises", assert_int(ft_atoi(" \n \v \f \r \t +42"),
            atoi(" \n \v \f \r \t +42")), atoi(" \n \v \f \r \t +42"),
            ft_atoi(" \n \v \f \r \t +42"));

    /* ---- itoa ---- */
    ft_test("itoa 0", assert_str(ft_itoa(0), "0"));
    ft_test("itoa positif", assert_str(ft_itoa(42), "42"));
    ft_test("itoa negatif", assert_str(ft_itoa(-123), "-123"));
    snprintf(tmp, sizeof(tmp), "%d", INT_MAX);
    ft_test("itoa INT_MAX", assert_str(ft_itoa(INT_MAX), tmp));
    snprintf(tmp, sizeof(tmp), "%d", INT_MIN);
    ft_test("itoa INT_MIN", assert_str(ft_itoa(INT_MIN), tmp));
    ft_test("itoa -1", assert_str(ft_itoa(-1), "-1"));
}

/* ------------------------------------------------------------------ */
/* PARTIE 6 : fonctions allouantes                                     */
/* ------------------------------------------------------------------ */

static void part_alloc(void)
{
    char **words;
    char *p;
    int *tab;
    size_t i;
    int ok;

    /* ---- calloc ---- */
    tab = ft_calloc(4, sizeof(int));
    ok = tab != NULL;
    i = 0;
    while (ok && i < 4)
    {
        if (tab[i] != 0)
            ok = 0;
        i++;
    }
    ft_test("calloc initialise a 0", ok);
    free(tab);

    p = ft_calloc(0, 8); /* comportement libre, doit juste ne pas crasher */
    ft_test("calloc(0, x) ne crash pas", 1);
    free(p);

    p = ft_calloc(SIZE_MAX, SIZE_MAX);
    ft_test("calloc overflow -> NULL", p == NULL);
    free(p);

    /* ---- strdup : comparé au vrai strdup ---- */
    p = ft_strdup("dup");
    ft_test("strdup copie (== libc)", p && strcmp(p, "dup") == 0);
    free(p);
    p = ft_strdup("");
    ft_test("strdup vide", p != NULL && ft_strlen(p) == ft_strlen(""));
    free(p);

    /* ---- substr ---- */
    p = ft_substr("bonjour", 3, 3);
    ft_test("substr milieu", p && assert_str(p, "jou"));
    free(p);
    p = ft_substr("bonjour", 10, 3);
    ft_test("substr start > len -> vide", p && ft_strlen(p) == 0);
    free(p);
    p = ft_substr("bonjour", 2, 100); /* 100 > ce qui reste */
    ft_test("substr len clamp au reste", p && assert_str(p, "njour"));
    free(p);
    p = ft_substr("hello", 2, 0);
    ft_test("substr len=0 -> vide", p && ft_strlen(p) == 0);
    free(p);

    /* ---- strjoin ---- */
    p = ft_strjoin("bon", "jour");
    ft_test("strjoin", p && assert_str(p, "bonjour"));
    free(p);
    p = ft_strjoin("", "");
    ft_test("strjoin double vide", p && ft_strlen(p) == 0);
    free(p);

    /* ---- strtrim ---- */
    p = ft_strtrim("  xbonjourx  ", " x");
    ft_test("strtrim debut et fin", p && assert_str(p, "bonjour"));
    free(p);
    p = ft_strtrim("xxx", "x");
    ft_test("strtrim tout retire -> vide", p && ft_strlen(p) == 0);
    free(p);
    p = ft_strtrim("abc", "");
    ft_test("strtrim set vide", p && assert_str(p, "abc"));
    free(p);
    p = ft_strtrim("", " x");
    ft_test("strtrim chaine vide", p && ft_strlen(p) == 0);
    free(p);

    /* ---- strmapi / striteri ---- */
    p = ft_strmapi("abc", upper_idx);
    ft_test("strmapi nouvelle chaine", p && assert_str(p, "ABC"));
    free(p);

    p = ft_strdup("abc");
    ft_striteri(p, upper_inplace);
    ft_test("striteri modifie en place", p && assert_str(p, "ABC"));
    free(p);

    /* ---- split : cas du sujet + cas limites ---- */
    words = ft_split("a,,bb,,ccc", ',');
    ok = words && words[0] && words[1] && words[2] && words[3] == NULL
        && assert_str(words[0], "a") && assert_str(words[1], "bb")
        && assert_str(words[2], "ccc");
    ft_test("split 'a,,bb,,ccc'", ok);
    free_words(words);

    words = ft_split("", ',');
    ft_test("split chaine vide -> terrain vide", words && words[0] == NULL);
    free_words(words);

    words = ft_split(",,,,", ',');
    ft_test("split que separateurs", words && words[0] == NULL);
    free_words(words);

    words = ft_split("abc", ',');
    ft_test("split sans separateur", words && words[0] && words[1] == NULL
            && assert_str(words[0], "abc"));
    free_words(words);

    words = ft_split(",abc,", ',');
    ft_test("split separateurs en bordures", words && words[0] && words[1] == NULL
            && assert_str(words[0], "abc"));
    free_words(words);

    /* ---- putX_fd : test visuel, comme dans le testeur d'origine ---- */
    ft_putchar_fd('\n', 1);
    ft_putstr_fd("putnbr_fd(-505) -> ", 1);
    ft_putnbr_fd(-505, 1);
    ft_putchar_fd('\n', 1);
    ft_putendl_fd("(ligne juste au-dessus, test visuel)", 1);
    ft_putchar_fd('\n', 1);
}

/* ------------------------------------------------------------------ */
/* PARTIE 7 : liste chainée (bonus)                                    */
/* ------------------------------------------------------------------ */

static void part_list(void)
{
    t_list *head;
    t_list *mapped;
    t_list *it;
    int *e0;
    int *e1;
    int *e2;
    int count;

    e0 = malloc(sizeof(int)); *e0 = 1;
    e1 = malloc(sizeof(int)); *e1 = 2;
    e2 = malloc(sizeof(int)); *e2 = 3;

    head = NULL;
    ft_lstadd_back(&head, ft_lstnew(e0));
    ft_lstadd_back(&head, ft_lstnew(e1));
    ft_test("lstadd_back sur liste NULL", head != NULL
            && assert_int(ft_lstsize(head), 2));
    ft_test("lstlast content = 2", *(int *)ft_lstlast(head)->content == 2);

    ft_lstadd_front(&head, ft_lstnew(e2));
    ft_test("add_front devient tete", head && *(int *)head->content == 3);
    ft_test("lstsize == 3", assert_int(ft_lstsize(head), 3));
    ft_test("lstsize(NULL) == 0", assert_int(ft_lstsize(NULL), 0));
    ft_test("lstlast(NULL) == NULL", ft_lstlast(NULL) == NULL);

    head = NULL;
    ft_lstadd_back(&head, ft_lstnew(e2));
    ft_test("lstadd_back apres reset a NULL", head != NULL
            && *(int *)ft_lstlast(head)->content == 3);
    ft_test("lstsize apres add_back == 1", assert_int(ft_lstsize(head), 1));

    it = head;
    ft_test("maillon isole : next = NULL", it && it->next == NULL
            && *(int *)it->content == 3);

    ft_lstiter(head, inc_content);
    ft_test("lstiter a incremente", head && *(int *)head->content == 4);

    g_del_called = 0;
    ft_lstdelone(head, mark_del);
    ft_test("lstdelone appelle del", g_del_called == 1);
    head = NULL;

    /* reconstruire pour lstmap/lstclear */
    e0 = malloc(sizeof(int)); *e0 = 1;
    e1 = malloc(sizeof(int)); *e1 = 2;
    e2 = malloc(sizeof(int)); *e2 = 3;
    head = ft_lstnew(e0);
    ft_lstadd_back(&head, ft_lstnew(e1));
    ft_lstadd_back(&head, ft_lstnew(e2));

    mapped = ft_lstmap(head, dup_times10, free_content);
    ft_test("lstmap x10 dernier", mapped
            && *(int *)ft_lstlast(mapped)->content == 30);
    count = 0;
    it = mapped;
    while (it)
    {
        count++;
        it = it->next;
    }
    ft_test("lstmap taille preservee", count == 3);
    ft_lstclear(&mapped, free_content);
    ft_test("lstclear -> liste vide", mapped == NULL);

    ft_lstclear(&head, free_content);
    ft_test("lstclear du head ok", head == NULL);
    ft_lstclear(&head, free_content);
    ft_test("lstclear sur NULL", head == NULL);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */

int main(void)
{
    int i;

    /* ---- Partie 1 : ctype, balayage complet -128..255 ---- */
    ft_test("ctype sur 384 octets (== libc)", test_ctype_all());
    ft_test("toupper/tolower sur 0..255 (== libc)", test_toupper_tolower_all());

    part_string_search();
    part_memory();
    part_strl();
    part_conversions();
    part_alloc();
    part_list();

    /* ---- Bilan ---- */
    printf("\n===== BILAN : %d / %d tests OK =====\n", g_ok, g_total);
    if (g_failed_count > 0)
    {
        printf("Tests en echec : ");
        i = 0;
        while (i < g_failed_count)
        {
            printf("%d", g_failed[i]);
            if (i + 1 < g_failed_count)
                printf(", ");
            i++;
        }
        printf("\n");
    }
    if (g_ok == g_total)
        return (0);
    return (1);
}