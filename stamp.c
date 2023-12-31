#include <stdio.h>
#include <string.h>
#include <math.h>

/*
This file takes any number WAV format audio files as input
then generates a JSON file containing the timestamps of who's talking at what times.
Naturally, this will work better with less input files.
For readability, it's best to name the audio files after the person whose audio it contains
*/

//this represents the metadata of a WAV file,
//some information is necessary for looping through data later
typedef struct WAVHeader {
    char riff[5];
    int chuckSize;
    char wave[5];
    char subChunk1ID[5];
    int subChunk1Size;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
    char subChunk2ID[5];
    int subChunk2Size;
} WAVHeader;

//creates a WAVHeader struct, given a WAV file
WAVHeader newWAVHeader(FILE* f)
{
    WAVHeader newHead;
    fread(newHead.riff, 4, 1, f); newHead.riff[4] = '\0';
    fread(&newHead.chuckSize, 4, 1, f);
    fread(newHead.wave, 4, 1, f); newHead.wave[4] = '\0';
    fread(newHead.subChunk1ID, 4, 1, f); newHead.subChunk1ID[4] = '\0';
    fread(&newHead.subChunk1Size, 4, 1, f);
    fread(&newHead.audioFormat, 2, 1, f);
    fread(&newHead.numChannels, 2, 1, f);
    fread(&newHead.sampleRate, 4, 1, f);
    fread(&newHead.byteRate, 4, 1, f);
    fread(&newHead.blockAlign, 2, 1, f);
    fread(&newHead.bitsPerSample, 2, 1, f);
    fread(newHead.subChunk2ID, 4, 1, f); newHead.subChunk2ID[4] = '\0';
    fread(&newHead.subChunk2Size, 4, 1, f);
    return newHead;
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

//finds a point at which the audio files align, that way
//Note: this works best if, when you activate the microphones, you clap TWICE loudly with the microphones next to each other
//returns the amount of time to cut off the first audio file (might be negative, in which case cut the second audio file)
double syncAudio(FILE* a1, FILE* a2, int beginSecond, int endSecond)
{
    WAVHeader h1 = newWAVHeader(a1);
    WAVHeader h2 = newWAVHeader(a2);

    //calculating begin and end bytes in which to check for double claps
    int startByte = 44 + (beginSecond * h1.sampleRate * (h1.bitsPerSample/8));
    int endByte = 44 + (endSecond * h1.sampleRate * (h1.bitsPerSample/8));

    //USAGE
    if(endSecond - beginSecond <= 0) {printf("You're stupid."); return 0;}
    if(endByte > (h1.subChunk2Size + 36)) {printf("endSecond is past the end of file1"); return 0;}
    if(endByte > (h2.subChunk2Size + 36)) {printf("endSecond is past the end of file2"); return 0;}

    double offset; //return value
    //audio spike indices, set to the first chunk of the spike, since a spike might last multiple chunks
    double s1a1; //first spike of a1
    double s2a1; //second spike of a1
    double s1a2; //first spike of a2
    double s2a2; //second spike of a2

    //used for finding the biggest spikes
    double previousMax = 0;
    double runningTotal = 0;

    //the core functionality comes next, two loops, one for each file
    fseek(a1, startByte, SEEK_SET);
    while (fread() == ) {

    }

    fseek(a2, startByte, SEEK_SET);
    while (0) {

    }

    //debugging
    printf("In file1, claps were %f seconds apart.", s2a1 - s1a1);
    printf("In file2, claps were %f seconds apart.", s2a2 - s1a2);

    return offset;
}


//input: two WAV files, and a time stamp from which to start
int createStamps(FILE* a1, FILE* a2, double startStamp) 
{
    
}

// "Is they Jacob's? Don't lie to me mayne." -Kanye
int main(int argc, char *argv[]) 
{
    //USAGE
    if(argc < 3) {printf("Usage: %s <wavFile1> <wavFile2>\n", argv[0]); return 1;}
    FILE *a1 = fopen(argv[1], "r");
    FILE *a2 = fopen(argv[2], "r");
    if(a1 == NULL || !isWAV(a1)) {printf("first argument is not an available WAV file"); return 1;}
    if(a2 == NULL || !isWAV(a2)) {printf("second argument is not an available WAV file"); return 1;}

    //reset pointers to beginning of file
    fseek(a1, 0, SEEK_SET);
    fseek(a2, 0, SEEK_SET);

    //pray this works first try
    printf("AUDIO SYNC AT: %f", syncAudio(a1,a2,0,30));

    //GOODBYE
    fclose(a1);
    fclose(a2);
    return 0;
}