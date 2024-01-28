#pragma once
//STL headers    :
#include <iostream>
#include <utility>            //For std::pair
#include <map>                //For std::map
#include <stdlib.h>           //For system("CLS")
#include <algorithm>
#include <thread>             //For sleep_for() functionality 
#include <chrono>

//Custom headers :
#include "Grid.h"             //For Grid::Grid2D 
#include "Random.h"           //For Random::get() to place mines at random locations
#include "InputValidation.h"  //For handling validation of user input.


namespace GameData
{
    //Namespace "GameData" starts here :

        //simple function that pauses the code's execution for specified 'counter' amount of seconds, not ideal but good enough for this program
    void timer(int counter)
    {
        //using the std::chrono_literals to not have an aneurysm from typing it over and over
        using namespace std::chrono_literals;

        //Setting up variable of 1 second and multiplying it with a specified time
        const auto second{ 1s };
        auto duration{ second * counter };

        const auto start = std::chrono::high_resolution_clock::now();           //Storing current time
        std::this_thread::sleep_for(duration);                                  //Pausing the Code's execution for specified time
        const auto end = std::chrono::high_resolution_clock::now();             //Storing the time at which the timer ends

        const std::chrono::duration<double, std::milli> elapsed = end - start;  //Optionally calculating the elapsed time
        //Example usage -----> std::cout << "Waited " << elapsed << '\n';
    }


    //An enumerator used for indexing an array of symvbols to be displayed on the game's grid
    enum Symbols
    {
        covered_field,
        uncovered_field,
        mine_close_one,
        mine_close_two,
        mine_close_three,
        mine_close_four,
        mine_close_five,
        mine_close_six,
        mine_close_seven,
        mine_close_eight,
        mine,
        flag,
        max_symbols,
    };
    char symbol[max_symbols]{ '0', ' ', '1', '2', '3', '4', '5', '6', '7', '8', 'X', '!' };

    //Enumerator Used for indexinf an array holding the difficulty levels in game
    enum DifficultyLevels
    {
        easy,
        medium,
        hard,
        max_levels,
    };
    //The aray which stores the number of difficulties
    int difficultyLevels[max_levels]{ 1, 2, 3 };
    std::array<std::string_view, max_levels> currentDiffArr{ "Easy", "Medium", "Hard" };

    std::string_view currentDifficulty{ currentDiffArr[easy] };

    //Using type aliases for ease of use
    using Location = std::pair<int, int>;                   //Location of a single mine on the grid represented by two integers                       
    using Coordinates = std::vector<Location>;              //Vector of type std::pair containing locations of multiple mines
    using GameGrid = Grid::Grid2D<char>;                    //Nested vector type from the Grid.h header, creates a grid of positions in a vector 
    //using the char variable type

//Namespace "GameData" ends here  :
}



class Minesweeper
{
private:
    /* --- Deprecated, setting the number of mines and grid size will be done through the choice of difficulty level
    int m_gridSize{ 8 };
    int m_mineAmount{ 10 };
    */

    GameData::Coordinates mineCoordinates{};    //A vector type array used to hold the location of all the mines present in game
    GameData::Coordinates flagCoordinates{};
    GameData::Coordinates uncoveredCoordinates{};

    int minesCount{ 10 };                               //Variable holding the number of mines on the grid
    int flagsLeft{ 10 };                                //Variable holding the amount of lfags players have left

    bool firstGame{ true };                             //Set to true if the game didn't start yet. If set to true, clicking on a spot will instead
                                                        //Place mines on the grid, in all the spots except the clicked on slot and surroundings


    GameData::GameGrid gameGrid;                        //Build the object of class Grid2D to serve as the game's minefield

public:
    Minesweeper()
    {
        gameGrid.setNewGridSize(8);
    }
    ~Minesweeper()
    {
    }
private: //private area with functions used to setup the funcitonality of the game 
    void setDifficulty(GameData::DifficultyLevels newLevel)
    {
        switch (newLevel)
        {
        case GameData::DifficultyLevels::easy:
        {
            gameGrid.setNewGridSize(8);    //Set the number of rows and columns (8x8 in this case)
            buildGameGrid();               //Build the grid
            minesCount = 10;
            flagsLeft = minesCount;        //And finally place 10 mines at random positions
            GameData::currentDifficulty = GameData::currentDiffArr[GameData::easy];
        }
        break;
        case GameData::DifficultyLevels::medium:
        {
            gameGrid.setNewGridSize(16);
            buildGameGrid();
            minesCount = 40;
            flagsLeft = minesCount;
            GameData::currentDifficulty = GameData::currentDiffArr[GameData::medium];
        }
        break;
        case GameData::DifficultyLevels::hard:
        {
            gameGrid.setNewGridSize(16, 30);
            buildGameGrid();
            minesCount = 99;
            flagsLeft = minesCount;
            GameData::currentDifficulty = GameData::currentDiffArr[GameData::hard];
        }
        break;
        default: std::cout << "If this message is printed, this is rather bad as it is not supposed to' \n";
            break;
        }
    }

    void buildGameGrid()
    {
        gameGrid.setGrid(GameData::symbol[GameData::Symbols::covered_field]);
    }

    void placeMines(int minesCount, const GameData::Location& slot) //Places a mine at a random position minesCount times
    {
        clearGameData();

        bool initialised{ false };

        for (int i{ 0 }; i < minesCount; i++)
        {
            //Creates a variable of the Location type to store random coordinates of a mine
            GameData::Location newMineLocation{ Random::get(0, (gameGrid.getRow() - 1)), Random::get(0, (gameGrid.getColumn() - 1)) };

            //Adds the first mine location to the mineCoordinates vector
            if (!initialised)
            {
                mineCoordinates.push_back(newMineLocation);
            }

            //Loops over the mineCoordinates vector and check whether the newly chosen location on the grid already contains a mine or not 
            for (auto minelocation : mineCoordinates)
            {
                //Runs a loop to check whether the new location is already added to the vector
                while (true)
                {
                    //If the location of the new map is already taken or the location is the initially chosen slot,
                    //keep generating new ones until one that isn't take is generated

                    //First check whether there is a mine located around the location player clicked on 
                    bool isMineNear{ false };

                    for (auto x : getNeighbours(slot.first, slot.second))
                    {
                        //If there is a mine around the location
                        //, set isMineNear to true
                        if (newMineLocation == x)
                        {
                            isMineNear = true;
                        }
                    }

                    /* heck whether the newly chosen spot for a mine is,
                    * 1 - in a place mine alreadyt ecists
                    * 2 - in a place of the location player clicked on
                    * 3 - around the location the player clicked on
                    *
                    * If any are true, generate a new mine location instead
                    */
                    if ((minelocation == newMineLocation) || (newMineLocation == slot) || isMineNear)
                    {
                        for (auto x : getNeighbours(slot.first, slot.second))
                        {

                        }
                        GameData::Location tempLocation{ Random::get(0, (gameGrid.getRow() - 1)), Random::get(0, (gameGrid.getColumn() - 1)) };
                        newMineLocation = tempLocation;
                    }
                    else //If the location is not taken, break out of the loop
                    {
                        break;
                    }
                }
            }
            //Pushes the newly created location onto the mineCoordinates vector 
            if (initialised)
            {
                mineCoordinates.push_back(newMineLocation);
            }
            initialised = true;
        }
    }

    void clearGameData()
    {
        flagCoordinates.clear();
        mineCoordinates.clear();
        uncoveredCoordinates.clear();
    }

private: //private area with functions used to play the game 
    GameData::Location selectCoordinates()
    {
        //Get the letter of the row and the number of the column from the user
        char rowLetter{ InputValidation::getInpuRowLetter("Enter the row letter : ", gameGrid.getRow()) };
        std::cout << '\n';
        //Convert the letter to an integer value
        //'A' is 65 in ASCI, so in order to convert the input letter into row number, I can just subtract 65
        int rowIndex{ rowLetter - 65 };
        int colIndex{ InputValidation::getInpuColumnNumber("Enter the column number : ", gameGrid.getColumn()) };
        std::cout << '\n';

        //Storing the input row and column as the location on the grid 
        GameData::Location selectedLocation{ rowIndex, (colIndex - 1) };

        return selectedLocation;
    }

    void choseSlotOption(const GameData::Location& slot)
    {
        //Ask user to provide a text input
        std::string userInput{ InputValidation::getInput("Choose what to do with the slot :") };

        //Create a map of viable respones
        std::map<std::string, int> mp;
        mp["ignore"] = 1;
        mp["click"] = 2;
        mp["flag"] = 3;
        mp["quit"] = 4;

        //Compare player's input with viable responses
        switch (mp[userInput])
        {
        case 1: return;
            break;
        case 2:
        {
            if (firstGame)
            {
                placeMines(minesCount, slot);
            }
            firstGame = false;
            action_click(slot);
        }
        break;
        case 3:
        {
            if (firstGame)
            {
                placeMines(minesCount, slot);
            }
            firstGame = false;
            action_flag(slot);
        }
        break;
        case 4: quit();
            break;
            //If the user's input does not match any of the viable responses, 
            //Inform them and go back to choosing the slot
        default: system("CLS");  std::cout << "incorrect option chosen, try again\n"; GameData::timer(1);
            break;
        }
    }

    //checks what happens when the user 'clicks' on a certain slot
    void action_click(const GameData::Location& slot)
    {
        for (auto mineLocation : mineCoordinates)    //loops over the locations of all the mines on the grid
        {                                           //If there is a match, that means the player stepped on the mine
            if (slot == mineLocation)                //thus it sends the player to the game finished screen on a fail state chosen with (false)
            {
                gameFinished(false);
            }
        }                                           //Otherwise reveals the chosen slot
        reveal(slot.first, slot.second);
    }

    //checks what happens when the user ''flags' a certain slot
    void action_flag(const GameData::Location& slot)
    {
        if (!flagCoordinates.empty())                //If there is at least one flag on the grid
        {                                           //loop over the flag locations
            for (auto x : flagCoordinates)
            {
                if (x == slot)                       //If the chosen location contains a flag  :
                {
                    //change the location to covered spot
                    gameGrid.setGridSlot(slot.first, slot.second, GameData::symbol[GameData::Symbols::covered_field]);
                    //find the place on the flagCoordinates where the flag that was placed on this spot is located 
                    auto i{ std::find(flagCoordinates.begin(), flagCoordinates.end(), slot) };
                    if (i != flagCoordinates.end())
                    {
                        //remove the flag from the array at the place it was
                        flagCoordinates.erase(i);
                        flagsLeft++;
                    }
                    return;
                }   //If the chpsen location does not contain a flag, there are flags left, and the chosen slot is a covered field
                else if ((gameGrid.getGridSlot(slot.first, slot.second) == GameData::symbol[GameData::Symbols::covered_field]) && (flagsLeft != 0))
                {
                    //Add the location to the flag locations array,
                    flagCoordinates.push_back(slot);
                    flagsLeft--;
                    gameGrid.setGridSlot(slot.first, slot.second, GameData::symbol[GameData::Symbols::flag]);
                    return;
                }
                //If player chose a location that does not meet the above requirements, inform them about it
                else { system("CLS"); std::cout << "cannot flag this position"; GameData::timer(2); return; }
            }

        }
        else if ((gameGrid.getGridSlot(slot.first, slot.second) == GameData::symbol[GameData::Symbols::covered_field]) && (flagsLeft != 0))
        {
            flagCoordinates.push_back(slot);
            flagsLeft--;
            gameGrid.setGridSlot(slot.first, slot.second, GameData::symbol[GameData::Symbols::flag]);
            return;
        }
        else {
            system("CLS"); std::cout << "cannot flag this position"; GameData::timer(2); return;
        }
    }

    //Function used to get locations around the slot
    //used when placing mines for the first time 
    GameData::Coordinates getNeighbours(int row, int column)
    {
        GameData::Coordinates   neighboursToPass{};

        constexpr Grid::Array2D<int, 8, 2> neighbours =
        { { { 1, 1 }, { -1, -1 }, { 0, 1 }, { 0, -1 },
                { 1, 0 }, { -1, 0 }, { -1, 1 }, { 1, -1 }} };


        for (int i{ 0 }; i < 8; i++)
        {
            int x = row + neighbours[i][0];
            int y = column + neighbours[i][1];
            if (x >= 0 && y >= 0 && x < gameGrid.getRow() && y < gameGrid.getColumn())
            {
                GameData::Location      neighbour{ x, y };
                neighboursToPass.push_back(neighbour);
            }
        }
        return neighboursToPass;
    }

    //used to reveal the slots the player clicked on 
    void reveal(int row, int column)
    {
        //Array containing 8 spots on the grid to check 
        constexpr Grid::Array2D<int, 8, 2> neighbours =
        { { { 1, 1 }, { -1, -1 }, { 0, 1 }, { 0, -1 },
                { 1, 0 }, { -1, 0 }, { -1, 1 }, { 1, -1 }} };

        //Counter of mines in the nearby locations
        int minesNearby{ 0 };

        //multiply the row and column values of the entered slot by the values in the array above
        for (int i{ 0 }; i < 8; i++)
        {
            int x = row + neighbours[i][0];
            int y = column + neighbours[i][1];
            //If the locations around the slot are valid check if they contain mines
            if (x >= 0 && y >= 0 && x < gameGrid.getRow() && y < gameGrid.getColumn())
            {
                for (auto mines : mineCoordinates)
                {
                    if ((mines.first == x) && (mines.second == y))
                    {
                        //if the neabry location contains a mine, increment mine counter
                        minesNearby++;
                    }
                }
            }
        }

        //If there is at least one mine nearby
        if (minesNearby > 0)
        {
            //set the slot to show the number of mines
            setMinesNearby(row, column, minesNearby);
            return;
        }

        //if there is no mine around, set the slot to empty
        gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::uncovered_field]);

        //Then grab the locations of the surrounding locations, and perform the same check for all of them
        for (int i{ 0 }; i < 8; i++)
        {
            int x = row + neighbours[i][0];
            int y = column + neighbours[i][1];
            if (x >= 0 && y >= 0 && x < gameGrid.getRow() && y < gameGrid.getColumn())
            {
                if (gameGrid.getGridSlot(x, y) == GameData::symbol[GameData::Symbols::covered_field])
                {
                    reveal(x, y);
                }
            }
        }
    }

    //used to set the slot to show the number of mines around it
    void setMinesNearby(int row, int column, int minesNearby)
    {
        //sets the numbers of mines around the desired slot
        switch (minesNearby)
        {
        case 1: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_one]);
            break;
        case 2: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_two]);
            break;
        case 3: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_three]);
            break;
        case 4: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_four]);
            break;
        case 5: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_five]);
            break;
        case 6: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_six]);
            break;
        case 7: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_seven]);
            break;
        case 8: gameGrid.setGridSlot(row, column, GameData::symbol[GameData::Symbols::mine_close_eight]);
            break;
        default:
            break;
        }
        return;
    }


    //Endgame Screen, shows winning or loosing state depending on the state of 'won' variable
    void gameFinished(const bool won)
    {
        system("CLS");
        if (won)
        {
            std::cout << "Congratulations, you won the game!\n";
        }
        else
        {
            //Show the player the places that contained mines
            for (auto x : mineCoordinates)
            {
                gameGrid.setGridSlot(x.first, x.second, GameData::symbol[GameData::Symbols::mine]);
            }
            gameGrid.printGrid(true);
            std::cout << '\n';
            //Then print a message informing them that they lost the game
            std::cout << "You stepped on the mine!\n" << "Better luck next time! \n";
        }
        char answer{ InputValidation::getInpuChar("Play again? 'Y/N' : ") };
        if (answer == 'Y')
        {
            menu();
        }
        else if (answer == 'N') { quit(); }
    }

public:
    //Function containing a loop that forms the core of the game
    void playGame()
    {
        firstGame = true;
        buildGameGrid();
        gameGrid.printGrid(true);
        while (true)
        {
            gameGrid.printGrid(true);
            printHelp();
            checkGameState();
            choseSlotOption(selectCoordinates());
        }
    }

    //used to exit the program
    void quit()
    {
        exit(0);
    }

    void checkGameState()
    {
        //Runs to see whether the flags are positioned in the same place as mines
        int overlapingPositions{};

        if (flagsLeft == 0)
        {
            for (auto x : flagCoordinates)
            {
                auto match{ std::find(mineCoordinates.begin(), mineCoordinates.end(), x) };
                if (match != mineCoordinates.end())
                {
                    overlapingPositions++;
                }
            }
        }
        if (overlapingPositions == minesCount)
        {
            gameFinished(true);
        }
    }

    void menu()
    {
        while (true)
        {
            system("CLS");
            std::cout << "Welcome to minesweeper. \n" << '\n' << "Type to begin :\n";
            std::cout << "Change difficulty ---->    Easy Mode   | 8x8 Grid, 10 mines    - 'easy'\n";
            std::cout << "---------------------->    Medium Mode | 16x16 Grid, 40 mines  - 'mid'\n";
            std::cout << "---------------------->    Hard Mode   | 30x16 Grid, 99 mines  - 'hard'\n" << '\n';
            std::cout << '\n';
            std::cout << "Currently chosen difficulty : " << GameData::currentDifficulty << '\n';
            std::cout << '\n';
            std::cout << "Start Game ----------->    'start'\n";
            std::cout << "Quit Game ------------>    'quit'\n";


            std::string userInput{ InputValidation::getInput("Please enter the name for the option you wish to select :") };

            std::map<std::string, int> mp;
            mp["easy"] = 1;
            mp["mid"] = 2;
            mp["hard"] = 3;
            mp["start"] = 4;
            mp["quit"] = 5;


            switch (mp[userInput])
            {
            case 1: setDifficulty(GameData::DifficultyLevels::easy);
                break;
            case 2: setDifficulty(GameData::DifficultyLevels::medium);
                break;
            case 3: setDifficulty(GameData::DifficultyLevels::hard);
                break;
            case 4: playGame();
                break;
            case 5: quit();
                break;
            default: system("CLS");  std::cout << "incorrect option chosen, try again\n"; GameData::timer(1);
                break;
            }
        }
    }

    void printHelp()
    {
        std::cout << '\n';
        std::cout << "Flags Left : " << flagsLeft << '\n';
        std::cout << '\n';
        std::cout << "-----------------------------------------'\n";
        std::cout << "Rules: Select the slot, then choose action. Flag all of the mine spots to win!'\n";
        std::cout << "-----------------------------------------'\n";
        std::cout << '\n';
        std::cout << "ignore ------------>    'leave the chosen spot alone'\n";
        std::cout << "click  ------------>    'click on the spot'\n";
        std::cout << "flag   ------------>    'flag the spot as a potential mine'\n";
        std::cout << "quit   ------------>    'quit the game'\n";
        std::cout << '\n';
    }
};