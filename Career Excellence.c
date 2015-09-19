#include<stdio.h>
#include<malloc.h>

struct request
{
    long int src, dest, t1, t2, status;
};

struct position
{
    long int pos, load, p1, p2, p3, p4, p5, t, d;
};

int min(long int a,long int b)
{
    if(a < b)
        return(a);
    else
        return(b);
}

void sorting(struct request req[], long int lim)
{
    long int i, j, min, chk;
    struct request temp;
    for(i = 0; i < lim-1; i++)
    {
        min = i;
        for(j = i + 1; j < lim; j++)
        {
            if(req[j].src < req[min].src)
                min = j;
            else if(req[j].src == req[min].src)
            {
                if(req[j].t1 < req[min].t1)
                    min = j;
            }
        }
        if(min != chk)
        {
            temp = req[i];
            req[i] = req[min];
            req[min] = temp;
        }
    }
}


int main()
{
    long int n, N, c, R, i, j, k, status = 0, temp1, temp2, flag = 0, cost = 0;
    struct request *req = NULL;
    struct position *p = NULL;
    FILE *fp = NULL;
    fp = fopen("Input.txt","r");
    fscanf(fp,"%ld%ld%ld%ld",&n,&N,&c,&R);
    long int **map = (long int **)malloc(n*sizeof(long int *));
    for(i = 0; i < n; i++)
        map[i] = (long int*)malloc(n*sizeof(long int));
    p = (struct position*)malloc(N * sizeof(struct position));
    req = (struct request*)malloc(R * sizeof(struct request));
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
        {
            fscanf(fp,"%ld",&map[i][j]);
            if(i==j)
                    map[i][j]=0;
            else
            {
                if(map[i][j] == -1)
                    map[i][j] = 9999;  //treating 9999 as infinity
            }
        }

    for(k = 0; k < n; k++)
        for(i = 0; i < n; i++)
            for(j = 0;j < n; j++)
                if(i != j)
                    map[i][j]=min(map[i][j],map[i][k]+map[k][j]);  /*Shortest distance from source to destination*/

    for(i = 0; i < N; i++)
    {
        fscanf(fp,"%ld",&p[i].pos);
        p[i].load = 0;
        p[i].t = 0;  //initial time is zero i.e midnight.
        p[i].d = 0;  //initial distance covered by ith vehicle is zero.
    }
    for(i = 0; i < R; i++)
    {
        fscanf(fp,"%ld%ld%ld%ld",&req[i].src,&req[i].dest,&req[i].t1,&req[i].t2);
        req[i].status = 0;
    }
    fclose(fp);
    sorting(req,R);

    while(status == 0)
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < R; j++)
            {
                if(p[i].pos == req[j].src && req[j].status == 0 && req[j].t2 >= p[i].t)
                {

                    if(p[i].load == 0)
                        p[i].p1 = j;
                    else if(p[i].load == 1)
                        p[i].p2 = j;
                    else if(p[i].load == 2)
                        p[i].p3 = j;
                    else if(p[i].load == 3)
                        p[i].p4 = j;
                    else
                        p[i].p5 = j;
                    p[i].pos = req[j].dest;
                    p[i].load++;       //number of people present in vehicle
                    req[j].status = 1; //setting request accept status
                    temp1 = req[j].src;
                    temp2 =req[j].dest;
                    if(p[i].t == 0)
                    {
                        p[i].t = p[i].t + req[j].t1;
                    }
                    p[i].t = p[i].t + (map[temp1-1][temp2-1])/2;
                    p[i].d = p[i].d + map[temp1-1][temp2-1];
                    if(p[i].load == 5)
                    {
                        p[i].load = 0;
                        break;
                    }
                }
            }
        }
        status = 1;
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < R; j++)
            {
                if(req[j].status == 0)
                {
                    temp1 = p[i].pos;
                    temp2 = req[j].src;
                    p[i].t = p[i].t + (map[temp1-1][temp2-1])/2;
                    if(req[j].t2 >= p[i].t)
                    {
                        p[i].pos = req[j].src;
                        status = 0;
                        flag = 0;
                        break;
                    }
                }
            }
            if(flag == 1)
                break;
        }
    }
    for(i = 0; i < N; i++)
        cost = cost + p[i].d;
    printf("Total Revenue Generated= %ld",cost);
    return 0;
}
