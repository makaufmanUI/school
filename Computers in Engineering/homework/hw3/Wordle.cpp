///////////////////////////////////////////////////////////////////////
//
// Programmer: Matt Kaufman
// Date: 2/22/2022
// Name: Wordle.cpp
// Description: Defines the class constructor and all the class methods
//              declared in the Wordle.h header file
//
///////////////////////////////////////////////////////////////////////

#include <ctime>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

#include "Wordle.h"



// Initiate the Wordle class constructor
//   Single input parameter of type string (name of text file), no return values
Wordle::Wordle(string filename) {              // take in argument for filename
    setFilename(move(filename));       // call the setter method to set the filename to the input argument
    setAvailableLetters(true);            // initialize availableLetter array
    srand( time(nullptr) );        // seed the random number for setRandomSecretWord method
}


// Define filename getter method
//   No input parameters, single return value of type string (current filename)
string Wordle::getFilename() const {           // make impossible for method to change any values
    return wordListFilename;
}


// Define filename setter method
//   Single input parameter (the filename you wish to set), no return values
void Wordle::setFilename(string filename) {
    wordListFilename = move(filename);         // set wordListFilename equal to the input argument
}


// Define the loadWordList method
//  Loads text file content into wordList vector
//   No input parameters, no return values
void Wordle::loadWordList() {
    ifstream txtFile(getFilename());
    if (!txtFile.is_open()) {                  // output error to console if can't open file
        cout << "Error: Word list filename has not been specified or was input incorrectly." << endl;
    }
    else {
        string line;
        int numWords = 0;
        while (getline(txtFile, line)) {
            numWords += 1;                     // count the number of words in the file
        }
        wordList.resize(numWords);             // resize the vector to make room for the words

        txtFile.clear();                       // clear the eof flag
        txtFile.seekg(0, ifstream::beg);       // return to beginning of wordList.txt

        int i = 0;
        while (getline(txtFile, line)) {
            wordList.at(i) = line;             // element by element, set equal to line in text file
            i ++;
        }
        txtFile.close();                       // close the file
    }
}


// Define printWordList method
//   No input parameters, no return values
void Wordle::printWordList() const {           // make impossible for method to change any values
    if (wordList.empty()) {                    // output error message if the vector is empty
        cout << "Warning: Word list is empty." << endl;
    }
    else {
        for (const auto & i : wordList) {
            cout << i << endl;                  // iterate through vector and print each element within
        }
    }
}


// Define secretWord getter method
//   No input parameters, single return value of type string (secretWord)
string Wordle::getSecretWord() const {         // make impossible for method to change any values
    return secretWord;
}


// Define the secretWord setter method
//   One input parameter of type string (the word you wish to set the secret word to), no return values
void Wordle::setSecretWord(const string& word) {
    if ( find(wordList.begin(), wordList.end(), word) == wordList.end() ) {
        cout << "! Error: The word entered is not valid." << endl;    // output error message if the word is not valid (not in wordList vector)
    }
    else {
        secretWord = word;                      // if word is valid (in wordList vector), set the secret word equal to the method argument
    }
}


// Define the setRandomSecretWord method
//   No input parameters, no return values
void Wordle::setRandomSecretWord() {
    int randomNum = rand() % (int) wordList.size();     // generate random number between 1 and total word count
    secretWord = wordList.at(randomNum);
}


// Define the getGuessNum getter method
//   No input parameters, single return value of type int (current number of guesses)
int Wordle::getGuessNum() const {               // prevent method's ability to change anything
    return guessNum;
}


// Define the makeGuess method
//   One input parameter of type string (the player's guess), one return value of type bool (match or no match)
bool Wordle::makeGuess(const string& guessWord) {
    if (guessWord == secretWord) {                      // if user's guess was the secret word:
        guess.push_back(guessWord);                     // append correct guess to the previous-guesses vector
        feedback.emplace_back(GenFeedback(guessWord));      // call GenFeedback method and add the return string to the feedback vector
        guessNum = guessNum + 1;                        // increment the number-of-guesses counter
        cout << endl << endl << endl;
        cout << ">>  GAME OVER! You won.  ~  ";         // output game over/correct guess message
        cout << "Your guess, '" << guessWord << "', was correct!";
        cout << "  <<" << endl << "    ---------------------------------------------------------";
        cout << endl << endl;
        return true;
    }
    else {
        if ( find(wordList.begin(), wordList.end(), guessWord) == wordList.end() ) {
            cout << "! Error: Invalid guess. ";         // output error message if the guess was not a valid word (not in wordList vector)
            cout << guessWord << " is not in word list.";
            cout << " Please enter a new guess." << endl;
            return false;                               // return false (can't be a match if it's an invalid word)
        }
        else {
            if (guessNum < MAX_GUESS_NUM) {             // if player hasn't maxed out their guesses:
                guess.push_back(guessWord);             // append the guess word to the previous-guesses vector
                feedback.emplace_back(GenFeedback(guessWord));  // call GenFeedback method and append the return string to the feedback vector
                guessNum = guessNum + 1;                // increment the number-of-guesses counter
                setAvailableLetters(feedback.back(), guessWord, false);   // call setAvailableLetters method to update availableLetter array elements

                if (guessNum == MAX_GUESS_NUM) {        // check if number of guesses = guess limit
                    cout << endl << endl << endl;       // output game over message
                    cout << ">>  GAME OVER! You lost.  ";
                    cout << "~  Max number of guesses reached. ";
                    cout << "The secret word was: '" << secretWord << "'  <<" << endl;
                    cout << "    ------------------------------------------------------------------------------------";
                    cout << endl << endl;
                    return false;                       // return false (game has ended) if number of guesses = guess limit
                }
            }
            return false;                               // return false (valid guess but still not a match)
        }
    }
}


// Define the print method
//   No input parameters, no return values
void Wordle::print() const {
    cout << "    Previous guesses:\t";
    for (int i = 0; i < guessNum; i++) {
        cout << guess.at(i) << " ";             // output all previous guesses
    }

    cout << endl << "    Matching feedback:\t ";
    for (int i = 0; i < guessNum; i++) {
        cout << feedback.at(i) << " ";          // output feedback corresponding to each previous guess
    }

    cout << endl << endl;
}


// Define GenFeedback method (handles the feedback logic & generates the feedback string)
//   One input parameter of type string (the player's guess), one return value (the feedback string)
string Wordle::GenFeedback(string guessWord) const {
    string feedbackString = ".....";                   // assume feedback string is all misses to start
    string secret = getSecretWord();                   // grab a copy of the secret word

    for (int i = 0; i < secret.length(); i++) {
        if (secret[i] == guessWord[i]) {                // if the ith element of secret word = the ith element of player's guess:
            feedbackString.replace(i, 1, "!");     // replace the ith element of the feedback string with "!"
            secret.replace(i, 1, "-");             // replace the ith element of the secret word with "-" to prevent another match
            guessWord.replace(i, 1, "*");          // replace the ith element of the player's guess with "*" to prevent unintended matches
        }
    }

    for (int i = 0; i < secret.length(); i++) {
        for (char j : secret) {
            if (guessWord[i] == j) {                         // if the ith index of player's guess = the jth index of the secret word:
                feedbackString.replace(i, 1, "+");      // replace the ith index of the feedback string with "+"
            }
        }
    }
    return feedbackString;
}


// Define first setAvailableLetters method (sets all array elements to input argument)
//   One input parameter of type bool (true/false), no return value
void Wordle::setAvailableLetters(bool truth) {
    for (bool & i : availableLetter) {      // for every letter in the alphabet:
        i = truth;                            // set the ith index of availableLetter array to the input argument
    }
}


// Define second setAvailableLetters method (sets only the elements corresponding to feedback string)
//   Three input parameters: feedback of type string, the guess word of type string, and the desired value of type bool (always passed in false)
//     No return value
void Wordle::setAvailableLetters(const string& feedbackStr, const string& guessWord, bool truth) {
    const string Alphabet = "abcdefghijklmnopqrstuvwxyz";
    char letters[6];                            // initialize character array 'letters' with a size of 6
    strcpy(letters, feedbackStr.c_str());       // copy the individual characters of the feedback string into the 'letters' array

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 26; j++) {                             // for every letter in the alphabet:
            if (letters[i] == '.' && guessWord[i] == Alphabet[j]) {  // if the ith element of 'letters' array is a "." AND the ith element of player's guess = the jth element of the alphabet:
                availableLetter[j] = truth;                            // set the jth index of the availableLetter array to the input argument (false)
            }
        }
    }
}


// Define printAvailableLetters method (used to show player what letters remain)
//   No input parameters, no return value
void Wordle::printAvailableLetters() {
    const string Alphabet = "abcdefghijklmnopqrstuvwxyz";
    int count = 0;                  // initialize counting variable to 0
    string letters;                 // initialize 'letters' string as an empty string
    for (int i = 0; i < 26; i++) {         // for every letter in the alphabet
        if (availableLetter[i]) {            // if the ith element of the availableLetters array is true:
            letters += Alphabet[i];            // append the ith element of the alphabet to the 'letters' string
            letters += "  ";
            count++;                           // increment the counting variable
        }
    }
    if (count == 0) {               // if count is 0, no letters remain (whole array is false -- only happens during unit testing)
        cout << "    (0) No available letters remain." << endl;
    }
    else {                          // if count is not 0, print to the console the available letters that remain
        cout << "    (" << count << ") Available letters remaining:  " << letters;
        cout << endl;
    }
}


// Define printLastFeedback method (used only for unit testing)
//   No input parameters, no return value
void Wordle::printLastFeedback() const {        // prevent method's ability to change anything
    cout << feedback.back() << endl;            // print the last feedback vector element to the console
}


// Define printAllFeedback method (used only for unit testing)
//   No input parameters, no return value
void Wordle::printAllFeedback() const {     // prevent method's ability to change anything
    for (const auto & i : feedback) {
        cout << i << " ";                   // print all feedback vector elements to the console
    }
}