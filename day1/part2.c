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

int word_to_number(const char *word) {
    if (strcmp(word, "one") == 0) {
        return 1;
    } else if (strcmp(word, "two") == 0) {
        return 2;
    } else if (strcmp(word, "three") == 0) {
        return 3;
    } else if (strcmp(word, "four") == 0) {
        return 4;
    } else if (strcmp(word, "five") == 0) {
        return 5;
    } else if (strcmp(word, "six") == 0) {
        return 6;
    } else if (strcmp(word, "seven") == 0) {
        return 7;
    } else if (strcmp(word, "eight") == 0) {
        return 8;
    } else if (strcmp(word, "nine") == 0) {
        return 9;
    } else if (strcmp(word, "0") == 0) {
        return 0;
    } else if (strcmp(word, "1") == 0) {
        return 1;
    } else if (strcmp(word, "2") == 0) {
        return 2;
    } else if (strcmp(word, "3") == 0) {
        return 3;
    } else if (strcmp(word, "4") == 0) {
        return 4;
    } else if (strcmp(word, "5") == 0) {
        return 5;
    } else if (strcmp(word, "6") == 0) {
        return 6;
    } else if (strcmp(word, "7") == 0) {
        return 7;
    } else if (strcmp(word, "8") == 0) {
        return 8;
    } else if (strcmp(word, "9") == 0) {
        return 9;
    } else {
        return -1; // Return -1 for words not recognized
    }
}
int solve_p2(char *str){
    int len = strlen(str);
    int firstpos = len;
    int lastpos = -1;
    const char *firstval = NULL;
    const char *lastval = NULL;

    char *words[] = {"one", "two", "three", "four", "five","six","seven","eight","nine","1","2","3","4","5","6","7","8","9","0"};
    int num_words = sizeof(words) / sizeof(words[0]);
    for (int i = 0; i < num_words; ++i) {
        const char *result = strstr(str, words[i]);
        while (result != NULL) {
            int position = result - str;
            if (position < firstpos) {
                firstpos = position;
                firstval = words[i];
            }
            if (position > lastpos) {
                lastpos = position;
                lastval = words[i];
            }
            result = strstr(result + 1, words[i]);
        }
    }

    int result = 0;
    if (firstval && lastval) {
        result = (word_to_number(firstval) * 10) + word_to_number(lastval);
        printf("%d + %d = %d\n", word_to_number(firstval), word_to_number(lastval), result);
    }

    return result;
}

int solve_p3(char *str){
  int len = strlen(str);
  int firstpos = len;
  int lastpos = -1;
  const char *firstval;
  const char *lastval;

  char *words[] = {"one", "two", "three", "four", "five","six","seven","eight","nine","1","2","3","4","5","6","7","8","9","0"};
  int num_words = sizeof(words) / sizeof(words[0]);

  for (int i = 0; i < num_words; ++i) {
    const char *result = strstr(str, words[i]);
    const char *more_result = NULL;
    if(result != NULL){
      more_result = strstr(result++, words[i]);
      if(more_result != NULL){
        printf("MULTIPLE OCCURANCES %s %s %s\n", words[i], result, more_result);
      }
      int position = result - str;
      if(position < firstpos){
        firstpos = position;
        firstval = words[i];
      }
      if(position > lastpos){
        lastpos = position;
        lastval = words[i];
      }

    }
  }
  
  int result = 0;
  result = (word_to_number(firstval) * 10) + word_to_number(lastval);
  printf("%d + %d = %d\n", word_to_number(firstval), word_to_number(lastval), result);

  return result;

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
    final_sum += solve_p2(buffer);
  }
  return final_sum;
}

int main(int argc, char *argv[])
{
  printf("%d\n", process_input());
  return EXIT_SUCCESS;
}
