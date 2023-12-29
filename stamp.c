#include <stdio.h>
#include <string.h>

/*
This file takes any number WAV format audio files as input
then generates a JSON file containing the timestamps of who's talking at what times.
Naturally, this will work better with less input files.
For readability, it's best to name the audio files after the person whose audio it contains
*/

//this represents the metadata of the WAV file,
//some information is necessary for looping through data
typedef struct WAVHeader {
    char riff[4];
    int chuckSize;
    char wave[4];
    char subChunk1ID[4];
    int subChunk1Size;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
    char subChunk2ID[4];
    int subChunk2Size;
} WAVHeader;

//verifies that the file is a WAV format
//1 if WAV, 0 if not
int isWAV(FILE* f) 
{
    char firstChunk[5];
    char riff[] = "RIFF"; //all WAV files start with "RIFF"
    fread(firstChunk, 4, 1, f);
    return strcmp(firstChunk, riff);
}

//finds a point at which the audio files align, that way
//Note: this works best if, when you activate the microphones, you clap loudly with the microphones next to each other
//returns the amount of time to cut off the first audio file (might be negative, in which case cut the second audio file)
int syncAudio(FILE* a1, FILE a2) 
{
    
}

//input: two WAV files, and a time stamp from which to start
int createStamps(FILE* a1, FILE* a2, double startStamp) 
{
    
}

// "Is they Jacob's? Don't lie to me mayne." -Kanye
int main(int argc, char *argv[]) 
{
    FILE* f;
    f = fopen(argv[1],"r");
    printf("%d", isWAV(f));
}