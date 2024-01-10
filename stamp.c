#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
This file takes TWO WAV format audio files as input
then generates a JSON file containing the timestamps of who's talking at what times.
For readability, it's best to name the audio files after the person whose audio it contains
*/

//this represents the metadata of a WAV file,
//some information is necessary for looping through data later
typedef struct WAVHeader {
    char riff[5];
    int chuckSize;
    char wave[5];
    char fmt[5];
    int fmtSize;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bytesPerSample;
    char data[5];
    int dataSize;
} WAVHeader;

//creates a WAVHeader struct, given a WAV file
WAVHeader newWAVHeader(FILE* f)
{
    WAVHeader newHead;

    fread(newHead.riff, 4, 1, f); newHead.riff[4] = '\0';
    fread(&newHead.chuckSize, 4, 1, f);
    fread(newHead.wave, 4, 1, f); newHead.wave[4] = '\0';

    //finds the fmt section
    fread(newHead.fmt, 4, 1, f); newHead.fmt[4] = '\0';
    fread(&newHead.fmtSize, 4, 1, f);
    while(strcmp("fmt ", newHead.fmt) != 0) {
        fseek(f, newHead.fmtSize, SEEK_CUR);
        fread(newHead.fmt, 4, 1, f); newHead.fmt[4] = '\0';
        fread(&newHead.fmtSize, 4, 1, f);
    }

    fread(&newHead.audioFormat, 2, 1, f);
    fread(&newHead.numChannels, 2, 1, f);
    fread(&newHead.sampleRate, 4, 1, f);
    fread(&newHead.byteRate, 4, 1, f);
    fread(&newHead.blockAlign, 2, 1, f);
    fread(&newHead.bytesPerSample, 2, 1, f); newHead.bytesPerSample /= 8;

    //finds the data section
    fread(newHead.data, 4, 1, f); newHead.data[4] = '\0';
    fread(&newHead.dataSize, 4, 1, f);
    while(strcmp("data", newHead.data) != 0) {
        fseek(f, newHead.dataSize, SEEK_CUR);
        fread(newHead.data, 4, 1, f); newHead.data[4] = '\0';
        fread(&newHead.dataSize, 4, 1, f);
        printf("%s\n", newHead.data);
    }
    
    return newHead;
}

//toString written by GPT-4 because I can't stand string concatentation in C
char* wavHeaderToString(WAVHeader *header) {
    // Estimate the required length
    int requiredLength = snprintf(NULL, 0,
                                "RIFF: %.4s\nChunk Size: %d\nWAVE: %.4s\nSubchunk1 ID: %.4s\nSubchunk1 Size: %d\n"
                                "Audio Format: %hd\nNum Channels: %hd\nSample Rate: %d\nByte Rate: %d\n"
                                "Block Align: %hd\nBytes Per Sample: %hd\nSubchunk2 ID: %.4s\nSubchunk2 Size: %d\n",
                                header->riff, header->chuckSize, header->wave, header->fmt, header->fmtSize,
                                header->audioFormat, header->numChannels, header->sampleRate, header->byteRate,
                                header->blockAlign, header->bytesPerSample, header->data, header->dataSize);

    // Allocate memory (+1 for the null terminator)
    char *str = malloc(requiredLength + 1);
    if (str == NULL) {
        perror("malloc failed");
        return NULL;
    }

    // Actually create the string
    snprintf(str, requiredLength + 1,
             "RIFF: %.4s\nChunk Size: %d\nWAVE: %.4s\nSubchunk1 ID: %.4s\nSubchunk1 Size: %d\n"
             "Audio Format: %hd\nNum Channels: %hd\nSample Rate: %d\nByte Rate: %d\n"
             "Block Align: %hd\nBytes Per Sample: %hd\nSubchunk2 ID: %.4s\nSubchunk2 Size: %d\n",
             header->riff, header->chuckSize, header->wave, header->fmt, header->fmtSize,
             header->audioFormat, header->numChannels, header->sampleRate, header->byteRate,
             header->blockAlign, header->bytesPerSample, header->data, header->dataSize);

    return str;
}

//verifies that the file is a WAV format
//1 if WAV, 0 if not
int isWAV(FILE* f) 
{
    FILE *a = f;
    char firstChunk[5];
    char riff[] = "RIFF"; //all WAV files start with "RIFF"
    fread(firstChunk, 4, 1, a);
    if (strcmp(firstChunk, riff) == 0) return 1;
    else return 0;
}

//for testing, written by GPT-4
int reverseBits(int n) {
    int reversed = 0;
    for (int i = 0; i < 32; i++) {
        reversed = (reversed << 1) | (n & 1);
        n >>= 1;
    }
    return reversed;
}

//finds a point at which the audio files align, based on the TWO-CLAP technique
//Note: When you activate the microphones, you clap TWICE loudly with the microphones next to each other
//returns the amount of time to cut off the first audio file (might be negative, in which case cut the second audio file)
double syncAudio(FILE* a1, FILE* a2, int beginSecond, int endSecond)
{
    WAVHeader h1 = newWAVHeader(a1);
    WAVHeader h2 = newWAVHeader(a2);
    printf("\n%s\n", wavHeaderToString(&h1));
    printf("\n%s\n", wavHeaderToString(&h2));

    //calculating begin and end bytes in which to check for double claps
    int startByte = ftell(a1) + (beginSecond * h1.sampleRate * (h1.bytesPerSample));
    int endByte = ftell(a1) + (endSecond * h1.sampleRate * (h1.bytesPerSample));
    printf ("\nA1: %d\n", ftell(a1));
    printf ("\nStart Byte: %d\n", startByte);
    printf ("End Byte: %d\n", endByte);

    //USAGE
    if(endSecond - beginSecond <= 0) {printf("You're stupid."); return 0;}
    if(endByte > (h1.dataSize + 36)) {printf("endSecond is past the end of file1"); return 0;}
    if(endByte > (h2.dataSize + 36)) {printf("endSecond is past the end of file2"); return 0;}

    double offset; //return value
    //audio spike indices, set to the first sample of the spike, since a spike might last multiple chunks
    int s1a1 = startByte; //first spike of a1
    int s2a1 = startByte; //second spike of a1
    int s1a2 = startByte; //first spike of a2
    int s2a2 = startByte; //second spike of a2

    //used for finding the biggest spikes
    int previousSample;
    signed int currentSample;
    int previousMax = 0;
    int firstMax = 0;
    int secondMax = 0;
    int runningTotal = 0;

    //the core functionality comes next, two loops, one for each file

    //finds the two claps in a1
    //DOES NOT WORK BECAUSE VOLUME IS EQUALIZED
    fseek(a1, startByte, SEEK_SET);
    while (ftell(a1) < endByte) {
        previousSample = currentSample;
        fread(&currentSample, h1.bytesPerSample, 1, a1);
        printf("\nCurrentByte: %d    CurrentSample: %d", ftell(a1), currentSample);
        if(currentSample >= previousSample) {
            runningTotal += currentSample - previousSample;
        } else {
            if(runningTotal > secondMax) {
                firstMax = secondMax;
                secondMax = runningTotal;
                runningTotal = 0;
                s1a1 = s2a2;
                s1a1 = ftell(a1);
            } else if (runningTotal > firstMax){
                firstMax = runningTotal;
                runningTotal = 0;
                s2a1 = ftell(a1);
            }
        }
    }

    previousMax = 0;
    firstMax = 0;
    secondMax = 0;
    runningTotal = 0;
    //finds the two claps in a2
    fseek(a2, startByte, SEEK_SET);
    while (ftell(a2) < endByte) {
        previousSample = currentSample;
        fread(&currentSample, h2.bytesPerSample, 1, a2);
        printf("\nCurrentByte: %d    CurrentSample: %d", ftell(a2), currentSample);
        if(currentSample >= previousSample) {
            runningTotal += currentSample - previousSample;
        } else {
            if(runningTotal > secondMax) {
                firstMax = secondMax;
                secondMax = runningTotal;
                runningTotal = 0;
                s1a1 = s2a2;
                s1a1 = ftell(a1);
            } else if (runningTotal > firstMax){
                firstMax = runningTotal;
                runningTotal = 0;
                s2a1 = ftell(a1);
            }
        }
    }

    //debugging
    printf("\n\nIn file 1, claps were %d bytes apart.\n", s2a1 - s1a1);
    printf("In file 2, claps were %d bytes apart.\n", s2a2 - s1a2);

    if(abs((s2a1-s1a1) - (s1a2 - s2a2)) < 100) {
        printf("FOUND THE CLAPS IN BOTH FILES\n");
    } else {
        
    }  

    return offset;
}


//input: two WAV files, and a time stamp from which to start
int createStamps(FILE* a1, FILE* a2, double startStamp) 
{
    
}

//  "Is they Jacob's? Don't lie to me mayne." -Kanye
int main(int argc, char *argv[]) 
{
    //USAGE
    if(argc < 3) {printf("\nUsage: %s <wavFile1> <wavFile2>\n", argv[0]); return 1;}
    FILE *a1 = fopen(argv[1], "rb");
    FILE *a2 = fopen(argv[2], "rb");
    if(a1 == NULL || !isWAV(a1)) {printf("\nfirst argument is not an available WAV file\n"); return 1;}
    if(a2 == NULL || !isWAV(a2)) {printf("\nsecond argument is not an available WAV file\n"); return 1;}

    //reset pointers to beginning of file
    fseek(a1, 0, SEEK_SET);
    fseek(a2, 0, SEEK_SET);

    //testing
    // WAVHeader h1 = newWAVHeader(a1);
    // char *s = wavHeaderToString(&h1);
    // printf("\n%s\n", s);
    // free(s);

    printf("AUDIO SYNC AT: %f\n", syncAudio(a1,a2,0,1));

    //GOODBYE
    fclose(a1);
    fclose(a2);
    return 0;
}
