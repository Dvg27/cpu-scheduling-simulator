#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

// ---------- Utility ----------
void avg(int wt[], int tat[], int n, float *awt, float *atat) {
    *awt = 0; *atat = 0;
    for(int i=0;i<n;i++) {
        *awt += wt[i];
        *atat += tat[i];
    }
    *awt /= n;
    *atat /= n;
}

// ---------- FCFS ----------
void fcfs(int bt[], int n, float *awt, float *atat) {
    int wt[MAX], tat[MAX];
    wt[0]=0;
    for(int i=1;i<n;i++) wt[i]=wt[i-1]+bt[i-1];
    for(int i=0;i<n;i++) tat[i]=wt[i]+bt[i];
    avg(wt,tat,n,awt,atat);
}

// ---------- SJF ----------
void sjf(int bt[], int n, float *awt, float *atat) {
    int temp[MAX], wt[MAX], tat[MAX];

    for(int i=0;i<n;i++) temp[i]=bt[i];

    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(temp[i]>temp[j]) {
                int t=temp[i]; temp[i]=temp[j]; temp[j]=t;
            }

    wt[0]=0;
    for(int i=1;i<n;i++) wt[i]=wt[i-1]+temp[i-1];
    for(int i=0;i<n;i++) tat[i]=wt[i]+temp[i];

    avg(wt,tat,n,awt,atat);
}

// ---------- Round Robin ----------
void rr(int bt[], int n, int tq, float *awt, float *atat) {
    int rem[MAX], wt[MAX]={0}, tat[MAX];
    int t=0;

    for(int i=0;i<n;i++) rem[i]=bt[i];

    int done;
    while(1) {
        done=1;
        for(int i=0;i<n;i++) {
            if(rem[i]>0) {
                done=0;
                if(rem[i]>tq) {
                    t+=tq;
                    rem[i]-=tq;
                } else {
                    t+=rem[i];
                    wt[i]=t-bt[i];
                    rem[i]=0;
                }
            }
        }
        if(done) break;
    }

    for(int i=0;i<n;i++) tat[i]=bt[i]+wt[i];
    avg(wt,tat,n,awt,atat);
}

// ---------- SRTF ----------
void srtf(int bt[], int n, float *awt, float *atat) {
    int rem[MAX], wt[MAX]={0}, tat[MAX];
    int complete=0, t=0;

    for(int i=0;i<n;i++) rem[i]=bt[i];

    while(complete<n) {
        int min=9999, idx=-1;
        for(int i=0;i<n;i++) {
            if(rem[i]>0 && rem[i]<min) {
                min=rem[i];
                idx=i;
            }
        }
        if(idx==-1) { t++; continue; }

        rem[idx]--;
        if(rem[idx]==0) {
            complete++;
            int finish=t+1;
            wt[idx]=finish-bt[idx];
            if(wt[idx]<0) wt[idx]=0;
        }
        t++;
    }

    for(int i=0;i<n;i++) tat[i]=wt[i]+bt[i];
    avg(wt,tat,n,awt,atat);
}

// ---------- Lottery ----------
void lottery(int bt[], int n, float *awt, float *atat) {
    int wt[MAX]={0}, tat[MAX], done[MAX]={0};
    int t=0, completed=0;

    while(completed<n) {
        int idx = rand()%n;
        if(done[idx]) continue;

        t += bt[idx];
        wt[idx] = t - bt[idx];
        tat[idx] = t;
        done[idx]=1;
        completed++;
    }
    avg(wt,tat,n,awt,atat);
}

// ---------- EDF ----------
void edf(int bt[], int deadline[], int n, float *awt, float *atat) {
    int wt[MAX], tat[MAX], order[MAX];

    for(int i=0;i<n;i++) order[i]=i;

    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
            if(deadline[order[i]] > deadline[order[j]]) {
                int t=order[i]; order[i]=order[j]; order[j]=t;
            }

    wt[0]=0;
    for(int i=1;i<n;i++)
        wt[i]=wt[i-1]+bt[order[i-1]];

    for(int i=0;i<n;i++)
        tat[i]=wt[i]+bt[order[i]];

    avg(wt,tat,n,awt,atat);
}

// ---------- MLFQ ----------
void mlfq(int bt[], int n, float *awt, float *atat) {
    float w1,t1,w2,t2;
    rr(bt,n,2,&w1,&t1);
    sjf(bt,n,&w2,&t2);

    *awt = (w1+w2)/2;
    *atat = (t1+t2)/2;
}

// ---------- CFS ----------
void cfs(int bt[], int n, float *awt, float *atat) {
    float sum=0;
    for(int i=0;i<n;i++) sum+=bt[i];

    *awt = sum/(2*n);
    *atat = sum/n + *awt;
}

// ---------- Hybrid ----------
void hybrid(int bt[], int n, float *awt, float *atat) {
    float w1,t1,w2,t2;
    sjf(bt,n,&w1,&t1);
    rr(bt,n,3,&w2,&t2);

    if(w1 < w2) {
        *awt = w1;
        *atat = t1;
    } else {
        *awt = w2;
        *atat = t2;
    }
}

// ---------- MAIN ----------
int main() {
    int n, bt[MAX], deadline[MAX];

    printf("Enter number of processes: ");
    scanf("%d",&n);

    printf("Enter burst times:\n");
    for(int i=0;i<n;i++) {
        printf("P%d: ",i+1);
        scanf("%d",&bt[i]);
        deadline[i] = bt[i] + 5;
    }

    float awt, atat;
    float best_wt = 9999;
    char best_algo[20];

    printf("\n==============================");
    printf("\n All Algorithm Results");
    printf("\n==============================\n");

    // FCFS
    fcfs(bt,n,&awt,&atat);
    printf("FCFS           -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"FCFS"); }

    // SJF
    sjf(bt,n,&awt,&atat);
    printf("SJF            -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"SJF"); }

    // RR
    rr(bt,n,3,&awt,&atat);
    printf("Round Robin    -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"Round Robin"); }

    // SRTF
    srtf(bt,n,&awt,&atat);
    printf("SRTF           -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"SRTF"); }

    // MLFQ
    mlfq(bt,n,&awt,&atat);
    printf("MLFQ           -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"MLFQ"); }

    // EDF
    edf(bt,deadline,n,&awt,&atat);
    printf("EDF            -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"EDF"); }

    // Lottery
    lottery(bt,n,&awt,&atat);
    printf("Lottery        -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"Lottery"); }

    // CFS
    cfs(bt,n,&awt,&atat);
    printf("CFS            -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"CFS"); }

    // Hybrid
    hybrid(bt,n,&awt,&atat);
    printf("Hybrid         -> WT: %.2f  TAT: %.2f\n",awt,atat);
    if(awt < best_wt) { best_wt = awt; strcpy(best_algo,"Hybrid"); }

    // ---------- BEST ----------
    printf("\n==============================");
    printf("\n BEST ALGORITHM");
    printf("\n==============================\n");

    printf("Best Algorithm : %s\n", best_algo);
    printf("Minimum Avg Waiting Time : %.2f\n", best_wt);

    return 0;
}