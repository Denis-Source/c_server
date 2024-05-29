#ifndef SERVER_SECRETS_H
#define SERVER_SECRETS_H


#include <stdlib.h>
#include <time.h>
#include <unistd.h>


/**
 * Generates a pseudo-random 64-bit unsigned integer.
 *
 * This function generates a pseudo-random 64-bit unsigned integer using the srand and rand functions.
 * It initializes the random number generator with a seed based on the current time and process ID,
 * ensuring different seeds for different invocations. It then generates a 64-bit random integer
 * by combining two consecutive calls to the rand function.
 *
 * @return A pseudo-random 64-bit unsigned integer.
 *
 * The function performs the following steps:
 * 1. Initializes a static variable to store the generated random value.
 * 2. If the stored value is zero, initializes the random number generator with a seed
 *    based on the current time and process ID, and generates a 64-bit random integer.
 * 3. Returns the generated random value.
 *
 * Example usage:
 * @code
 * u_int64_t random_value = static_generate_random();
 * printf("Random Value: %llu\n", random_value);
 * @endcode
 */
u_int64_t static_generate_random();


#endif //SERVER_SECRETS_H
