#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void add();
void sub();
void mult();
void division();
void bin();
void mod();

int main()
{
	printf("Greetings from ...");
	printf("Enter the operation you want to execute:");

	char choice = getchar();

	if(choice == '+')
	{
		add();
	}
	else if(choice == '-')
	{
		sub();
	}
	else if(choice == '*')
	{
		mult();
	}
	else if(choice == '/')
	{
		division();
	}
	else if(choice == 'b')
	{
		bin();
	}
	else if(choice == 'm')
	{
		mod();
	}
	else
	{
		printf("Sorry .... doesn't provide this option. Bye");
	}
	return 0;
}

void add()
{

}

void sub()
{

}

void mult()
{

}

void division()
{

}

void bin()
{

}

void mod()
{

}

