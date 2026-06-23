# CPU Scheduling Lab Implementations

This folder contains separate C implementations for the scheduling algorithms in `OS_Lab_Ex_Scheduling.docx`.

## Files

- `fcfs.c` - First-Come, First-Served scheduling
- `sjf.c` - Non-preemptive Shortest Job First scheduling
- `srtn.c` - Preemptive Shortest Remaining Time Next scheduling
- `round_robin.c` - Round Robin scheduling with configurable time quantum
- `lottery.c` - Lottery Scheduling with configurable tickets, quantum, and repeated runs

## Compile

```sh
gcc -std=c11 -Wall -Wextra -pedantic fcfs.c -o fcfs
gcc -std=c11 -Wall -Wextra -pedantic sjf.c -o sjf
gcc -std=c11 -Wall -Wextra -pedantic srtn.c -o srtn
gcc -std=c11 -Wall -Wextra -pedantic round_robin.c -o round_robin
gcc -std=c11 -Wall -Wextra -pedantic lottery.c -o lottery
```

## Run

```sh
./fcfs
./sjf
./srtn
./round_robin
./lottery
```

## Sample Test Cases

Each test case uses five processes.

### FCFS

Run:

```sh
./fcfs
```

Input:

```text
5
10
4
6
2
8
```

Expected summary:

```text
Average waiting time: 13.20 ms
Average turnaround time: 19.20 ms
```

### SJF

Use the same burst times as FCFS to compare the average waiting time.

Run:

```sh
./sjf
```

Input:

```text
5
10
4
6
2
8
```

Expected execution order:

```text
P4 -> P2 -> P3 -> P5 -> P1
```

Expected summary:

```text
Average waiting time: 8.00 ms
Average turnaround time: 14.00 ms
```

### SRTN

This input uses different arrival times to demonstrate preemption.

Run:

```sh
./srtn
```

Input:

```text
5
0 8
1 4
2 9
3 5
4 2
```

Expected summary:

```text
P1 completion=19, waiting=11, turnaround=19
P2 completion=5, waiting=0, turnaround=4
P3 completion=28, waiting=17, turnaround=26
P4 completion=12, waiting=4, turnaround=9
P5 completion=7, waiting=1, turnaround=3
Average waiting time: 6.60 ms
Average turnaround time: 12.20 ms
```

### Round Robin

Use the same five burst times with two different quantum values.

Run:

```sh
./round_robin
```

Input with quantum `2`:

```text
5
10
4
6
2
8
2
```

Expected summary for quantum `2`:

```text
Average waiting time: 14.40 ms
Average turnaround time: 20.40 ms
```

Input with quantum `4`:

```text
5
10
4
6
2
8
4
```

Expected summary for quantum `4`:

```text
Average waiting time: 14.80 ms
Average turnaround time: 20.80 ms
```

### Lottery Scheduling

Lottery Scheduling is probabilistic, so the exact output changes from run to run. Set the number of runs to `3` to show different outcomes using the same input.

Run:

```sh
./lottery
```

Input:

```text
5
10 10
6 20
4 5
8 15
5 25
2
3
```

Input meaning:

```text
P1: burst=10, tickets=10
P2: burst=6, tickets=20
P3: burst=4, tickets=5
P4: burst=8, tickets=15
P5: burst=5, tickets=25
Time quantum=2
Runs=3
```
