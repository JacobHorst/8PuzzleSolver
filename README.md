8 Puzzle Solver Program

Overview:

This program was done during my Intro to AI class at the University of Southern Mississippi.
The purpose of the program is to solve an 8 slider puzzle and display the results. The program
can accomplish this via a depth first search, a breadth first search, or two different methods 
of A* search. 

This repository includes both the program file and a report that details tests of each search 
method, their results, and a comparison of each search method with the conclusion of which method 
was the most optimal in terms of speed and memory usage. 

Program Use:

The program is written in C++ and can be run via the console or your preferred environment.
On startup, the following menu is displayed:

Please choose an option. Enter 7 to exit the program.

1: Create a random initial state.

2: Manually enter the initial state.

3: Perform breadth first search.

4: Perform depth first search.

5: Perform A* search using misplaced tiles.

6: Perform A* search using Manhattan distance.

7: Exit the program.

Once you have either randomly generated or manually entered a starting state for the puzzle,
choices 3-6 will perform a search to find the solution to the puzzle. If a solution can be found,
the steps to solve the puzzle will be displayed. Then you will be given the option to return to 
the main menu.
