#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int id;
    int burst;
    int tickets;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
} Process;

static int total_active_tickets(Process p[], int n) {
    int total = 0;

    for (int i = 0; i < n; i++) {
        if (p[i].remaining > 0) {
            total += p[i].tickets;
        }
    }

    return total;
}

static int choose_winner(Process p[], int n, int draw) {
    int sum = 0;

    for (int i = 0; i < n; i++) {
        if (p[i].remaining > 0) {
            sum += p[i].tickets;
            if (draw <= sum) {
                return i;
            }
        }
    }

    return -1;
}

static void copy_processes(Process destination[], Process source[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
        destination[i].remaining = source[i].burst;
        destination[i].completion = 0;
        destination[i].waiting = 0;
        destination[i].turnaround = 0;
    }
}

int main(void) {
    int n, quantum, runs;
    Process *base = NULL;

    printf("Lottery Scheduling\n");
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    base = malloc((size_t)n * sizeof(Process));
    if (base == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        base[i].id = i + 1;
        printf("Enter burst time and tickets for P%d: ", base[i].id);
        if (scanf("%d %d", &base[i].burst, &base[i].tickets) != 2 ||
            base[i].burst <= 0 || base[i].tickets <= 0) {
            printf("Invalid burst time or ticket count.\n");
            free(base);
            return 1;
        }
        base[i].remaining = base[i].burst;
        base[i].completion = 0;
        base[i].waiting = 0;
        base[i].turnaround = 0;
    }

    printf("Enter time quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Invalid time quantum.\n");
        free(base);
        return 1;
    }

    printf("Enter number of runs: ");
    if (scanf("%d", &runs) != 1 || runs <= 0) {
        printf("Invalid number of runs.\n");
        free(base);
        return 1;
    }

    srand((unsigned int)time(NULL));

    for (int run = 1; run <= runs; run++) {
        int completed = 0;
        int time = 0;
        double total_waiting = 0.0, total_turnaround = 0.0;

        Process *p = malloc((size_t)n * sizeof(Process));
        if (p == NULL) {
            printf("Memory allocation failed.\n");
            free(base);
            return 1;
        }
        copy_processes(p, base, n);

        printf("\nRun %d execution log:\n", run);

        while (completed < n) {
            int total_tickets = total_active_tickets(p, n);
            int draw = (rand() % total_tickets) + 1;
            int winner = choose_winner(p, n, draw);
            int run_time;

            if (winner == -1) {
                printf("Internal scheduler error.\n");
                free(p);
                free(base);
                return 1;
            }

            run_time = p[winner].remaining < quantum ? p[winner].remaining : quantum;
            printf("Time %d-%d: draw=%d/%d, winner=P%d\n",
                   time, time + run_time, draw, total_tickets, p[winner].id);

            p[winner].remaining -= run_time;
            time += run_time;

            if (p[winner].remaining == 0) {
                p[winner].completion = time;
                p[winner].turnaround = p[winner].completion;
                p[winner].waiting = p[winner].turnaround - p[winner].burst;
                completed++;
            }
        }

        printf("\nProcess\tBurst\tTickets\tCompletion\tWaiting\tTurnaround\n");
        for (int i = 0; i < n; i++) {
            total_waiting += p[i].waiting;
            total_turnaround += p[i].turnaround;
            printf("P%d\t%d\t%d\t%d\t\t%d\t%d\n",
                   p[i].id, p[i].burst, p[i].tickets, p[i].completion,
                   p[i].waiting, p[i].turnaround);
        }

        printf("Average waiting time: %.2f ms\n", total_waiting / n);
        printf("Average turnaround time: %.2f ms\n", total_turnaround / n);

        free(p);
    }

    free(base);
    return 0;
}
