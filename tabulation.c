#include <stdio.h>
#include<stdlib.h>
#define last 100000

int var,limit=1,nmin,mintermsGiven[last],tabletop,tablecount[1000],maxgrp,nmaxgrp;;
int **arr,**brr;                        //Double Pointers To Form Prime Implicants Table
struct Node                  //node declaration to hold the information about minterms such as number of pairs and number of pairs formed
{
    struct Node* next;        //link to next node in the list
    int flag;                //stores 1 or 0 to denote the state of pairing
    int num1;            //stores the number of ones in the minterm
    int paired[1000];      //struct check to denote other paired minterms
    int group;                  //denote the group according to the number of ones
    int bin[1000];   //stores the binary of the term then formed paired expression
    int numterm;    //stores how many pairs have been formed 4 denoted 2 pairs
}*head,*head1;

typedef struct Node node;
//declarations of function prototype.
node* getnode(int);
void makeList(int);
void binaryFill(node*,node*,node*);
void pair();
void display();
void displayTable();
node* createNodePair(node*,node*);
void iniialiseTable();
int iPairP(node*,node*);
int ifmintermPImp(int,int);
void addPair(node*,node*);
void addToTable();
void analyzeTable();
void cBtMN(int);
int maxinTable(int*);
int noOfImp(int,int*);
void rMfTable(int);

void main()
{
    printf("\t\t\t\t****************Program For Tabulation Method****************\n");
    printf("\t\t1.Output is Minimised Boolean Algebra.\n\t\t2.The Complement is shown by small alphabet.\n\n\n");
    int i,temp;
    maxgrp=nmaxgrp=-1;
    printf("Enter Number Of Variables:");
    scanf("%d",&var);
    for(i=0;i<var;i++)
        limit*=2;
    printf("Enter the number of Minterms...\n");
    scanf("%d",&nmin);
    if(nmin==0){
     printf("Simplified Expression is 0\n");
     return;
    }
    if(nmin==limit){
        printf("No need to give minterms \nSimplified Expression is 1\n");
        return;
    }
    printf("Enter the minterms....\n");
    for(i=0; i<nmin; i++)
    {
        scanf("%d",&temp);
        mintermsGiven[temp]=1;
        makeList(temp);
    }
    arr=(int **)malloc(limit *sizeof(int *));
    brr=(int **)malloc(1000 *sizeof(int *));
    for(i=0;i<limit;i++)
        arr[i]=(int *)malloc(1000 * sizeof(int));
    for(i=0;i<1000;i++)
        brr[i]=(int *)malloc((limit)*sizeof(int));
    tabletop=0;
    initialiseTable();    //initialize the Prime Implicants Table with all cells -1 to denote empty.
    pair();           //do the pairing
    displayTable(); //display the Prime Implicants Table
    printf("\nSimplified Boolean Expression is.......");
    analyseTable();  //Analyse the table and print the result.
    printf("\n");
}
node* getnode(int n)    //creates a node to store the minterm data
{
    int c=var-1;
    node *p;
    p=(node *)malloc(sizeof(node));
    if(p==NULL)
        printf("Insertion Failed.....\n");
    else
        {
            p->num1=0;
            p->paired[0]=n;
            p->numterm=1;
            while(n!=0)
            {
                p->bin[c]=n%2;
                if(p->bin[c]==1)
                    p->num1++;
                c--;
                n=n/2;
            }
        while(c!=-1)
            {
                p->bin[c]=0;
                c--;
            }
            p->flag=0;
        }
    p->group=p->num1;
    if(p->group>maxgrp)
        maxgrp=p->group;
    return p;
}
void makeList(int n)     //creates a linked list to store given minterms
{
    node *p,*q,*temp;
    p=getnode(n);
    if(p!=NULL)
    {
            if(head==NULL)
            {
                head=p;
                head->next=NULL;
                return;
            }
        else
            {
                q=head;
                if(p->group<head->group)
                {
                    p->next=head;
                    head=p;
                    return;
                }
            while(q->next!=NULL&&((q->next->group)<=(p->group)))
                {
                    q=q->next;
                }
            if(q->next==NULL)
                {
                    q->next=p;
                    p->next=NULL;
                }
            else
                {
                    temp=q->next;
                    q->next=p;
                    p->next=temp;
                }
            }
    }
}
void binaryFill(node *p,node *q,node *r)         /*fills the binary values in r
                                                    using p and q.If both bits are same keep as it is else put -1. */
{
    int c=var-1;
    while(c!=-1)
    {
        if(p->bin[c]==q->bin[c])
            {
                r->bin[c]=p->bin[c];
            }
        else
            {
                r->bin[c]=-1;
            }
        c--;
    }
}
node* createNodePair(node *p,node *q)    //creates a new node using given nodes
{
    int i,j;
    node *r;
    r=(node *)malloc(sizeof(node));
    if(r==NULL)
        printf("Insertion Failed.....\n");
    else
        {
            for(i=0; i<p->numterm; i++)
            {
                r->paired[i]=p->paired[i];
            }
            r->numterm=p->numterm*2;
            for(j=0; j<q->numterm; j++)
            {
                r->paired[i++]=q->paired[j];
            }
            r->flag=0;
            r->next=NULL;
            r->group=p->group;
            binaryFill(p,q,r);
        }
    return r;
}
void addPair(node *p,node *q)   //create a linked list to store the paired minterms
{
    node *r,*temp;
    r=createNodePair(p,q);
    if(head1==NULL)
        {
            head1=r;
        }
    else
        {
            temp=head1;
            while(temp->next!=NULL)
                temp=temp->next;
            temp->next=r;
        }
}
int iPairP(node *a,node *b)   //checks if there is a change of only one bit
{
    int c=var-1;
    int ifone=0;
    while(c!=-1)
    {
        if(a->bin[c]!=b->bin[c])
            {
                if(ifone)
                    return 0;
                else
                    ifone=1;
            }
        c--;
    }
    return 1;
}
void pair()    //does the pairing work
{
    node *p,*q;
    int match=0;
    static int i=1;  //stores the iteration or pass count
    p=head;
    q=p;
    printf("\n\nIteration  %d\n",i);
    i++;
    display();
    nmaxgrp=-1;
    while(p->group!=maxgrp)
    {
        q=q->next;
        while(q!=NULL && (q->group==p->group))
        {
            q=q->next;
        }
        if(q==NULL)
        {
            p=p->next;
            q=p;
            continue;
        }
        else
        {
            if(q->group!=(p->group+1))
            {
                p=p->next;
                q=p;
                continue;
            }
            if(iPairP(p,q))        //checks if pairing possible and if yes put flag value 1 and add them to the new linked list...
            {
                match=1;
                p->flag=1;
                q->flag=1;
                addPair(p,q);
                if((p->group)>nmaxgrp)
                    nmaxgrp=p->group;
            }
        }
    }
    addToTable();
    if(match)      //checks if atleast one pair has been formed else it returns
        {
            head=head1;
            head1=NULL;
            maxgrp=nmaxgrp;
            pair();
        }
}
void initialiseTable()
{
    int i,j;
    for(j=0; j<1000; j++)
        for(i=0; i<limit; i++)
            {
                brr[j][i]=-1;
            }
}
void display()     //displays the minterms and their pairing and binary values at each pass
{
    int c=1,count=0,j=0;
    node *p;
    p=head;
    while(p!=NULL)
    {
        j=0;
        count=0;
        while(count<(p->numterm))
        {
            printf("%d,",p->paired[count]);
            count++;
        }
        printf("\b\t\t");
        while(j<var)
        {
            if(p->bin[j]==-1)
                printf("%c",'-');
            else
                printf("%d",p->bin[j]);
            j++;
        }
        printf("\n");
        c++;
        p=p->next;
    }
}
void displayTable()   //display the prime implicants table
{
    int i,j;
    printf("\n\nPrime Implicants Table---------\n");
    for(i=0; i<tabletop; i++)
    {
        cBtMN(i);
        printf("\t");
        for(j=0; j<=limit-1; j++)
        {
            if(brr[i][j]==1)
            printf("%d  ",j);
        }
        printf("\n");
    }
}
void addToTable()
{
    int i,j,k,allMch;
    node *p;
    p=head;
    while(p!=NULL)
    {
        if(!(p->flag))
            {
            if(tabletop!=0)                                //checking for duplicates
                {
                for(j=0; j<tabletop; j++)
                    {
                    allMch=1;
                    for(k=0; k<p->numterm; k++)
                        {
                        if(brr[j][p->paired[k]]==1)
                            continue;
                        else
                            {
                            allMch=0;
                            break;
                            }
                        }
                    if(allMch==1)
                        {
                        break;
                        }
                    }
                if(allMch==1)
                    {
                    p=p->next;
                    continue;
                    }
                }
            for(i=var-1; i!=-1; i--)                //Used For Minterm Notation
                {
                    arr[tabletop][i]=p->bin[i];
                }
            for(i=0; i<p->numterm; i++)
                {
                tablecount[tabletop]++;
                brr[tabletop][p->paired[i]]=1;
                }
            tabletop++;
            }
        p=p->next;
    }
}
int maxNumterms(int *row)       //finds the prime implicant which has the greatest number of minterms
{
    int i,max=-1;
    for(i=0; i<tabletop; i++)
        {
        if(tablecount[i]>max)
            {
                *row=i;
                max=tablecount[i];
            }
        }
    return max;
}
void analyseTable()      //does the analysing work  of the table i.e. selecting essential prime implicants
{
    int i,j,k,max,start=1,temp,c,s;
    int essential[limit];      //stores the row number of all essential prime implicants
    for(i=0; i<=limit-1; i++)
        essential[i]=-1;
    for(i=0; i<nmin; i++)
    {
        s=mintermsGiven[i];
            if(noOfImp(s,&temp)==1)
            {
                essential[s]=temp;
            }
    }
    for(i=0; i<=limit-1; i++)
    {
        if(essential[i]!=-1)
        {
            if(start!=1)
                printf(" + ");
            else
                start=0;
            cBtMN(essential[i]);
            rMfTable(essential[i]);
            for(j=i+1; j<=limit-1; j++)
            {
                if(essential[j]==essential[i])
                    essential[j]=-1;
            }
            essential[i]=-1;
        }
    }
    while(maxNumterms(&max)!=0)
    {
        if(start!=1)
            printf(" + ");
        else
            start=0;
        cBtMN(max);
        rMfTable(max);
    }
}
void rMfTable(int n)    //given a implicant row it deletes all the minterms from it as well as delete all the minterms from that respective columns too.
{
    int i,j,k;
    for(i=0; i<=limit-1; i++)
    {
        if(brr[n][i]==1)
        {
            mintermsGiven[i]=-1;
            for(j=0; j<tabletop; j++)
            {
                if(brr[j][i]==1)
                {
                    brr[j][i]=-1;
                    tablecount[j]--;
                }
            }
        }
    }
}
int noOfImp(int n,int *temp)     //returns in how many implicants a particular minterm is present
{
    int i,j;
    int count=0;
    for(i=0; i<tabletop; i++)
        {
            if(brr[i][n]==1)
            {
                j=i;
                count++;
            }
        }
    *temp=j;
    return count;
}
void cBtMN(int n)   //converts and prints the binary into a variable notation
{
    int c=0;
    char normal[]= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char comp[]= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    while(c!=var)
    {
        if(arr[n][c]!=-1)
        {
            if(arr[n][c]==1)
                printf("%c",normal[c]);
            else
                printf("%c",comp[c]);
        }
        c++;
    }
}


