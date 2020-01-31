#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

int nodes_no = 0,cinf = 0,tq,cpu = 0,ocnt = 0,ecnt = 0;

int *wt;
int *tat;
int oe[50];
int *cs;

struct node{
    int node_no;
    int bt;
    int pid;
    struct node *next;
} *front=NULL,*rear=NULL;

struct node *octmline;

void ins_oc(struct node *m){
    *(octmline+ocnt) = *m;
    ocnt+=1;
}

void delay(int secs){
    int ms = 1000 * secs;
    clock_t starttime = clock();
    while(clock() < starttime + ms);
}

void tat_disp(){
    int p;
    struct node *x=front;
    printf("\nEnter process no. :");
    scanf("%d",&p);
    if(p<0 || p>nodes_no){
        printf("\nWrong process no. Exiting.\n");
        return;
    }
    for(int i=1;i<p;i++)
        x=x->next;
    printf("\nExecution time of process with Process_ID : %d = %d\n",x->pid,*(tat+p-1));
}


void disp_wt_proc(){
    int p;
    struct node *x=front;
    printf("\nEnter process no. :");
    scanf("%d",&p);
    if(p<0 || p > nodes_no ){
        printf("\nWrong process no. Exiting.\n");
        return;
    }
    for(int i=1;i<p;i++)
        x=x->next;
    printf("\nWaiting time of process with Process_ID :%d = %d\n",x->pid,*(wt+p-1));
}

void avg_wt(){
    int s=0;
    for(int j=0;j<nodes_no;j++)
        s=s+*(wt+j);
    printf("\nAverage waiting time for each process: %d\n",s/nodes_no);
}



void disp_oc(){

    for(int i=0;i<ocnt;i++)
        printf("Serial no:%d\tNode:%d\tProcess_ID:%d\tTurn around time:%d\n",i+1,(octmline+i)->node_no,(octmline+i)->pid,tat[(octmline+i)->node_no-1]);

}

struct node* pop(){
    struct node *q=front;
    front=front->next;
    return q;
}

void disp_oe(){
    for(int j=0;j<ecnt;j++)
        printf("%d,",oe[j]);
    printf("\n");
}

void disp_cs(){

    for(int i=0;i<ocnt;i++){
        int nn = (octmline+i)->node_no;
        int cnt = 0;
        for(int j=0;j<ecnt;j++){
            if(nn == oe[j])
                cnt++;
        }
        cs[nn-1] = cnt - 1;
    printf("Serial no.:%d\tNode:%d\tProcess_ID:%d\tNo. of context switches:%d\n",i+1,nn,(octmline+nn-1)->pid,cs[nn-1]);
    }
}

void compute(){
    wt = (int *)calloc(nodes_no,sizeof(int));
    tat = (int *)calloc(nodes_no,sizeof(int));
    octmline = (struct node *)malloc(nodes_no*sizeof(octmline));
    cs = (int *)calloc(nodes_no,sizeof(int));
    while(cinf!=nodes_no){

        //Extracting the node with min. arrival time
       /* struct node *q=front;
        struct node *m=front;
        int min = q->at;
        do{
            if (min > q->at){
                min = q->at;
                m = q;
            }
            q=q->next;

        }while(q!=rear->next);*/
        struct node *m = pop();
        if(m->bt==0)
            continue;

        if(m->bt < tq){
            wt[m->node_no -1] = wt[m->node_no -1] + cpu;
            if(wt[m->node_no - 1] < 0)
                wt[m->node_no - 1] = 0;
            cpu = cpu + tq - m->bt;
            tat[m->node_no -1] = tat[m->node_no -1] + tq - m->bt;
            m->bt = 0;
            //m->at = MAX_INT;
            cinf+=1;
            oe[ecnt++] = m->node_no;
            ins_oc(m);
        }

         else if(m->bt == tq){
            wt[m->node_no - 1]= wt[m->node_no - 1] + cpu;
            if(wt[m->node_no - 1] < 0)
                wt[m->node_no - 1] = 0;
            cpu = cpu + tq;
            tat[m->node_no - 1] = tat[m->node_no - 1] + tq;
            m->bt=0;
            //m->at=MAX_INT;
            cinf+=1;
            oe[ecnt++] = m->node_no;
            ins_oc(m);
        }

        else{
            wt[m->node_no - 1]= wt[m->node_no - 1] + cpu;
            if(wt[m->node_no - 1] < 0)
                wt[m->node_no - 1] = 0;
            cpu = cpu + tq;
            tat[m->node_no - 1] = tat[m->node_no - 1] + tq;
            m->bt = m->bt - tq;
            //m->at = cpu;
            oe[ecnt++] = m->node_no;
        }
        delay(tq);

    }

    printf("\nComputation complete.\n");
}



void update_nodeIndex(){
    struct node *q = front;
    do{
        q->node_no-=1;
        q=q->next;
    }while(q!=rear->next);
}

void push(int pid,int bt){
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->pid = pid;
    temp->bt = bt;
    temp->next = NULL;
    temp->node_no = nodes_no+1;
    nodes_no+=1;
    if(nodes_no==1)
        front = rear = temp;
    else{
        struct node *q = rear;
        q->next = temp;
        rear = temp;
    }
    rear->next = front;
    printf("\nProcess queued.\n");
}



void rem(){
    struct node *q=front;
    if(front==NULL){
        printf("\nQueue empty.\n");
        return;
    }
    else if(front == rear){
        free(q);
        front = rear = NULL;
        printf("\nDeletion successful.\n");
        return;
    }
    else{
        front = front->next;
        rear->next = front;
        free(q);
    }
    nodes_no-=1;
    update_nodeIndex();
    printf("\nDeletion successful.\n");

}

void disp_proc(){
    struct node *q=front;
    if(q==NULL){
        printf("\nQueue empty.\n");
        return;
    }
    do{
        printf("\nNode:%d\tProcess ID:%d\tBurst Time:%d",q->node_no,q->pid,q->bt);
        q = q->next;
    }while(q!=rear->next);
    printf("\n");
}

int main(){

    int pid,bt,choice;
    printf("\nEnter value of time quanta:");
    scanf("%d",&tq);
    while(1){
        printf("\n1.Insert process in queue\n2.Remove process from queue\n3.Display all process(s)\n4.Display average waiting time\n"
                "5.Display total CPU time (Execution time)\n6.Display execution time of a process\n7.Display waiting time of a process\n"
                "8.Display order of execution\n9.Display order of completion\n10.Display context switch details\n11.Compute\n12.Exit\nEnter your choice:");
        scanf("%d",&choice);
        switch(choice){
            case 1:printf("\nEnter process ID & Burst Time:");
                   scanf("%d%d",&pid,&bt);
                   push(pid,bt);
                   break;
            case 2:rem();
                   break;
            case 3:disp_proc();
                   break;
            case 4:avg_wt();
                   break;
            case 5:printf("\nTotal CPU time:%d\n",cpu);
                   break;
            case 6:tat_disp();
                   break;
            case 7:disp_wt_proc();
                   break;
            case 8:disp_oe(); 
                   break;
            case 9:disp_oc();
                   break;
            case 10:disp_cs();
                   break;
            case 11:compute();
                    break;
            case 12:exit(0);
            default:printf("\nWrong choice.Try again.\n");
        }
    }
    free(wt);
    free(tat);
    free(octmline);
    free(cs);
    return 0;

}