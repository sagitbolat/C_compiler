#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_CHARS 4095

// SECTION: Function declarations
int SplitStr(char*, char*, char**); 
int TokenizeLine(char*, char**);
int TokenizeFile(FILE*, char**);

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
    
    char* tokens[MAX_LINE_CHARS] = {0};
    int tokens_found = TokenizeFile(source_file_stream, tokens);
    
    for(int i = 0; i < tokens_found; ++i) {
        printf("%s\n", tokens[i]);
    }



    printf("%d\n", tokens_found);

    return 0;
}


int TokenizeFile(FILE* stream, char** final_tokens) {
    
    int final_tokens_found = 0;
    
    int EOF_found = 0;
    
    while (!EOF_found) {
        char read_buffer[MAX_LINE_CHARS] = {0};
        char* source_text_line = fgets(read_buffer, MAX_LINE_CHARS, stream);
        
        if (!source_text_line) {
            EOF_found = 1;
            continue;
        } else {
            // NOTE: lexer/tokenizer
            char* token_array[MAX_LINE_CHARS] = {0}; 
            int tokens_found = TokenizeLine(source_text_line, token_array); 
            
            for (int i = 0; i < tokens_found; ++i) {
                final_tokens[final_tokens_found++] = token_array[i];
            }

        }
    }
    
    return final_tokens_found;
}

// Splits line into tokens. Writes the tokens to the passed parameter char** tokens
// Returns number of tokens.
int TokenizeLine(char* line, char** final_tokens) {
    
    char* tokens[MAX_LINE_CHARS] = {0};
    int tokens_found = SplitStr(line, " ", tokens); 
         
    
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

