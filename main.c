/**! 
 * Rogue is an automated interview editor
 * 
 * @file main.c
 * 
 * @author Kai Sorensen
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// log module
#include <log/log.h>

// json module
#include <json/json.h>

// rogue
#include <rogue/rogue.h>
#include <rogue/wav.h>

/** ! 
 * Print a usage message, and abort program execution
 * 
 * @param void
 * 
 * @return void
 */
u0 print_usage ( u0 )
{

    // Print a usage message to standard out
    printf("Usage: rogue <wavFile1> <wavFile2>\n");

    // Abort
    exit(EXIT_FAILURE);

    // Done
    return;
}

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialize the log module
    log_init(0, true);

    // TODO: Parse command line arguments
    if ( argc < 3 ) print_usage();

    // Initialized data
    wav _A = { 0 },
        _B = { 0 };

    // Say hi
    log_info("Rogue\n");

    // Construct the first WAV file
    if ( wav_construct(&_A, argv[1]) == 0 ) goto failed_to_load_wav;

    // Construct the second WAV file
    if ( wav_construct(&_B, argv[2]) == 0 ) goto failed_to_load_wav;
    
    // Success
    return EXIT_SUCCESS;

    // TODO: Error handling
    failed_to_load_wav:

        // Error
        return EXIT_FAILURE;
}