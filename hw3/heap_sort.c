//
// Created by Kelei Gong on 10/25/15.
//

#include "heap_sort.h"
#include <stdio.h>
#include <stdlib.h>
void sift(int *heap, int i, int n);
void heapify(int *heap, int n);
void heap_sort(int *heap, int n);


int main()
{
//    int *input = malloc(10 * sizeof(int));
    int input[10] = {1, 9, 38 ,10, 5, 21, 12, 2, 3, 14};
    heap_sort(input, 10);
    int i;
    for( i = 0; i < 10; i++)
    {
        printf("%d\n", input[i]);
    }
    return 0;
}

void sift(int *heap, int i, int n)
{
    int j, k, sm, tmp;
    while(i * 2 + 1 < n)
    {
        j = i * 2 + 1;
        k = j + 1;
        if(k < n && heap[k] < heap[j])
        {
            sm = k;
        }
        else
        {
            sm = j;
        }
        if(heap[i] < heap[sm])
        {
            return ;
        }
        tmp = heap[i];
        heap[i] = heap[sm];
        heap[sm] = tmp;
        i = sm;
    }
}

void heapify(int *heap, int n)
{
    int i = n / 2;
    for(i; i >= 0; i--)
    {
        sift(heap, i, n);
    }
//    while(n-1 > 0)
//    {
//        int tmp = heap[n-1];
//        heap[n-1] = heap[0];
//        heap[0] = tmp;
//        sift(heap, 0, n-1);
//        n--;
//    }
}

void heap_sort(int *heap, int n)
{
    int i = n / 2;
    for(i; i >= 0; i--)
    {
        sift(heap, i, n);
    }
    int j = 0, k ,m = n;
    while(n-1 > 0)
    {
//        int tmp = heap[n-1];
//        heap[n-1] = heap[0];
//        heap[0] = tmp;
        printf("%d %d in sorting\n", j, n);
        for(k = 0; k < m; k++)
        {
            printf("%d ", heap[k]);
        }
        printf("\n");
        heapify(heap + j, n);
        for(k = 0; k < m; k++)
        {
            printf("%d ", heap[k]);
        }
        printf("----\n");
        j++;
        n--;
    }
}

