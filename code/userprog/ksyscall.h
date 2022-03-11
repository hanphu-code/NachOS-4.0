/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"

// for using INT_MAX and INT_MIN to check case: overflow integer
#include <bits/stdc++.h> 
// for using the random number function
#include <stdlib.h>
// define
/* maximum length of an interger (included the minus sign) */
#define MAX_NUM_LENGTH 11
/* A buffer to read and write number */
char numberBuffer[MAX_NUM_LENGTH + 2];

// Halt function
void SysHalt()
{
  kernel->interrupt->Halt();
}

// Add function
int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

// Subtract function
int SysSub(int op1, int op2) 
{
  return op1 - op2;
}

// ReadNum function
int SysReadNum() 
{
  // kernel->synchConsoleIn->GetChar();
  int MAX_BUFFER = 255, i = 0;
  char* buffer = new char [MAX_BUFFER + 1];

  do
  {
    char n = kernel->synchConsoleIn->GetChar();
    if(n == '\n')
      break;
    buffer[i] = n;
    i++;
  } while (true);
  long long tmp = 0, ans = 0;
  if(buffer[0] == '-')
  {
    for(int j = i - 1; j >= 1; j--)
    {
      if(buffer[j] - '0' < 0 || buffer[j] - '0' > 9)
        return 0;
      ans += (buffer[j] - 48) * pow(10, tmp);
      tmp++;
    }
    ans = 0 - ans;
  }
  else
  {
    for(int j = i - 1; j >= 0; j--)
    {
      if(buffer[j] - '0' < 0 || buffer[j] - '0' > 9)
        return 0;
      ans += (buffer[j] - 48) * pow(10, tmp);
      tmp++;
    }
  } 
  if(ans > INT_MAX || ans < INT_MIN)
    ans = 0;
  return ans;
}

void SysPrintNum(int number) 
{
  if (number == 0) return kernel->synchConsoleOut->PutChar('0');

  if (number == INT_MIN) 
  {
      kernel->synchConsoleOut->PutChar('-');
      for (int i = 0; i < 10; ++i)
          kernel->synchConsoleOut->PutChar("2147483648"[i]);
      return;
  }

  if (number == INT_MAX) 
  {
    for (int i = 0; i < 10; ++i)
          kernel->synchConsoleOut->PutChar("2147483647"[i]);
      return;
  }

  if (number < INT_MIN && number > INT_MAX) {
    return kernel->synchConsoleOut->PutChar('0');
  } 

  if (number < 0) 
  {
      kernel->synchConsoleOut->PutChar('-');
      number = -number;
  }
  int n = 0;
  while (number)
  {
      numberBuffer[n] = number % 10;
      number /= 10;
      n++;
  }
  for (int i = n - 1; i >= 0; --i)
      kernel->synchConsoleOut->PutChar(numberBuffer[i] + '0');
}

char SysReadChar() 
{ 
  return kernel->synchConsoleIn->GetChar(); 
}

void SysPrintChar(char character) {
  kernel->synchConsoleOut->PutChar(character);
  return;
}

int SysRandomNum() {
  return rand();
}

void SysReadString(int buffer, int length)
{
	char *buff = NULL;
  // de gan gia tri tra ve cua kernel->synchConsoleIn->ReadStr(buff,length);, lay vi tri cua dau \0
	int userStringMaxLength = 0;
	if (length > 0) 
	{
		buff = new char[length];
		if (buff == NULL) 
		{
			char msg[] = "Not enough memory in system.\n\0";
			kernel->synchConsoleOut->PrintStr(msg,strlen(msg));
		}
		else
		{
			char msg[] = "Enter string: \0";
			kernel->synchConsoleOut->PrintStr(msg);
			userStringMaxLength = kernel->synchConsoleIn->ReadStr(buff,length); 
			//Return the pos of '\0' suppose to be
			buff[userStringMaxLength] = '\0';
		}
	}
	else
	{
		memset(buff, 0, length);
	}
  //cerr << "Doc duoc: ";
	if (buff != NULL) 
	{
		for (int i = 0; i <= userStringMaxLength ; i++) 
		{
      //cerr << buff[i];
			//Kernel buff -> user buffer
			kernel->machine->WriteMem(buffer+i,1,(int)buff[i]); 
      // Lay du lieu cua Kernel(buff) Write vao du lieu cua user (buffer).Thi ben cai user (file string io no se co du lieu)
		}
        delete[] buff;
  }
  //cerr << "\n";	    
  return;
}

void SysPrintString(int buffer)
{
  int ch = 0;
	//int ch[128]
	int MAX_STRING_LENGTH = 256;
	for(int i=0; i < MAX_STRING_LENGTH; i++)
	{
		//user buffer -> kernel p
		kernel->machine->ReadMem(buffer+i,1,&ch); 
    // Lay du lieu tu user tai dia chi buffer+i bo vao kernel ch
		// kernel->machine->ReadMem(buffer+i,1,ch+i);
		// Vi lay string de print ra thoi nen khong can char array
		kernel->synchConsoleOut->PutChar(char(ch));
		if(char(ch) == '\0' || char(ch) == EOF)
		{
			break;
		}
	}
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
