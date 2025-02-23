/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Programmer: Matt Kaufman
// Date: 2/22/2022
// Name: main.cpp
// Description: WWordle game (Milestones 1-6)
//              Implements file loading, vector content storing and printing, secret word functionality (explicit or random),
//              word guessing, guess-history and feedback printing, logic for checking guesses and when the game has ended,
//              with printed instructions on how to play, as well as a 'possible letters remaining' feature
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include "Wordle.h"


int main() {
    cout << endl;
    cout << endl << ">>   Milestones 1 and 2 Unit Tests   <<";
    cout << endl << "---------------------------------------" << endl << endl;

    // Constructor tests
    Wordle test1;
    Wordle test2("wordList.txt");
    cout << ">Expected output of test1.getFilename() is empty string (default constructor). Actual output is: " << test1.getFilename() << endl;
    cout << ">Expected output of test2.getFilename() is wordList.txt. Actual output is: " << test2.getFilename() << endl << endl;


    // Filename setter and getter tests
    test1.setFilename("test.txt");
    cout << ">Expected output of test1.setFilename(\"test.txt\") is test.txt. Actual output is: " << test1.getFilename() << endl;
    test1.setFilename("moo");
    cout << ">Expected output of test1.setFilename(\"moo\") is moo. Actual output is: " << test1.getFilename() << endl << endl;


    // loadWordList method tests
    cout << ">Expected output of test1.loadWordList() is an error message. Actual output is: ";
    test1.loadWordList();
    cout << ">Expected output of test2.loadWordList() is nothing (should succeed). Actual output is: " << endl << endl;
    test2.loadWordList();


    // printWordList method tests
    cout << ">Expected output of test1.printWordList() is a warning message. Actual output is: ";
    test1.printWordList();
    cout << ">Expected output of test2.printWordList() is the full list of valid words. Actual output is: " << endl;
    //test2.printWordList();


    // setSecretWord method tests
    cout << endl << ">Expected output of test2.setSecretWord(\"cow\") is an error message. Actual output is: ";
    test2.setSecretWord("cow");
    cout << ">Expected output of test2.setSecretWord(\"cigar\") is nothing (should succeed). Actual output is: " << endl;
    test2.setSecretWord("cigar");


    // getSecretWord method test
    cout << ">Expected output of test2.getSecretWord() is cigar. Actual output is: " << test2.getSecretWord() << endl;


    // setRandomSecretWord method test
    test2.setRandomSecretWord();
    cout << ">Expected output of test2.setRandomSecretWord is a random valid word. Actual output is: " << test2.getSecretWord() << endl;

    cout << endl << "---------------------------------------" << endl << endl << endl;





    cout << endl << ">>   Milestones 3 and 4 Unit Tests   <<";
    cout << endl << "---------------------------------------" << endl << endl;

    // getGuessNum method tests
    cout << ">Expected output of test2.getGuessNum() is 0. Actual output is: " << test2.getGuessNum() << endl;
    test2.makeGuess("cigar");   test2.makeGuess("clown");   test2.makeGuess("roses");
    cout << ">Expected output of test2.getGuessNum() (after 3 valid guesses) is 3. Actual output is: " << test2.getGuessNum() << endl << endl;


    // makeGuess method tests (testing the game over message last)
    test2.setSecretWord("dwarf");
    cout << ">Expected output of test2.makeGuess(\"oink\") is an error message (invalid guess). Actual output is: ";
    test2.makeGuess("oink");
    cout << ">Expected output of test2.makeGuess(\"hunch\") is nothing (valid guess, not a match, haven't reached max guesses). Actual output is: ";
    test2.makeGuess("hunch");
    cout << endl << ">Expected output of test2.makeGuess(\"dwarf\") is a game over correct guess message on a new line. Actual output is: ";
    test2.makeGuess("dwarf");


    // print method test (guesses that triggered an iterate so far: cigar, clown, roses, hunch, dwarf)
    cout << ">Expected output of test2.print() is the thus far guess history and feedback (5 each). Actual output is: " << endl;
    test2.print();


    // final makeGuess method test (testing "game over" message)
    cout << ">Expected output of test2.makeGuess(\"pluto\") is a game over losing message on a new line. Actual output is: ";
    test2.makeGuess("pluto");

    cout << "---------------------------------------" << endl << endl << endl;





    cout << endl << ">>   Milestones 5 and 6 Unit Tests   <<";
    cout << endl << "---------------------------------------" << endl << endl;

    Wordle test3("wordList.txt");   //
    test3.loadWordList();                   //
    test3.setSecretWord("chirp");     // instantiate new class objects for next unit tests and set their secret words
    Wordle test4("wordList.txt");   //
    test4.loadWordList();                   //
    test4.setSecretWord("house");     //


    // makeGuess method tests (testing new changes to feedback mechanism)
    cout << R"(>Expected feedback for makeGuess("nasty") is  "....." . Actual output is:  )";  test3.makeGuess("nasty");
    test3.printLastFeedback();
    cout << R"(>Expected feedback for makeGuess("birds") is  ".++.." . Actual output is:  )";  test3.makeGuess("birds");
    test3.printLastFeedback();
    cout << R"(>Expected feedback for makeGuess("paper") is  "+.+.+" . Actual output is:  )";  test3.makeGuess("paper");
    test3.printLastFeedback();
    cout << R"(>Expected feedback for makeGuess("chips") is  "!!!+." . Actual output is:  )";  test3.makeGuess("chips");
    test3.printLastFeedback();
    cout << ">Expected output for all feedback vector contents is the previous four strings. Actual output is:  ";
    test3.printAllFeedback();


    // printAvailableLetters method tests
    cout << endl << endl;
    cout << ">Expected output of printAvailableLetters() is the entire alphabet. Actual output is:";  // test4 object initialized with all true elements
    test4.printAvailableLetters();

    test4.makeGuess("truck");
    cout << ">Expected output of printAvailableLetters() is the alphabet minus c, k, r, and t. Actual output is:";
    test4.printAvailableLetters();

    test4.makeGuess("abeam");
    cout << ">Expected output of printAvailableLetters() is the alphabet minus a, b, c, k, m, r, and t. Actual output is:";
    test4.printAvailableLetters();

    test4.makeGuess("defog");
    cout << ">Expected output of printAvailableLetters() is the alphabet minus a, b, c, d, f, g, k, m, r, and t. Actual output is:";
    test4.printAvailableLetters();

    test4.setAvailableLetters(false);
    cout << ">Expected output of printAvailableLetters() is a no letters remaining message (set all array elements to false with setAvailableLetters method). Actual output is:";
    test4.printAvailableLetters();

    cout << endl << "---------------------------------------" << endl << endl << endl;





    // print a list of instructions for playing the game
    cout << "+ ========================= WORDLE ========================= +" << endl;
    cout << "|                                                            |" << endl;
    cout << "|        You have 6 attempts to guess the secret word        |" << endl;
    cout << "|          Each guess must be a valid 5-letter word          |" << endl;
    cout << "|       Press enter to submit a guess after your input       |" << endl;
    cout << "|                                                            |" << endl;
    cout << "|   After each guess, you will get the following feedback:   |" << endl;
    cout << "|    !  is the correct letter in the correct position        |" << endl;
    cout << "|    +  is the correct letter but in the wrong position      |" << endl;
    cout << "|    .  is a letter that doesn't appear in the word at all   |" << endl;
    cout << "|                                                            |" << endl;
    cout << "+ ========================================================== +" << endl << endl;


    Wordle wordle("wordList.txt");  // instantiate wordle class object with filename
    wordle.loadWordList();                  // populate the wordList vector
    wordle.setSecretWord("clown");    // set the secret word
    //wordle.setRandomSecretWord();

    cout << ">> Secret word: " << wordle.getSecretWord();
    cout << endl << endl;

    bool tmp;                               // temp variable
    string guess;                           // string to store user input
    int count = 1;                          // iteration counter

    while (count < 7) {                     // automatically terminates after 6 guesses (player lost)
        cout << endl;
        cout << ">Enter guess " << count << ": ";
        cin >> guess;                       // read in user's guess

        tmp = wordle.makeGuess(guess);      // call the makeGuess method and store return value in tmp (bool)
        count = wordle.getGuessNum() + 1;   // update counting variable with getGuessNum method
        wordle.print();                     // call print method to display guesses/feedback

        if (tmp) {                          // check if last makeGuess method call returned true
            count = 7;                      // break out of the while loop if it did (player won)
        }
        else {
            wordle.printAvailableLetters(); // print available letters remaining if the player hasn't won yet
            cout << endl;
        }
    }

    cout << endl << endl;
    return 0;
}
