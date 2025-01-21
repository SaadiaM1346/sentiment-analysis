#ifndef MYSA_FUNCTIONS_H
#define MYSA_FUNCTIONS_H

// Define struct words
struct words {
    char *word;
    float score;
    float SD;
    int *SIS_array;
};

// Define functions
void remove_punct(char *token, int *num_exclamation);
void make_lower(char *token);
void has_space_words(struct words *vader_word, int count, char *sentence, double *total_score, int *num_words);
void make_vader_struct(struct words **vader_word, int *count, const char *lexicon_filename);
bool is_all_space(char *sentence);
void sentence_scoring(FILE *validation_file, char *sentence, int max_length, struct words *vader_word, int count);
void print_display(int max_length);

#endif /* MYSA_FUNCTIONS_H */
