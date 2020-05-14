#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include<strings.h>
#include<fstream>
#include<conio.h>
#include<stdio.h>
#include<time.h>
using namespace std;

void import_data();
void export_data();
int authenticate();
void search();
void display();
void insert();
void delete_data();
void modify();
void modify_acc();
void book_slot();
void adjust();
int *index(char bldg[],int lab_no,int slot);

struct slot
{
    int index;
    int check=0;
    char bldg[4];
    int lab_no;
    int slot;
    int book[4]={0,0,0,0};
};

char passc[10],useridc[20];
struct slot s[500];

int main()
{
    import_data();
    adjust();
    int choice=0,check;
    cout<<"LAB SLOT SEARCH\n";
    do
    {
        switch(choice)
        {
        case 0:
            check=1;
            cout<<"\nAdmin\t1";
            cout<<"\nUser\t2";
            cout<<"\nExit\t9\n";
            cin>>choice;
            continue;
        case 1:
            if(check)
            {
                int l=authenticate();
//                cout<<l;
                if(l)
                {
                    cout<<"\nWrong userid or password";
                    choice=0;
                    continue;
                }
            }
            check=0;
            cout<<"\nMain menu\t0";
            cout<<"\nSearch\t\t3";
            cout<<"\nDisplay\t\t4";
            cout<<"\nInsert\t\t5";
            cout<<"\nDelete\t\t6";
            cout<<"\nModify\t\t7";
//            cout<<"\nEdit Account\t8";
            cout<<"\nBook Slot\t9\n";
            cin>>choice;
            continue;
        case 2:
            cout<<"Main menu\t0";
            cout<<"\nSearch\t\t3";
            cin>>choice;
            continue;
        case 3:
            search();
            choice=0;
            break;
        case 4:
            display();
            choice=1;
            break;
        case 5:
            insert();
            choice=1;
            break;
        case 6:
            delete_data();
            choice=1;
            break;
        case 7:
            modify();
            choice=1;
            break;
        case 8:
            modify_acc();
            choice=1;
            break;
        case 10:
            export_data();
            choice=11;
            break;
        case 9:
            book_slot();
            choice=1;
            break;
        }
    }while(choice!=11);
}

int* index(char bldg[], int lab_no, int slot)
{
    char c;
    static int indx[3];
    indx[0]=0;indx[1]=499;indx[2]=1;
    c=bldg[0];
    switch(c)
    {
    case 'S':
        indx[0]=0;
        indx[1]=320;
        break;
    case 'T':
        indx[0]=321;
        indx[1]=499;
        break;
    }
    if(lab_no!=-1)
    {
        indx[0]+=(lab_no/100)*40+((lab_no%100)-16)*10;
        indx[1]=indx[0]+10;
    }

    if(lab_no!=-1 && slot!=-1)
    {
        indx[0]+=(((slot/100)%30)/6);
        indx[1]=indx[0];
        indx[2]=1;
    }
    else if(slot!=-1)
    {
        indx[0]+=(((slot/100)%30)/6);
        indx[2]=10;
    }

    return indx;
}

void import_data()
{
    char i[4],lab_num[4],lab_slot[5]; //index
    int indx;
    ifstream fin;
    fin.open("Database.txt",ios::in);

    if(!fin)
    {
        cout<<"Error! Can not open file";
        return;
    }
    fin.seekg(0);
    fin.getline(useridc,20);
    fin.getline(passc,10);
    while(fin)
    {
        fin.getline(i,4);
        indx=atoi(i);
        if(s[indx].check==0)
            s[indx].check=1;
        else
            indx++;

        s[indx].index=indx;

        fin.getline(s[indx].bldg,4);


        fin.getline(lab_num,4);
        s[indx].lab_no=atoi(lab_num);

        fin.getline(lab_slot,5);
        s[indx].slot=atoi(lab_slot);

        s[indx].check=1;
    }
    fin.close();


    char num[3];
    fin.open("Bookings.txt",ios::in);
    if(!fin)
    {
        cout<<"Error! Can not open file";
        return;
    }
    fin.seekg(0);
    while(fin)
    {
        fin.getline(i,4);
        indx=atoi(i);

        for(int j=0;j<4;j++)
        {
            fin.getline(num,3);
            s[indx].book[j]=atoi(num);
        }

    }
    fin.close();

}

void export_data()
{
    ofstream fout,f1out;
    fout.open("Database.txt",ios::out);

    fout<<useridc<<"\n";
    fout<<passc<<"\n";
    for(int i=1;i<500;i++)
    {
        if(s[i].check==0)
            continue;

        fout<<setw(3)<<setfill('0')<<i<<"\n";
        fout<<s[i].bldg<<"\n";
        fflush(stdin);
        fout<<setw(3)<<setfill('0')<<s[i].lab_no<<"\n";
        fout<<setw(4)<<setfill('0')<<s[i].slot<<"\n";
    }
    fout.close();

    f1out.open("Bookings.txt",ios::out);
    for(int i=1;i<500;i++)
    {
        if(s[i].check==0)
            continue;

        f1out<<setw(3)<<setfill('0')<<i<<"\n";
        for(int j=0;j<4;j++)
            f1out<<setw(2)<<setfill('0')<<s[i].book[j]<<"\n";
    }
    f1out.close();
}

void search()
{
    int dd;
    time_t now = time(0);
	tm *ltm = localtime(&now);
	dd=ltm->tm_mday;

    char build[4];
    int num,slt,count=0;
    int *p;
    cout<<"Enter the building (enter - if none)\t: ";
    cin>>build;
    cout<<"Enter class number (enter -1 if none)\t:";
    cin>>num;
    cout<<"Enter the lab slot (enter -1 if none)\t: ";
    cin>>slt;

    p=index(build,num,slt);
    for(int i=p[0];i<=p[1];i=i+p[2])
    {
        if(s[i].check!=0)
        {
            cout<<s[i].bldg<<"\t"<<s[i].lab_no<<"\tL"<<s[i].slot/100<<"+L"<<s[i].slot%100;
            if(s[i].book[0]<dd+7 && s[i].book[0]!=0)
                cout<<"\tBOOKED\n";
            else
                cout<<endl;
            count++;
        }
    }

    if(count==0)
        cout<<"\nLAB SLOT IS NOT EMPTY";
    else if(count==1)
        cout<<"\nYES IT IS EMPTY";
}

void display()
{
    for(int i=1;i<500;i++)
    {
        if(s[i].check==0)
            continue;
        cout<<s[i].bldg<<"\t"<<s[i].lab_no<<"\t"<<"L"<<s[i].slot/100<<"+L"<<s[i].slot%100<<endl;
    }
}

void insert()
{
    char build[4];
    int num,slt,*p,i;
    cout<<"Enter the building\t: ";
    cin>>build;
    cout<<"Enter class number\t:";
    cin>>num;
    cout<<"Enter the lab slot\t: ";
    cin>>slt;

    p=index(build,num,slt);
    i=p[0];
    if(s[i].check==1)
    {
        cout<<"Slot Already Free!\n";
        return;
    }
    s[i].index=i;
    s[i].check=1;
    strcpy(s[i].bldg,build);
    s[i].lab_no=num;
    s[i].slot=slt;
}

void delete_data()
{
    char build[4];
    int num,slt,*p,i;
    cout<<"Enter the building\t: ";
    cin>>build;
    cout<<"Enter class number\t:";
    cin>>num;
    cout<<"Enter the lab slot\t: ";
    cin>>slt;

    p=index(build,num,slt);
    if(s[p[0]].index==0)
        cout<<"Slot Is Already Full!\n";
    s[p[0]].check=0;
}

void modify()
{
    char build[4];
    int num,slt,*p,i;
    cout<<"Enter the building\t: ";
    cin>>build;
    cout<<"Enter class number\t:";
    cin>>num;
    cout<<"Enter the lab slot\t: ";
    cin>>slt;

    p=index(build,num,slt);
    s[p[0]].check=0;

    cout<<"Enter the modified building\t: ";
    cin>>build;
    cout<<"Enter modified class number\t:";
    cin>>num;
    cout<<"Enter the modified lab slot\t: ";
    cin>>slt;

    p=index(build,num,slt);
    s[p[0]].check=1;
    s[p[0]].lab_no=num;
    s[p[0]].slot=slt;
    strcpy(s[p[0]].bldg,build);
}

//returns 0 when correct and 1 when incorrect
int authenticate()
{
    char userid[20],pass[20],a;
    cout<<"Enter Username\t: ";
    cin>>userid;
    cout<<"Enter Password\t: ";
    for(int i=0;i<10;i++)
    {
        a=getch();
        if(a==13)
            break;
        cout<<"*";
        pass[i]=a;
    }
    a=0;
    if(strcmp(useridc,userid) || strcmp(passc,pass))
        return 1;
    return 0;
}

void modify_acc()
{
    char userid[20],pass[20],a;
    cout<<"Enter old userid: ";
    cin>>userid;
    cout<<"Enter old password: ";
    for(int i=0;i<10;i++)
    {
        a=getch();
        if(a==13)
            break;
        cout<<"*";
        if(passc[i]!=a)
        {
            printf("\nWrong Userid or password");
            return;
        }
    }
    a=0;
    if(strcmp(userid,useridc)==1)
    {
        cout<<"\nWrong Userid or password";
        return;
    }

    cout<<"\nEnter new userid: ";
    cin>>useridc;
    cout<<"Enter new password: ";
    int i;
    cout<<pass;
    for(i=0;i<10;i++)
    {
        a=getch();
        if(a==13)
            break;
        cout<<"*";
        pass[i]=a;
    }
    strcpy(passc,pass);
    cout<<passc;
    a=0;
}

void book_slot()
{
    cout<<"Enter the slot you want to book\n";
    char build[4];
    int num,slt,indx,n,choice;
    int dd,mm,ww;
    int *p;
    time_t now = time(0);
	tm *ltm = localtime(&now);

    cout<<"Enter the building\t: ";
    cin>>build;
    cout<<"Enter class number\t:";
    cin>>num;
    cout<<"Enter the lab slot\t: ";
    cin>>slt;
    n=((slt/100)%30)/6+8;
    ww=ltm->tm_wday;
    ww=(n-ww)%7;
    dd=ltm->tm_mday+ww;
    mm=ltm->tm_mon+1+dd/31;
    dd=dd%31;
    n=dd;
    cout<<"Choose the date you want to book the slot\n";
    for(int i=0;i<4;i++)
    {
        cout<<dd<<"/"<<mm<<"\t"<<i<<"\n";
        dd+=7;
        mm+=dd/31;
        dd=dd%31;
    }
    cin>>choice;

    p=index(build,num,slt);

    indx=p[0];

    if(s[indx].book[choice]!=0 || s[indx].check==0)
    {
        cout<<"Slot not available";

        cout<<"Suggested slots available today\n";
        p=index(build,-1,slt);
        for(int i=p[0];i<=p[1];i=i+p[2])
        {
            if(s[i].check!=0)
            {
                cout<<s[i].bldg<<"\t"<<s[i].lab_no<<"\tL"<<s[i].slot/100<<"+L"<<s[i].slot%100;
                if(s[i].book[0]<dd+7 && s[i].book[0]!=0)
                    cout<<"\tBOOKED\n";
                else
                    cout<<endl;
            }
        }
        book_slot();
    }
    s[indx].book[choice]=n+7*(choice);

}

void adjust()
{
    int dd;
    time_t now = time(0);
	tm *ltm = localtime(&now);
	dd=ltm->tm_mday;
	for(int i=0;i<500;i++)
    {
        if(s[i].book[0]==dd-1)
        {
            for(i=0;i<3;i++)
                s[i]=s[i+1];
        }
        s[i].book[3]=0;
    }
}
