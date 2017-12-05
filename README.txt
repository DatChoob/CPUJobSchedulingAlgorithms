main.cpp   
    Contains the main mehtod
    Takes in Input parameters: file, scheduling algorthm, and Time Quantum if algorith is Round Robin
    Uses the input file and parses it into a list of Jobs.
    It then passes the Jobs to be processed to the appropriate shceduling algorith
    Uses all of the files below.
main.h 
    contains the data structures for Input Jobs, PCB items and Stats
LinkedList.h
Contains the class definitions for a linked list,
    Uses Node, LinkedList, and ListIterator
LinkedList.cpp 
    Contains implementation of a Linked List


How To Run:
    Assuming Input file is in the same directory as main.cpp 
        Compile: 'c++ main.cpp'
        Run: ./a.out <Input File Name> <Scheduling Algorithm> <Time Quantum if <Scheduling Algorithm> is RR>
        Possible Scheduling Algorithms:
            FCFS(First Come First Serve)
            SRTF(Shortest Remaining Time First)
            RR(Round Robin)
            
    Assuming input file name is inputFile.txt
        Example: FCFS 
            ./a.out inputFile.txt FCFS
        Example: RR Time Quantum is 3
            ./a.out inputFile.txt RR 3
        Example: SRTF
            ./a.out inputFile.txt SRTF