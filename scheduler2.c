#include stdio.h

 ---------- FCFS ----------
void fcfs() {
    int n, i;

    printf(Enter number of processes );
    scanf(%d, &n);

    int bt[n], wt[n], tat[n];

    printf(Enter burst timesn);
    for(i = 0; i  n; i++) {
        printf(P%d , i+1);
        scanf(%d, &bt[i]);
    }

    wt[0] = 0;
    for(i = 1; i  n; i++) {
        wt[i] = wt[i-1] + bt[i-1];
    }

    for(i = 0; i  n; i++) {
        tat[i] = wt[i] + bt[i];
    }

    printf(nProcesstBTtWTtTATn);
    for(i = 0; i  n; i++) {
        printf(P%dt%dt%dt%dn, i+1, bt[i], wt[i], tat[i]);
    }

    printf(nGantt Chartn);
    int time = 0;
    printf( 0 );
    for(i = 0; i  n; i++) {
        time += bt[i];
        printf( P%d  %d , i+1, time);
    }
    printf(n);
}

 ---------- SJF ----------
void sjf() {
    int n, i, j;

    printf(Enter number of processes );
    scanf(%d, &n);

    int bt[n], wt[n], tat[n];

    printf(Enter burst timesn);
    for(i = 0; i  n; i++) {
        printf(P%d , i+1);
        scanf(%d, &bt[i]);
    }

     Sort burst times
    for(i = 0; i  n; i++) {
        for(j = i+1; j  n; j++) {
            if(bt[i]  bt[j]) {
                int temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;
            }
        }
    }

    wt[0] = 0;
    for(i = 1; i  n; i++) {
        wt[i] = wt[i-1] + bt[i-1];
    }

    for(i = 0; i  n; i++) {
        tat[i] = wt[i] + bt[i];
    }

    printf(nProcesstBTtWTtTATn);
    for(i = 0; i  n; i++) {
        printf(P%dt%dt%dt%dn, i+1, bt[i], wt[i], tat[i]);
    }

    printf(nGantt Chartn);
    int time = 0;
    printf( 0 );
    for(i = 0; i  n; i++) {
        time += bt[i];
        printf( P%d  %d , i+1, time);
    }
    printf(n);
}

 ---------- Round Robin ----------
void roundRobin() {
    int n, i, time = 0, remain, tq;

    printf(Enter number of processes );
    scanf(%d, &n);

    int bt[n], rt[n], wt[n], tat[n];

    printf(Enter burst timesn);
    for(i = 0; i  n; i++) {
        printf(P%d , i+1);
        scanf(%d, &bt[i]);
        rt[i] = bt[i];
    }

    printf(Enter time quantum );
    scanf(%d, &tq);

    remain = n;

    printf(nGantt Chartn);

    while(remain != 0) {
        for(i = 0; i  n; i++) {
            if(rt[i]  0) {
                if(rt[i] = tq) {
                    time += rt[i];
                    rt[i] = 0;
                    remain--;
                    printf( P%d , i+1);
                } else {
                    rt[i] -= tq;
                    time += tq;
                    printf( P%d , i+1);
                }
            }
        }
    }

     Calculate WT & TAT
    int totalTime = 0;
    for(i = 0; i  n; i++) {
        totalTime += bt[i];
        tat[i] = totalTime;
        wt[i] = tat[i] - bt[i];
    }

    printf(nnProcesstBTtWTtTATn);
    for(i = 0; i  n; i++) {
        printf(P%dt%dt%dt%dn, i+1, bt[i], wt[i], tat[i]);
    }
}

 ---------- MAIN ----------
int main() {
    int choice;

    do {
        printf(n==============================);
        printf(n CPU Scheduling Simulator);
        printf(n==============================);
        printf(n1. FCFS);
        printf(n2. SJF);
        printf(n3. Round Robin);
        printf(n4. Exit);
        printf(nEnter your choice );
        scanf(%d, &choice);

        switch(choice) {
            case 1
                fcfs();
                break;
            case 2
                sjf();
                break;
            case 3
                roundRobin();
                break;
            case 4
                printf(Exiting...n);
                break;
            default
                printf(Invalid choice!n);
        }

    } while(choice != 4);

    return 0;
}