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

//        printf("Matched: %s (Digit: %s)\n", match+digit_len+1, digit);
        // need to check PER-ROUND.. NOT PER GAME! 
        if(colours[0] > 12 || colours[1] > 13 || colours[2] > 14){
          results[0] = atoi(game_id);
          results[1] = colours[0];
          results[2] = colours[1];
          results[3] = colours[2];
          return 0;
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
  results[1] = colours[0];
  results[2] = colours[1];
  results[3] = colours[2];
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
    if(parse_game(buffer,results_array) == 1){
      printf("Game %3d. Red: %5d Green: %5d Blue: %5d\tPASS\n",results_array[0],results_array[1],results_array[2],results_array[3]);
      total += results_array[0];
    }else{
      printf("Game %3d. Red: %5d Green: %5d Blue: %5d\tFAIL\n",results_array[0],results_array[1],results_array[2],results_array[3]);
    }
  }
  return total;
}


int main(int argc, char *argv[])
{
  const char *pattern = "([0-9]*) (blue|red|green)";
  if(compile_regex(pattern) == -1){
    return EXIT_FAILURE;
  }

  printf("Totals of game_id's that work: %d\n", process_input());


  regfree(&regex);


  return EXIT_SUCCESS;
}
