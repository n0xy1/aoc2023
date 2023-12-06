#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CARD_MAX 10
#define WINNING_MAX 25

typedef struct{
  int game_id;
  int num_winners;
  int copies;
} CardGame;

int process_line(char *str, CardGame *game){
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


  //printf("id: %d, matches: %d\n", card_id, num_winners);
  game->game_id = card_id;
  game->num_winners = num_winners;

  return num_winners;
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
    //printf("%d: %d\n", tmp.game_id, tmp.num_winners);
    ++c;
  }




  // go through the cards, adding copies of cards depending on how many numbers match.
  // dont decrement the games.copies, because we need to tally it at the end.
  int iterations = 0;
  for(int x = 0; x < c; ++x){
    iterations = games[x].copies + 1;
    while(iterations > 0){
      for(int y = 0; y < games[x].num_winners; ++y){
        games[x+y+1].copies +=1;
      }
      --iterations;
    }
    games[x].copies += 1;
  }

  int total = 0;
  for(int x = 0; x < c; ++x){
    total += games[x].copies; 
  }
  printf("Total: %d\n", total);



  return EXIT_SUCCESS;
}
