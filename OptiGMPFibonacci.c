#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

//The function to manually multiply two square matrices of order two (2)
void matrixMultiplier(mpz_t a[2][2], mpz_t b[2][2], mpz_t c[2][2]){
    //Manually multplying the matrices to increase speed and use lesser memory
    mpz_t t1, t2, t3, t4;

    mpz_init(t1);
    mpz_init(t2);
    mpz_init(t3);
    mpz_init(t4);

    mpz_mul(t1, a[0][0], b[0][0]);
    mpz_addmul(t1, a[0][1], b[1][0]);

    mpz_mul(t2, a[0][0], b[0][1]);
    mpz_addmul(t2, a[0][1], b[1][1]);

    mpz_mul(t3, a[1][0], b[0][0]);
    mpz_addmul(t3, a[1][1], b[1][0]);

    mpz_mul(t4, a[1][0], b[0][1]);
    mpz_addmul(t4, a[1][1], b[1][1]);

    mpz_set(c[0][0], t1);
    mpz_set(c[0][1], t2);
    mpz_set(c[1][0], t3);
    mpz_set(c[1][1], t4);

    mpz_clear(t1);
    mpz_clear(t2);
    mpz_clear(t3);
    mpz_clear(t4);
    return;
}

//The function to do matrix exponentiation to calculate the Fibonacci
void matrixExponenter(unsigned long int n, mpz_t r){
    //Declaring and initialing the base matrix for matrix exponentiation
    mpz_t base[2][2];
    mpz_init_set_ui(base[0][0], 1);
    mpz_init_set_ui(base[0][1], 1);
    mpz_init_set_ui(base[1][0], 1);
    mpz_init_set_ui(base[1][1], 0);
    
    //Declaring and initialization a result matrix as an identity matrix
    mpz_t result[2][2];
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            mpz_init(result[i][j]);
            mpz_set_ui(result[i][j], (i == j) ? 1 : 0);
        }
    }
    //Declaring and initializing the temporary results matrix to avoid overwrite of the base matrix
    mpz_t temp[2][2];
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            mpz_init(temp[i][j]);
        }
    }

    //Doing the the iterative matrix exponentiation
    for(unsigned long int k = n; k > 0; k /= 2){
        if(k%2 == 1){
            //Multiplying the base matrix by the identity matrix if k is odd
            matrixMultiplier(result, base, temp);

            //Setting the identity matrix
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 2; j++){
                    mpz_set(result[i][j], temp[i][j]);
                }
            }
        }
        //Squaring the base
        matrixMultiplier(base, base, temp);

            //Setting the base matrix
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 2; j++){
                    mpz_set(base[i][j], temp[i][j]);
                }
            }
    }

    //Storing the result
    mpz_set(r, result[0][0]);

    //Freeing the allocated memory
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 2; j++){
            mpz_clear(result[i][j]);
            mpz_clear(base[i][j]);
            mpz_clear(temp[i][j]);
        }
    }
}

//The fibonacci function using a new iterative algorithm using matrix exponentiation
void fibonacci(unsigned long int n, mpz_t r){
    if(n == 0){
        mpz_set_ui(r, 0);
    }
    else if(n == 1){
        mpz_set_ui(r, 1);
    }
    else{
        //Calling the matrix exponentiation function
        matrixExponenter(n - 1, r);
    }
}
    

int main(int argc, char* argv[]){

    //Checking for command-line arguments
    if(argc < 2){
        printf("Use: %s <output_filename>\n", argv[0]);
        return 1;
    }

    unsigned long int n = 0;
    mpz_t r;
    mpz_init(r);

     //Taking input and handling errors
    do{
        printf("\nInput a whole number: ");
        scanf("%lu", &n);

        if (n < 0){
            printf("\nInvalid input! Please enter a whole number.\n");
        }

    } while(n < 0);
    
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

    fprintf(fp, "fib(%lu) = ", n);
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