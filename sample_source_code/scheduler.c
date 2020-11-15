/*
 *  Nicholas Belvin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduler.h"
#include "print.h"
#include "open.h"
#include "read.h"

int RRB(task_t task_list[], int size, int quantum);
int FCFS(task_t task_list[], int size);
int SRTF(task_t task_list[], int size);
int selection_sort(task_t task_list[], int size);

int main( int argc, char *argv[] )  {
    char *file_name; /* file name from the commandline */
    FILE *fp; /* file descriptor */
    task_t task_array[MAX_TASK_NUM];

    int error_code;
    u_int i;
    u_int count;

    if (argc != 2) {
        printf("Usage: input <file_name>\n");
        return EXIT_FAILURE;
    }

    error_code = open_file(argv[1], &fp);
    if (error_code == 1)
        return EXIT_FAILURE;

    read_file(fp, task_array, &count);
    print_task_list(task_array, count);
    selection_sort(task_array, count);

    fclose(fp);
    return EXIT_SUCCESS;
}

int RRB(task_t task_list[], int size, int quantum)
{
    int* bt_remain;
    bt_remain = malloc(size * sizeof(int));
    for(int i = 0; i < size; i++)
    {
        bt_remain[i] = task_list[i].burst_time;
    }

    int time = 0;

    while(1)
    {
        int done = 1;

        for (int i = 0; i < size; i++)
        {
            if(bt_remain[i] > 0)
            {
                done = 0;

                if (bt_remain[i] > quantum)
                {
                    return 1;
                }
            }
        }
    }


    
    return 0;
}

int FCFS(task_t task_list[], int size)
{
    u_int time = 0;

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < task_list[i].burst_time; j ++)
        {
            if(j != task_list[i].burst_time - 1)
            {
                printf("<time %d> process %d is running", time, task_list[i].pid);
            }
            else
            {
                printf("<time %d> process %d is finished", time, task_list[i].pid);
            }
            time++;

        }

    }
    return 1;
}

int SRTF(task_t task_list[], int size)
{
    return 0;
}

int selection_sort(task_t task_list[], int size)
{
    for (int i = 0; i < size-1; i++)
    {
        int min = i;
        for(int j = i+1; j < size - 1; j++)
        {
            if(task_list[j].arrival_time < task_list[min].arrival_time)
            { 
                min = j;
                task_t temp = task_list[i];
                task_list[i] = task_list[j];
                task_list[j] = temp;
            }
        }
    }
    return 1;
}