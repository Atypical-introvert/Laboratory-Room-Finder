#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include<strings.h>
#include<fstream>
#include<conio.h>
#include<stdio.h>
using namespace std;

void import_data();
void export_data();
int authenticate();
void search();
void display();
void insert();
void delete_data();
void modify();
int *index(char bldg[],int lab_no,int slot);

struct slot
{
    int index;
    int check=0;
    char bldg[4];
    int lab_no;
    int slot;
};

//char passc[8]="qwertyui",useridc[20]="Mihir";
struct slot s[500];

int main()
{
    import_data();
    int choice=0,check=0;
    cout<<"Lab Slot Search\n";
    do
    {
        switch(choice)
        {
        case 0:
            check=1;
            cout<<"\nAdmin\t1";
            cout<<"\nUser\t2";
            cout<<"\nExit\t8\n";
            cin>>choice;
            continue;
        case 1:
            if(check && authenticate())
            {
                cout<<"\nWrong userid or password";
                choice=0;
                continue;
            }
            check=0;
            cout<<"\nMain menu\t0";
            cout<<"\nSearch\t\t3";
            cout<<"\nDisplay\t\t4";
            cout<<"\nInsert\t\t5";
            cout<<"\nDelete\t\t6";
            cout<<"\nModify\t\t7\n";
            cin>>choice;
            continue;
        case 2:
            cout<<"Main menu\t0";
            cout<<"\nSearch\t\t3";
            cin>>choice;
            continue;
        case 3:
            search();
            choice=1;
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
            export_data();
            choice=10;
        }
    }while(choice!=10);
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
    if(lab_no>=0);
    {
        indx[0]+=(lab_no/100)*40+((lab_no%100)-16)*10;
        indx[1]=indx[0]+10;
    }
    if(lab_no>=0)
    {
        indx[0]+=(((slot/100)%30)%6);
        indx[1]=indx[0];
        indx[2]=1;
    }
    else
    {
        indx[0]+=(((slot/100)%30)%6);
        indx[2]=10;
    }
    indx[0]-=1;
    indx[1]-=1;

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
}

void export_data()
{
    ofstream fout;
    fout.open("Database.txt",ios::out);

    for(int i=0;i<500;i++)
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
}

void search()
{
    char build[4];
    int num,slt;
    int *p;
    cout<<"Enter the building (enter - if none)\t: ";
    cin>>build;
    cout<<"Enter class number (enter -1 if none)\t:";
    cin>>num;
    cout<<"Enter the lab slot (enter -1 if none)\t: ";
    cin>>slt;

    p=index(build,num,slt);
    for(int i=p[0];i<=p[1];i+=p[2])
    {
        if(s[i].check==0 && s[i+1].check==0)
            cout<<"NOT EMPTY";
        else if(s[i+1].check==1)
            cout<<s[i+1].bldg<<"\t"<<s[i+1].lab_no<<"\t"<<s[i+1].slot<<"\n";
        else
            cout<<s[i].bldg<<"\t"<<s[i].lab_no<<"\t"<<s[i].slot<<"\n";

    }
}

void display()
{
    for(int i=0;i<500;i++)
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
        cout<<"Already exitsts!";
        return;
    }
    cout<<i;
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

int authenticate()
{
    char userid[20],pass[20],a;
    cout<<"Enter Username\t: ";
    cin>>userid;
    cout<<"Enter Password\t: ";
    for(int i=0;i<8;i++)
    {
        a=getch();
        cout<<"*";
        pass[i]=a;
    }
    a=0;
    if(strcmp("Mihir",userid) || strcmp("qwertyui",pass))
        return 1;
    return 0;
}
