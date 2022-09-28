#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_CHARS 4095

// SECTION: Function declarations
int SplitStr(char*, char*, char**); 
int TokenizeFile(char*, char**);

int main(int argc, char* argv[]) {
    
    // NOTE: check command line args
    if (argc < 2) {
        printf("Please pass a file name!");
        return -1;
    } else if (argc > 2) {
        printf("Compiler does not support multiple files. Please use a unity build with a preprocessor.");
    } 
    

    // NOTE: load file to be compiled
    char* file_name = argv[1];
    FILE* source_file_stream = fopen(file_name, "r");
    
    char read_buffer[MAX_LINE_CHARS] = {0};
    size_t chars_read = fread(read_buffer, 1, MAX_LINE_CHARS, source_file_stream);
    char* token_array[MAX_LINE_CHARS] = {0}; 
    int tokens_found = TokenizeFile(read_buffer, token_array); 
    
    for (int i = 0; i < tokens_found; ++i) {
        printf("%s\n", token_array[i]);
    }
    printf("%d\n", tokens_found); // TODO: This outputs wrong number for some reason.
    
    fclose(source_file_stream); 

    //printf("%d\n", tokens_found);

    return 0;
}


// Splits line into tokens. Writes the tokens to the passed parameter char** tokens
// Returns number of tokens.
int TokenizeFile(char* file_text, char** final_tokens) {
    
    char* tokens[MAX_LINE_CHARS] = {0};
    int tokens_found = SplitStr(file_text, " ", tokens); 
         
    
    int sub_tokens_found = 0;
    
    for (int i = 0; i < tokens_found; ++i) {
        char* token = tokens[i];
        
        if (strstr(token, "(")) {
            char* sub_token_array[2]; 
            int num = SplitStr(token, "(", sub_token_array);
            if (num > 2) {
                printf("Unsupported token!");
                return -1;
            }
            final_tokens[sub_tokens_found++] = sub_token_array[0];
            final_tokens[sub_tokens_found++] = "(";
            final_tokens[sub_tokens_found++] = sub_token_array[1];
        } else if (strcmp(token, "\t")) {
            final_tokens[sub_tokens_found] = "TAB"; 
        } else {
            final_tokens[sub_tokens_found++] = token;
        } 
        
    }
    return sub_tokens_found; 

}


// NOTE: splits a string into tokens based on delim value. Saves the token list into the passed char** tokens
// RETURNS: number of tokens.
int SplitStr(char* string, char* delim, char** tokens) {
    
    char* token = strtok(string, delim);
    int tokens_found = 0;
    while(token != 0) {
        tokens[tokens_found++] = token;
        token = strtok(0, delim); 
    }
    return tokens_found;
}

