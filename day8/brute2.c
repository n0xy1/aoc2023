#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 786

// Struct for the value pair
typedef struct{
  char *left;
  char *right;
} ValuePair;

// Hashmap entry struct  
typedef struct HashmapNode{
  char *key;
  ValuePair value;
  struct HashmapNode *next;
} HashmapNode;

typedef struct {
  HashmapNode *entries[HASHMAP_SIZE];
} Hashmap;


// simple hash func.
unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    // Convert our string to an integer
    while (i < key_len) {
        value = value * 37 + key[i];
        i++;
    }

    return value % HASHMAP_SIZE;
}

//Init Hashmap
Hashmap* create_hashmap(){
  Hashmap *hm = malloc(sizeof(Hashmap));
  memset(hm->entries, 0, sizeof(hm->entries));
  return hm;
}

//insert into Hashmap
void hashmap_insert(Hashmap *hm, const char *key, ValuePair value){
  unsigned int slot = hash(key);

  //Make space for it.
  HashmapNode *newNode = malloc(sizeof(HashmapNode));
  newNode->key = strdup(key);
  newNode->value = value;
  newNode->next = NULL;

  //insert it
  if(hm->entries[slot] == NULL){
    hm->entries[slot] = newNode;
  }else{
    newNode->next = hm->entries[slot];
    hm->entries[slot] = newNode;
  }
}

ValuePair* hashmap_search(Hashmap *hm, const char *key){
  if(hm == NULL || key == NULL){
    printf("hm or key is null!\n");
    return NULL;
  }
  unsigned int slot = hash(key);
  HashmapNode *node = hm->entries[slot];

  while(node != NULL){
    if(strcmp(node->key,key) == 0){
      return &node->value;
    }
    node = node->next;
  }
  //not found
  return NULL;
}

void free_hashmap(Hashmap *hm) {
  for(int i = 0; i < HASHMAP_SIZE; ++i){
    if(hm->entries[i]){
      free(hm->entries[i]->key);
      free(hm->entries[i]);
    }
  }
  free(hm);
}


int main(int argc, char *argv[])
{
  Hashmap *game = create_hashmap();


  FILE *file=fopen("input","r");
  if(file == NULL){
    perror("Error openinig file\n");
    exit(EXIT_FAILURE);
  }

  char buffer[1024];
  fgets(buffer, sizeof(buffer),file);
  int len = strlen(buffer);
  
  char *instructions = malloc(len);
  if(instructions == NULL){
    perror("instructions malloc failed\n");
    exit(EXIT_FAILURE);
  }

  strncpy(instructions, buffer,len);
  // remove the \n
  instructions[len-1] = '\0';

  //ignore blank
  fgets(buffer,sizeof(buffer),file);

  //process the maps;
  int num_elem = 0;
  while(fgets(buffer,sizeof(buffer),file)){
    int len = strlen(buffer);
    buffer[len - 1] = '\0';

    char key[4] = {0};
    char left[4] = {0};
    char right[4] = {0};

    strncpy(key,buffer,3);
    key[3] = '\0';
    strncpy(left, &buffer[7], 3);
    left[3] = '\0';
    strncpy(right, &buffer[12], 3);
    right[3] = '\0';

    ValuePair tmp;
    tmp.left = strdup(left);
    tmp.right = strdup(right);
    hashmap_insert(game, key,tmp);
    ++num_elem;

  }
  printf("Elements: %d, instructions: %ld\n", num_elem, strlen(instructions));
  fclose(file);


  printf("%s\n", instructions);

  char keys[6][4] = {"JTA", "BLA", "AAA", "NBA", "QXA", "TSA"};
  int instruction_positions[6] = {0}; // Position in the instruction set for each key
  long iterations = 0;

  int all_finished = 0;
  while (!all_finished) {
    all_finished = 1; //set it as done at the start.
    for (int i = 0; i < 6; ++i) {
      //printf("key %d = %s: ", i, keys[i]);
      ValuePair *res = hashmap_search(game, keys[i]);
      if (res == NULL) {
        printf("Cannot find key: %s!\n", keys[i]);
        continue;
      }
      //printf("(%s, %s) ", res->left, res->right);

      if (instructions[instruction_positions[i]] == 'R') {
        strcpy(keys[i], res->right);
      } else {
        strcpy(keys[i], res->left);
      }

      //printf("Taking %s\n", keys[i]);
      instruction_positions[i] = (instruction_positions[i] + 1) % strlen(instructions);
      if (keys[i][2] != 'Z') {
        all_finished = 0; // If any key does not end in 'Z', continue processing
      }   
    }

    printf(" iterations: %ld\n", iterations);
    ++iterations;
  }

  printf("Done! All keys ended with 'Z'. Total iterations: %ld\n", iterations);


  return EXIT_SUCCESS;
}
