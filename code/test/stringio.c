#include "syscall.h"


int
main() 
{
    char a[256];
    PrintString("String length: (<= 256):\n \0");
    ReadString(a, 256);
    PrintString("Ban da nhap: \0");
    PrintString(a);
    PrintString("\n");
    Halt();
}
