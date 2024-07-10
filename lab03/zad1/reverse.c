#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#ifdef BYTES

void reverse(FILE* input_file, FILE* output_file){

    fseek(input_file, -1, SEEK_END);
    int to_read = ftell(input_file) + 1;

    while(to_read > 0){
        
        int c = getc(input_file);
        fwrite(&c, sizeof(char), 1, output_file);
        to_read--;
        fseek(input_file, -2, SEEK_CUR);
    } 
}

#else

void reverse(FILE* input_file, FILE* output_file){
    
    fseek(input_file, 0, SEEK_END);
    int bytes_left = ftell(input_file);
    char blok[1024];
    
    while(bytes_left > 1024){

        fseek(input_file, -1024, SEEK_CUR);
        int bytes_read = fread(blok, sizeof(char), 1024, input_file);
        char c;
        for(int i = 0; i < bytes_read/2; i++){
            c = blok[i];
            blok[i] = blok[bytes_read - 1 - i];
            blok[bytes_read - 1 - i] = c;
        }

        fwrite(blok, sizeof(char), bytes_read, output_file);
        fseek(input_file, -1024, SEEK_CUR);
        bytes_left -= bytes_read;
    }

    fseek(input_file, -bytes_left, SEEK_CUR);
    int bytes_read = fread(blok, sizeof(char), bytes_left, input_file);
    char c;
    for(int i = 0; i < bytes_read/2; i++){
        c = blok[i];
        blok[i] = blok[bytes_read - 1 - i];
        blok[bytes_read - 1 - i] = c;
    }

    fwrite(blok, sizeof(char), bytes_read, output_file);
    
}

#endif

int main(int argc, char** argv) {
   
    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    FILE *fp = fopen("pomiar_zad_2.txt", "a");

    struct timespec start_ts, end_ts;
    clock_gettime(CLOCK_REALTIME, &start_ts);
    reverse(input_file, output_file);
    clock_gettime(CLOCK_REALTIME, &end_ts);
    fprintf(fp, "Reverse time: %f\n", (end_ts.tv_nsec - start_ts.tv_nsec) / 1000000000.0f);

    fclose(input_file);
    fclose(output_file);
    fclose(fp);
}

