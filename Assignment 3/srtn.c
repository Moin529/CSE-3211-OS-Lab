#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
} Process;

static int choose_shortest_remaining(Process p[], int n, int time) {
    int best = -1;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival <= time && p[i].remaining > 0) {
            if (best == -1 ||
                p[i].remaining < p[best].remaining ||
                (p[i].remaining == p[best].remaining && p[i].arrival < p[best].arrival) ||
                (p[i].remaining == p[best].remaining &&
                 p[i].arrival == p[best].arrival &&
                 p[i].id < p[best].id)) {
                best = i;
            }
        }
    }

    return best;
}

int main(void) {
    int n;
    int completed = 0;
    int time = 0;
    double total_waiting = 0.0, total_turnaround = 0.0;

    printf("Shortest Remaining Time Next (SRTN) Scheduling\n");
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
        printf("Enter arrival time and burst time for P%d: ", p[i].id);
        if (scanf("%d %d", &p[i].arrival, &p[i].burst) != 2 ||
            p[i].arrival < 0 || p[i].burst <= 0) {
            printf("Invalid arrival or burst time.\n");
            free(p);
            return 1;
        }
        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].waiting = 0;
        p[i].turnaround = 0;
    }

    printf("\nExecution log:\n");

    while (completed < n) {
        int current = choose_shortest_remaining(p, n, time);

        if (current == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining > 0 && p[i].arrival > time &&
                    p[i].arrival < next_arrival) {
                    next_arrival = p[i].arrival;
                }
            }
            printf("Time %d-%d: Idle\n", time, next_arrival);
            time = next_arrival;
            continue;
        }

        printf("Time %d-%d: P%d\n", time, time + 1, p[current].id);
        p[current].remaining--;
        time++;

        if (p[current].remaining == 0) {
            p[current].completion = time;
            p[current].turnaround = p[current].completion - p[current].arrival;
            p[current].waiting = p[current].turnaround - p[current].burst;
            completed++;
        }
    }

    printf("\nProcess\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].completion,
               p[i].waiting, p[i].turnaround);
    }

    printf("\nAverage waiting time: %.2f ms\n", total_waiting / n);
    printf("Average turnaround time: %.2f ms\n", total_turnaround / n);

    free(p);
    return 0;
}
