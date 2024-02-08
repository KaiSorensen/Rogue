from pyAudioAnalysis import audioSegmentation as ass

'''
REQUIRES pyAudioAnalysis TO BE INSTALLED
$ pip install pyAudioAnalysis
https://github.com/tyiannak/pyAudioAnalysis/tree/master

This program will provide an interface for labeling training data.
The idea is to create a file that contains timestamps that the user creates.
The user listens to the audio and presses a button upon each voice switch.
Labeling data takes work, and I won't be able to have a program that labels data until start here.
'''



win = 5
step = 1
def diarizeWav(f):
    return ass.speaker_diarization(f, 2, win,step)

d = diarizeWav("test_files/ctest1.wav")

with open('outC1.txt','w') as file:
    i = 0
    for w in d[0]:
        file.write(str(w) +",")
        i+=1
        if (i%(1//step) == 0):
             file.write("\n")

print("finished")