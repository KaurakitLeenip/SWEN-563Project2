#include "Util.h"

int Str_To_Int(char str[]){
	int len = strlen(str);
	unsigned int dec = 0;
	int i;
	
	for(i = 0; i < len; i++)
	{
		dec = dec * 10 + ( str[i] - '0');
	}
	
	return dec;
}

void Remove_Spaces(char* str){
	//removes leading and trailing spaces on a string
  char* temp = str;
  char* temp1 = str;
  while(*temp1 != '\0')
  {
    *temp = *temp1++;
    if(*temp != ' ')
      temp++;
  }
  *temp = '\0';
}
