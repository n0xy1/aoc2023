#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CARD_MAX 10
#define WINNING_MAX 25

typedef struct{
  int game_id;
  int matching_nums;
  int game_score;
} CardGame;

void process_line(char *str, CardGame *game){
  char *line;
  char *token;
  int card_id = 0;
  const char delimiters[] = ":|";
  line = strdup(str);
  token = strtok(line, delimiters);

  // card id
  int card_len = strlen(token);
  card_id = atoi( &token[4] );
  //printf("card_id: %d %s\n", card_id, token);

  // my numbers
  int card_numbers[CARD_MAX] = {0};
  int c = 0;
  token = strtok(NULL, " ");
  while(token != NULL && (strcmp(token, "|") != 0)){
    //printf("%s\n", token);
    card_numbers[c] = atoi(token);
    token = strtok(NULL, " ");
    ++c;
  }

  //winning numbers
  
  //skip the |
  token += 2;
  int winning_numbers[WINNING_MAX];
  c = 0;
  token = strtok(NULL, " ");
  while(token != NULL){
    winning_numbers[c] = atoi(token); 
    //printf("%s\n", token);
    token = strtok(NULL, " ");
    ++c;
  }


  // check winners.
  int num_winners = 0;
  for(int i = 0; i < CARD_MAX; ++i){
    for(int j = 0; j < WINNING_MAX; ++j){
      //printf("%d %d me: %d winning: %d\n",i,j, card_numbers[i], winning_numbers[j]);
      if (card_numbers[i] == winning_numbers[j]){
        ++num_winners;
      }
    }
  }



  //calc the total
  int res = 0;
  if(num_winners > 0){
    res = 1;
    for(int k = 0; k < num_winners - 1; ++k ){
      res *= 2;
    }
  }


  game->game_id = card_id;
  game->matching_nums = num_winners;
  game->game_score = res;
}

int main(int argc, char *argv[])
{
  //open file.


  FILE *file = fopen("input","r");
  if(file == NULL){
    perror("failed to open input\n");
    exit(EXIT_FAILURE);
  }

  CardGame games[194] = {0};
  CardGame tmp = {0};
  char buffer[1024];
  int c = 0;
  while(fgets(buffer, sizeof(buffer), file)){
    process_line(buffer, &tmp);
    games[c] = tmp;
    //printf("%d: %d\n", tmp.game_id, tmp.matching_nums);
    ++c;
  }

  int total = 0;
  for(int x = 0; x < c; ++x){
   // printf("ID: %d, Matches: %d, Score: %d\n", games[x].game_id, games[x].matching_nums, games[x].game_score);
    total += games[x].game_score;
  }

  printf("Total: %d\n", total);

  return EXIT_SUCCESS;
}
