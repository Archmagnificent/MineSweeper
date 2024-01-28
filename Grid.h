#pragma once
#include <iostream>  
#include <array>     
#include <vector>    
#include <stdlib.h>  //For system("CLS")

namespace Grid
{
//Namespace "Grid" Starts here : 

	template <typename T, std::size_t Row, std::size_t Column>		//Usage example : Array2D<int, 4, 7>
	using Array2D = std::array<std::array<T, Column>, Row>;			//Creates a grid with 4 rows and 7 columns

	template <typename T, std::size_t Size>							//Usage example : UniformArray2D<int, 4>
	using UniformArray2D = Array2D<T, Size, Size>;					//Creates a 4 by 4 grid





    //Template Class used for creating a two dimensional array of type std::vector 
																	/* 
																	*  Usage example : Grid2D<char> Grid_Char(8, 8)
																	*  creates a 2 dimensional Grid, containing char type values
																	*  with 8 rows and 8 columns
																	*/ 
	template<typename T>
	class Grid2D
	{
	private :
		int m_row							  {}; //Used for determining the number of rows in the grid
		int m_column						  {}; //Used for determining the number of rows in the grid
		std::vector<std::vector<T>> m_vector  {}; //Vector containing vectors of determined types, used to create the body of the grid

	//Creating a public access space for constructor and destructor... for aesthetics sake
	public  :
		//Constructor
		Grid2D(int row, int column) : m_row{row}, m_column{column}
		{
			resizeGrid();
		}
		//Overloaded constructor for when we want to create a uniform grid by calling the constructor with one argument
		Grid2D(int size) : m_row{ size }, m_column{size}
		{
			resizeGrid();
		}
		//Default Constructor
		Grid2D()
		{

		}
		//Destructor
		~Grid2D()
		{
			//Not sure If it needs one but I'm leaving for now
		}

	public :
		//Used for changing the Grid size -- I wanted to use .shrink_to_fit() as it removes the excess length but I think its too much work for too little benefit ngl. 
		//Should be called each time Grid data is changed. 
		void resizeGrid()                                  
		{
			m_vector.resize(m_row);
			for (int i{0}; i < m_row; i++)
			{
				m_vector[i].resize(m_column);
			}
		}
		//Overloaded functions for changing the row and column sizes of the grid.
		void setNewGridSize(int row, int column)
		{
			m_row	 = row;
			m_column = column;
		}
		void setNewGridSize(int size)
		{
			m_row    = size;
			m_column = size;
		}

		//Gets a specific slot in the grid
		T  getGridSlot(int row, int column)
		{
			return m_vector[row][column];
		}

		//Optional getters 
		int getRow() { return m_row; }
		int getColumn() {return m_column;}

		//Setter
		void setGridSlot(int row, int column, T value)
		{
			m_vector[row][column] = value;
		}

		//Returns the  2D vector
		std::vector<std::vector<T>> getGrid()
		{
			return m_vector;
		}

		int getGridSize()
		{
			return m_vector.size();
		}

	public  :
		//Used for populating the grid with variable of the chosen type
		void setGrid(T value)
		{
			resizeGrid();										//resizes the row to the chosen lenght, used in case the user chooses to change the grid size
			for (int i{0}; i < m_row; i++)						// Loops the number of times equal to rows 
			{													// each loop creates a vector of length equal to columns
				m_vector[i] = std::vector<T>(m_column);			// thenthe nested loop populates the vector with variables of the chosen type 
				for (int j{0}; j < m_column; j++)
				{
					m_vector[i][j] = value;
				}
			}
		}
		//Used for printing the values from the grid to the console
		void printGrid(bool printCoordinates) //bool variable to decide whether to print the grid with coordinate index (A1, B7 etc.)
		{
			system("CLS");											//Cleans the all of the text on the console
			char rowIndex{ 'A' };									//Used to index rows of the grid, this value is incremented after each row is printed
			if(printCoordinates)									//Runs only if id printCordinates is set to true
			{
				std::cout << "   ";									//printing space to better align column numbers with the grid 
				for (int i{1}; i <= m_column; i++)					//loop runs the amount of time equal to the number of columns in the grid
				{
					if(i < 10)										//If statement to  deduce whether the number of column is greater than 10
					{												//If it isn't, print 2 spaces, If it is, print one space
						std::cout << i << "  ";						//This is done purely to alignt the numbers to the grid
					}
					else { std::cout << i << " "; }
				}
				std::cout << '\n';									//finishing the line once all of the column numbers have been printed
			}
			for (int i{0}; i < m_row; i++)							//Loops for the amount of times equal to the numnber of rows 
			{
				if(printCoordinates)								//Runs only if printCoordinates is set to true and prints letters in alphabetical
				{													//At the start of each row 
					std::cout << rowIndex << "  ";				
					rowIndex++;
				}
				for (int j{ 0 }; j < m_vector[i].size(); j++)		//Iterates over every vector in the m_vector (vector containing vectors)
				{													//printing its contents by iterating over it with j
					std::cout << m_vector[i][j] << "  ";			//for the number of times equal to the sieze of the vector (row columns)
				}
				std::cout << '\n';
			}
		}
	};

//Namespace "Grid" ends here :
}