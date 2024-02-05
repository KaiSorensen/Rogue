// Header
#include <rogue/wav.h>

int wav_create ( wav *p_wav )
{

    // Argument check
    if ( p_wav == (void *) 0 ) goto no_wav;

    // Initialized data
    wav _wav = { 0 };

    // Zero set the struct
    memset(&_wav, '\0', sizeof(wav));

    // Copy the struct to the return
    memcpy(p_wav, &_wav, sizeof(wav));

    // Success 
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_wav:
                #ifndef NDEBUG
                    log_error("[Rogue] Null pointer provided for parameter \"p_wav\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int wav_construct ( wav *p_wav, const char *p_path )
{

    // Argument check
    if ( p_wav  == (void *) 0 ) goto no_wav;
    if ( p_path == (void *) 0 ) goto no_path;

    // Initialized data
    wav _wav = { 0 };
    FILE *p_file = fopen(p_path, "r");
    u8 wav_file_header[sizeof(_wav.header)] = { 0 };

    // Error check
    if ( p_file == (void *) 0 ) goto failed_to_open_file;

    // Seek the start
    fseek(p_file, 0, SEEK_SET);

    // Read the header
    fread(&wav_file_header, 1, sizeof(_wav.header), p_file);

    // Populate the wav file struct
    _wav = (wav)
    {
        .header = 
        {
            .riff = 
            {
                .id = 
                {
                    wav_file_header[0],
                    wav_file_header[1],
                    wav_file_header[2],
                    wav_file_header[3]
                },
                .size = *(u32*)(&wav_file_header[4]),
                .format = 
                {
                    wav_file_header[8],
                    wav_file_header[9],
                    wav_file_header[10],
                    wav_file_header[11]
                }
            },
            .format = 
            {
                .id              = *(u32*)(&wav_file_header[12]),
                .size            = *(u32*)(&wav_file_header[16]),
                .format          = *(u16*)(&wav_file_header[20]),
                .num_channels    = *(u16*)(&wav_file_header[22]),
                .sample_rate     = *(u32*)(&wav_file_header[24]),
                .byte_rate       = *(u32*)(&wav_file_header[28]),
                .block_align     = *(u16*)(&wav_file_header[32]),
                .bits_per_sample = *(u16*)(&wav_file_header[34])
            },
            .data = 
            {
                .id   = *(u32*)(&wav_file_header[36]),
                .size = *(u32*)(&wav_file_header[40]),
            }
        },
        .p_file = p_file
    };

    // Copy the struct
    memcpy(p_wav, &_wav, sizeof(wav));

    // Success 
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_wav:
                #ifndef NDEBUG
                    log_error("[Rogue] Null pointer provided for parameter \"p_wav\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[Rogue] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
        // Standard library errors
        {
            failed_to_open_file:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to open file \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int wav_info ( wav *p_wav )
{



    // Success
    return 1;
}