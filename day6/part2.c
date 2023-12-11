#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
  long hold;
  long distance;
} HoldTime;

int process_input(long **times, long **records, int max_races){
  FILE *file = fopen("input", "r");
  if(file == NULL){
    perror("Unable to open input\n");
    exit(EXIT_FAILURE);
  }
  char buffer[1024];
  fgets(buffer,sizeof(buffer),file);


  //process the time.
  *times = (long *)malloc(max_races * sizeof(long));
  if(times == NULL){
    perror("Times malloc error\n");
    exit(EXIT_FAILURE);
  }
  int race_num = 0;

  char *token = strtok(buffer, "Time:");
  int len = strlen(token);
  char time[1024];
  int time_len = 0;
  for(int i = 0; i < len; ++i){
    if(isdigit(token[i])){
      time[time_len++] = token[i];
    }
  }
  time[time_len] = '\0';
  char *end;
  (*times)[race_num]= strtol(time,&end,10);


  fgets(buffer,sizeof(buffer), file);
  *records = (long *)malloc(max_races * sizeof(long));
  if(records == NULL){
    perror("records malloc error\n");
    exit(EXIT_FAILURE);
  }

  //process the records.
  race_num = 0;
  token = strtok(buffer, "Distance:");
  len = strlen(token);
  char record[1024];
  int record_len = 0;
  for(int i = 0; i < len; ++i){
    if(isdigit(token[i])){
      record[record_len++] = token[i];
    }
  }
  record[record_len] = '\0';
  (*records)[race_num] = strtol(record,&end,10);

  // the single race is completed!
  ++race_num;
  
  for(int i = 0; i < race_num; ++i){
    printf("Race %d: time: %ld record: %ld\n", i, (*times)[i],(*records)[i]);
  }

  return race_num;
}

long calc_button(long time, long record){
  
  // setup a data structre to record all possible options.
  HoldTime *hold_times;
  hold_times =  (HoldTime *)malloc(time * sizeof(HoldTime));
  if(hold_times == NULL){
    perror("Alloc err in calc_button\n");
    exit(EXIT_FAILURE);
  }


  // store the hold time and the distances;
  for(long i = 0; i < time; ++i){
    // check the distance it'll go.
    long tmp = (time - i) * i;
    hold_times[i].hold = i;
    hold_times[i].distance = (time - i)*i;
  }


  // find how many beat the record.
  long number_record_beat = 0;
  for(long i = 0; i < time; ++i){
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

  long *times, *records;
  int max_races = 1;


  int num_races = process_input(&times, &records, max_races);
  printf("Num races: %d\n", num_races);

  long *totals;
  totals = (long *)malloc(num_races * sizeof(long));
  if(totals == NULL){
    perror("Totals malloc failed\n");
    exit(EXIT_FAILURE);
  }


  printf("Totals: ");
  long power = 1; // she cant be zero to start!
  for(int i = 0; i < num_races; ++i){
    totals[i] = calc_button(times[i],records[i]);
    printf(" %ld, ", totals[i]);
    if(totals[i] > 0){
      power *= totals[i];
    }
  }
  printf("\nPower: %ld\n", power);
 


  free(totals);
  return EXIT_SUCCESS;
}
