#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
} Process;

int main(void) {
    int n, quantum;
    int completed = 0;
    int time = 0;
    double total_waiting = 0.0, total_turnaround = 0.0;

    printf("Round Robin Scheduling\n");
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    Process *p = malloc((size_t)n * sizeof(Process));
    if (p == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter burst time for P%d: ", p[i].id);
        if (scanf("%d", &p[i].burst) != 1 || p[i].burst <= 0) {
            printf("Invalid burst time.\n");
            free(p);
            return 1;
        }
        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].waiting = 0;
        p[i].turnaround = 0;
    }

    printf("Enter time quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Invalid time quantum.\n");
        free(p);
        return 1;
    }

    printf("\nExecution order:\n");

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0) {
                int run_time = p[i].remaining < quantum ? p[i].remaining : quantum;

                printf("P%d (%d-%d)\n", p[i].id, time, time + run_time);
                p[i].remaining -= run_time;
                time += run_time;

                if (p[i].remaining == 0) {
                    p[i].completion = time;
                    p[i].turnaround = p[i].completion;
                    p[i].waiting = p[i].turnaround - p[i].burst;
                    completed++;
                }
            }
        }
    }

    printf("\nProcess\tBurst Time\tCompletion\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].burst, p[i].completion,
               p[i].waiting, p[i].turnaround);
    }

    printf("\nAverage waiting time: %.2f ms\n", total_waiting / n);
    printf("Average turnaround time: %.2f ms\n", total_turnaround / n);

    free(p);
    return 0;
}
