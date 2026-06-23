#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burst;
    int waiting;
    int turnaround;
} Process;

int main(void) {
    int n;
    double total_waiting = 0.0, total_turnaround = 0.0;

    printf("First-Come, First-Served (FCFS) Scheduling\n");
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
        if (scanf("%d", &p[i].burst) != 1 || p[i].burst < 0) {
            printf("Invalid burst time.\n");
            free(p);
            return 1;
        }
    }

    p[0].waiting = 0;
    p[0].turnaround = p[0].burst;

    for (int i = 1; i < n; i++) {
        p[i].waiting = p[i - 1].waiting + p[i - 1].burst;
        p[i].turnaround = p[i].waiting + p[i].burst;
    }

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;
        printf("P%d\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].burst, p[i].waiting, p[i].turnaround);
    }

    printf("\nAverage waiting time: %.2f ms\n", total_waiting / n);
    printf("Average turnaround time: %.2f ms\n", total_turnaround / n);

    free(p);
    return 0;
}
