#include<stdio.h>
//#include<conio.h>
#include<string.h>
#define epsilon 238

int flag=0,len,pt=0;
char st[25];
void E();
void T();
void F();
void EPRIME();
void TPRIME();
void ADVANCE();

int main()
{
//clrscr();
printf("\nRecursive Descent parser\n");
printf("\nE ->TE'\nE' ->+TE' /%c\nT ->FT'\nT' ->*FT'/%c\nF -> (E)/a\n",epsilon,epsilon);
printf("enter the string :");
scanf("%s",st);
len=strlen(st);
E();
if((flag==0)&&(len==pt))
printf("\n\nstringis accepted\n\n");
else
printf("\nstring rejected\n");
//getch();
return 0;
}
void E()
{
T();
EPRIME();
}
void T()
{
F();
TPRIME();
}
void EPRIME()
{
if(st[pt]=='+')
{
ADVANCE();
T();
EPRIME();
}
}
void TPRIME()
{
if(st[pt]=='*')
{
ADVANCE();
F();
TPRIME();
}
}
void F()
{
if(st[pt]=='(')
{
ADVANCE();
E();
if(st[pt]==')')
ADVANCE();
else
flag=1;
}
else if(st[pt]=='a')
ADVANCE();
else
flag=1;
}
void ADVANCE()
{
pt++;
}

