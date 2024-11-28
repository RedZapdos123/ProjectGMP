#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Creating the memoization array
mpz_t *memo = NULL;

// The factorial function using memoization
void factorial(int n, mpz_t r){
    if (mpz_cmp_ui(memo[n], 0) != 0){
        mpz_set(r, memo[n]);
        return;
    }

    if (n == 0 || n == 1){
        mpz_set_ui(memo[n], 1);
        mpz_set(r, memo[n]);
        return;
    }

    else{
        //Temporary variable to store result
        mpz_t rt;
        mpz_init(rt);

        //Calculating the factorial
        factorial(n - 1, rt);
        mpz_mul_ui(r, rt, n);

        mpz_set(memo[n], r);
        mpz_clear(rt);
        return;
    }
}

int main(int argc, char* argv[]){
    int n = 0;
    mpz_t r;
    mpz_init(r);

     //Taking input and handling errors
    do{
        printf("\nInput a number: ");
        scanf("%d", &n);

        if (n < 0){
            printf("\nInvalid input! Please enter a non-negative integer.\n");
        }

    } while (n < 0);
    
    //Allocating memory to the array and checking if it was successful
    memo = (mpz_t *)malloc((n + 1) * sizeof(mpz_t));

    if (memo == NULL) {
        printf("\nFailed memory allocation.\n");
        exit(1);
    }

    //Initalizing the array
    for (int j = 0; j <= n; j++) {
        mpz_init(memo[j]);
        mpz_set_ui(memo[j], 0);
    }

    //Calling the factorial function
    factorial(n, r);

    //Taking the output in a file
    FILE *fp = fopen(argv[1], "w");

    //Checking if the file opened
    if(!fp){
        printf("\nFailed to open the file.");

        //Freeing the allocated memory
        for (int k = 0; k <= n; k++){
            mpz_clear(memo[k]);
        }

        free(memo);
        mpz_clear(r);

        return 1;
    }
    else{
        printf("Opened the file, sucessfully.\n");
    }

    fprintf(fp, "%d! = ", n);
    mpz_out_str(fp, 10, r);
    fprintf(fp, "\n");

    //Closing the file
    fclose(fp);
    
    //Printing the results
    printf("Copied the output to the file.\n");
    
    printf("\n%d! = ", n);
    mpz_out_str(stdout, 10, r);
    printf("\n");

    //Freeing the allocated memory
    for (int k = 0; k <= n; k++){
        mpz_clear(memo[k]);
    }

    free(memo);
    mpz_clear(r);

    return 0;
}

