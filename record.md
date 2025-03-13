Experience Record
===

## Existing Problem
+ ~~probability abnormal: sometimes it is bigger than 1~~
+ ~~Lock on some certain score index. The select score is not valid.~~

## Status
+ Solved problem above. 
+ The log function may add the target information. It is because I use the log with biggest index to estimate the probability. However, the log with biggest index sometimes is not the probability of guarding conditions in current fuzzing target.
+ Moreover, the switching should not be so frequency, it is better to setup a timer to switch.