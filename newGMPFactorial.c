#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// The factorial function
void factorial(int n, mpz_t r){
    mpz_set_ui(r, 1);
    for (int k = 2; k <= n; k++) {
        mpz_mul_ui(r, r, k);
    }
}

int main(int argc, char* argv[]){

    //Checking for command-line arguments
    if(argc < 2){
        printf("Use: %s <output_filename>\n", argv[0]);
        return 1;
    }

    int n = 0;
    mpz_t r;
    mpz_init(r);

     //Taking input and handling errors
    do{
        printf("\nInput a whole number: ");
        scanf("%d", &n);

        if (n < 0){
            printf("\nInvalid input! Please enter a whole number.\n");
        }

    } while (n < 0);
    
    //Calling the factorial function
    factorial(n, r);

    //Taking the output in a file
    FILE *fp = fopen(argv[1], "w");

    //Checking if the file opened
    if(!fp){
        printf("\nFailed to open the file.");

        //Freeing the allocated memory
        mpz_clear(r);

        return 1;
    }
    else{
        printf("\nOpened the file, sucessfully.\n");
    }

    fprintf(fp, "%d! = ", n);
    mpz_out_str(fp, 10, r);
    fprintf(fp, "\n");

    //Closing the file
    fclose(fp);
    
    //Printing the results
    printf("\nCopied the factorial result to the file.\n");

    //Freeing the allocated memory
    mpz_clear(r);

    return 0;
}