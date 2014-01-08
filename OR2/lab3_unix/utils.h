void gen_rands(int *tab, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        tab[i] = rand()%1000;
    }
}

int compare_a(const void *a, const void *b)
{
    return *(int*)b - *(int*)a;
}

int compare_b(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

void debug_pring(char* msg, int *tab, size_t size)
{
	printf("%s :\n", msg);
	for(int i = 0; i < size; ++i)
	{
		printf("%d ", tab[i]);
	}
	printf("\n");
}