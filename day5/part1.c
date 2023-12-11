#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    SEED_TO_SOIL,
    SOIL_TO_FERTILIZER,
    FERTILIZER_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMPERATURE,
    TEMPERATURE_TO_HUMIDITY,
    HUMIDITY_TO_LOCATION
} MapType;  

typedef struct {
  long src_start;
  long dest_start;
  long length;
  MapType type;
} MapEntry;



typedef struct {
    int seed_count;
    long *seeds; // Pointer to an array of ints
    int maps_count;
    MapEntry *maps; // Flexible array member for MapEntry
} GameStruct;

void process_input(GameStruct *game){
  FILE *file = fopen("input", "r");
  if(file == NULL){
    perror("Error opening input data\n");
    exit(EXIT_FAILURE);
  }
  

  // process the seeds.
  char buffer[1024];
  long *seeds = calloc(4, sizeof(long));
  if(seeds == NULL){
    perror("failed to alloc seeds\n");
    exit(EXIT_FAILURE);
  }

  fgets(buffer,sizeof(buffer),file);
  int seed_count = 0;
  char *end;
  char *token = strtok(buffer+7, " ");
  while(token != NULL){
    seeds[seed_count] = strtol(token, &end, 10);
    token = strtok(NULL, " ");
    ++seed_count;
  }
  printf("seeds: %d\n", seed_count);


  // process the maps.
  int initial_maps_count = 10;
  MapEntry *maps = calloc(initial_maps_count,sizeof(MapEntry));
  if(maps == NULL){
    perror("Mapentry alloc failure\n");
    exit(EXIT_FAILURE);
  }

  MapEntry tmp = {0};
  int map_count = 0;
  int map_name_tracker = -1;
  while(fgets(buffer, sizeof(buffer), file)){
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';

    }
    // skip blank lines.
    if(len == 1){
      continue;
    }
    // update number for title lines
    if(buffer[len - 2] == ':' || len == 1){
      // cause i dont want to parse the whole seed-to-soil thing. im just using ints.
      ++map_name_tracker;
      continue;
    }


    if(map_count >= initial_maps_count){
      MapEntry *tmp = realloc(maps, (map_count + 1) * sizeof(MapEntry));
      if(tmp == NULL){
        perror("Realloc failed\n");
        exit(EXIT_FAILURE);
      }else{
        maps = tmp;
      }
    }

    char *token = strtok(buffer, " ");
    char *end;
    maps[map_count].dest_start = strtol(token, &end, 10);
    token = strtok(NULL, " ");
    maps[map_count].src_start = strtol(token, &end, 10);
    token = strtok(NULL, " ");
    maps[map_count].length = strtol(token, &end, 10);
    maps[map_count].type = map_name_tracker;
   // printf("Map %d: Type: %d, SRC: %ld, DEST: %ld, LEN: %ld\n", map_count, maps[map_count].type, maps[map_count].src_start, maps[map_count].dest_start, maps[map_count].length);
    ++map_count;

  }

  game->seed_count = seed_count;
  game->maps_count = map_count;
  game->seeds = seeds;
  game->maps = maps;
  fclose(file);
}

// Function to translate a number through a mapping
long translate_number(long number, MapEntry mapping[], int map_count) {


  for(int d = 0; d < map_count; ++d){
    long destination_start = mapping[d].dest_start;
    long source_start = mapping[d].src_start;
    long range_length = mapping[d].length;
    if (source_start <= number && number < source_start + range_length) {
      // Calculate the offset within the range
      long offset = number - source_start;
      // Translate the number using the offset
      return destination_start + offset;
    }

  }
  // If the number isn't in the mapping, it stays the same
  return number;
}


int main(int argc, char *argv[])
{

  GameStruct game = {0};
  process_input(&game);

  MapEntry* s2s_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int s2s_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == SEED_TO_SOIL) {
      s2s_Map[s2s_count++] = game.maps[i];
    }
  }

  MapEntry* s2f_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int s2f_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == SOIL_TO_FERTILIZER) {
      s2f_Map[s2f_count++] = game.maps[i];
    }
  }

  MapEntry* f2w_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int f2w_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == FERTILIZER_TO_WATER) {
      f2w_Map[f2w_count++] = game.maps[i];
    }
  }

  MapEntry* w2l_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int w2l_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == WATER_TO_LIGHT) {
      w2l_Map[w2l_count++] = game.maps[i];
    }
  }

  MapEntry* l2t_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int l2t_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == LIGHT_TO_TEMPERATURE) {
      l2t_Map[l2t_count++] = game.maps[i];
    }
  }

  MapEntry* t2h_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int t2h_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == TEMPERATURE_TO_HUMIDITY) {
      t2h_Map[t2h_count++] = game.maps[i];
    }
  }

  MapEntry* h2l_Map = malloc(game.maps_count * sizeof(game.maps[0]));
  int h2l_count = 0;
  for(int i = 0; i < game.maps_count; ++i){
    if (game.maps[i].type == HUMIDITY_TO_LOCATION) {
      h2l_Map[h2l_count++] = game.maps[i];
    }
  }

  printf("Counts: s2s: %d, s2f: %d, f2w: %d, w2l: %d, l2t: %d, t2h: %d, h2l: %d\n\n", s2s_count, s2f_count, f2w_count, w2l_count,l2t_count,t2h_count,h2l_count);

  long res = LONG_MAX;
  for(int c = 0; c < game.seed_count; c++) {
    long output = 0;
    printf("Initial Seed: %ld\n", game.seeds[c]);
    output = game.seeds[c];
    output = translate_number(output, s2s_Map, s2s_count); 
    output = translate_number(output, s2f_Map, s2f_count);
    output = translate_number(output, f2w_Map, f2w_count);
    output = translate_number(output, w2l_Map, w2l_count);
    output = translate_number(output, l2t_Map, l2t_count);
    output = translate_number(output, t2h_Map,t2h_count);
    output = translate_number(output, h2l_Map, h2l_count);
    printf("Final Output for Seed %ld: %ld\n\n", game.seeds[c], output);
    if(output < res){
      res = output;
    }
  }

  printf("Min num = %ld\n", res);

  return EXIT_SUCCESS;
}
