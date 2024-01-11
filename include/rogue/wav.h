/** !
 * Include header for WAV file
 * 
 * @file rogue/wav.h
 * 
 * @author Kai Sorensen
 * @author Jacob Smith
*/

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// log module
#include <log/log.h>

// Rogue
#include <rogue/rogue.h>

// Preprocessor definitions
#define WAV_FILE_HEADER_SIZE 

// Structure definitions
struct wav_s
{

    // Header
    struct
    {
    
        // RIFF chunk descriptor
        struct
        {
            u8 id[4];
            u32 size;
            u8 format[4];
        } riff;
        
        // Format sub-chunk
        struct
        {
            u32 id;
            u32 size;
            u16 format;
            u16 num_channels;
            u32 sample_rate;
            u32 byte_rate;
            u16 block_align;
            u16 bits_per_sample;
        } format;
        
        // Data sub-chunk
        struct
        {
            u32 id;
            u32 size;
        } data;
    } header;

    FILE *p_file;
};

int wav_create ( wav *p_wav );

int wav_construct ( wav *p_wav, const char *p_path );