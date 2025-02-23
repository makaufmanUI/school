//////////////////////////////////////////////////////
//
// Programmer: Matt Kaufman
// Date: 2/22/2022
// Name: Wordle.h
// Description: Declares the class and all its methods
//
//////////////////////////////////////////////////////

#ifndef HW3_WORDLE_H
#define HW3_WORDLE_H

#include <string>
#include <vector>
using namespace std;


class Wordle {
public:
    explicit Wordle(string filename = "");      // constructor (with default value "")

    string getFilename() const;                 // filename getter
    void setFilename(string filename);          // filename setter

    void loadWordList();                        // wordList vector element loader
    void printWordList() const;                 // wordList vector element printer

    string getSecretWord() const;               // secretWord getter
    void setSecretWord(const string& word);     // secretWord setter
    void setRandomSecretWord();                 // secretWord randomizer

    int getGuessNum() const;                    // guessNum getter
    bool makeGuess(const string& guessWord);    // makeGuess method
    void print() const;                         // print method

    string GenFeedback(string guessWord) const; // feedback provision handler
    void setAvailableLetters(bool truth);       // sets all elements in availableLetter array to true or false (called by constructor)
    void setAvailableLetters(const string& feedbackStr, const string& guessWord, bool truth);    // sets specific elements in availableLetter array depending on feedback string

    void printAvailableLetters();               // method to print the list of available letters for the player

    void printLastFeedback() const;             // method to print last element in the feedback vector (only used in unit tests)
    void printAllFeedback() const;              // method to print all elements in the feedback vector (only used in unit tests)

private:
    string wordListFilename;                    // txt file name
    vector<string> wordList;                    // word-holding vector

    string secretWord;                          // secret word

    int guessNum = 0;                           // number of guesses
    const int MAX_GUESS_NUM = 6;                // maximum number of guesses

    vector<string> guess;                       // list of previous guesses
    vector<string> feedback;                    // list of feedback strings

    bool availableLetter[26];                   // array to 'hold' all available letters that may be in secretWord
};


#endif //HW3_WORDLE_H
