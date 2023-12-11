#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  T,
  J,
  Q,
  K,
  A
} CardLabel;

typedef enum{
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND
} HandClassification;

typedef struct{
  char Cards[5];
  int Bid;
  HandClassification Class;
  int Num;
} Hand;


int process_input(Hand **hands, int init_hands){

  *hands=malloc(init_hands * sizeof(Hand));
  if(*hands == NULL){
    perror("Alloc failed on hands.\n");
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen("input", "r");
  if(file == NULL){
    perror("Failed to open\n");
    exit(EXIT_FAILURE);
  }

  int total_hands = 0;
  char buffer[1024];
  while(fgets(buffer, sizeof(buffer), file)){
    for(int i = 0; i < 5; ++i){
      (*hands)[total_hands].Cards[i] = buffer[i];
    }
    char *token = strtok(buffer+5, " ");
    (*hands)[total_hands].Bid = atoi(token);
    (*hands)[total_hands].Num = total_hands;
    ++total_hands;
  }

  fclose(file);
  return total_hands;
}

CardLabel char_to_card(char c){
  switch(c){
    case '2': return TWO;
    case '3': return THREE;
    case '4': return FOUR;
    case '5': return FIVE;
    case '6': return SIX;
    case '7': return SEVEN;
    case '8': return EIGHT;
    case '9': return NINE;
    case 'T': return T;
    case 'J': return J;
    case 'Q': return Q;
    case 'K': return K;
    case 'A': return A;
    default: return -1;
  }
}

HandClassification classify_hand(char cards[5]){
  int counts[14] = {0};
  int unique_cards = 0;
  int pairs = 0;
  int threes = 0;
  int fours = 0;

  //count frequency.
  for(int i=0; i < 5; ++i){
    CardLabel label = char_to_card(cards[i]);
    if(label != -1 && ++counts[label] == 1){
      ++unique_cards;
    }
  }
  
  // freq analysis.
  for(int i = 0; i < 14; ++i){
    if(counts[i] == 2) pairs++;
    if(counts[i] == 3) threes++;
    if(counts[i] == 4) fours++;
  }
  
  if(unique_cards == 1){
    return FIVE_OF_A_KIND;
  }else if (fours == 1){
    return FOUR_OF_A_KIND;
  }else if (threes == 1 && pairs == 1){
    return FULL_HOUSE;
  }else if (threes == 1){
    return THREE_OF_A_KIND;
  }else if (pairs == 2){
    return TWO_PAIR;
  }else if (pairs == 1){
    return ONE_PAIR;
  } else {
    return HIGH_CARD;
  }
  

}


// custom qsort?
int compare_hands(const void *a, const void *b){
  const Hand *hand1 = (const Hand *)a;
  const Hand *hand2 = (const Hand *)b;

  if(hand1->Class != hand2->Class){
    return hand2->Class - hand1->Class;
  }

  for(int i = 0; i < 5; ++i){
    CardLabel card_1 = char_to_card(hand1->Cards[i]);
    CardLabel card_2 = char_to_card(hand2->Cards[i]);

    if(card_1 != card_2){
      return card_2 - card_1;
    }

  }


  //theyre the same.. and the first cards the same.
  return 0;
}


int main(int argc, char *argv[])
{
  Hand *hands;
  int inital_hands = 10;

  //process the hands
  int total_hands = process_input(&hands, inital_hands);
  printf("Total: %d\n", total_hands);

  //classify the hands
  for(int i = 0; i < total_hands; ++i){
    hands[i].Class = classify_hand(hands[i].Cards);
    printf("Hand %d is: %s\n", i, hands[i].Cards);
  } 

  // sweet quick sort magic.
  qsort(hands, total_hands, sizeof(Hand), compare_hands);

  // invert the ranks - get the total winnings.
  long total_winnings = 0;
  for(int i = 0; i < total_hands; ++i){
    int rank = total_hands - i;
    total_winnings += hands[i].Bid * rank;
    printf("Hand: %d - [%s] - Type %d - Rank: %d * %d == Total: %ld\n", hands[i].Num,hands[i].Cards, hands[i].Class, rank, hands[i].Bid, total_winnings);
  }

  printf("TESTING!\n");

  free(hands); 
  return EXIT_SUCCESS;
}
