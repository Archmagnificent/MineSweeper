#pragma once
#include <iostream>
#include <limits>


/*A header containing all the functions used to validate usee input for various inputs needed in the minesweeper game.
* 
* I should probably make a good Input validation header at some point, that instead of containing a lot of different functions, 
* swould just be a template class that allows the user to basically grab input for whatever situation they need. 
* Maybe it would hold several functions for standardised stuff people might want like choosing an operator, or a letter between 
* 2 different letters, a type, etc. etc.
* and then several custmisable functions, like : using an array as an argument of a function for it to check input against everything inside. 
* 
* Yeah, I should do that right after I'm done with this minesweeper, and maybe integrate it into this code. 
*/




namespace InputValidation
{
//Namespace "InputValidation" starts here :


    void ignoreLine()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    //Creating a template function to receive input of any type
	std::string getInput(std::string_view message) //Adding messgae as a parameter to inform the user of the kind of input they need to provide
	{
        while (true) // Loop until user enters a valid input
        {
            std::cout << message ;
            std::string x{};
            std::cin >> x;

            // Check for failed extraction
            if (!std::cin) // if the previous extraction failed
            {
                if (std::cin.eof()) // if the stream was closed
                {
                    exit(0); // shut down the program now
                }

                // let's handle the failure
                std::cin.clear(); // put us back in 'normal' operation mode
                ignoreLine();     // and remove the bad input

                std::cout << "Oops, that input is invalid.  Please try again.\n";
            }
            else
            {
                ignoreLine(); // remove any extraneous input
                return x;
            }
        }
	}

    char getInpuRowLetter(std::string_view message, int row)
    {
        while (true) // Loop until user enters a valid input
        {
            std::cout << message;
            char x{};
            std::cin >> x;

            // Check for failed extraction
            if (!std::cin) // if the previous extraction failed
            {
                if (std::cin.eof()) // if the stream was closed
                {
                    exit(0); // shut down the program now
                }

                // let's handle the failure
                std::cin.clear(); // put us back in 'normal' operation mode
                ignoreLine();     // and remove the bad input

                std::cout << "Oops, that input is invalid.  Please try again.\n";
            }
            else
            {
                ignoreLine(); // remove any extraneous input

                // Check whether the user entered a letter between A and a maximum number of rows
                if ((x >= 'A') && (x <= ('A' + (row - 1))))
                {
                    return x;
                }
                else { std::cout << "incorrect row index, please try again"; }
            }
        }
    }

    int getInpuColumnNumber(std::string_view message, int column)
    {
        while (true) // Loop until user enters a valid input
        {
            std::cout << message;
            int x{};
            std::cin >> x;

            // Check for failed extraction
            if (!std::cin) // if the previous extraction failed
            {
                if (std::cin.eof()) // if the stream was closed
                {
                    exit(0); // shut down the program now
                }

                // let's handle the failure
                std::cin.clear(); // put us back in 'normal' operation mode
                ignoreLine();     // and remove the bad input

                std::cout << "Oops, that input is invalid.  Please try again.\n";
            }
            else
            {
                ignoreLine(); // remove any extraneous input

                // Check whether the user entered a letter between A and a maximum number of rows
                if ((x >= 1) && (x <= column))
                {
                    return x;
                }
                else { std::cout << "incorrect column index, please try again"; }
            }
        }
    }

    char getInpuChar(std::string_view message)
    {
        while (true) // Loop until user enters a valid input
        {
            std::cout << message;
            char x{};
            std::cin >> x;

            // Check for failed extraction
            if (!std::cin) // if the previous extraction failed
            {
                if (std::cin.eof()) // if the stream was closed
                {
                    exit(0); // shut down the program now
                }

                // let's handle the failure
                std::cin.clear(); // put us back in 'normal' operation mode
                ignoreLine();     // and remove the bad input

                std::cout << "Oops, that input is invalid.  Please try again.\n";
            }
            else
            {
                ignoreLine(); // remove any extraneous input

                // Check whether the user entered a letter Y or N 
                if((x == 'Y') || (x == 'N'))
                {
                    return x;
                }
                else { std::cout << "incorrect column index, please try again"; }
            }
        }
    }
//Namespace "InputValidation" ends here   :
}