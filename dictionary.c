//Author: Mike Urbano

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
struct node
{
   char word[200];
   struct node *next;
};
void open_data_file(char[], FILE*, int*, struct node*[], int[]);
void h1_function(char[], int*);
void h2_function(char[], int*);
void process_data_file(char[], int*, struct node *[], char*, int*);
void search_function(char[], int*, struct node *[], int*);
void insert_function(char[], char[], int*, struct node *[], int*);
void delete_function(char[], int*, struct node*[], int*);
int main() {
   char option[2];
   char data_file_name[100];
   char key_word[100];
   char tr_word[100];
   FILE *file_ptr;
   int *k = malloc(sizeof(int));
   int *num_probes = malloc(sizeof(int));
   int  i;
   int probes[300];
   *k = 0;    //k_value;
   *num_probes = 0;
   float res;
   struct node *table[35027];
   //for (i=0; i < 35027; i++) table[*k] = NULL;
   for (i=0; i < 300; i++) probes[i] = 0;
   printf("Enter file name: ");
   scanf("%s", data_file_name);
   open_data_file(data_file_name, file_ptr, k, table, probes);
   i = 0;    //used to track number of operations
   while(*option != 'q')  {
      printf("\nEnter s to search, i to insert, d to delete and q to quit: ");
      scanf("%s", option);
      if (*option == 'q') break;
      if (*option == 's') {
         printf("Enter a word to search: ");
         scanf("%s", key_word);
         printf("\nRead op: %s query: %s\n", option, key_word);
         h1_function(key_word, k);
         search_function(key_word, k, table, num_probes);
         *k = 0;
         i += 1;
         probes[0] += *num_probes;  //using element 0 of probes array to store num of probes during ops
         *num_probes = 0;
         continue;
      }
      if (*option == 'i')   {
         printf("Enter a word and its transation to add: ");
         scanf("%s %s", key_word, tr_word);
         printf("\nRead op: %s query: %s\n", option, key_word);
         h1_function(key_word, k);
         insert_function(key_word, tr_word, k, table, num_probes);
         i += 1;
         *k = 0;
         probes[0] += *num_probes;
         *num_probes = 0;
         continue;
      }
      if (*option == 'd')    {
         printf("Enter the word to delete: ");
         scanf("%s", key_word);
         printf("\nRead op: %s, query: %s\n", option, key_word);
         h1_function(key_word, k);
         delete_function(key_word, k, table, num_probes);
         *k = 0;
         i += 1;
         probes[0] += *num_probes;
         *num_probes = 0;
         continue;
      }
   }
   float v = i;
   res = probes[0]/v;
   printf("\nTotal probes: %d\n", probes[0]);
   printf("\nAverage probes per operation: %.2f\n", res);
   free(k);
   free(num_probes);
   struct node *current, *temp;

   for(i = 0; i < 30000; i++)  {
      if(table[i] != NULL) {
         current = table[i];
         while(current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
         }
      }
   }
}
void open_data_file(char data_file_name[], FILE *file_ptr, int *k, struct node *table[], int probes[]) {
  char line[1000];
  char *line_ptr;
  char key_word[100];
  int *n_probes;
  int max_probes, i;
  float average_probes;
  int  total_probes = 0;
  int number_of_probes = 1, n_items = 0;
  n_probes = &number_of_probes;
  file_ptr = fopen(data_file_name, "r");
  if(file_ptr == NULL) {
     printf("Error, unable to open file.\n");
     return;
  }
  else printf("File opened!\n");
  do {
     fgets(line, 1000, file_ptr);
     line_ptr = line;
     line[strlen(line)-1] = '\0';
     sscanf(line_ptr, "%[^\t]\ts", key_word);
     line_ptr += strlen(key_word)+1;
     //printf("The key is %s and the rest is %s\n", key_word, line_ptr);
     h1_function(key_word, k);
     //*n_probes = 1;
     process_data_file(key_word, k, table, line_ptr, n_probes);
     //*n_items +=1;
     //probes_array[*n_probes] += 1;
     //*n_probes = 0;
     total_probes += *n_probes;
     probes[*n_probes] += 1;
     n_items += 1;
     *k = 0;
     *n_probes = 1;
  }while(!feof(file_ptr));
  fclose(file_ptr);
  //for(i = 0; i < 15017; i++) total_probes = total_probes + probes[i];
  max_probes = probes[1];
  for(i = 2; i < 200; i++)  {
     if(probes[i] != 0)
        max_probes = i;
  }
  float fn_items = n_items;
  average_probes = total_probes/fn_items;
  printf("\nHash table\n");
  printf("Total number of items hashed: %d\n", n_items);
  printf("Average number of probes: %.2f \n", average_probes);
  printf("Max run of probes: %d \n", max_probes);
  printf("Total probes for 7477 items: %d \n", total_probes);
  printf("Items not hashed out of 7477: 0\n\n");
  printf("Probes | Count of keys\n");
  for(i = 0; i < 100; i++) {
     printf("-----------------\n");
     printf("   %4d|     %4d\n", i, probes[i]);
     //printf("------------------\n");
  }
}
void h1_function(char key_word[], int *k) {
  int i, j = 0, M = 15017;
  for(i = 0; i < strlen(key_word); i++) {
     j += (key_word[i]*128^i);
  }
  *k = j%M;
  //printf("\nThe value of k is %d\n", *k);
}
void h2_function(char key_word[], int *k)   {
  int M = 15017;
  int i = *k;
  *k += (1 + 3^2)%M;
  //printf("\nThe NEW value of k is %d for %s\n", *k, key_word);
}
void process_data_file(char key_word[], int *k, struct node *table[], char *line_ptr, int *n_probes) {
  struct node *current;
  if(table[*k] == NULL) {
     char entry[200];
     table[*k] = malloc(sizeof(struct node));
     current = table[*k];
     strcpy(current->word, key_word);
     current->next = malloc(sizeof(struct node));
     current = current->next;
     sscanf(line_ptr, "%[^\n]\ns", entry);
     sscanf(entry, "%[^\n]\ns", current->word);
     current->next = NULL;
     return;
  }
  if(table[*k] != NULL && strcmp(key_word, table[*k]->word) == 0) {
     char entry[200];
     struct node *current;
     struct node *temp;
     current = table[*k];
     while(current != NULL) {
        temp = current;
        current = current->next;
     }
     current = malloc(sizeof(struct node));
     sscanf(line_ptr, "%[^\n]\ns", entry);
     sscanf(entry, "%[^\n]\ns", current->word);
     current->next = NULL;
     temp->next = current;
     current = table[*k];
     return;
  }
  if(table[*k] != NULL && strcmp(key_word, table[*k]->word) != 0) {
     while(table[*k] != NULL && strcmp(key_word, table[*k]->word) != 0) {
        //*n_probes += 1;
        h2_function(key_word, k);
        *n_probes += 1;
     }
     process_data_file(key_word, k, table, line_ptr, n_probes);
  }
}
void search_function(char key_word[], int *k, struct node *table[], int *num_probes) {
  struct node* current;
  current = table[*k];
  int i = 1;
  if(table[*k] != NULL && strcmp(key_word, table[*k]->word) == 0)  {
     current = current->next;
     *num_probes += 1;
     printf("%d probes\n", *num_probes);
     //printf("%s stored at %d\n", key_word, *k);
     printf("\nTranslations:\n");
     while(current != NULL)  {
        printf("%d)%s\n", i, current->word);
        current = current->next;
        i++;
     }
  printf("\n\n");
  return;
  }
  if(table[*k] != NULL && strcmp(key_word, table[*k]->word) != 0)  {
     while(table[*k] != NULL && strcmp(key_word, table[*k]->word) != 0) {
        h2_function(key_word, k);
        *num_probes += 1;
     }
     search_function(key_word, k, table, num_probes);
     return;
  }
  if(table[*k] == NULL)  {
     printf("%d probes\n", *num_probes);
     printf("Key word not found.\n");
     return;
  }
}
void insert_function(char key_word[], char tr_word[], int *k, struct node *table[], int *num_probes)  {
  //printf("Key word is %s and translated word is %s.\n", key_word, tr_word);
  char deleted_word[100] = "-999";
  if(table[*k] == NULL || strcmp(table[*k]->word, deleted_word) == 0) {
     struct node *current;
     table[*k] = malloc(sizeof(struct node));
     *num_probes += 1;
     current = table[*k];
     strcpy(current->word, key_word);
     //printf("Word %s is saved at index %d\n", key_word, *k);
     //printf("word saved at old spot is %s\n", table[7178]->word);
     current->next = malloc(sizeof(struct node));
     current = current->next;
     strcpy(current->word, tr_word);
     current->next = NULL;
     printf("%d probes\n", *num_probes);
     printf("Will insert pair [%s, %s]\n", key_word, tr_word);
     return;
  }
  if(table[*k] != NULL && (strcmp(key_word, table[*k]->word) == 0)) {
     struct node *current;
     *num_probes += 1;
     struct node *temp;
     current = table[*k];
     while(current != NULL)  {
        temp = current;
        current = current->next;
     }
     current = malloc(sizeof(struct node));
     strcpy(current->word, tr_word);
     current->next = NULL;
     temp->next = current;
     printf("%d probes\n", *num_probes);
     printf("Will add transation %s to %s.\n", tr_word, key_word);
     return;
  }
  if(table[*k] != NULL && (strcmp(deleted_word,table[*k]->word) != 0 && strcmp(table[*k]->word, key_word) != 0))  {
     while(table[*k] != NULL && (strcmp(deleted_word, table[*k]->word) != 0 && strcmp(key_word, table[*k]->word) != 0))  {
        //printf("Called h2 function, value of k is %d\n", *k);
        h2_function(key_word, k);
        *num_probes += 1;
     }
     insert_function(key_word, tr_word, k, table, num_probes);
     return;
  }
}
void delete_function(char key_word[], int *k, struct node *table[], int *num_probes)  {
  if(table[*k] == NULL)  {
     *num_probes += 1;
     printf("%d probes\n", *num_probes);
     printf("Word not found!\n\n");
  }
  if(table[*k] != NULL && strcmp(key_word, table[*k]->word) == 0)  {
     struct node *temp, *current;
     *num_probes += 1;
     current = table[*k]->next;
     while(current != NULL)   {
        temp = current;
        current = current->next;
        free(temp);
     }
     strcpy(table[*k]->word, "-999");
     table[*k]->next = NULL;
     printf("%d probes\n", *num_probes);
     printf("Item delted.\n");
     return;
  }

  if(table[*k] != NULL && strcmp(key_word, table[*k]->word) != 0)  {
     while(table[*k] != NULL && strcmp(key_word, table[*k]->word)!= 0) {
        h2_function(key_word, k);
        *num_probes += 1;
     }
     delete_function(key_word, k, table, num_probes);
     return;
  }
}
