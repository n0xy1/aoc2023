#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROWS 152
#define MAX_COLS 152

typedef struct{
  int x;
  int y;
  char c;
  int num_matches;
  int nums[2];
} SymbolCoord;

int extractNumber(const char arr[MAX_ROWS][MAX_COLS], int y, int *x, int maxLength, int processed[MAX_ROWS][MAX_COLS]) {
    int num = 0;
    int length = 0;
    while (*x < MAX_COLS && isdigit(arr[y][*x]) && length < maxLength) {
        num = num * 10 + (arr[y][*x] - '0');
        processed[y][*x] = 1; // Mark this digit as processed
        (*x)++;
        length++;
    }
    return num;
}

int numberExists(int num, int numbers[], int count) {
    for (int i = 0; i < count; i++) {
        if (numbers[i] == num) {
            return 1; // Number exists
        }
    }
    return 0; // Number does not exist
}


int main(){
  FILE *file = fopen("input","r");
  if (file == NULL){
    perror("error opening input\n");
    exit(EXIT_FAILURE);
  }

  char arr[MAX_ROWS][MAX_COLS];
  memset(arr,0,sizeof(arr));


  int col = 0;
  int row = 0;
  while(fgets(arr[row], MAX_COLS, file) && row < MAX_ROWS){
    //strip the trailing \n
    arr[row][strcspn(arr[row], "\n")] = '\0';
    ++row;
  }
  fclose(file);
  col = strlen(arr[0]);


  //for(int i = 0; i < row; ++i){
 //   printf("%s\n",arr[i]);
  //}

  printf("num rows: %d, row width: %d\n", row, col);


  //loop through data to find symbols.
  int capacity = 10;
  SymbolCoord *symbols = (SymbolCoord *)calloc(capacity, sizeof(SymbolCoord));
  if(symbols == NULL){
    perror("Intial mem allocation failed\n");
    exit(EXIT_FAILURE);
  }

  int sym_count = 0;
  for(int i = 0; i < row; ++i){
    for (int j = 0; j < col; ++j){
      if(arr[i][j] == '*' 
  //      || arr[i][j] == '#' 
  //      || arr[i][j] == '+' 
  //      || arr[i][j] == '$'
  //      || arr[i][j] == '%' 
  //      || arr[i][j] == '='
  //      || arr[i][j] == '/'
  //      || arr[i][j] == '@'
  //      || arr[i][j] == '&'
  //      || arr[i][j] == '-'
      ){

        //printf("Symbol found: %c x: %d y: %d\n",arr[i][j], j, i);
        if (sym_count >= capacity){
          capacity *= 2; //double capacity.. saves heaps of reallocs than if we just added 1 more.
          SymbolCoord *temp = realloc(symbols, capacity * sizeof(SymbolCoord));
          if (temp == NULL){
            perror("realloc failed\n");
            free(symbols);
            exit(EXIT_FAILURE);
          }
          symbols = temp;
        }
        symbols[sym_count].x = j;
        symbols[sym_count].y = i;
        symbols[sym_count].c = arr[i][j];
        sym_count++;
      }
    }
  }
  printf("%d symbols found.\n", sym_count);

  int extracted_nums[5000] = {0};
  int extracted_count = 0;
  int processed[MAX_ROWS][MAX_COLS] = {{0}}; // Initialize all to 0

  for (int c = 0; c < sym_count; ++c) {
    // Assume MAX_ROWS and MAX_COLS are defined as the dimensions of 'arr'
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int y = symbols[c].y + i;
            int x = symbols[c].x + j;

            // Check bounds
                if (y >= 0 && y < MAX_ROWS && x >= 0 && x < MAX_COLS && !processed[y][x] && isdigit(arr[y][x])) {
                    // Move to the start of the number
                    while (x > 0 && isdigit(arr[y][x - 1])) {
                        x--;
                    }

                    symbols[c].num_matches += 1;

                    // Extract the number up to 3 digits
                    if (!processed[y][x]) { // Check again in case the start of the number was already processed
                        int number = extractNumber(arr, y, &x, 3, processed);
                            symbols[c].nums[symbols[c].num_matches -1] = number;
                            printf("Found a number: %d at (%d, %d)\n", number, y, x);
                            extracted_nums[extracted_count] = number;
                            ++extracted_count;
                    }
                }
            }
        }
    }

  int total = 0;
  for(int c = 0; c <= extracted_count; ++c){
    if(symbols[c].c == '*'){
      printf("Symbol: %c - Num matches: %d - Num: %d, %d\n", symbols[c].c, symbols[c].num_matches, symbols[c].nums[0], symbols[c].nums[1]);
      total += symbols[c].nums[0] * symbols[c].nums[1];
    }

  }
  
  printf("Extracted: %d  Total: %d\n", extracted_count, total);


  return 0;
}
