#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int hold;
  int distance;
} HoldTime;

int process_input(int **times, int **records, int max_races){
  FILE *file = fopen("input", "r");
  if(file == NULL){
    perror("Unable to open input\n");
    exit(EXIT_FAILURE);
  }
  char buffer[1024];
  fgets(buffer,sizeof(buffer),file);


  //process the times.
  *times = (int *)malloc(max_races * sizeof(int));
  if(times == NULL){
    perror("Times malloc error\n");
    exit(EXIT_FAILURE);
  }
  int race_num = 0;

  char *token = strtok(buffer, " ");
  token = strtok(NULL, " ");
  while(token != NULL && token[0] != '\0'){
    if(token[0] != '\n'){
      (*times)[race_num] = atoi(token);
      ++race_num;
    }
    token = strtok(NULL, " ");
  }

  fgets(buffer,sizeof(buffer), file);
  *records = (int *)malloc(max_races * sizeof(int));
  if(records == NULL){
    perror("records malloc error\n");
    exit(EXIT_FAILURE);
  }

  //process the records.
  race_num = 0;
  token = strtok(buffer, " ");
  token = strtok(NULL, " ");
  while(token != NULL && token[0] != '\0'){
    if(token[0] != '\n'){
      (*records)[race_num] = atoi(token);
      ++race_num;
    }
    token = strtok(NULL, " ");
  }

  for(int i = 0; i < race_num; ++i){
    printf("Race %d: time: %d record: %d\n", i, (*times)[i],(*records)[i]);
  }

  return race_num;
}

int calc_button(int time, int record){
  
  // setup a data structre to record all possible options.
  HoldTime *hold_times;
  hold_times =  (HoldTime *)malloc(time * sizeof(HoldTime));
  if(hold_times == NULL){
    perror("Alloc err in calc_button\n");
    exit(EXIT_FAILURE);
  }


  // store the hold time and the distances;
  for(int i = 0; i < time; ++i){
    // check the distance it'll go.
    int tmp = (time - i) * i;
    hold_times[i].hold = i;
    hold_times[i].distance = (time - i)*i;
  }


  // find how many beat the record.
  int number_record_beat = 0;
  for(int i = 0; i < time; ++i){
//    printf("Hold: %d, Dist: %d\n", hold_times[i].hold, hold_times[i].distance);
    if(hold_times[i].distance > record){
      ++number_record_beat;
    }
  }

  free(hold_times);
  //return the number that beat the record.
  return number_record_beat;
}

int main(int argc, char *argv[])
{

  int *times, *records;
  int max_races = 10;


  int num_races = process_input(&times, &records, max_races);
  printf("Numn races: %d\n", num_races);

  int *totals;
  totals = (int *)malloc(num_races * sizeof(int));
  if(totals == NULL){
    perror("Totals malloc failed\n");
    exit(EXIT_FAILURE);
  }


  printf("Totals: ");
  int power = 1; // she cant be zero to start!
  for(int i = 0; i < num_races; ++i){
    totals[i] = calc_button(times[i],records[i]);
    printf(" %d, ", totals[i]);
    if(totals[i] > 0){
      power *= totals[i];
    }
  }
  printf("\nPower: %d\n", power);
 


  free(totals);
  return EXIT_SUCCESS;
}
