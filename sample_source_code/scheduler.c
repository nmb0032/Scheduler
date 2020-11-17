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
    task_t finish_task_list[MAX_TASK_NUM];
    task_t ready_queue[MAX_TASK_NUM];
    char *algo_type; /*Stores algorithm type*/

    int error_code;
    u_int clock = 0; /*clock*/
    u_int quantum;
    u_int task_count;
    int ready_count = 0;
    int finishtask_count = 0;

    if (argc != 2) {
        printf("Usage: input <file_name>\n");
        return EXIT_FAILURE;
    }
    if (argc == 3) {
        algo_type = argv[2];
        quantum = argv[3];
    }
    else
    {
        algo_type = argv[2];
    }

    error_code = open_file(argv[1], &fp);
    if (error_code == 1)
        return EXIT_FAILURE;

    read_file(fp, task_array, &task_count);
    print_task_list(task_array, task_count);
    selection_sort(task_array, task_count);
    while(task_count != 0 || ready_count != 0)
    {
        task_t ran;
        for(int i = 0; i >= task_count; i++)
        {
            if (task_array[i].arrival_time <= clock)
            {
                task_array[i].remaining_time = task_array[i].burst_time;
                task_t temp = task_array[i];
                shift(task_array, i, task_count);
                task_count--;
                ready_queue[ready_count] = temp;
                ready_count++;
            }
            else break;
        }
        task_t temp = ready_queue[0];
        if (temp.remaining_time == temp.burst_time)
        {
             temp.arrival_time = clock;
        }
        u_int index;
        if(algo_type == "FCFS")
        {
            index = FCFS(ready_queue, ready_count);
        }
        else if (algo_type == "RR")
        {
            index = RRB(ready_queue, ready_count, quantum);
        }
        else
        {
            index = SRTF(ready_queue, ready_count);
        }

        if(ready_queue[index].remaining_time == 0)
        {
            printf("<time %d> process %d is finished...",clock,ready_queue[index].pid;
            ready_queue[index].finish_time = clock;
            finish_task_list[finishtask_count] = ready_queue[index];
            shift(ready_queue, index, ready_count);
            ready_count--;
            finishtask_count++;
        }
        else
        {
            printf("<time %d> process %d is running",clock,ready_queue[index].pid);
        }
        
        clock ++;
    }

    //to do compute stat info function

    //to do display stat info function



    fclose(fp);
    return EXIT_SUCCESS;
}

u_int RRB(task_t task_list[], int size, int quantum)
{
    task_list[0].burst_time / quantum 

}

u_int FCFS(task_t task_list[], int size)
{
    task_list[0].remaining_time--;
    return 0;
}

u_int SRTF(task_t task_list[], int size)
{
    u_int shortest_index;
    for(int i = 0; i < size; i++)
    {
        if(i == 0)
        {
            shortest_index = i;
        }
        else if(task_list[i].burst_time < task_list[shortest_index].burst_time)
        {
            shortest_index = i;
        }
    }
    task_list[shortest_index].remaining_time--;
    return shortest_index;
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
    return task_list;
}

void shift(task_t *task_list, int var,int size)
{
    for (int j = size; j > var; j--)
    {        
    task_list[j] = task_list[j-1];
    }
}