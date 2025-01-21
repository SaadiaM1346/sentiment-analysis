// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "mySA_functions.h"

int main(int argc, char *argv[]) {
    // Ensure correct amount of arguments
    if (argc != 3) {
        printf("Usage: %s <lexicon> <data_input>\n", argv[0]);
        return 1; // Exit program
    }

    // Define variables
    struct words *vader_word; 
    int count; 
    double token_score = 0;
    int num_words = 0;
    int max_length = 0;

    // Load the lexicon into struct
    make_vader_struct(&vader_word, &count, argv[1]);

    // Open the validation.txt file
    FILE *validation_file = fopen(argv[2], "r");
    
    // Error if file can't be opened
    if (validation_file == NULL) {
        printf("Error opening the validation file.\n");
        return 1; // Exit program
    }
    
    // Allocate memory for reading sentences in files
    char *sentence = malloc(100*sizeof(char)); 
    //char sentence[100];

    // Error if memory can't be allocated
    if (sentence == NULL) {
        printf("Memory allocation failed.\n");
        return 1; // Exit program
    }

    // Loop through validation.txt and store the max length of a sentence in the file
    while (fgets(sentence, 100, validation_file) != NULL) {
        int length = strlen(sentence); // Set to length of current sentence
        
        // Check if length is more than current max_length
        if (length > max_length) {
            max_length = length; // Replace max_length with current length of string
        }
    }

    // Reallocate memory for sentence based one max_length
    sentence = realloc(sentence, (max_length + 1) * sizeof(char));

    // Reset file pointer to the beginning of the file
    fseek(validation_file, 0, SEEK_SET);

    // Print display based on max_length in file
    print_display(max_length);

    // Score every sentence in validation.txt
    sentence_scoring(validation_file, sentence, max_length, vader_word, count);

    // Free dynamically allocated memory
    for (int i = 0; i < count; i++) {
        free(vader_word[i].word);
        free(vader_word[i].SIS_array);
    }

    free(vader_word);
    free(sentence);

    // Sucessfully exit program
    return 0;

}