#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

//The fibonacci function using the normal fibonacci as other algorithms sacrifice either time or mrmory for the other.
void fibonacci(unsigned int n, mpz_t r){
    if(n == 0){
        mpz_set_ui(r, 0);
        return;
    }
    else if(n == 1){
        mpz_set_ui(r, 1);
    }
    else{
        //Declaring and initializing the temporary variables for calculations
        mpz_t n1;
        mpz_t n2;

        mpz_init_set_ui(n1, 0);
        mpz_init_set_ui(n2, 1);

        //Calculating the fibonacci numbers
        for(unsigned int i = 2; i <= n; i++){
            mpz_add(r, n1, n2);
            mpz_set(n1, n2);
            mpz_set(n2, r);
        }

        //Storing the final result
        mpz_set(r, n2);

        //Freeing the memory for the temporary variables.
        mpz_clear(n1);
        mpz_clear(n2);

    }
}

int main(int argc, char* argv[]){

    //Checking for command-line arguments
    if(argc < 2){
        printf("Use: %s <output_filename>\n", argv[0]);
        return 1;
    }

    unsigned int n = 0;
    mpz_t r;
    mpz_init(r);

     //Taking input and handling errors
    do{
        printf("\nInput a whole number: ");
        scanf("%u", &n);

        if (n < 0){
            printf("\nInvalid input! Please enter a whole number.\n");
        }

    } while (n < 0);
    
    //Calling the fibonacci function
    fibonacci(n, r);

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

    fprintf(fp, "fib(%u) = ", n);
    mpz_out_str(fp, 10, r);
    fprintf(fp, "\n");

    //Closing the file
    fclose(fp);
    
    //Printing the results
    printf("\nCopied the fibonacci result to the file.\n");

    //Freeing the allocated memory
    mpz_clear(r);

    return 0;
}