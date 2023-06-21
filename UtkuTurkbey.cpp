#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main (int argc, char * argv[]) //In order to give command line command necessary arguments added to the main function
{
	
	//1)DECLARATION OF NECESSARY VARIABLES TO BE USED LATER
	
	ifstream myInputFile; //stream object in order to read from files
	ofstream myOutputFile; //stream object in order to write to files
	
	const double eMACH = 1e-7; //IEEE SP machine epsilon is used throughtout this program 
	
	int size = 0; //to make it has a meaningfull name it is called size, size of our b vector, and size of one dimension of our square matrix A.
				  //simply it can be called also n for nx1 vector b and nxn matrix A 

	double currentInput; //a dummy variable to hold the read data, during measuring size of the vector
						 
	double ** aMatrix;	//pointer pointer variable, later to be defined as a 2D array for holding matrix A
	double * bVector;	//pointer variable, later to be defined as a 1D array for holding vector b
	double * solutionVector;	//pointer variable, later to be defined as a 1D array for holding solution vector 

	//2)READING INPUTS MATRIX A AND VECTOR B FROM TEXT FILES GIVEN AS COMMAND LINE ARGUMENTS USING DYNAMIC MEMORY ALLOCATION
 	
	/*2.1)We have to determine the size of the matrix A and vector b in order to allocate dynamic memories.  
	Therefore we go through the b vector data and count the number of elements. Since the number of elements
	within this vector is also equal to the number of elements in each row of the matrix A, we will have
	driven the number "n"(called size variable) which defines bot nxn matrix A and nx1 vector b.    	
	*/
	myInputFile.open(argv[2]); //open vector data file
    while ( myInputFile >> currentInput) //as long as an input exists keep on scanning, increment size in each step 
    {
    	size = size + 1; 
	}
	myInputFile.close(); //close vector data file
	
	/*2.2)Since we now know the size of the vector b, we can create new array to represent our vector of
	 size n(which is size variable in my code)
	 */
	bVector = new double [size]; //initialize 1D vector array using dynamic memory allocation 
	
	myInputFile.open(argv[2]); //need to reopen the same file to be able to read from the beginning
	for(int i = 0; i < size; i++)
	{
    	myInputFile >> bVector[i]; //record each element in order
	}
	myInputFile.close();//close vector data file 
   
    //2.3)Since we now know the size of matrix A we can initialize A and read it from the file 
    aMatrix = new double * [size];
    for(int i = 0; i < size; i++)
    {
    	aMatrix[i] = new double [size];
	}
	myInputFile.open(argv[1]); //open the matrix data file 
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			myInputFile >> aMatrix[i][j];
		}
	}
	myInputFile.close();//close matrix data file 
    
	
	//3)COMPUTING CONDITION NUMBER FOR 2X2 MATRICES AND PRINTING TO THE CONSOLE
	if (size == 2)
	{
		//REMARK: Since we know that A matrix is 2x2, general solutions using loops and use of size will not be used.
		
		//3.1)Find A matrix 1-norm
		double norm1A; //it is the maximum abs column sum of matrix A
		if ( abs(aMatrix[0][0]) + abs(aMatrix[1][0]) >= abs(aMatrix[1][1]) + abs(aMatrix[0][1])) //First column's abs sum vs. Second column's abs sum
		{
			norm1A = abs(aMatrix[0][0]) + abs(aMatrix[1][0]);
		}
		else
		{
			norm1A = abs(aMatrix[1][1]) + abs(aMatrix[0][1]);
		}
		
		//3.2)Find A matrix infinity-norm
		double normInfA; //it is the maximum abs row sum of matrix A
		if ( abs(aMatrix[0][0]) + abs(aMatrix[0][1]) >= abs(aMatrix[1][0]) + abs(aMatrix[1][1])) //First row's abs sum vs Second row's abs sum
		{
			normInfA =abs(aMatrix[0][0]) + abs(aMatrix[0][1]);
		}
		else
		{
			normInfA = abs(aMatrix[1][0]) + abs(aMatrix[1][1]);
		}
		
		//3.3)Find determinant of A matrix;
		double detA = aMatrix[0][0] * aMatrix[1][1] - aMatrix[0][1] * aMatrix[1][0];
		
		//3.4)Find inverse of A matrix 1-norm
		double norm1AInverse; //it is the maximum abs column sum of matrix A
		if ( abs(aMatrix[1][1]) + abs(aMatrix[1][0]) >= abs(aMatrix[0][0]) + abs(aMatrix[0][1])) //First column's abs sum vs. Second column's abs sum
		{
			norm1AInverse = abs( (abs(aMatrix[1][1]) + abs(aMatrix[1][0]) ) / detA );
		}
		else
		{
			norm1AInverse = abs( (abs(aMatrix[0][0]) + abs(aMatrix[0][1]) ) / detA );
		}
		
		//3.5)Find inverse of A matrix infinity-norm
		double normInfAInverse; //it is the maximum abs row sum of matrix A
		if ( abs(aMatrix[1][1]) + abs(aMatrix[0][1]) >= abs(aMatrix[1][0]) + abs(aMatrix[0][0])) //First row's abs sum vs Second row's abs sum
		{
			normInfAInverse = abs( (abs(aMatrix[1][1]) + abs(aMatrix[0][1]) ) / detA );
		}
		else
		{
			normInfAInverse = abs( (abs(aMatrix[1][0]) + abs(aMatrix[0][0]) ) / detA );
		}
		
		//3.6) Print condotion numbers 
		cout << "Condition number at 1: " << norm1A * norm1AInverse << "\n";
		cout << "Condition number at infinity: " << normInfA * normInfAInverse << "\n";
		 
	
	}	
	
	
	//4)APPLYING GAUSSIAN ELIMINATION WITH PARTIAL PIVOTING 
	
	/*4.0) Loop over columns and apply the Gaussian elimination with partial pivoting
	Main idea of this algorithm is obtained from the course book "Scientific Computation" page 73 Algorithm 2.4
	Necessarry manupilations for implementation are done by myself 
	*/
	for (int k = 0; k < size - 1; k++) //loop for traveling through columns 
	{
		//4.1)Defining necessary variables
		double intermediateRow[size]; //dummy variable for row exchanging, in order to hold maximum pivot row while original pivot row is being transferred
									  // !!! this matrix will also be used as the single column of elementary elimination matrix M
		double intermediateVectorElement; //dummy variable for element exchanging in b vector
		double largestInColumn = abs (aMatrix[k][k]); //dummy variable to find the index of absolutely largest element index on or below the current pivot 
		int indexOfLargest = k; //initially set to location of pivot
		
		//4.2)PARTIAL PIVOTING
		for(int p = k + 1; p < size; p++) //loop over elements of the current column to find the largest
		{
			if (abs(aMatrix[p][k]) > largestInColumn) //if larger than previos larger update tracking values
			{
				largestInColumn = abs (aMatrix[p][k]);
				indexOfLargest = p;
			}
		}
		if (k != indexOfLargest) //if pivot should change, then change the rows of matrix and the vector
		{
			for (int x = 0; x < size; x++)
			{
				//change two rows of matrix using an intermediate element to not loose data
				intermediateRow[x] = aMatrix[indexOfLargest][x];
				aMatrix[indexOfLargest][x] = aMatrix[k][x];
				aMatrix[k][x] = intermediateRow[x];
			}
			//change two elements of the vector accordingly using an intermediate element to not loose data
			intermediateVectorElement = bVector[indexOfLargest];
			bVector[indexOfLargest] = bVector[k];
			bVector[k] = intermediateVectorElement;
		}
		
		//4.3)Singularity Check
		if ( abs(aMatrix[k][k]) < eMACH) //if largest pivot is 0 then the matrix is singular. Program terminates.
		{
			cout << "A matrix is singular. Program terminated";
			return 0;
		}
		
		//4.4)Compute elimination matrix which will eliminate elements under current pivot  
		for (int i = k + 1; i  < size; i++) // this loop computes the multipliers of elementary elimination matrix and csave them to the "intermediateRow" array
		{
			intermediateRow[i] = aMatrix[i][k] / aMatrix[k][k]; //kth column of the elementary elimination matrix elements are being computed
		}
		
		//4.5)Apply computed elimintaion matrices to the submatrice of A and subvectror of b 
		for (int j = k + 1; j < size; j++) //looping through columns of submatrix
										   //applying transformation to the remaining submatrix of A and subvector of b
		{
			for (int i = k + 1; i < size; i++) //applyingg transformation through each elements in a column
			{
				aMatrix[i][j] = aMatrix[i][j] - intermediateRow[i] * aMatrix[k][j]; //updating each element in sub matrix
			}
			bVector[j] = bVector[j] - intermediateRow[j] * bVector[k]; //updating each element in subVector
		}
		
	}
	
	//4.6)Check the last pivot since loop ends after updating last pivot but without checking whether it is zero 
	if ( abs(aMatrix[size-1][size-1]) < eMACH) //if largest pivot is 0 then the matrix is singular. Program terminates.
	{
		cout << "A matrix is singular. Program terminated";
		return 0;
	}
	
	//5)APPLYING BACKWARD SUBSTITUTION TO OBTAINED UPPER TRIANGULAR MATRIX
	
	solutionVector = new double [size]; 
	/*5.0)Loop backwards over columns and apply backward substitution
	Main idea of this algorithm is obtained from the course book "Scientific Computation", page 66, Chapter 2, Algorithm 2.2
	Necessarry manupilations for implementation are done by myself 
	*/
	for (int j = size - 1; j >= 0; j--)
	{
		//5.1)Compute solution component starting from last pivot	
		solutionVector[j] = bVector[j] / aMatrix[j][j];
		
		//5.2)Update b vector according to found component of the solution vector
		for (int i = 0; i < j; i++)
		{
			bVector[i] = bVector[i] - aMatrix[i][j] * solutionVector[j];
		}
	}

		
	
	//6)PRINTING SOLUTION TO THE TEXT FILE
	
	myOutputFile.open("solution.txt"); //Name of the text file is fixed and "solution.txt". Solution vector is being printed with the format of input vector b
	cout << "Result vector: (Also stored in solution.txt)\n";
	for (int i = 0; i < size; i++)
	{
		cout << solutionVector[i] << "\n";
		myOutputFile << solutionVector[i] << "\n";
	}
	myOutputFile.close();
	/*Vector b is printed 
	*/

  return 0;
}
