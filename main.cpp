#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

#define PRIME_LENGTH 15 // bits

bool isPrime(unsigned long long n)
{
    unsigned long long s = ceil(sqrt(n));
    if( (n % 2) == 0 ) return false;
    for(unsigned long long i = 3; i <= s; i += 2) if( (n % i) == 0 ) return false;
    return true;
}

unsigned long int generatePrime(int bits)
{
    unsigned long long p;
    int fd = open("/dev/urandom", O_RDONLY);

    do {
        read(fd, &p, 5);
        p = p % (unsigned long long) floor(pow(2, bits)) + (unsigned long long)(pow(2, bits)*0.8);
    }while(!isPrime(p));

    close(fd);
    return p;
}

unsigned long long modular_pow(unsigned long long base, unsigned long long exponent, unsigned long long modulus)
{
    unsigned long long result = 1;

    while( exponent > 0 ) {

        if( (exponent % 2) == 1 ) {
            result = (result * base) % modulus;
        }

        exponent = exponent >> 1;
        base = (base * base) % modulus;

    }

    return result;
}

std::pair<unsigned long long, unsigned long long> extended_gcd(unsigned long long a, unsigned long long b)
{
    if( b == 0 ) return std::pair<unsigned long long, unsigned long long>(1, 0);
    unsigned long long q = a/b;
    double r = a - b*q;
    std::pair<unsigned long long, unsigned long long> st = extended_gcd(b, r);
    return std::pair<unsigned long long, unsigned long long>(st.second, st.first - q*st.second);
}

// 1. Generate prime numbers
// 2. Generate phi(n) & e & d
// 3. Base64_encode & number translate
// 4. Encrypt
// 5. Decrypt
// 6. Inverse number translate & Base64_decode
int main(int argc, char* argv[])
{
    // 1. Generate prime numbers
    unsigned long long p = generatePrime(PRIME_LENGTH);
    unsigned long long q = generatePrime(PRIME_LENGTH);
    while( p == q ) q = generatePrime(PRIME_LENGTH);
    //p=61;q=53;
    unsigned long long n = p*q;

    std::cout << "p=" << p << std::endl << "q=" << q << std::endl << "n = pq = " << n << std::endl;

    // 2. Generate phi(n) & e & d
    unsigned long long phi = (p-1)*(q-1);
    std::cout << "phi(n) = phi(p) * phi(q) = " << phi << std::endl;

    unsigned long long e = generatePrime(PRIME_LENGTH);
    while( (phi % e) == 0 ) e = generatePrime(PRIME_LENGTH);
    std::cout << "e = " << e << std::endl;

    std::pair<unsigned long long, unsigned long long> egcd = extended_gcd(e, phi);
    unsigned long long d = (egcd.first+phi) % phi;
    std::cout << "d = " << d << std::endl;

    unsigned long long m = 65, c = 0;
    std::cout << "Plaintext: " << m << std::endl;
    c = modular_pow(m, e, n);
    std::cout << "Chiper: " << c << std::endl;
    m = modular_pow(c, d, n);
    std::cout << "Plaintext: " << m << std::endl;

    // 3. Base64_encode & number translate
    // 4. Encrypt
    // 5. Decrypt
    // 6. Inverse number translate & Base64_decode
    // 7. Tests
    /*std::cout << "Test 1 [isPrime] ...." << std::endl << std::endl;
    std::cout << "Prime number 7 results in: " << isPrime(7) << std::endl;
    std::cout << "Non-Prime number 6 results in: " << isPrime(6) << std::endl;*/
}

