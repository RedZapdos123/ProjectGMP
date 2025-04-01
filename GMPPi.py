#A python program to calculate till the nth digits of pi (3.14...), after '3.' (decimal place).
#This program makes use of the chudnovsky algorithm with binary splitiing, the the GMP and MPFR libraries for the computation.
#The processes and computations making use of multi-core processors.

import gmpy2
from gmpy2 import mpz, mpfr
import time
from multiprocessing import Pool, cpu_count

#Set the precision as per the required number of digits.
def setPrecision(digits):
    gmpy2.get_context().precision = int(digits * 3.32193) + 50

#Combine the triples (in (P, Q, T) form) as per the binary splitting formula.
def combine(triple1, triple2):
    P1, Q1, T1 = triple1
    P2, Q2, T2 = triple2
    return (P1*P2, Q1*Q2, T1*Q2+T2*P1)

def chudnovskyRange(a, b):
    #Computing the triple (P, Q, T) for the Chudnovsky series for k terms in [a, b) using an iterative binary splitting.
    terms = []
    for k in range(a, b):
        if k == 0:
            P = mpz(1)
            Q = mpz(1)
            T = mpz(13591409)
        else:
            P = mpz((6*k - 5)*(2*k - 1)*(6*k - 1))
            Q = mpz(k**3)*mpz(640320**3)//24
            T = P*(13591409 + 545140134*k)
            if k%2:
                T = -T
        terms.append((P, Q, T))
    
    #Combining terms in binary splitting style.
    while len(terms) > 1:
        newTerms = []
        for i in range(0, len(terms), 2):
            if i + 1 < len(terms):
                newTerms.append(combine(terms[i], terms[i + 1]))
            else:
                newTerms.append(terms[i])
        terms = newTerms
    return terms[0]

def computeChunk(args):
    #Compute a chunk of the series for k in [a, b) for multiprocessing.
    a, b = args
    return chudnovskyRange(a, b)

def mergeTriples(triples):
    #Merge a list of triples using iterative pairwise combination.
    while len(triples) > 1:
        newTriples = []
        for i in range(0, len(triples), 2):
            if i + 1 < len(triples):
                newTriples.append(combine(triples[i], triples[i + 1]))
            else:
                newTriples.append(triples[i])
        triples = newTriples
    return triples[0]

def computePi(digits):
    #Computing Pie using the Chudnovsky algorithm with binary splitting and multiprocessing.
    setPrecision(digits)
    #Each term gives roughly 14 digits of π.
    n = (digits// 14) + 1

    numCores = min(cpu_count(), 16)
    #Partition the range [0, n) into chunks.
    chunkSize = max(1, n//numCores)
    ranges = [(i, min(i + chunkSize, n)) for i in range(0, n, chunkSize)]

    #Compute each chunk in parallel.
    with Pool(processes=numCores) as pool:
        triples = pool.map(computeChunk, ranges)

    #Merge all triples from the chunks.
    Ptotal, Qtotal, Ttotal = mergeTriples(triples)

    C = mpfr("426880")*gmpy2.sqrt(mpfr("10005"))
    #The Chudnovsky formula: Pi = (426880*sqrt(10005)*Qtotal)/Ttotal.
    pi = (C*mpfr(Qtotal))/mpfr(Ttotal)
    return pi

def validatePi(piStr):
    #Validate the first 50 digits of Pi.
    knownPi = "3.14159265358979323846264338327950288419716939937510"
    return piStr.startswith(knownPi), knownPi

def main():
    digits = int(input("Enter number of digits for Pie: "))
    print(f"Computing Pi to {digits} digits after 3.: ...")
    print(f"Using {min(cpu_count(), 16)} CPU cores.")
    startTime = time.time()
    pi = computePi(digits)
    elapsed = time.time() - startTime

    #Get the string representation with the requested number of digits (accounting for '3.').
    piStr = str(pi)[:digits + 2]
    
    isValid, knownPi = validatePi(piStr)
    if isValid:
        print("Validation: First 50 digits after 3. match the known value.")
    else:
        print("Validation failed.")
        print(f"Computed: {piStr[:52]}")
        print(f"Expected: {knownPi}")

    #Wrting the output to a text file for storage.
    with open("pi.txt", "w") as f:
        f.write(piStr)

    #Printing the validation results.
    print(f"Computation completed in {elapsed:.2f} seconds.")
    print(f"Result saved to pi.txt.")
    print(f"First 50 digits: {piStr[:52]}...")

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"An error occurred: {str(e)}")
