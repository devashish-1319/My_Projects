#include <stdio.h>

struct Process {
    char name;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completed; // To track if already completed
};

void round_robin_scheduler(struct Process processes[], int num_processes, int time_slice) {
    int current_time = 0;
    int completed_processes = 0;
    int queue[100]; // To store indices of processes
    int front = 0, rear = 0;
    int in_queue[100] = {0}; // Flags to prevent duplicate enqueuing

    while (completed_processes < num_processes) {
        // Enqueue newly arrived processes
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                !in_queue[i]) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (front == rear) {
            // No process is ready to execute, move time forward
            current_time++;
            continue;
        }

        int index = queue[front++];
        struct Process *p = &processes[index];

        if (p->remaining_time <= time_slice) {
            current_time += p->remaining_time;
            p->remaining_time = 0;
            printf("Process %c completed at time %d\n", p->name, current_time);
            completed_processes++;
        } else {
            current_time += time_slice;
            p->remaining_time -= time_slice;
            printf("Process %c is preempted at time %d\n", p->name, current_time);
        }

        // Enqueue new arrivals that came during this time
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time > current_time - time_slice &&
                processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0 &&
                !in_queue[i]) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        // If not completed, enqueue it back
        if (p->remaining_time > 0) {
            queue[rear++] = index;
        } else {
            in_queue[index] = 0; // Allow future entry if needed (not needed here but good practice)
        }
    }
}

int main() {
    struct Process processes[] = {
        {'P', 0, 5, 5},
        {'Q', 1, 4, 4},
        {'R', 2, 2, 2},
        {'S', 3, 1, 1}
    };
    int num_processes = sizeof(processes) / sizeof(processes[0]);
    int time_slice = 2;

    printf("Round Robin Scheduling Example:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %c (Arrival Time: %d, Burst Time: %d)\n",
               processes[i].name, processes[i].arrival_time, processes[i].burst_time);
    }

    printf("Time Slice: %d\n", time_slice);
    round_robin_scheduler(processes, num_processes, time_slice);

    return 0;
}
