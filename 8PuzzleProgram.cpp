/**************************************************************************************************
* Author: Jacob Horst
* Date: 11/14/2018
* CSC 412
*
* 8 Slider Puzzle Program
* This program searches for and records solutions to the 8 slider puzzle. The puzzle may be
* generated randomly or provided by the user. It can search for a solution using breadth first
* searching, depth first searcing, or A* (number of misplaced tiles or Manhattan distance) searching.
* The results of the search will be displayed to the user.
**************************************************************************************************/

#include<iostream>
#include<string>
#include<algorithm>
#include<list>
#include<map>
#include<stack>
#include<queue>

using namespace std;

// Node used during solution searches
struct node
{
	// String used to represent the environment state
	string stateString;

	// Array used to hold the values of the environment state
	int stateArray[9];

	// Int used to hold the value representing the depth of the search tree
	int depth = 0;

	// List of all the states in the solution path
	list <string> path;

	// Heuristic used for the A* searches
	int heuristic = 0;

	// Operator used to give states with fewer misplaced tiles priority when
	// being inserted into the priority queue
	bool operator < (const node& other) const
	{
		return heuristic > other.heuristic;
	}

};

// A string representing what the goal state looks like
string goalState = "123456780";

// An array representing what the goal state looks like
int goalStateArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

// Array representing the initial state of the environment
int initialState[9];

// Array used to work with so as not to lose the initial state
int workingState[9];

// Map used to keep track of seen states
map<string, int> seenStates;

// Queue used for breadth first search and stack used for depth first search
// Priority queue used for A* search using misplaced tiles
queue <node> nodeQueue;
stack <node> nodeStack;
priority_queue <node> priorityQueue;

// Function prototypes
int userMenu();
int generateInitialState();
int inputInitialState();
int breadthFirstSearch();
int depthFirstSearch();
int misplacedTilesSearch();
int manhattanDistanceSearch();
void generateSuccessors(node state, int num);
bool checkSeenStates(string state);

int main()
{
	// Displays message describing program to the user
	cout << "Welcome to the 8 slider puzzle solution finder." << endl;
	cout << "This program will search for a solution path and display the result." << endl << endl;

	// Int used to represent the user's menu choice
	int userMenuChoice = 0;

	// Loops until the user enters 7 to exit the program
	while (userMenuChoice != 7)
	{
		// Displays the menu and gets the user's choice
		userMenuChoice = userMenu();

		// Performs a function depending on the user's choice
		switch (userMenuChoice)
		{
		case 1:
			// Clears the screen for formatting
			system("cls");
			// Generates and displays a random initial state
			generateInitialState();
			break;
		case 2:
			// Clears the screen for formatting
			system("cls");
			// Has the user enter the initial state
			inputInitialState();
			break;
		case 3:
			// Clears the screen for formatting
			system("cls");
			// Performs a breadth first search and displays the result
			breadthFirstSearch();
			break;
		case 4:
			// Clears the screen for formatting
			system("cls");
			// Performs a depth first search and displays the result
			depthFirstSearch();
			break;
		case 5:
			// Clears the screen for formatting
			system("cls");
			// Performs an A* search using misplaced tiles and displays the result
			misplacedTilesSearch();
			break;
		case 6:
			// Clears the screen for formatting
			system("cls");
			// Performs an A* search using misplaced tiles and displays the result
			manhattanDistanceSearch();
			break;
		case 7:
			// Exits the program
			return 0;
		}
	}

	system("pause");
	return 0;
}

// Function definitions
int userMenu()
{
	// Var used to hold the value for the user's choice
	int choice = 0;

	// Used for formatting purposes
	system("cls");

	// Displays the menu to the user
	cout << "Please choose an option. Enter 7 to exit the program." << endl
		<< "1: Create a random initial state." << endl
		<< "2: Manualy enter the initial state." << endl
		<< "3: Perform breadth first search." << endl
		<< "4: Perform depth first search." << endl
		<< "5: Perform A* search using misplaced tiles." << endl
		<< "6: Perform A* search ushing Manhattan distance." << endl
		<< "7: Exit the program." << endl
		<< "Current initial state: ";

	for (int i = 0; i < 9; i++)
	{
		cout << initialState[i];
	}

	cout << endl << endl;
	cout << "Enter your choice: ";

	// Gets the user's choice
	cin >> choice;
	cout << endl;

	// Checks to see if the choice was valid
	while ( (cin.fail()) || (choice < 1) || (choice > 7) )
	{
		// Clears the cin and buffer
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// Displays an error message to the user and asks for the choice again
		cout << "Please enter a valid number." << endl;
		cin >> choice;
		cout << endl;
	}

	// Returns the user's choice
	return choice;
}

int generateInitialState()
{
	// Message to let the user know what is going to happen
	cout << "An initial state will be randomly generated." << endl << endl;

	// Fills the initial array with values 0-8
	for (int i = 0; i < 9; i++)
	{
		initialState[i] = i;
	}

	// Shuffles the values in the array to create a random initial state
	random_shuffle(initialState, initialState + 9);

	// Displays the initial state that was generated
	cout << "Initial state: ";
	for (int i = 0; i < 9; i++)
	{
		cout << initialState[i];
	}

	// Formatting
	cout << endl << endl;

	system("pause");
	return 0;
}

int inputInitialState()
{
	// Var to hold the user input
	int tempNum = 0;

	// Message to let the user know what is going to happen
	cout << "An initial state will be manually set."
		 << " Enter numbers that are in the range 0-8 with no repeats." << endl << endl;

	// Sets all the values of the initial state array to a value that will not be used
	// so the numbers 0-8 may be checked and inserted correctly
	for (int i = 0; i < 9; i++)
	{
		initialState[i] = 9;
	}

	// Loops through 9 times
	for (int i = 0; i < 9; i++)
	{
		// Gets a number from the user 
		cout << "Enter a number: ";
		cin >> tempNum;
		cout << endl;

		// Checks to see if the number was valid and if it has been used yet
		while ( ((cin.fail()) || (tempNum < 0) || (tempNum > 8)) || 
			(find(initialState, initialState + 9, tempNum) != initialState + 9) )
		{
			// Clears the cin and buffer
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			// Displays an error message to the user and asks for the number again
			cout << "Please enter a valid number that has not been used yet." << endl;
			cin >> tempNum;
			cout << endl << endl;
		}

		// Places the number into the initial state array
		initialState[i] = tempNum;
	}

	// Displays the initial state that was generated
	cout << "Initial state: ";
	for (int i = 0; i < 9; i++)
	{
		cout << initialState[i];
	}

	// Formatting
	cout << endl << endl;

	system("pause");
	return 0;
}

int breadthFirstSearch()
{
	// Message to let the user know what is going to happen
	cout << "A breadth first search will be performed and the result displayed." << endl << endl;

	// Creates a string to represent the state
	string stateStr;
	for (int i = 0; i < 9; i++)
	{
		stateStr += to_string(initialState[i]);
	}
	
	// Counter used to keep track of the depth
	int depthCounter = 0;

	// Var used to keep track of how many nodes are expanded
	int nodesExpanded = 0;

	// Creates a node to store the first state
	node state;

	// Fills in the data for the node
	state.stateString = stateStr;
	for (int i = 0; i < 9; i++)
	{
		state.stateArray[i] = initialState[i];
	}
	state.depth = depthCounter;
	state.path.push_back(state.stateString);

	// Puts the node in the queue
	nodeQueue.push(state);

	// Puts the current state and depth counter in the map
	seenStates[stateStr] = depthCounter;

	// Sentinel value for while loop exit condition
	bool goalStateReached = false;

	// Loops search until the goal state is found
	while (goalStateReached == false)
	{
		// Checks to see if the the queue is empty
		if (nodeQueue.empty())
		{
			//If so, then no solution was found since the loop didn't exit via sentinel value
			cout << endl << "No solution was found using breadth first searching."
				<< endl << "Depth reached was: " << depthCounter
				<< endl << "Nodes expanded: " << nodesExpanded << endl;
			system("pause");
			return 1;
		}

		// Checks to see if the next node in the queue has the goal state
		if (nodeQueue.front().stateString == goalState)
		{
			// Sets sentinel value to true
			goalStateReached = true;

		}
		// If the next node in the queue isn't the goal state
		else
		{
			// Variable to let generateSuccessor function know to use the queue
			int breadth = 1;

			// Passes that state to a function to generate successor states
			generateSuccessors(nodeQueue.front(), breadth);

			// Records that a node was expanded
			nodesExpanded += 1;

			// Records the depth reached in case a solution isn't found
			depthCounter = nodeQueue.front().depth;

			// Pops the looked at state off the queue
			nodeQueue.pop();
		}
	}

	// Displays how many nodes were expanded
	cout << "Nodes expanded: " << nodesExpanded << endl;

	// Displays the depth of the solution
	cout << "Depth of solution path: " << nodeQueue.front().depth << endl;

	// Displays the path that was recorded
	cout << "Breadth first solution path: " << endl;
	// While the list still contains locations
	while (!nodeQueue.front().path.empty())
	{
		//Displays the location in the front of the list
		cout << nodeQueue.front().path.front();
		// Removes the location in the front of the list
		nodeQueue.front().path.pop_front();

		cout << endl;
	}

	// Empties the queue so it can be used again
	while (!nodeQueue.empty())
	{
		nodeQueue.pop();
	}

	// Empties the map
	seenStates.clear();

	system("pause");
	return 0;
}

int depthFirstSearch()
{
	// Message to let the user know what is going to happen
	cout << "A depth first search will be performed and the result displayed." << endl << endl;

	// Creates a string to represent the state
	string stateStr;
	for (int i = 0; i < 9; i++)
	{
		stateStr += to_string(initialState[i]);
	}

	// Var to keep track of how many nodes are expanded
	int nodesExpanded = 0;

	// Counter used to keep track of the depth
	int depthCounter = 0;

	// Creates a node to store the first state
	node state;

	// Fills in the data for the node
	state.stateString = stateStr;
	for (int i = 0; i < 9; i++)
	{
		state.stateArray[i] = initialState[i];
	}
	state.depth = depthCounter;

	// Puts the node in the stack
	nodeStack.push(state);

	// Puts the current state and depth counter in the map
	seenStates[stateStr] = depthCounter;

	// Sentinel value for while loop exit condition
	bool goalStateReached = false;

	// Loops search until the solution state is found
	while (goalStateReached == false)
	{
		// Checks to see if the the stack is empty
		if (nodeStack.empty())
		{
			//If so, then no solution was found since the loop didn't exit via sentinel value
			cout << endl << "No solution was found using depth first searching."
				<< endl << "Depth reached was: " << depthCounter
				<< endl << "Nodes expanded: " << endl;
			system("pause");
			return 1;
		}

		// Checks to see if the next node in the stack has the goal state
		if (nodeStack.top().stateString == goalState)
		{
			// Sets sentinel value to true
			goalStateReached = true;
		}
		// If the next node in the stack isn't the goal state
		else
		{
			// Variable to let generateSuccessor function know to use the stack
			int depthSearch = 2;

			// Temp node to hold the state's data
			node tempState;

			// Copy the state's data to tempState
			tempState.stateString = nodeStack.top().stateString;
			for (int i = 0; i < 9; i++)
			{
				tempState.stateArray[i] = nodeStack.top().stateArray[i];
			}
			tempState.depth = nodeStack.top().depth;

			// Records the depth reached in case a solutin isn't found
			depthCounter = nodeStack.top().depth;

			// Pops the looked at state off the stack
			nodeStack.pop();

			// Passes that state to a function to generate successor states
			generateSuccessors(tempState, depthSearch);

			// Records that a node was expanded
			nodesExpanded += 1;
		}
	}

	// Displays how many nodes were expanded
	cout << "Nodes expanded: " << nodesExpanded << endl;

	// Displays the depth of the solution
	cout << "Depth of solution path: " << nodeStack.top().depth << endl;

	// Empties the stack so it can be used again
	while (!nodeStack.empty())
	{
		nodeStack.pop();
	}

	// Empties the map
	seenStates.clear();

	system("pause");
	return 0;
}

int misplacedTilesSearch()
{
	// Message to let the user know what is going to happen
	cout << "An A* search using the number of misplaced tiles will be performed and the result displayed." << endl << endl;

	// Creates a string to represent the state
	string stateStr;
	for (int i = 0; i < 9; i++)
	{
		stateStr += to_string(initialState[i]);
	}

	// Counter used to keep track of the depth
	int depthCounter = 0;

	// Var to keep track of how many nodes are expanded
	int nodesExpanded = 0;

	// Creates a node to store the first state
	node state;

	// Fills in the data for the node
	state.stateString = stateStr;
	for (int i = 0; i < 9; i++)
	{
		state.stateArray[i] = initialState[i];
	}
	state.depth = depthCounter;
	// Calculates the number of misplaced tiles
	for (int i = 0; i < 9; i++)
	{
		if (state.stateArray[i] != goalStateArray[i] && state.stateArray[i] != 0)
		{
			state.heuristic += 1;
		}
	}
	state.heuristic += state.depth;
	state.path.push_back(state.stateString);

	// Puts the node in the priority queue
	priorityQueue.push(state);

	// Puts the current state and depth counter in the map
	seenStates[stateStr] = depthCounter;

	// Sentinel value for while loop exit condition
	bool goalStateReached = false;

	// Loops search until the goal state is found
	while (goalStateReached == false)
	{
		// Checks to see if the the priority queue is empty
		if (priorityQueue.empty())
		{
			//If so, then no solution was found since the loop didn't exit via sentinel value
			cout << endl << "No solution was found using A*(misplaced tiles) searching."
				<< endl << "Depth reached was: " << depthCounter
				<< endl << "Nodes expanded: " << nodesExpanded << endl;
			system("pause");
			return 1;
		}

		// Checks to see if the next node in the priority queue has the goal state
		if (priorityQueue.top().stateString == goalState)
		{
			// Sets sentinel value to true
			goalStateReached = true;

		}
		// If the next node in the priority queue isn't the goal state
		else
		{
			// Variable to let generateSuccessor function know to use the priority queue
			int misplacedTilesSearch = 3;

			// Temp node to hold the state's data
			node tempState;

			// Copy the state's data to tempState
			tempState.stateString = priorityQueue.top().stateString;
			for (int i = 0; i < 9; i++)
			{
				tempState.stateArray[i] = priorityQueue.top().stateArray[i];
			}
			tempState.heuristic = priorityQueue.top().heuristic;
			tempState.depth = priorityQueue.top().depth;
			tempState.path = priorityQueue.top().path;

			// Records the depth reached in case a solution isn't found
			depthCounter = priorityQueue.top().depth;

			// Pops the looked at state off the priority queue
			priorityQueue.pop();

			// Passes that state to a function to generate successor states
			generateSuccessors(tempState, misplacedTilesSearch);

			// Records that a node was expanded
			nodesExpanded += 1;
		}
	}

	// Displays how many nodes were expanded
	cout << "Nodes expanded: " << nodesExpanded << endl;

	// Displays the depth of the solution
	cout << "Depth of solution path: " << priorityQueue.top().depth << endl;

	// Creates an iterator to move through the list
	list<string>::const_iterator it;
	it = priorityQueue.top().path.begin();

	// Moves through the list, displaying the solution path
	for (it; it != priorityQueue.top().path.end(); ++it)
	{
		cout << *it << endl;
	}

	// Empties the priority queue so it can be used again
	while (!priorityQueue.empty())
	{
		priorityQueue.pop();
	}

	// Empties the map
	seenStates.clear();

	system("pause");
	return 0;
}

int manhattanDistanceSearch()
{
	// Message to let the user know what is going to happen
	cout << "An A* search using the Manhattan distance will be performed and the result displayed." << endl << endl;

	// Creates a string to represent the state
	string stateStr;
	for (int i = 0; i < 9; i++)
	{
		stateStr += to_string(initialState[i]);
	}

	// Counter used to keep track of the depth
	int depthCounter = 0;

	// Var to keep track of how many nodes were expanded
	int nodesExpanded = 0;

	// Creates a node to store the first state
	node state;

	// Fills in the data for the node
	state.stateString = stateStr;
	for (int i = 0; i < 9; i++)
	{
		state.stateArray[i] = initialState[i];
	}
	state.depth = depthCounter;
	// Calculates the Manhattan distance
	for (int i = 0; i < 9; i++)
	{
		if (state.stateArray[i] != 0)
		{
			for (int j = 0; j < 9; j++)
			{
				if (state.stateArray[i] == goalStateArray[j])
				{
					state.heuristic += abs(i - j);
				}
			}
		}
	}
	state.heuristic += state.depth;
	state.path.push_back(state.stateString);

	// Puts the node in the priority queue
	priorityQueue.push(state);

	// Puts the current state and depth counter in the map
	seenStates[stateStr] = depthCounter;

	// Sentinel value for while loop exit condition
	bool goalStateReached = false;

	// Loops search until the goal state is found
	while (goalStateReached == false)
	{
		// Checks to see if the the priority queue is empty
		if (priorityQueue.empty())
		{
			//If so, then no solution was found since the loop didn't exit via sentinel value
			cout << endl << "No solution was found using A*(Manhattan distance) searching."
				<< endl << "Depth reached was: " << depthCounter
				<< endl << "Nodes expanded: " << nodesExpanded << endl;
			system("pause");
			return 1;
		}

		// Checks to see if the next node in the priority queue has the goal state
		if (priorityQueue.top().stateString == goalState)
		{
			// Sets sentinel value to true
			goalStateReached = true;

		}
		// If the next node in the priority queue isn't the goal state
		else
		{
			// Variable to let generateSuccessor function know to use the priority queue
			int manhattanDistanceSearch = 4;

			// Temp node to hold the state's data
			node tempState;

			// Copy the state's data to tempState
			tempState.stateString = priorityQueue.top().stateString;
			for (int i = 0; i < 9; i++)
			{
				tempState.stateArray[i] = priorityQueue.top().stateArray[i];
			}
			tempState.heuristic = priorityQueue.top().heuristic;
			tempState.depth = priorityQueue.top().depth;
			tempState.path = priorityQueue.top().path;

			// Records the depth reached in case a solution isn't found
			depthCounter = priorityQueue.top().depth;

			// Pops the looked at state off the priority queue
			priorityQueue.pop();

			// Passes that state to a function to generate successor states
			generateSuccessors(tempState, manhattanDistanceSearch);

			// Records that a node was expanded
			nodesExpanded += 1;
		}
	}

	// Displays how many nodes were expanded
	cout << "Nodes expanded: " << nodesExpanded << endl;

	// Displays the depth of the solution
	cout << "Depth of solution path: " << priorityQueue.top().depth << endl;

	// Creates an iterator to move through the list
	list<string>::const_iterator it;
	it = priorityQueue.top().path.begin();

	// Moves through the list, displaying the solution path
	for (it; it != priorityQueue.top().path.end(); ++it)
	{
		cout << *it << endl;
	}

	// Empties the priority queue so it can be used again
	while (!priorityQueue.empty())
	{
		priorityQueue.pop();
	}

	// Empties the map
	seenStates.clear();

	system("pause");
	return 0;
}

void generateSuccessors(node state, int num)
{
	// String var used to check states in the map
	string stateStr;

	// Finds the index value of the 0 (blank piece) in the state array
	int blankIndex = 0;
	for (int i = 0; i < 9; i++)
	{
		// Records the index of the blank piece in a var
		if (state.stateArray[i] == 0)
		{
			blankIndex = i;
		}
	}

	// Vars used to swap the values
	int temp1 = 0;
	int temp2 = 0;

	// Series of if statements that will create successor states depending on the blankIndex
	if (blankIndex == 0)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[0];
		temp2 = workingState[1];
		
		// Swaps the array values
		workingState[0] = temp2;
		workingState[1] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;

		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[0];
		temp2 = workingState[3];

		// Swaps array values
		workingState[0] = temp2;
		workingState[3] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

	else if (blankIndex == 1)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[1];
		temp2 = workingState[2];

		// Swaps array values
		workingState[1] = temp2;
		workingState[2] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
			// Copies the state in the node into the working state
			for (int i = 0; i < 9; i++)
			{
				workingState[i] = state.stateArray[i];
			}

			// Vars used to swap the values
			temp1 = workingState[1];
			temp2 = workingState[4];

			// Swaps array values
			workingState[1] = temp2;
			workingState[4] = temp1;

			// Empties out stateStr 
			stateStr = "";

			// Converts the workingState to a string to be checked
			for (int i = 0; i < 9; i++)
			{
				stateStr += to_string(workingState[i]);
			}

			// Checks to see if that state has been seen
			if (checkSeenStates(stateStr) == false)
			{
				// Creates a node to store the state
				node newState;

				// Fills in the data for the node
				newState.stateString = stateStr;
				for (int i = 0; i < 9; i++)
				{
					newState.stateArray[i] = workingState[i];
				}
				newState.depth = state.depth + 1;
				// Depending on the type of search, will calculate either the number of misplaced tiles
				// or the Manhattan distance for the heuristic
				if (num == 3)
				{
					for (int i = 0; i < 9; i++)
					{
						if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
						{
							newState.heuristic += 1;
						}
					}
				}
				else if (num == 4)
				{
					for (int i = 0; i < 9; i++)
					{
						if (state.stateArray[i] != 0)
						{
							for (int j = 0; j < 9; j++)
							{
								if (state.stateArray[i] == goalStateArray[j])
								{
									state.heuristic += abs(i - j);
								}
							}
						}
					}
				}
				newState.heuristic += newState.depth;
				// Records the path unless a depth first search is being performed
				if (num != 2)
				{
					newState.path = state.path;
					newState.path.push_back(stateStr);
				}

				// If the variable passed in is a 1, pushes to queue
				// If the variable passed in is a 2, pushes to stack
				// If the variable passed in is a 3 or 4, pushes to priority queue
				if (num == 1)
				{
					nodeQueue.push(newState);
				}
				else if (num == 2)
				{
					nodeStack.push(newState);
				}
				else if (num == 3 || num == 4)
				{
					priorityQueue.push(newState);
				}

				// Records the state and depth in the map
				seenStates[newState.stateString] = newState.depth;
			}
				// Copies the state in the node into the working state
				for (int i = 0; i < 9; i++)
				{
					workingState[i] = state.stateArray[i];
				}

				// Vars used to swap the values
				temp1 = workingState[1];
				temp2 = workingState[0];

				// Swaps array values
				workingState[1] = temp2;
				workingState[0] = temp1;

				// Empties out stateStr 
				stateStr = "";

				// Converts the workingState to a string to be checked
				for (int i = 0; i < 9; i++)
				{
					stateStr += to_string(workingState[i]);
				}

				// Checks to see if that state has been seen
				if (checkSeenStates(stateStr) == false)
				{
					// Creates a node to store the state
					node newState;

					// Fills in the data for the node
					newState.stateString = stateStr;
					for (int i = 0; i < 9; i++)
					{
						newState.stateArray[i] = workingState[i];
					}
					newState.depth = state.depth + 1;
					// Depending on the type of search, will calculate either the number of misplaced tiles
					// or the Manhattan distance for the heuristic
					if (num == 3)
					{
						for (int i = 0; i < 9; i++)
						{
							if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
							{
								newState.heuristic += 1;
							}
						}
					}
					else if (num == 4)
					{
						for (int i = 0; i < 9; i++)
						{
							if (state.stateArray[i] != 0)
							{
								for (int j = 0; j < 9; j++)
								{
									if (state.stateArray[i] == goalStateArray[j])
									{
										state.heuristic += abs(i - j);
									}
								}
							}
						}
					}
					newState.heuristic += newState.depth;
					// Records the path unless a depth first search is being performed
					if (num != 2)
					{
						newState.path = state.path;
						newState.path.push_back(stateStr);
					}

					// If the variable passed in is a 1, pushes to queue
					// If the variable passed in is a 2, pushes to stack
					// If the variable passed in is a 3 or 4, pushes to priority queue
					if (num == 1)
					{
						nodeQueue.push(newState);
					}
					else if (num == 2)
					{
						nodeStack.push(newState);
					}
					else if (num == 3 || num == 4)
					{
						priorityQueue.push(newState);
					}

					// Records the state and depth in the map
					seenStates[newState.stateString] = newState.depth;
				}
			}

	else if (blankIndex == 2)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[2];
		temp2 = workingState[5];

		// Swaps array values
		workingState[2] = temp2;
		workingState[5] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

			// Copies the state in the node into the working state
			for (int i = 0; i < 9; i++)
			{
				workingState[i] = state.stateArray[i];
			}

			// Vars used to swap the values
			temp1 = workingState[2];
			temp2 = workingState[1];

			// Swaps array values
			workingState[2] = temp2;
			workingState[1] = temp1;

			// Empties out stateStr 
			stateStr = "";

			// Converts the workingState to a string to be checked
			for (int i = 0; i < 9; i++)
			{
				stateStr += to_string(workingState[i]);
			}

			// Checks to see if that state has been seen
			if (checkSeenStates(stateStr) == false)
			{
				// Creates a node to store the state
				node newState;

				// Fills in the data for the node
				newState.stateString = stateStr;
				for (int i = 0; i < 9; i++)
				{
					newState.stateArray[i] = workingState[i];
				}
				newState.depth = state.depth + 1;
				// Depending on the type of search, will calculate either the number of misplaced tiles
				// or the Manhattan distance for the heuristic
				if (num == 3)
				{
					for (int i = 0; i < 9; i++)
					{
						if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
						{
							newState.heuristic += 1;
						}
					}
				}
				else if (num == 4)
				{
					for (int i = 0; i < 9; i++)
					{
						if (state.stateArray[i] != 0)
						{
							for (int j = 0; j < 9; j++)
							{
								if (state.stateArray[i] == goalStateArray[j])
								{
									state.heuristic += abs(i - j);
								}
							}
						}
					}
				}
				newState.heuristic += newState.depth;
				// Records the path unless a depth first search is being performed
				if (num != 2)
				{
					newState.path = state.path;
					newState.path.push_back(stateStr);
				}

				// If the variable passed in is a 1, pushes to queue
				// If the variable passed in is a 2, pushes to stack
				// If the variable passed in is a 3 or 4, pushes to priority queue
				if (num == 1)
				{
					nodeQueue.push(newState);
				}
				else if (num == 2)
				{
					nodeStack.push(newState);
				}
				else if (num == 3 || num == 4)
				{
					priorityQueue.push(newState);
				}

				// Records the state and depth in the map
				seenStates[newState.stateString] = newState.depth;
			}
	}

	else if (blankIndex == 3)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[3];
		temp2 = workingState[0];

		// Swaps array values
		workingState[3] = temp2;
		workingState[0] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[3];
		temp2 = workingState[4];

		// Swaps array values
		workingState[3] = temp2;
		workingState[4] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[3];
		temp2 = workingState[6];

		// Swaps array values
		workingState[3] = temp2;
		workingState[6] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

	else if (blankIndex == 4)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[4];
		temp2 = workingState[1];

		// Swaps array values
		workingState[4] = temp2;
		workingState[1] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[4];
		temp2 = workingState[5];

		// Swaps array values
		workingState[4] = temp2;
		workingState[5] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[4];
		temp2 = workingState[7];

		// Swaps array values
		workingState[4] = temp2;
		workingState[7] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[4];
		temp2 = workingState[3];

		// Swaps array values
		workingState[4] = temp2;
		workingState[3] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

	else if (blankIndex == 5)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[5];
		temp2 = workingState[2];

		// Swaps array values
		workingState[5] = temp2;
		workingState[2] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[5];
		temp2 = workingState[8];

		// Swaps array values
		workingState[5] = temp2;
		workingState[8] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[5];
		temp2 = workingState[4];

		// Swaps array values
		workingState[5] = temp2;
		workingState[4] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

	else if (blankIndex == 6)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[6];
		temp2 = workingState[3];

		// Swaps array values
		workingState[6] = temp2;
		workingState[3] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[6];
		temp2 = workingState[7];

		// Swaps array values
		workingState[6] = temp2;
		workingState[7] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

	else if (blankIndex == 7)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[7];
		temp2 = workingState[4];

		// Swaps array values
		workingState[7] = temp2;
		workingState[4] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[7];
		temp2 = workingState[8];

		// Swaps array values
		workingState[7] = temp2;
		workingState[8] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[7];
		temp2 = workingState[6];

		// Swaps array values
		workingState[7] = temp2;
		workingState[6] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

	else if (blankIndex == 8)
	{
		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[8];
		temp2 = workingState[5];

		// Swaps array values
		workingState[8] = temp2;
		workingState[5] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}

		// Copies the state in the node into the working state
		for (int i = 0; i < 9; i++)
		{
			workingState[i] = state.stateArray[i];
		}

		// Vars used to swap the values
		temp1 = workingState[8];
		temp2 = workingState[7];

		// Swaps array values
		workingState[8] = temp2;
		workingState[7] = temp1;

		// Empties out stateStr 
		stateStr = "";

		// Converts the workingState to a string to be checked
		for (int i = 0; i < 9; i++)
		{
			stateStr += to_string(workingState[i]);
		}

		// Checks to see if that state has been seen
		if (checkSeenStates(stateStr) == false)
		{
			// Creates a node to store the state
			node newState;

			// Fills in the data for the node
			newState.stateString = stateStr;
			for (int i = 0; i < 9; i++)
			{
				newState.stateArray[i] = workingState[i];
			}
			newState.depth = state.depth + 1;
			// Depending on the type of search, will calculate either the number of misplaced tiles
			// or the Manhattan distance for the heuristic
			if (num == 3)
			{
				for (int i = 0; i < 9; i++)
				{
					if (newState.stateArray[i] != 0 && newState.stateArray[i] != goalStateArray[i])
					{
						newState.heuristic += 1;
					}
				}
			}
			else if (num == 4)
			{
				for (int i = 0; i < 9; i++)
				{
					if (state.stateArray[i] != 0)
					{
						for (int j = 0; j < 9; j++)
						{
							if (state.stateArray[i] == goalStateArray[j])
							{
								state.heuristic += abs(i - j);
							}
						}
					}
				}
			}
			newState.heuristic += newState.depth;
			// Records the path unless a depth first search is being performed
			if (num != 2)
			{
				newState.path = state.path;
				newState.path.push_back(stateStr);
			}

			// If the variable passed in is a 1, pushes to queue
			// If the variable passed in is a 2, pushes to stack
			// If the variable passed in is a 3 or 4, pushes to priority queue
			if (num == 1)
			{
				nodeQueue.push(newState);
			}
			else if (num == 2)
			{
				nodeStack.push(newState);
			}
			else if (num == 3 || num == 4)
			{
				priorityQueue.push(newState);
			}

			// Records the state and depth in the map
			seenStates[newState.stateString] = newState.depth;
		}
	}

}

bool checkSeenStates(string state)
{
	// Creates an iterator to run through the map to see if the state is there
	map<string, int>::iterator it;
	it = seenStates.find(state);
	
	if (it == seenStates.end())
	{
		// If the state isn't in the map, returns false
		return false;
	}
	else
	{
		// If the state is in the map, returns true
		return true;
	}
	
}
