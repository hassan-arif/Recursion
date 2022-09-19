#include <iostream>
#include <Windows.h>
using namespace std;

void SetColorAndBackground(int ForgC, int BackC);													//graphics for queen's problem
void consult(int& n, int below);																	//throws error based loop until user enter valid entry
void consult(int& n, int below, int above);

template <typename t>
void print(t*, int);																				//prints 1d array (array set)
template <typename t>
void printsubsets(t** a, int r, int c);																//prints 2d array (array of subsets)

void extendedPlaceQueens(int* array, const int queens, int& count, int row);						//recursive function within placequeens because placequeens had zero parameters
int* placeQueens();

int** ComputeSubsets(int* set, int size, int k);													//for main
void extendedSubsetComputation(int**& subset, int& row, int* set, int size, int k, int* content);	//recursive function to calculate all valid subsets

int main() {
	int size = 0, n = 0, * A1 = NULL;

	cout << "---------------------------EIGHT QUEENS PROBLEM---------------------------\n\n";
	A1 = placeQueens();
	size = _msize(A1) / 4;
	cout << size << " chess queens placed on " << size << " x " << size << " chessboard...\n\n";
	for (int i = 0; i < size; i++) {
		SetColorAndBackground(15, 0);
		cout << " ";
		for (int j = 0; j < size; j++) {
			if (A1[i] == j) {
				SetColorAndBackground(4, 4);
				cout << "Q ";
			}
			else {
				if (i % 2 == 0 && j % 2 == 0) SetColorAndBackground(7, 7);
				else if (i % 2 == 0 && j % 2 != 0) SetColorAndBackground(8, 8);
				else if (i % 2 != 0 && j % 2 == 0) SetColorAndBackground(8, 8);
				else if (i % 2 != 0 && j % 2 != 0) SetColorAndBackground(7, 7);
				cout << false << " ";
			}SetColorAndBackground(15, 0);
		}cout << endl;
	}cout << endl;
	SetColorAndBackground(15, 0);

	delete[]A1;
	A1 = NULL;

	cout << "------------------------------FINDING SUBSETS-----------------------------\n\n";
	cout << "please enter set size (>0): ";
	cin >> size;
	consult(size, 1);

	cout << "please enter " << size << " elements: ";
	A1 = new int[size] {};
	for (int i = 0; i < size; i++) cin >> A1[i];

	cout << "please enter subset size (0 <= k <= " << size << "): ";
	int k = 0;
	cin >> k;
	consult(k, 0, size);

	cout << "S = ";
	print(A1, size);
	cout << "k = " << k << endl;

	int** ptr = ComputeSubsets(A1, size, k);

	cout << "subsets =\n";
	if (ptr) {
		n = _msize(ptr) / 4;
		printsubsets(ptr, n, k);
	}
	else cout << "{}\n";
	cout << endl;

	delete[]A1;
	A1 = NULL;
	if (ptr) {
		for (int i = 0; i < n; i++) delete[] ptr[i];
		delete[] ptr;
		ptr = NULL;
	}

	system("pause");
	return 0;
}

int** ComputeSubsets(int* set, int size, int k) {							//wrapper function
	if (k <= 0 || k > size) return NULL;										//invalid subset size

	int* content = new int[k];												//array which holds indexes of subset's entries
	for (int i = 0; i < k; i++) content[i] = i;

	int** subsets = NULL;
	int row = 0;
	extendedSubsetComputation(subsets, row, set, size, k, content);			//call to recursive function

	delete[] content;
	content = NULL;
	return subsets;
}

void extendedSubsetComputation(int**& subset, int& row, int* set, int size, int k, int* content) {
	int i = k - 1, j = 0;
	while (i > 0) {
		if (content[i] == size - j) {																//if current index exceeds limit, it sets it to zero and increments left one
			content[i - 1]++;
			int l = i;
			while (l < k) {
				content[l] = content[l - 1] + 1;
				l++;
			}
		}
		i--;
		j++;
	}
	if (content[k - 1] == size) return;																//base condition

	if (row >= 1) {																					//for 2nd and onwards subsets (if exists)
		row++;
		int** newset = new int* [row];
		for (int i = 0; i < row; i++) newset[i] = new int[k];

		for (int i = 0; i < row - 1; i++) for (int j = 0; j < k; j++) newset[i][j] = subset[i][j];
		for (int i = 0; i < k; i++) newset[row - 1][i] = set[content[i]];
		content[k - 1]++;

		for (int i = 0; i < row - 1; i++) delete[] subset[i];
		delete[] subset;
		subset = newset;
		newset = NULL;
	}
	else {																							//for first subset
		row++;
		subset = new int* [row];
		for (int i = 0; i < row; i++) subset[i] = new int[k];

		for (int i = 0; i < row; i++) for (int j = 0; j < k; j++) subset[i][j] = set[content[j]];
		content[k - 1]++;
	}
	extendedSubsetComputation(subset, row, set, size, k, content);									//call to find next subset
}

int* placeQueens() {													//assumed as wrapper function because it has no parameters by default
	cout << "please enter total number of queens (4 <= x <= 13): ";		//queens attack each other below table of 4. function crashes due to multiple recursive call for table above 13.
	int size = 0;
	cin >> size;
	consult(size, 4, 13);
	int count = size;

	int* array = new int[size];
	for (int i = 0; i < size; i++) array[i] = -1;						//initially, there's no queen on table

	extendedPlaceQueens(array, size, count, 0);
	return array;
}

void extendedPlaceQueens(int* array, const int queens, int& count, int row) {
	if (count == 0 || queens < 4) {													//base case (all queens correctly placed)
		return;
	}

	array[row]++;																	//placement of first queen
	if (array[row] == queens) {														//if queen exceeds column's limit
		array[row] = -1;
		count++;
		extendedPlaceQueens(array, queens, count, row - 1);							//deletion of queen and recall previous recursion
		return;
	}

	bool sameCol = false, sameLeftDiagonal = false, sameRightDiagonal = false;
	int prev = row - 1;
	while (prev >= 0) {																//checks if newly placed queen doesn't attack other
		if (array[prev] == array[row]) {
			sameCol = true;
			break;
		}
		else if (array[prev] + (row - prev) == array[row]) {
			sameLeftDiagonal = true;
			break;
		}
		else if (array[prev] - (row - prev) == array[row]) {
			sameRightDiagonal = true;
			break;
		}
		prev--;
	}

	if (sameCol || sameLeftDiagonal || sameRightDiagonal) {
		extendedPlaceQueens(array, queens, count, row);								//newly placed queen's position isn't valid, recursive call to update position
	}
	else {
		count--;
		extendedPlaceQueens(array, queens, count, row + 1);							//newly placed queen's position is valid, recursive call to place next queen
	}
}

template <typename t>
void printsubsets(t** a, int r, int c) {
	int nextline = 1;
	cout << "{ ";
	for (int i = 0; i < r - 1; i++) {
		if (i == (5 * nextline)) {
			cout << endl << "  ";
			nextline++;
		}
		cout << "{ ";
		for (int j = 0; j < c - 1; j++) {
			cout << a[i][j] << ", ";
		}cout << a[i][c - 1] << " }, ";
	}
	cout << "{ ";
	for (int j = 0; j < c - 1; j++) cout << a[r - 1][j] << ", ";
	cout << a[r - 1][c - 1] << " } }\n";
}

template <typename t>
void print(t* array, int size) {
	if (array) {
		cout << "{ ";
		for (int i = 0; i < size - 1; i++)
			cout << array[i] << ", ";
		cout << array[size - 1] << " }";
		cout << endl;
	}
	else cout << "array is empty!\n";
}

void consult(int& n, int below, int above) {
	while (n < below || n>above) {
		cout << "invalid entry! enter again: ";
		cin >> n;
	}
}

void consult(int& n, int below) {
	while (n < below) {
		cout << "invalid entry! enter again: ";
		cin >> n;
	}
}

void SetColorAndBackground(int ForgC, int BackC) {
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}