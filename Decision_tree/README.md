Folders src_baseline, src_sequential and src_parallel contain source files of three implementations.

digger.c/digger.h: the main function
load.c: loading the dataset into memory
plant.c: training process
predict.c/navigate.c: testing process

Since we only focus on the training and loading part,
our modifications are mainly in plant.c & load.c.

Baseline code: https://github.com/athi-prakash/ID3
(We did some modifications, including printing timing of each round to a file,
    eliminating unnecessary printf() calls or changing the stdout to a file)
Links for three datasets:
KDD: ​​http://kdd.ics.uci.edu/databases/kddcup99/kddcup99.html
Poker Hand:​ ​https://archive.ics.uci.edu/ml/datasets/Poker+Hand
Covertype: ​https://archive.ics.uci.edu/ml/datasets/Covertype
-------------------- Important --------------------
After downloading datasets, please use vim to replace all ',' with a space ' '.
Usage: type ":%s/,/ /g" in vim
Since some datasets use ',' as separator and some use space, we decided to use space.

Usage:
1.  Enter one of the folders, put dataset into this folder
2.  Type "make" to compile
    Type "make clean" to remove executable file and results
3.  After compiling, type "./Digger" to run
4.  Type in the filename of dataset (should be in the same folder)
5.  Enter the index of column which is result (enter the last printed number
    since almost all dataset use the last column as result)
6.  Wait the training process to finish (For baseline implementation, no
    more than 300 seconds on provided dataset)
7.  Type "1" to test, Type arbitrary other string to quit
8.  If "1" is typed, type the filename of testing dataset
    (We usually use the same file for testing and training)
9.  Wait the testing process to finish
10. -------------------- Very Important --------------------
    The time printed to terminal is the time of whole process, and is not the time we want. We
    only focus on the time of first round in the recursive process. And our timing doesn't
    include times for printing and other unnecessary operations. The correct timing information
    is stored in Timing_baseline/Timing_sequential/Timing_parallel files, which will be generated
    in the training process and will be in the same folder.
11. Open the timing result files and look at the first line, which is the training time for
    first round (also the value we used for plotting).

[Note]: The decision tree will be stored in file Rules (only for visualization, the real
        tree is stored in a struct). The test result will be stored in file Test.

        We only optimized loading and training process. For testing, actually a large amount of
        time is used for printing results to file Test and we do not care about it.

By Sheng Guan, Xu Chen and Jennifer Li
