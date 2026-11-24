/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melan <melan@learner.42.tech>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 16:56:52 by melan             #+#    #+#             */
/*   Updated: 2026/11/24 16:56:52 by melan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_test.h"
#include "libft.h"

// cc main.c ft_test.h ft_test.c -I ../Common-Core---Libft -L ../Common-Core---Libft -lft
// ft_display : OK si le retour est 1
// ft_assert : retourne 0 (faux) ou 1 (vrai)

int main(void)
{
    ft_count(0);
    ft_display(!ft_isalpha('0'));
    ft_count(1);
    ft_display(ft_assert_p("abc", "abc", 3));
    ft_count(2);
    ft_display(!ft_assert_p("abc", "abd", 3));
    ft_count(3);
    ft_display(ft_assert_p("abc", "abc", 4));
    ft_count(4);
    ft_display(ft_assert_p("abc", "abd", 2));
    ft_count(5);
    ft_display(ft_assert_p("abc", "abc", 5));
    ft_count(6);
    ft_display(!ft_assert_p("abc", "abcde", 4));

    ft_count(7);
    int s[3] = {0, 1, 2};
    int t[3] = {0, 1, 2};
    ft_display(ft_assert_p(s, t, 3*4));

    ft_count(8);    
    int u[3] = {0, 1, 2};
    int v[3] = {0, 1, 3};
    ft_display(!ft_assert_p(u, v, 3*4));

    ft_count(9);
    int w[2] = {0, 1};
    int x[3] = {0, 1, 3};
    ft_display(!ft_assert_p(w, x, 3*4));

    ft_count(10);    
    ft_display(ft_assert_int(123123, 123123));

    ft_count(11);    
    ft_display(!ft_assert_int(123123, 123125));

    ft_count(12);    
    ft_display(ft_assert_int('a', 'a'));

    ft_count(13);    
    ft_display(!ft_assert_int('a', 'j'));

    ft_count(14);
    int y[2] = {0, 1};
    int z[3] = {0, 1, 3};
    ft_display(ft_assert_p(y, z, 2*4));
}