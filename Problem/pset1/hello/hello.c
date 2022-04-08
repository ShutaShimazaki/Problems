#include <cs50.h>
#include <stdio.h>

int main(void)  //Greetingsubmit50 cs50/problems/2021/x/hello
{
    string name = get_string("What is your name?\n"); 
    printf("hello, %s\n", name);
}