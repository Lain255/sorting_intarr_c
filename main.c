#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ARR_SIZE 1024
#define MAX_INT 10000
#define AVG_SIZE 10000

void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void insertion_sort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        for (int j = i; j > 0; j--) {
            if(arr[j-1] > arr[j]) {
                swap(arr + j - 1, arr + j); 
            }
            else {
                break;
            }            
        }
    }
}

void selection_sort(int* arr, int n) {
    for (int i = 0; i < n-1; i++) {
        int mini = i;
        int min = arr[i];
        for (int j = i; j < n; j++) {
            if(min > arr[j]) {
                mini = j;
                min  = arr[j];
            }
        }
        swap(arr + i, arr + mini);
    }
}
void selection_sort_r(int* arr, int n) {

}

void heapify_down(int* arr, int n) {
    for(int i = 0; 2*i+1 < n;) {
        int l = i*2+1;
        int r = i*2+2;
        int maxi = i;
        if (l < n && arr[l] > arr[maxi]) {
            maxi = l;
        }
        if (r < n && arr[r] > arr[maxi]) {
            maxi = r;
        }

        if(maxi == i) {
            break;
        }
        else {
            swap(arr + i, arr + maxi);
            i = maxi;
        }
    }
}
void heap_sort(int* arr, int n) {
    //heapify
    for(int i = (n-2)/2; i >= 0; i--) {
        heapify_down(arr + i, n - i);
    }

    //remove max
    for(int N = n-1; N > 0; N--) {
        swap(arr, arr + N);
        heapify_down(arr, N);

    }
}

void merge(int* arr, int* arr_1, int n_1, int* arr_2, int n_2) {
    int i = 0;
    int j = 0;
    int k = 0;

    while(i < n_1 && j < n_2) {
        if(arr_1[i] < arr_2[j]) {
            arr[k] = arr_1[i];
            i++; k++;
        }
        else {
            arr[k] = arr_2[j];
            j++; k++;
        }
    }
    while(i < n_1) {
        arr[k] = arr_1[i];
        i++; k++;
    }
    while(j < n_2) {
        arr[k] = arr_2[j];
        j++; k++;
    }
}
void merge_sort_r(int* arr, int n) {
    if (n == 1) {
        return;
    }
    else {
        int i = 0;
        int j = n/2;
        int k = 0;

        int* arr_cpy = malloc(sizeof(int) * n);
        memcpy(arr_cpy, arr, sizeof(int) * n);

        merge_sort_r(arr_cpy, j);
        merge_sort_r(arr_cpy + j, n - j);

        merge(arr, arr_cpy, j, arr_cpy + j, n - j);

        free(arr_cpy);
    }
}
void merge_sort(int* arr, int n) {
    int* arr_cpy = malloc(sizeof(int) * n);
    memcpy(arr_cpy, arr, sizeof(int) * n);

    int lowest_pow_2 = 1<<(31 - __builtin_clz(n));
    for(int merge_size = 1; merge_size < lowest_pow_2; merge_size <<= 1) {
        for(int i = 0; i*merge_size < lowest_pow_2; i += 2) {
            int i_1 =  i*merge_size;
            int i_2 =  i*merge_size + merge_size;

            merge(arr + i_1, arr_cpy + i_1, merge_size, arr_cpy + i_2, merge_size);
        }

        memcpy(arr_cpy, arr, sizeof(int) * n);
    }
    if(n > lowest_pow_2) {
        merge_sort(arr + lowest_pow_2, n - lowest_pow_2);
        merge(arr, arr_cpy, lowest_pow_2, arr_cpy + lowest_pow_2, n - lowest_pow_2);
        memcpy(arr_cpy, arr, sizeof(int) * n);
    }

    free(arr_cpy);
}


int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
void std_quick_sort(int* arr, int n) {
    qsort(arr, n, sizeof(int), cmpfunc);
}


void randomize_array(int* arr, int n) {
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % MAX_INT;
    }
}
void print_array(int* arr, int n) {
    for(int i = 0; i < n; i++) {
        printf("%i ", arr[i]);
    }
    printf("\n");
}
int is_sorted_array(int* arr, int n) {
    int sorted = 1;
    for(int i = 0; i < n-1; i++) {
        if (arr[i] > arr[i+1]) {
            sorted = 0;
        }
    }
    return sorted;
}

double time_sort(void (*sort)(int*, int), int* arr, int n){
    clock_t start, end;     
    start = clock();
    (*sort)(arr, n);
    end = clock();
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}

double sort_avg_time(void (*sort)(int*, int), int* arr, int n) {
    double avg_rand_time = 0;
    for(int i = 0; i < AVG_SIZE; i++) {
        avg_rand_time += time_sort(randomize_array, arr, n);
    }
    avg_rand_time /= AVG_SIZE;

    double avg_time = 0;
    for(int i = 0; i < AVG_SIZE; i++) {
        randomize_array(arr, n);
        avg_time += time_sort(sort, arr, n);
    }
    avg_time /= AVG_SIZE;
    return avg_time - avg_rand_time;
}

int main() {
    srand(time(0));

    double avg_time;
    int arr[ARR_SIZE];
    int n = ARR_SIZE ;

    randomize_array(arr, n);
    //print_array(arr,n);

    merge_sort(arr,n);
    //print_array(arr,n);
    printf("merge_sort sorts: %i\n", is_sorted_array(arr,n));

    printf("With array size %d, the sorting algorithms take the following times (AVG over %i)\n", ARR_SIZE, AVG_SIZE);
    //printf("INSERTION SORT: %lf\n", sort_avg_time(insertion_sort, arr, n));
    //printf("SELECTION SORT: %lf\n", sort_avg_time(selection_sort, arr, n));
    printf("HEAP      SORT: %lf\n", sort_avg_time(heap_sort, arr, n));
    printf("MERGE_R   SORT: %lf\n", sort_avg_time(merge_sort_r, arr, n));
    printf("MERGE     SORT: %lf\n", sort_avg_time(merge_sort, arr, n));
    printf("STD_QUICK SORT: %lf\n", sort_avg_time(std_quick_sort, arr, n));

}