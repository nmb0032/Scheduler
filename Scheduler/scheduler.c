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

u_int RRB(task_t task_list[], int size, int quantum);
u_int FCFS(task_t task_list[], int size);
u_int SRTF(task_t task_list[], int size);
void swap(task_t *task_list, int first, int second);
void shift(task_t *task_list, int var,int size);
void compute_stats(task_t task_list[], int size, int process, int tick);
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
    u_int process_clock = 0;
    int quantum;
    u_int task_count = 0;
    int ready_count = 0;
    int finishtask_count = 0;
    if (argc == 1 || argc > 4) {
        printf("Usage: command file_name [FCFS|RR|SRFT] [time_quantum]\n");
        return 0;
    }
    /* Handle the RR policy */
    if (strcmp(argv[2], "RR") == 0) {
        if (argc == 4)
            printf("time_quantum is set to %s\n", argv[3]);  
        else printf("Please enter time_quantum for the RR policy!\n");
        quantum = atoi(argv[3]);
        algo_type = argv[2];
    }
    else
    {
        algo_type = argv[2];
    }

    file_name = argv[1];
    if (! (fp = fopen(file_name, "r"))) {
        printf("File %s can't be opened. Please retry ...\n");
        return EXIT_FAILURE;
    }


    read_file(fp, task_array, &task_count);
    if(strcmp(algo_type,"RR") == 0)
    {
        for(int i = 0; i < task_count; i++)
        {
            task_array[i].quantum_time = quantum;
        }
    }
    selection_sort(task_array, task_count);
    print_task_list(task_array,task_count);
    printf("Usage: scheduler task_list_file [FCFS|RR|SRTF] [time_quantum]\n\n\
Scheduling Policy: %s\n There are %d tasks loaded from \"%s\".\
                Press any key to continue ...\n\
==================================================================\n"\
                , algo_type, task_count, file_name);

    while(task_count != 0 || ready_count != 0)
    {
        task_t ran;
        for(int i = 0; i < task_count; i++)
        {
            //print_task_list(task_array, task_count);
            if (task_array[i].arrival_time <= clock)
            {
                task_array[i].waiting_time = 0;
                task_array[i].remaining_time = task_array[i].burst_time;
                task_t temp = task_array[i];
                ready_queue[ready_count] = temp;
                ready_count++;
                shift(task_array, i, task_count);
                task_count--;
            }
            else break;

        }
        for(int i = 0; i < ready_count; i++)
        {
            if (ready_queue[i].remaining_time == ready_queue[i].burst_time)
            {
                    ready_queue[i].arrival_time = clock;
                    ready_queue[i].waiting_time++;
            }
        }
        //print_task_list(ready_queue,ready_count);
        if (ready_count > 0)
        {
            u_int index;
            if(strcmp(algo_type, "FCFS") == 0)
            {
                index = FCFS(ready_queue, ready_count);
            }
            else if (strcmp(algo_type, "RR") == 0)
            {
                index = RRB(ready_queue, ready_count, quantum);
            }
            else if (strcmp(algo_type,"SRTF") == 0)
            {
                index = SRTF(ready_queue, ready_count);
            }
            else
            {
                printf("Incorrect algotype: %s\n",algo_type);
                return EXIT_FAILURE;
            }
            
            if(ready_queue[index].remaining_time == 0)
            {
                printf("<time %d> process %d is finished...\n",clock,ready_queue[index].pid);
                ready_queue[index].finish_time = clock;
                finish_task_list[finishtask_count] = ready_queue[index];
                shift(ready_queue, index, ready_count);
                ready_count--;
                finishtask_count++;
            }
            else
            {
                printf("<time %d> process %d is running\n",clock,ready_queue[index].pid);
            }
            process_clock++;
        }

        clock ++;
    }
    printf("<time %d> All processes finish ......\n",clock);

    //to do compute stat info function
    compute_stats(finish_task_list, finishtask_count, process_clock, clock);
    //to do display stat info function


    fclose(fp);
    return EXIT_SUCCESS;
}

u_int RRB(task_t task_list[], int size, int quantum)
{
    if(task_list[0].quantum_time == 0)
    {
        task_list[0].quantum_time = quantum;
        shift(task_list, 0, size);
    }
    task_list[0].quantum_time--;
    task_list[0].remaining_time--;
    return 0;
}

u_int FCFS(task_t task_list[], int size)
{
    task_list[0].remaining_time--;
    return 0;
}

u_int SRTF(task_t task_list[], int size)
{
    u_int shortest_index = 0;
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
    return 1;
}

void shift(task_t *task_list, int var,int size)
{   task_t temp = task_list[var];
    for (int j = var; j < size; j++)
    {        
        task_list[j] = task_list[j+1];
    }
    task_list[size - 1] = temp;
}

void swap(task_t *task_list, int first, int second)
{
    task_t temp = task_list[first];
    task_list[first] = task_list[second];
    task_list[second] = temp;
}

void compute_stats(task_t task_list[], int size, int process, int tick)
{
    double avg_turn_around = 0;
    double avg_response_time = 0;
    double avg_waiting_time = 0;
    for(int i = 0; i < size; i++)
    {
        avg_turn_around += (double)task_list[i].waiting_time + task_list[i].finish_time - task_list[i].arrival_time;
        avg_waiting_time += (double)task_list[i].waiting_time;
        avg_response_time += (double)task_list[i].finish_time - task_list[i].arrival_time;
    }
    avg_turn_around = avg_turn_around/size;
    avg_response_time = avg_response_time/size;
    avg_waiting_time = avg_waiting_time/size;
    double cpu_usage = (double)(process/tick) * 100;
    printf("============================================================\n\
    Average Waiting time: %.2f\n\
    Average Response time: %.2f\n\
    Average turnaround time: %.2f\n\
    Overall CPU usage: %.2f \%\n\
============================================================\n",\
            avg_waiting_time, avg_response_time, avg_turn_around, cpu_usage);
}