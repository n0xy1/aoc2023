#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int solve_line(char *str){
  int len = strlen(str);
  char first = 0;
  char last = 0;
  for (int i = 0; i < len; ++i) {
    if (str[i] >= '0' && str[i] <='9'){
      if (first == 0){
        first = str[i];
      }
      last = str[i];
    }
  }
  char combined[3];
  combined[0] = first;
  combined[1] = last;
  combined[2] = '\0';
  int retdigit =  atoi(combined);
  return retdigit;
}

int process_input(){
  FILE *file = fopen("input","r");
  if(file == NULL){
    perror("Error opening input\n");
    exit(EXIT_FAILURE);
  }

  int final_sum = 0;

  char buffer[1024];
  memset(buffer, 0,sizeof(buffer));

  while(fgets(buffer,sizeof(buffer),file) != NULL){
    final_sum += solve_line(buffer);
  }
  return final_sum;
}

int main(int argc, char *argv[])
{
  printf("%d\n", process_input());
  return EXIT_SUCCESS;
}
