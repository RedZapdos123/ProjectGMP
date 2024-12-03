#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

//The fibonacci function using a new recursive algorithm
/* The formulae used to calculate the fibonacci number are:
    fib(2n+1) = fib((n/2))*fib((n/2)-1) + fib((n/2))*fib((n/2)+1)
    fib(2n) = fib(n/2) * fib(n/2) + fib((n/2) - 1) * fib((n/2) - 1)
*/

void fibonacci(unsigned int n, mpz_t r){
    //The base cases
    if(n == 0){
        mpz_set_ui(r, 0);
    }
    else if(n == 1){
        mpz_set_ui(r, 1);
    }
    else{
        unsigned int k = n/2;

        //For even number th fibonacci number
        if(n%2 == 0){
            //Temporary variables for calculations
            mpz_t fk1, fk2, mulTem1, mulTem2;
            mpz_inits(fk1, fk2, mulTem1, mulTem2, NULL);

            fibonacci(k, fk1);
            fibonacci(k-1, fk2);

            mpz_mul(mulTem1, fk1, fk1);
            mpz_mul(mulTem2, fk2, fk2);
            mpz_add(r, mulTem1, mulTem2);

            //Freeing the allocated memory
            mpz_clears(fk1, fk2, mulTem1, mulTem2, NULL);

        }

        //For odd number th fibonacci number
        else{
            //Temporary variables for calculations
            mpz_t fk1, fk2, fk3, mulTem1, mulTem2;
            mpz_inits(fk1, fk2, fk3, mulTem1, mulTem2, NULL);

            fibonacci(k, fk1);
            fibonacci(k-1, fk2);
            fibonacci(k+1, fk3);

            mpz_mul(mulTem1, fk1, fk3);
            mpz_mul(mulTem2, fk1, fk2);
            mpz_add(r, mulTem1, mulTem2);

            //Temporary variables for calculations
            mpz_clears(fk1, fk2, fk3, mulTem1, mulTem2, NULL);
        }
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