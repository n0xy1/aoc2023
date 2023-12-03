#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <regex.h>


// global vars are bad?
regex_t regex;

int compile_regex(const char *pattern){
  int rc = regcomp(&regex, pattern, REG_EXTENDED);
  if (rc != 0){
    printf("Regex comp failed\n");
    return -1;
  }
  return 0;
}

int parse_game(char *str, int results[]){

  // get game id.
  char game_id[4];
  if(sscanf(str, "Game %3[^:]", game_id) == 1){
    // its good. 
  }else{
    printf("Error finding game id\n");
    return -1;
  }

  // process games.
  int rounds = 0;
  char *delimiter = ";";
  // skip over the prefix "Game XXX: " and get the first round.
  char *token = strtok(str+7+strlen(game_id), delimiter);
 

  int max[3] = {0};
  int colours[3] = {0};

  while(token != NULL){
    int rc = 0;
    regmatch_t matches[10];
    int match_offset = 0;
    while ((rc = regexec(&regex, token + match_offset, 3, matches, 0)) == 0) {
      //reset the colours to zero. PER ROUND. NOT PER GAME!.
      colours[0] = 0;
      colours[1] = 0;
      colours[2] = 0;
        int start = matches[0].rm_so + match_offset;
        int end = matches[0].rm_eo + match_offset;
        int digit_start = matches[1].rm_so + match_offset;
        int digit_end = matches[1].rm_eo + match_offset;

        // Extract and process the matched substrings
        char match[end - start + 1];
        strncpy(match, token + start, end - start);
        match[end - start] = '\0';

        char digit[digit_end - digit_start + 1];
        strncpy(digit, token + digit_start, digit_end - digit_start);
        digit[digit_end - digit_start] = '\0';

        int digit_len = digit_end - digit_start;
        if(strcmp("blue", match+digit_len+1) == 0){
          colours[2] += atoi(digit);
        }
        if(strcmp("red", match+digit_len+1) == 0){
          colours[0] += atoi(digit);
        }
        if(strcmp("green", match+digit_len+1) == 0){
          colours[1] += atoi(digit);
        }
        
      if(max[0] < colours[0]){
        max[0] = colours[0];
      }
      if(max[1] < colours[1]){
        max[1] = colours[1];
      }
      if(max[2] < colours[2]){
        max[2] = colours[2];
      }

        // Update the offset to search for the next match
        match_offset = end;
    }


    if (rc != REG_NOMATCH) {
        printf("Error matching regex\n");
    }
  
    
    token = strtok(NULL, delimiter);
    ++rounds;
  }
  results[0] = atoi(game_id);
  results[1] = max[0];
  results[2] = max[1];
  results[3] = max[2];
  return 1;
}


int process_input(){
  FILE *file = fopen("input", "r");
  if (file == NULL){
    perror("error opening input\n");
    exit(EXIT_FAILURE);
  }
  char buffer[1024];
  int total = 0;
  int results_array[4] = {0};
  while(fgets(buffer,sizeof(buffer),file)){
    parse_game(buffer,results_array);
    int power = 0;
    power = results_array[1] * results_array[2] * results_array[3];
   // printf("Power: %3d  R: %3d  G: %3d  B: %3d\n", power,results_array[1],results_array[2],results_array[3]);
    total += power; 
  }
  return total;
}


int main(int argc, char *argv[])
{
  const char *pattern = "([0-9]*) (blue|red|green)";
  if(compile_regex(pattern) == -1){
    return EXIT_FAILURE;
  }

  printf("Total: %d\n", process_input());


  regfree(&regex);


  return EXIT_SUCCESS;
}
