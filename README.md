# Rogue
Kai's Automated Interview Editor


## TO DO

0. Decide how this project will work
    - will it be ran as a bash script?
        1. a bash script could that FFMpeg and gcc (and whatever else) are installed
        2. then, if installed, it could then ask for user input for audio file names 1 and 2
        3. it will call the c script using the filenames as arguments in order
        4. the c script will take the file names as input and create a json file as output
        5. it will then ask the user if the user wants to proceed
    - if not a bash script
        1. then the user will manually call the c files
        2. the bash script just makes this simpler for someone who isn't familiar with the program
1. Bash Script
    - this is the easy part, and defines the overall structure of the project

2. Audio timestamping
    - the brains of the project
    - it will create a json file with all the information of who's talking when (for any number of people, though it wouldn't work for a ton of people who are all talking at once, maybe they should learn how to conduct an interview instead)

3. Video editing
    - takes the json file and two video files as input


## CHALLENGES

- synchronizing the audio/video files. 
    What if the audio tracks were turned on at different times? We'd that information, because the syncing has to be perfect.
    - solution 1: clap as soon as the audio and video are turned on for each person, and align them based on when the volume peaks
    - solution 2: call on some other software 
    - solution 3: ask for extra user input indicating at what point in the audio/video to treat at the start

