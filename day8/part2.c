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

// Function to calculate GCD
long gcd(long a, long b) {
    if (b == 0) 
        return a;
    return gcd(b, a % b);
}

// Function to calculate LCM of two numbers
long lcm(long a, long b) {
    return (a / gcd(a, b)) * b;
}

// Function to calculate LCM of an array of numbers
long lcm_of_array(long arr[], int n) {
    long result = arr[0];
    for (int i = 1; i < n; i++) {
        result = lcm(result, arr[i]);
    }
    return result;
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
char *keys[] = {"JTA", "BLA", "AAA", "NBA", "QXA", "TSA"};
int num_keys = sizeof(keys) / sizeof(keys[0]);
long res[6] = {0};

for (int key_index = 0; key_index < num_keys; ++key_index) {
    char current_key[4]; // Assuming keys are always 3 characters + null terminator
    strcpy(current_key, keys[key_index]);

    int instruction_pos = 0;
    long iteration = 0;

    while (current_key[2] != 'Z') {
        ValuePair *res = hashmap_search(game, current_key);
        if (res == NULL) {
            // Key not found.
            printf("Cannot find key: %s!\n", current_key);
            break;
        }

        if (instructions[instruction_pos] == 'R') {
            strcpy(current_key, res->right);
        } else {
            strcpy(current_key, res->left);
        }

        // Cycle through instructions.
        ++iteration;
        instruction_pos = (instruction_pos + 1) % strlen(instructions);
    }
    res[key_index] = iteration;
    printf("Done with key %s! %ld iterations\n", current_key, iteration);
}

  // brute would take ages.. i read around on the net to solve this without bruting.
  // lowest common multiple finds it.
  printf("LCM of the entered numbers is: %ld\n", lcm_of_array(res, 6));

  return EXIT_SUCCESS;
}
