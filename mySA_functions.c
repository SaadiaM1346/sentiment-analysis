// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "mySA_functions.h"

// Function to remove the punctuation of a token
// Keeps track of number of exclamation marks if present
void remove_punct(char *token, int *num_exclamation) {
    // Set number of excalmation points to zero
    *num_exclamation = 0; 

    // Loop through every character in token
    for (int i = 0; i < strlen(token); ++i) {

        // Check if character is an exclamation point
        if (token[i]=='!') {
            *num_exclamation += 1; // Increase by 1 if character is an exclamation point
        }

        // Check is character is punctuation
        if (ispunct(token[i])) { 
            token[i]='\0'; // Removes it from string
        }
    }    
}

// Function to turn token lower case
void make_lower(char *token) {
    // Loop through every character in token
    for (int i = 0; i < strlen(token); ++i) {
        token[i] = tolower(token[i]); // Replace with lowercase letter
    }    
}

// Function to check if the sentence is all spaces
bool is_all_space(char *sentence){
    for (int i = 0; i < strlen(sentence); i++) {
        if (!isspace(sentence[i])) {
            return false; // Return false if sentence contains nonspace characters
        }
    }
    return true; // Return true if all characters are spaces
}

// Function to check if "words" with spaces in them appear in the sentence
void has_space_words(struct words *vader_word, int count, char *sentence, double *total_score, int *num_words) {
    // Create a copy of the sentence for lowercase comparison
    char *temp_sentence = malloc(strlen(sentence) + 1);

    // Print error is memory could not be allocated
    if (temp_sentence == NULL) {
        printf("Memory for sentence could not be allocated!\n");
        return;
    }
    
    // Copy sentence to temp_sentence so alterations don't change original sentence
    strcpy(temp_sentence, sentence);

    // Convert all characters in temp_sentence to lowercase
    make_lower(temp_sentence); 
    
    // Loop through each word in vader_word
    for (int i = 0; i < count; i++) {

        // Check if the word has a space
        if (strstr(vader_word[i].word, " ") != NULL) {
            // Check if the word appears in the sentence
            if (strstr(sentence, vader_word[i].word) != NULL) {
                // Increase word count by 1 
                (*num_words)++;

                // Add the word score to the total score
                *total_score += vader_word[i].score;

                // Remove the word from the sentence by replacing it with an empty string
                char *position = strstr(sentence, vader_word[i].word);
                if (position != NULL) {
                    memset(position, ' ', strlen(vader_word[i].word));
                }
               
            } else {
                // Check if the lowercase word appears in the sentence
                if (strstr(temp_sentence, vader_word[i].word) != NULL) {
                    // Increase word count by 1 
                    (*num_words)++;

                    // Add the word score to the total score
                    if (vader_word[i].score < 0) {
                        *total_score += vader_word[i].score - 1;
                         // Minus 1 for emphasis of word being uppercase
                    } else{
                        *total_score += vader_word[i].score+1; // PLus 1 for emphasis of word being uppercase
                    }
                    
                    // Remove the word from the temp_sentence by replacing it with an empty string
                    char *position = strstr(temp_sentence, vader_word[i].word);
                    if (position != NULL) {
                        memset(position, ' ', strlen(vader_word[i].word));

                        // Remove the word from the original sentence based on the position in the temp_sentence
                        int index = position - temp_sentence;
                        memset(sentence + index, ' ', strlen(vader_word[i].word));
                    }
                }
            }
        }
    }
    // Free the allocated memory for the temporary sentence
    free(temp_sentence);
}

// Function to create a struct words using the lexicon
void make_vader_struct(struct words **vader_word, int *count, const char *lexicon_filename) {
    // Read file 
    FILE *lexicon_file = fopen(lexicon_filename, "r");

    // Error if file cannot be opened
    if (lexicon_file == NULL) {
        printf("Error opening the lexicon file.\n");
        return;
    }

    // Keep track of line number
    *count = 0;

    // Allocate memory for struct
    *vader_word = malloc(sizeof(struct words));

    // Error if memory cannot be allocated
    if (*vader_word == NULL) {
        printf("Memory allocation failed.\n");
        fclose(lexicon_file);
        return;
    }

    // Define variables for struct
    char word[100];
    float score, SD;
    int SIS_array[10];

    while (1) {
        // Dynamically allocate memory for the new word
        (*vader_word)[*count].word = malloc(100 * sizeof(char));

        // Error if memory cannot be allocated
        if ((*vader_word)[*count].word == NULL) {
            printf("Memory allocation failed.\n");
            fclose(lexicon_file);
            return;
        }

        // Dynamically allocate memory for the array
        (*vader_word)[*count].SIS_array = malloc(10 * sizeof(int));

        // Error if memory cannot be allocated
        if ((*vader_word)[*count].SIS_array == NULL) {
            printf("Memory allocation failed.\n");
            fclose(lexicon_file);
            return;
        }
        
        // Check if file line is the right format
        if (fscanf(lexicon_file, "%[^\t] %f %f [%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
                    (*vader_word)[*count].word, &score, &SD, &SIS_array[0], &SIS_array[1], &SIS_array[2],
                    &SIS_array[3], &SIS_array[4], &SIS_array[5], &SIS_array[6],
                    &SIS_array[7], &SIS_array[8], &SIS_array[9]) == 13) {      
            
            // Set struct value to score
            (*vader_word)[*count].score = score;

            // Set struct value to standard dev
            (*vader_word)[*count].SD = SD;

            // Set struct value to array
            for (int i = 0; i < 10; i++) {
                (*vader_word)[*count].SIS_array[i] = SIS_array[i];
            }
            
            // Increase count by 1
            (*count)++;

            // Resize the array of word struct
            *vader_word = realloc(*vader_word, (*count + 1) * sizeof(struct words));
            if (*vader_word == NULL) {
                // Handle reallocation failure
                printf("Memory allocation failed.\n");
                fclose(lexicon_file);
                return;
            }

        } else if (fscanf(lexicon_file, "%[^\t] %f %f [%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n",
                    (*vader_word)[*count].word, &score, &SD, &SIS_array[0], &SIS_array[1], &SIS_array[2],
                    &SIS_array[3], &SIS_array[4], &SIS_array[5], &SIS_array[6],
                    &SIS_array[7], &SIS_array[8], &SIS_array[9]) == EOF) { 
            break; // Break loop if end of file is reached

        } else {
            // Error for incorrect formatting 
            printf("Incorrect format on line %d in %s\n", *count + 1, lexicon_filename);
            break;
        }
        free((*vader_word)[*count].word);
    }
    // Close lexicon file
    fclose(lexicon_file);
}

// Function to calculate score for a given sentence
void sentence_scoring(FILE *validation_file, char *sentence, int max_length, struct words *vader_word, int count) {
    double token_score = 0; // Initialize token score
    int num_words = 0; // Initialize word count

    // Loop through lines in the validation file
    while (fgets(sentence, max_length, validation_file) != NULL) {
        sentence[strcspn(sentence, "\n")] = '\0'; // Remove newline character

        // Skip empty lines
        if (strcmp(sentence, "") == 0){
            continue;
        }

        // If the line is not empty and contains non-space characters
        if ((strcmp(sentence, "") != 0) && (!is_all_space(sentence))){
            printf("%s", sentence); // Print the sentence

            // Print spaces to align the output
            printf("%*s", (int)(max_length - strlen(sentence) + 4), " ");

            // Call function to process words in the sentence
            has_space_words(vader_word, count, sentence, &token_score, &num_words);
            
            // If the sentence is not empty
            if ((strcmp(sentence, "") != 0) && (!is_all_space(sentence))) {
                char *token = strtok(sentence, " "); // Tokenize the sentence
                
                // Loop through tokens in the sentence
                while (token != NULL) {
                    int num_exclamation = 0; // Initialize count for exclamation marks
                    
                    // Lookup token in lexicon
                    for (int i = 0; i < count; i++) {
                        if (strcmp(vader_word[i].word, token) == 0) {
                            token_score += vader_word[i].score; // Add score of token
                            break;
                        } else {                        
                            char *temp_token = malloc(strlen(token) + 1); 
                            strcpy(temp_token, token);                     

                            // Remove punctuation from temporary token
                            remove_punct(temp_token, &num_exclamation);     

                            // Check if modified token matches a word in the lexicon
                            if (strcmp(vader_word[i].word, temp_token) == 0) {
                                token_score += vader_word[i].score; // Add score of token
                                // Adjust score based on the number of exclamation marks
                                if (vader_word[i].score < 0) {
                                    token_score -= 0.1 * num_exclamation;
                                } else {
                                    token_score += 0.1 * num_exclamation;
                                }
                                break;
                            } else {
                                make_lower(temp_token); // Convert token to lowercase
                                
                                // Check if modified token matches a word in the lexicon
                                if (strcmp(vader_word[i].word, temp_token) == 0) {
                                    token_score += vader_word[i].score; // Add score of token
                                    remove_punct(token, &num_exclamation); // Remove punctuation
                                    // Adjust score based on the number of exclamation marks
                                    if (vader_word[i].score < 0) {
                                        token_score -= 0.1 * num_exclamation;
                                        token_score -= 1; // Adjust score for negative words
                                    } else {
                                        token_score += 0.1 * num_exclamation;
                                        token_score += 1; // Adjust score for positive words
                                    }
                                    break;
                                }
                            }
                            free(temp_token);
                        }
                    }
                    token = strtok(NULL, " "); // Move to the next token
                    num_words++; // Increment word count
                }
            }

            token_score = token_score / num_words; // Calculate average token score

            printf("%f\n", token_score); // Print the calculated score
            num_words = 0; // Reset word count
            token_score = 0; // Reset token score
        }
    }

    fclose(validation_file); // Close the validation file
}


// Function to print the output display
void print_display (int max_length) {
    // Print header
    printf("\tstring sample");
    for (int i = 0; i < max_length - strlen("string sample")-3; i++) { // Print enough space for display to work for different max sized sentence
        printf(" ");
    }
    printf("score\n");

    // Print dashes
    for (int i = 0; i < max_length + 15; i++) { // Print enough dashes for display to work for different max sized sentence
        printf("-");
    }
    printf("\n");
}