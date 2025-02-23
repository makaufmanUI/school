///////////////////////////////////////////////////////////////////////////////////////////
//
//  Programmer: Matt Kaufman
//  Date: 1/29/2022
//  Name: hw1.cpp
//  Description: Opens a CSV file containing stats for several college basketball teams,
//               parses the CSV file to log and calculate stats for desired team,
//               then reports these stats to the console
//
///////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;


// Function declarations/prototypes
void pruneData(vector<string> allTeamData, float allTeamData_pruned[], int arrayLength);
int getPostSeasonAppearances(vector<string> allTeamData, int activeSeasons);
float getTotalGamesWon(float allTeamData_pruned[], int arrayLength);
float getTotalGamesPlayed(float allTeamData_pruned[], int arrayLength);
void getHighestWinPercentage(float allTeamData_pruned[], int arrayLength, float highestWinPercentage[], int arrayLength2, int activeSeasons);



int main()
{
    const string TEAM = "Iowa St.";
    string tmp;
    string line;
    vector<string> allTeamData;     // stores all Iowa State data
    float allTeamData_pruned[21];   // stores only the program-relevant data from allTeamData
    float highestWinPercentage[2];  // stores highest win percentage for whatever season, and the year it occurred

    int activeSeasons = 0;
    float totalGamesWon;
    float totalGamesPlayed;
    float totalWinPercentage;
    int postSeasonAppearances;


    // Open the CSV file; output error message and quit program if failed
    ifstream inputFile("cbb.csv");
    cout << "Opening file: cbb.csv." << endl;
    if (!inputFile.is_open()) {
        cout << "Failed to open cbb.csv." << endl;
        return -1;
    }


    // Loop through the entire CSV file and append all Iowa State data to the allTeamData vector
    while (!inputFile.eof())
    {
        getline(inputFile, line);
        stringstream ss(line);

        if (line.substr(0, 8) == TEAM)      // check if first 8 characters of each row matches the team name
        {
            activeSeasons += 1;             // if so, iterate activeSeasons
            while (getline(ss, tmp, ','))
            {
                allTeamData.push_back(tmp); // push that row's data into a vector
            }
        }
    }


    // Populates the allTeamData_pruned array, holding only the useful data contained in the allTeamData vector
    pruneData(allTeamData, allTeamData_pruned, 21);


    cout << endl << endl << "---------------------------- SUMMARY ----------------------------" << endl;

    cout << endl << "TEAM OF INTEREST: " << TEAM << endl;

    cout << "Number of seasons Iowa State made an appearance: " << activeSeasons << endl;

    postSeasonAppearances = getPostSeasonAppearances(allTeamData, activeSeasons);
    cout << "Number of seasons where they made the postseason: " << postSeasonAppearances << endl;

    totalGamesWon = getTotalGamesWon(allTeamData_pruned, 21);
    cout << "Number of games won across all seasons: " << totalGamesWon << endl;

    totalGamesPlayed = getTotalGamesPlayed(allTeamData_pruned, 21);
    cout << "Number of games played across all seasons: " << totalGamesPlayed << endl;

    totalWinPercentage = totalGamesWon / totalGamesPlayed * 100;
    cout << "Total win percentage across all seasons: " << totalWinPercentage << "%" << endl;

    getHighestWinPercentage(allTeamData_pruned, 21, highestWinPercentage, 2, activeSeasons);
    cout << "Highest single-season win percentage: " << highestWinPercentage[0] << "%, ";
    cout << "occurring in " << highestWinPercentage[1] << endl;

    cout << endl << "-----------------------------------------------------------------" << endl << endl << endl;


    cout << "Closing file: cbb.csv." << endl;
    inputFile.close();


    return 0;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description: Filters all nonrelevant data from the allTeamData vector and stores it
//               in the allTeamData_pruned array (also converts strings to floats with stof() function)
//
//  Inputs: 1. allTeamData, type vector<string>;  holds all Iowa State data from the CSV file
//          2. allTeamData_pruned, type array (float);  empty when passed to the function
//          3. arrayLength, type int;  required so the function knows the size of allTeamData_pruned
//
//  Output: None (type void);  still allows the use of allTeamData_pruned elements in main() since
//          a pointer to the array is passed to the function instead of a copy of the array
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void pruneData(vector<string> allTeamData, float allTeamData_pruned[], int arrayLength)
{
    int j = 0;
    int yearStartIndex = 23;        // 24th column in CSV file corresponds to 23rd index in allTeamData vector
    int gamesWonStartIndex = 3;     // 4th column in CSV file corresponds to 3rd index in allTeamData vector
    int gamesPlayedStartIndex = 2;  // 3rd column in CSV file corresponds to 2nd index in allTeamData vector

    // Puts into the form: YEAR, GAMES PLAYED, GAMES WON; YEAR, GAMES PLAYED, GAMES WON; ......
    for (int i = 0; i < arrayLength; i = i+3) {
        allTeamData_pruned[i] = stof(allTeamData.at(yearStartIndex + (24 * j)));
        allTeamData_pruned[i+1] = stof(allTeamData.at(gamesPlayedStartIndex + (24 * j)));
        allTeamData_pruned[i+2] = stof(allTeamData.at(gamesWonStartIndex + (24 * j)));
        j += 1;
    }
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description: Starts by assuming the team made it to the postseason (postSeasonAppearances = activeSeasons),
//               then loops through all contents of the allTeamData vector, checking each index pos. corresponding
//               to the postseason column of the CSV file along the way, decrementing postSeasonAppearances if "NA"
//
//  Inputs: 1. allTeamData, type vector<string>;  holds all Iowa State data from the CSV file
//          2. activeSeasons, type int;  number of times Iowa State appeared in the CSV file
//
//  Output: postSeasonAppearances, type int;  number of times Iowa State made it to the postseason
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int getPostSeasonAppearances(vector<string> allTeamData, int activeSeasons)
{
    bool firstRow = false;	// used to ensure index position 21 is only counted once
    int postSeasonAppearances = activeSeasons;

    // Check for any "NA"s in the vector at index pos. 21 or any integer multiple of 24 thereafter
    for (int i = 0; i < activeSeasons; i++) {
        if ( (allTeamData.at(21) == "NA" && !firstRow)  ||  allTeamData.at(21 + (24 * i)) == "NA" ) {
            postSeasonAppearances -= 1;
            firstRow = true;
        }
    }

    return postSeasonAppearances;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description: Iterates through the allTeamData_pruned array and accumulates each 'games won' value
//
//  Inputs: 1. allTeamData_pruned, type array (float);  holds the Iowa State data relevant to the program
//          2. arrayLength, type int;  required so the function knows the size of allTeamData_pruned
//
//  Output: totalGamesWon, type float;  total number of games won across all seasons
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
float getTotalGamesWon(float allTeamData_pruned[], int arrayLength)
{
    float totalGamesWon = 0.0;

    // Accumulate the 'games won' values
    for (int i = 2; i < arrayLength; i = i+3) {
        totalGamesWon += allTeamData_pruned[i];
    }

    return totalGamesWon;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description: Iterates through the allTeamData_pruned array and accumulates each 'games played' value
//
//  Inputs: 1. allTeamData_pruned, type array (float);  holds the Iowa State data relevant to the program
//          2. arrayLength, type int;  required so the function knows the size of allTeamData_pruned
//
//  Output: totalGamesPlayed, type float;  total number of games played across all seasons
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
float getTotalGamesPlayed(float allTeamData_pruned[], int arrayLength)
{
    float totalGamesPlayed = 0.0;

    // Accumulate the 'games played' values
    for (int i = 1; i < arrayLength; i = i+3) {
        totalGamesPlayed += allTeamData_pruned[i];
    }

    return totalGamesPlayed;
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Description: Gets the highest win percentage out of all seasons and the year in which it occurred;
//               creates a new array seasonalWinPercentages that stores the win percentage for each season,
//               uses the max_element() function from <algorithm> to find the maximum value in that array,
//               then finds the index pos. in seasonalWinPercentages corresponding to that maximum value,
//               calculates the index pos. corresponding to that season in allTeamData_pruned array,
//               then populates the highestWinPercentage array with the maximum win percentage and year
//
//  Inputs: 1. allTeamData_pruned, type array (float);  holds the Iowa State data relevant to the program
//          2. arrayLength, type int;  required so the function knows the size of allTeamData_pruned
//          3. highestWinPercentage, type array (float); empty when passed to the function
//          4. arrayLength2, type int;  required so the function knows the size of highestWinPercentage
//          5. activeSeasons, type int;  number of times Iowa State appeared in the CSV file
//
//  Output: None (type void); use the updated highestWinPercentage array elements in main() after calling
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getHighestWinPercentage(float allTeamData_pruned[], int arrayLength, float highestWinPercentage[], int arrayLength2, int activeSeasons)
{
    int j = 1;      // iteration variable for games played
    int k = 2;      // iteration variable for games won
    int maxIndex;   // index in seasonalWinPercentages array where maximum occurs
    int correspondingIndex;
    float seasonalWinPercentages[activeSeasons];

    // Calculate the win percentage for each season and store
    for (int i = 0; i < activeSeasons; i++) {
        seasonalWinPercentages[i] = allTeamData_pruned[k] / allTeamData_pruned[j] * 100;
        j += 3;
        k += 3;
    }

    // Find the highest number in the new seasonalWinPercentages array
    float* max;
    max = max_element(seasonalWinPercentages, seasonalWinPercentages + activeSeasons);

    // Find the index where this highest win percentage occurred
    for (int i = 0; i < activeSeasons; i++) {
        if (seasonalWinPercentages[i] == *max) {
            maxIndex = i;
        }
    }

    // Convert this maximum index value to the corresponding index in allTeamData_pruned (the corresponding year)
    correspondingIndex = maxIndex * 3;

    // Assign this maximum seasonal win percentage and year it occurred to highestWinPercentage array
    highestWinPercentage[0] = *max;
    highestWinPercentage[1] = allTeamData_pruned[correspondingIndex];
}