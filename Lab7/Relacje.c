#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct
{
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair* p, int n, pair to_ins){
    for (int i = 0; i < n; i++){
        if (p[i].first == to_ins.first && p[i].second == to_ins.second){
            return 0;
        }
    }
    p[n].first = to_ins.first;
    p[n].second = to_ins.second;
    return 1;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *tab, int size)
{
	int flag;
	for (int i = 0; i < size; i++)
	{
		flag = 0;
		for (int j = 0; j < size; j++)
		{
			if (tab[i].first == tab[j].first && tab[j].first == tab[j].second)
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			return 0;
		}
	}
	return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *tab, int size)
{
	int flag;
	for (int i = 0; i < size; i++)
	{
		flag = 0;
		for (int j = 0; j < size; j++)
		{
			if (tab[i].first == tab[j].first && tab[j].first == tab[j].second)
			{
				flag = 1;
			}
		}
		if (flag == 1)
		{
			return 0;
		}
	}
	return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *tab, int size)
{
	int flag;
	for (int i = 0; i < size; i++)
	{
		flag = 0;
		for (int j = 0; j < size; j++)
		{
			if (tab[i].first == tab[j].second && tab[i].second == tab[j].first)
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			return 0;
		}
	}
	return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *tab, int size)
{
	int flag;
	for (int i = 0; i < size; i++)
	{
		flag = 1;
		for (int j = 0; j < size; j++)
		{
			if (tab[i].first == tab[j].second && tab[i].second == tab[j].first && tab[i].first != tab[j].first)
			{
				flag = 0;
			}
		}
		if (flag == 0)
		{
			return 0;
		}
	}
	return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *tab, int size)
{
	int flag;
	for (int i = 0; i < size; i++)
	{
		flag = 0;
		for (int j = 0; j < size; j++)
		{
			if (tab[i].first == tab[j].second && tab[i].second == tab[j].first)
			{
				flag = 1;
			}
		}
		if (flag == 1)
		{
			return 0;
		}
	}
	return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* p, int n)
{   
    int found = 0;
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if(i==j){
                continue;
            }
            if(p[i].second == p[j].first){
                found = 0;
                for(int k =0;k<n;k++){
                    if(p[k].first == p[i].first && p[k].second == p[j].first){
                        found = 1;
                    }
                }
                if(found == 0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *tab, int size)
{
	if (is_reflexive(tab, size) == 1 && is_antisymmetric(tab, size) == 1 && is_transitive(tab, size) == 1)
	{
		return 1;
	}
	return 0;
}

int is_connected(pair *tab, int size)
{
	int t[2 * size];
	int licznik = 0; // wprowadzenie wszystkich wartości do listy
	int check;
	for (int i = 0; i < size; i++){
		check = 0;
		for (int j = 0; j < licznik; j++)
		{
			if (tab[i].first == t[j])
			{
				check = 1;
			}
		}
		if (check == 0)
		{
			t[licznik] = tab[i].first;
			licznik++;
		}
		check = 0;
		for (int j = 0; j < licznik; j++)
		{
			if (tab[i].second == t[j])
			{
				check = 1;
			}
		}
		if (check == 0)
		{
			t[licznik] = tab[i].second;
			licznik++;
		}
	}
	for (int i = 0; i < licznik; i++)
	{
		for (int j = 0; j < licznik; j++)
		{
			int check = 0;
			for (int k = 0; k < size; k++)
			{
				if ((t[i] == tab[k].first && t[j] == tab[k].second) || (t[j] == tab[k].first && t[i] == tab[k].second) || t[i] == t[j])
				{
					check = 1;
					break;
				}
			}
			if (check == 0)
			{
				return 0;
			}
		}
	}
	return 1;
}

int find_max_elements(pair *tab, int size, int *max)
{
	int x, g, counter = 0, flag = 0, flag_2 = 0;

	for (int i = 0; i < size; i++)
	{
		flag = 0;
		g = tab[i].first;
		for (int j = 0; j < size; j++)
		{
			x = tab[j].first;
			if (x == g)
			{
				if (tab[j].second != g)
				{
					flag = 1;
				}
			}
		}
		if (flag == 0)
		{
			flag_2 = 0;
			for (int i = 0; i < counter; i++)
			{
				if (max[i] == g)
				{
					flag_2 = 1;
				}
			}
			if (flag_2 == 0)
			{
				max[counter] = g;
				counter++;
			}
		}
	}

	int temp;
	for (int i = 0; i < counter - 1; i++)
	{
		for (int j = 0; j < counter - i - 1; j++)
		{
			if (max[j] > max[j + 1])
			{
				temp = max[j];
				max[j] = max[j + 1];
				max[j + 1] = temp;
			}
		}
	}
	return counter;
}

int find_min_elements(pair *tab, int size, int *min)
{
	int x, g, counter = 0, flag = 0, flag_2 = 0;

	for (int i = 0; i < size; i++)
	{
		flag = 0;
		g = tab[i].first;
		for (int j = 0; j < size; j++)
		{
			x = tab[j].second;
			if (x == g)
			{
				if (tab[j].first != g)
				{
					flag = 1;
				}
			}
		}
		if (flag == 0)
		{
			flag_2 = 0;
			for (int i = 0; i < counter; i++)
			{
				if (min[i] == g)
				{
					flag_2 = 1;
				}
			}
			if (flag_2 == 0)
			{
				min[counter] = g;
				counter++;
			}
		}
	}

	int temp;
	for (int i = 0; i < counter - 1; i++)
	{
		for (int j = 0; j < counter - i - 1; j++)
		{
			if (min[j] > min[j + 1])
			{
				temp = min[j];
				min[j] = min[j + 1];
				min[j + 1] = temp;
			}
		}
	}
	return counter;
}

int get_domain(pair *tab, int size, int *domain)
{
	int counter = 0;
	int el, flag;
	for (int i = 0; i < size; i++)
	{
		flag = 1;
		el = tab[i].first;
		for (int j = 0; j < counter; j++)
		{
			if (domain[j] == el)
			{
				flag = 0;
			}
		}
		if (flag == 1)
		{
			domain[counter] = tab[i].first;
			counter++;
		}
		flag = 1;
		el = tab[i].second;
		for (int j = 0; j < counter; j++)
		{
			if (domain[j] == el)
			{
				flag = 0;
			}
		}
		if (flag == 1)
		{
			domain[counter] = tab[i].second;
			counter++;
		}
	}
	int tmp;
	for (int i = 0; i < counter - 1; i++)
	{
		for (int j = 0; j < counter - i - 1; j++)
		{
			if (domain[j] > domain[j + 1])
			{
				tmp = domain[j];
				domain[j] = domain[j + 1];
				domain[j + 1] = tmp;
			}
		}
	}
	return counter;
}

// Case 3:
int composition(pair *tab1, int size1, pair *tab2, int size2, pair *zlozenie)
{
	int licznik = 0;
	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size2; j++)
		{
			int por = 0;
			if (tab1[i].second == tab2[j].first)
			{
			    pair p ={tab1[i].first, tab2[j].second};
				por = add_relation(zlozenie, licznik, p);
				if (por == 1)
				{
					licznik++;
				}
			}
		}
	}
	return licznik;
}

int cmp(pair p1, pair p2)
{
	if (p1.first == p2.first)
		return p1.second - p2.second;
	return p1.first - p2.first;
}
// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *tab)
{
	int ilosc;
	scanf("%d", &ilosc);
	int por;
	int licznik = 0;
	for (int i = 0; i < ilosc; ++i)
	{
		pair p1;
		scanf("%d %d", &p1.first, &p1.second);
		por = add_relation(tab, licznik, p1);
		if (por == 1)
		{
			licznik++;
		}
	}
	return licznik;
}
// A total order relation is a partial order relation that is connected
int is_total_order(pair *tab, int size)
{
	if (is_partial_order(tab, size) == 1 && is_connected(tab, size) == 1)
	{
		return 1;
	}
	return 0;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)

void print_int_array(int *array, int n)
{
	printf("%d\n", n);
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void)
{
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d", &to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do)
	{
	case 1:
		printf("%d ", is_reflexive(relation, size));
		printf("%d ", is_irreflexive(relation, size));
		printf("%d ", is_symmetric(relation, size));
		printf("%d ", is_antisymmetric(relation, size));
		printf("%d ", is_asymmetric(relation, size));
		printf("%d\n", is_transitive(relation, size));
		break;
	case 2:
		ordered = is_partial_order(relation, size);
		n_domain = get_domain(relation, size, domain);
		printf("%d %d\n", ordered, is_total_order(relation, size));
		print_int_array(domain, n_domain);
		if (!ordered)
			break;
		int max_elements[MAX_REL_SIZE];
		int min_elements[MAX_REL_SIZE];
		int no_max_elements = find_max_elements(relation, size, max_elements);
		int no_min_elements = find_min_elements(relation, size, min_elements);
		print_int_array(max_elements, no_max_elements);
		print_int_array(min_elements, no_min_elements);
		break;
	case 3:
		size_2 = read_relation(relation_2);
		printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}
