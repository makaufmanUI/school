/**************************************************************************************************
*                                                                                                 *
*        Matt Kaufman      3/30/2022                                                              *
*                                                                                                 *
*        DoodleFunctions.cpp                                                                      *
*                                                                                                 *
*        Defines the functions, as well as the Point class, declared in DoodleFunctions.h         *
*                                                                                                 *
***************************************************************************************************/

#include <fstream>
#include "DoodleFunctions.h"
using namespace sf;




// Point class definitions
Point::Point() : x(0), y(0) { }
unsigned int Point::GetX() const { return x; }
unsigned int Point::GetY() const { return y; }
void Point::SetX(unsigned int new_x) { x = new_x; }
void Point::SetY(unsigned int new_y) { y = new_y; }



// Random number generator; Engine and Rand used in conjunction
//   Inputs: min and max values for random number (type unsigned int)
//   Outputs: random number between min and max (inclusive, type unsigned int)
std::default_random_engine& Engine() {
    static std::default_random_engine Rand(std::random_device{ }());
    return Rand;
}
unsigned int Rand(unsigned int min, unsigned int max) {
    std::uniform_int_distribution<unsigned int> dist(min, max);
    return dist(Engine());
}



// Function for reading the all-time-high score from file
//   Inputs: none
//   Outputs: the player's highest score across sessions (type int)
int ReadHighScore() {
    std::ifstream file
            ("ath_score.txt");
    if (!file)
        return EXIT_FAILURE;
    int high_score;
    file >> high_score;
    file.close();
    return high_score;
}



// Function for writing a new all-time-high score to file
//   Inputs: the player's new highest score (type int)
//   Outputs: none
void WriteHighScore(int high_score) {
    std::ofstream file
            ("ath_score.txt");
    if (!file) return;
    file << high_score;
    file.close();
}



// Function for loading the background music file
//   Inputs: a reference to the Music object (type sf::Music)
//   Outputs: none (EXIT_FAILURE if file not found)
int LoadMusic(Music& bgMusic) {
    // background.ogg music file  - "Game level music" by Mixkit, https://mixkit.co/,  License: Free commercial and non-commercial use
    if (!bgMusic.openFromFile("sounds/background.ogg"))
        return EXIT_FAILURE;
    else {
        bgMusic.setLoop(true);       // set background music to loop
        bgMusic.setVolume(15.f);   // reduce volume of background music
    }
}



// Function for loading the game's font files
//   Inputs: references to the font objects (type sf::Font)
//   Outputs: none
int LoadFonts(Font& font1, Font& font2) {
    // ConcertOne-Regular.ttf font file    - From https://fonts.google.com/specimen/Concert+One,  License: OFL, included in fonts folder as per license requirement
    // RobotoMono-BoldItalic.ttf font file - From https://fonts.google.com/specimen/Roboto+Mono,  License: Apache 2.0, copy of license included in fonts folder as per license requirement
    if (!font1.loadFromFile("fonts/ConcertOne-Regular.ttf")) return EXIT_FAILURE;
    if (!font2.loadFromFile("fonts/RobotoMono-Bold.ttf")) return EXIT_FAILURE;
    else return 0;
}



// Function for loading the game's texture files
//   Inputs: references to the texture objects (type sf::Texture)
//   Outputs: none
int LoadTextures(Texture& t1, Texture& t2, Texture& t3, Texture& t4) {
    if (!t1.loadFromFile("images/background.png")) return EXIT_FAILURE;
    if (!t2.loadFromFile("images/platform.png")) return EXIT_FAILURE;
    if (!t3.loadFromFile("images/doodle.png")) return EXIT_FAILURE;
    if (!t4.loadFromFile("images/platform_bad.png")) return EXIT_FAILURE;
    else return 0;
}



// Function for loading the game's sound files and setting buffers to the sound objects
//   Inputs: references to the sound buffer objects (type sf::SoundBuffer), and the sound objects (type sf::Sound)
//   Outputs: none
int LoadSounds(SoundBuffer& jump, SoundBuffer& fall, Sound& jumpSound, Sound& fallSound) {
    // jump.ogg sound file  - "Quick jump arcade game" by Mixkit, https://mixkit.co/,  License: Free commercial and non-commercial use
    // fall.ogg sound file  - "Player losing or failing" by Mixkit, https://mixkit.co/,  License: Free commercial and non-commercial use

    if (!jump.loadFromFile("sounds/jump.ogg")) return EXIT_FAILURE;     // load jump sound
    else jumpSound.setBuffer(jump);                                             // assign jump buffer to jumpSound sound
    if (!fall.loadFromFile("sounds/fall.ogg")) return EXIT_FAILURE;     // load fall sound
    else {
        fallSound.setBuffer(fall);        // assign fall buffer to fallSound sound
        fallSound.setVolume(25);   // reduce volume of fallSound
        return 0;
    }
}



// Function for the player's jump ability
//   Inputs: references to dy (type float), the player's total jump count (type int),
//           the current cooldown for the jump ability (type int), and the current state of the jump ability cooldown (type bool)
//   Outputs: none
void Jump(float& dy, int& jumpCount, int& jumpCooldownTimer, bool& jumpOffCooldown) {
    dy += -7;           // jump
    jumpCount += 1;     // increment jump count
    jumpCooldownTimer = 10;   // reset jump cooldown timer
    jumpOffCooldown = false;  // set jumpOffCooldown to false
}



// Function for scrolling the platforms currently present on screen (for progression mechanism)
//   Inputs: pointers to the platform objects (type Platform), and references to the difficulty multiplier (type int), and current score (type int)
//   Outputs: none
void ScrollPlatforms(Point platform[20], Point bad_platform[4], int& difficultyMultiplier, int& score) {
    for (int i = 0; i < 9; i++) {
        platform[i].SetY(platform[i].GetY() + difficultyMultiplier);      // shifts platforms down
        if (platform[i].GetY() > 533) {                       //
            score += 1;                                       // ***** SCORE COUNTER *****
            platform[i].SetY(0);                        //
            platform[i].SetX(Rand(0, 400));    // add new platform from the top
        }
    }
    for (int i = 0; i < 3; i++) {
        bad_platform[i].SetY(bad_platform[i].GetY() + difficultyMultiplier);  // same as above but for bad platforms

        if (bad_platform[i].GetY() > 533) {
            score += 1;
            bad_platform[i].SetY(0);
            bad_platform[i].SetX(Rand(0, 400));
        }
    }
}



// Function for moving/adjusting the platform positions and adding new ones when they reach the bottom of the screen
//   Inputs: pointers to the platform objects (type Point), and references to the player's x and y positions (type int), dy (type float), and the player's current score (type int)
//   Outputs: none
void MoveAndAddPlatforms(Point platform[20], Point bad_platform[4], int& y, int& h, float& dy, int& score) {
    for (int i = 0; i < 9; i++) {
        y = h;
        platform[i].SetY(platform[i].GetY() - dy);      // move platform in y

        if (platform[i].GetY() > 533) {                   // if platform reaches the bottom of the screen
            score += 1;                                        // increment score
            platform[i].SetY(0);                        // reset platform position in y
            platform[i].SetX(Rand(0, 400));   // add new platform from the top
        }
    }
    for (int i = 0; i < 3; i++) {
        bad_platform[i].SetY(bad_platform[i].GetY() - dy);  // same as above but for bad platforms

        if (bad_platform[i].GetY() > 533) {
            score += 1;
            bad_platform[i].SetY(0);
            bad_platform[i].SetX(Rand(0, 400));
        }
    }
}



// Function for checking if the player has collided with a platform
//   Inputs: pointers to the platform objects (type Point), and references to the jump sound object (type sf::Sound),
//           the player's x and y positions (type int), dy (type float), and the player's current score (type int)
//   Outputs: none
void CheckForPlatformCollisions(Point platform[20], Point bad_platform[4], Sound& jumpSound, int& x, int& y, float& dy, int& score) {
    for (int i = 0; i < 9; i++) {
        if ( (x+50 > platform[i].GetX())  &&  (x+20 < platform[i].GetX()+68)  &&  (y+70 > platform[i].GetY())  &&  (y+70 < platform[i].GetY()+14)  &&  (dy > 0) ) {
            dy = -10;
            jumpSound.play();
        }
    }
    for (int i = 0; i < 3; i++) {
        if ( (x+50 > bad_platform[i].GetX())  &&  (x+20 < bad_platform[i].GetX()+68)  &&  (y+70 > bad_platform[i].GetY())  &&  (y+70 < bad_platform[i].GetY()+14)  &&  (dy > 0) ) {
            bad_platform[i].SetX(600);  // make the bad platform disappear when player lands on it (move it off screen)
            score += 5;    // reward the player if they actively seek the risk of hitting bad platforms
        }
    }
}



// Function for setting player and platform sprite positions as well as drawing them to the window
//   Inputs: references to the window object (type sf::RenderWindow), player, background, and platform sprites (type sf::Sprite),
//           and the player's x and y positions (type int), as well as pointers to the platform objects (type Point)
//   Outputs: none
void SetAndDrawSprites(RenderWindow& app, Point platform[20], Point bad_platform[4], Sprite& sPers, Sprite& sBackground, Sprite& sPlat, Sprite& sPlat_bad, int& x, int& y) {
    sPers.setPosition(x, y);    // character position
    app.draw(sBackground);      // draw background
    app.draw(sPers);            // draw character

    for (int i = 0; i < 9; i++) {
        sPlat.setPosition(platform[i].GetX(), platform[i].GetY());  // set platform position
        app.draw(sPlat);                                            // draw platform
    }
    for (int i = 0; i < 3; i++) {
        sPlat_bad.setPosition(bad_platform[i].GetX(), bad_platform[i].GetY());
        app.draw(sPlat_bad);
    }
}



// Function for resetting the game; called on game start and when the player loses; resets all relevant game variables to their initial values
//   Inputs: references to the player's score (type int), the player's x and y positions (type int), dy (type float), the player's current score (type int),
//           player jump cooldown status (type bool), the current jump cooldown timer (type int), and pointers to the platform objects (type Point)
//   Outputs: none
void ResetGame(int& start, int& x, int& y, int& h, float& dy, int& score, Point platform[20], Point bad_platform[4], Music& bgMusic, bool& jumpOffCooldown, int& jumpCooldownTimer) {
    y = 100;
    x = 100;
    h = 200;
    dy = 0;
    score = 0;
    jumpCooldownTimer = 0;
    jumpOffCooldown = true;

    for (int i = 0; i < 9; i++) {
        platform[i].SetX(Rand(0, 400));
        platform[i].SetY(Rand(0, 533));
    }
    for (int i = 0; i < 3; i++) {
        bad_platform[i].SetX(Rand(0, 400));
        bad_platform[i].SetY(Rand(0, 533));
    }

    bgMusic.play();
    start = 1;
}



// Function for updating the player's top five scores
//   Inputs: pointer to the player's top scores history (array of type int), and a reference to the player's last score (type int)
//   Outputs: none
void UpdateScoreHistory(int scores[5], int& finalScore) {
    for (int i = 0; i < 5; i++) {
        if (finalScore == scores[i]) return;    // leave the function if the player got the same score as a previous top 5 score, to prevent duplicate top scores
    }

    // if no array elements are -1 anymore, and the most recent final score is greater than any of the previous top scores
    if (scores[0] != -1 && scores[1] != -1 && scores[2] != -1 && scores[3] != -1 && scores[4] != -1 && (finalScore > scores[0] || finalScore > scores[1] || finalScore > scores[2] || finalScore > scores[3] || finalScore > scores[4])) {
        int lowestScoreIndex;
        int lowestScore = 100000;
        for (int i = 0; i < 5; i++) {
            if (scores[i] < lowestScore) lowestScoreIndex = i;      // find lowest score of the three and store its index position
        }
        scores[lowestScoreIndex] = -1;      // replace this lowest score with -1
    }

    for (int i = 0; i < 5; i++) {
        if (scores[i] == -1) {
            scores[i] = finalScore;         // replace first occurrence of -1 with the latest score
            std::sort(scores, scores + 5, std::greater<int>());     // sort the scores in descending order
            return;
        }
    }
}



// Function for setting the properties of the current score text, as well as drawing it to the window
//   Inputs: references to the window object (type sf::RenderWindow), the score text object (type sf::Text),
//           the player's current score (type int), and the font object (type sf::Font)
//   Outputs: none
void SetAndDrawScoreText(RenderWindow& app, Text& scoreText, int& score, Font& font) {
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(12.5f,3.7f);
    scoreText.setString("SCORE: " + std::to_string(score));
    app.draw(scoreText);
}



// Function for setting the properties of the jump cooldown text, as well as drawing it to the window
//   Inputs: references to the window object (type sf::RenderWindow), the cooldown and jump text objects (type sf::Text),
//           the player's current jump count (type int), the current cooldown timer on the jump ability (type float), and the font object (type sf::Font)
//   Outputs: none
void SetAndDrawJumpCooldownText(RenderWindow& app, Text& cdText, Text& jumpText, int& jumpCount, int& jumpCooldownTimer, Font& font) {
    jumpText.setFont(font);
    jumpText.setCharacterSize(30);
    jumpText.setFillColor(Color::Black);
    jumpText.setPosition(199.0f, 3.7f);
    jumpText.setString("JUMPS LEFT: " + std::to_string(5 - jumpCount));

    cdText.setFont(font);
    cdText.setCharacterSize(24);
    cdText.setFillColor(Color::Black);
    cdText.setString("COOLDOWN: " + std::to_string(jumpCooldownTimer));

    if (jumpCooldownTimer < 10)                                     //
        cdText.setPosition(252, 30.5f);                       //
    else if (jumpCooldownTimer < 20 && jumpCooldownTimer >= 10)     // helps keep text in the right spot
        cdText.setPosition(243, 30.5f);                       //
    else cdText.setPosition(240, 30.5f);                      //

    app.draw(cdText);
    app.draw(jumpText);
}



// Function for setting text properties for the game's main menu as well as rendering to the window
//   Inputs: references to the window object (type sf::RenderWindow), text objects (type sf::Text), font object (type sf::Font), and a loop counter (type int)
//   Outputs: none
void SetAndDrawMainMenuText(RenderWindow& app, Text& titleText, Text& descriptionTitleText, Text& descriptionText, Text& startText, Font& font, int& count) {
    FloatRect titleBounds = titleText.getLocalBounds();
    FloatRect startBounds = startText.getLocalBounds();
    FloatRect descBounds = descriptionText.getLocalBounds();
    FloatRect descTitleBounds = descriptionTitleText.getLocalBounds();
    RectangleShape titleBox(Vector2f(titleBounds.width + 80, titleBounds.height + 20));
    RectangleShape descBox(Vector2f(descBounds.width + 24.5f, descBounds.height + 20));

    app.clear(Color::White);
    int startTextAlpha = count%85*3;

    titleText.setFont(font);
    titleText.setCharacterSize(35);
    titleText.setFillColor(Color::Black);
    titleText.setString("DOODLE JUMP!");
    titleText.setPosition(Vector2f(200, 26.5f));
    titleText.setOrigin(titleBounds.left + titleBounds.width/2, titleBounds.top + titleBounds.height/2);

    startText.setFont(font);
    startText.setCharacterSize(30);
    startText.setString("Press Enter to start");
    startText.setPosition(Vector2f(200, 85));
    startText.setFillColor(Color(174,51,51,startTextAlpha));
    startText.setOrigin(startBounds.left + startBounds.width/2, startBounds.top + startBounds.height/2);

    descriptionTitleText.setFont(font);
    descriptionTitleText.setCharacterSize(30);
    descriptionTitleText.setFillColor(Color::Black);
    descriptionTitleText.setString("INSTRUCTIONS:");
    descriptionTitleText.setPosition(Vector2f(200, 150));
    descriptionTitleText.setOrigin(descTitleBounds.left + descTitleBounds.width/2, descTitleBounds.top + descTitleBounds.height/2);

    descriptionText.setFont(font);
    descriptionText.setCharacterSize(18);
    descriptionText.setFillColor(Color::Black);
    descriptionText.setPosition(Vector2f(198, 352));
    descriptionText.setString("Stay on platforms and get as high as you can\n\n\n\n\n"
                              "  Watch out for discolored platforms though,\n\n      they offer you no ability to jump!\n\n\n\n\n"
                              "     If you're up for the added challenge,\n\n        5 extra points will be awarded\n\n     for each one you hit, but be careful!\n\n\n\n\n"
                              "  You're also given 5 free jumps (Spacebar),\n\n    in case you get stuck or need a boost.");
    descriptionText.setOrigin(descBounds.left + descBounds.width/2, descBounds.top + descBounds.height/2);

    titleBox.setOutlineColor(Color::Black);
    titleBox.setOutlineThickness(5);
    titleBox.setFillColor(Color::Transparent);
    titleBox.setPosition(Vector2f(165, 29));
    titleBox.setOrigin(titleBounds.left + titleBounds.width/2, titleBounds.top + titleBounds.height/2);

    descBox.setOutlineColor(Color::Black);
    descBox.setOutlineThickness(3);
    descBox.setFillColor(Color::Transparent);
    descBox.setPosition(Vector2f(188, 343));
    descBox.setOrigin(descBounds.left + descBounds.width/2, descBounds.top + descBounds.height/2);

    app.draw(descBox);
    app.draw(titleBox);
    app.draw(titleText);
    app.draw(startText);
    app.draw(descriptionText);
    app.draw(descriptionTitleText);

    app.display();
}



// Function for setting text properties for the game's "restart" menu as well as rendering to the window
//   Inputs: references to the window object (type sf::RenderWindow), text objects (type sf::Text), font object (type sf::Font),
//           the player's score (type int), the player's all-time high score (type int), a loop counter (type int), and a pointer to the player's top five scores (array of type int)
//   Outputs: none
void SetAndDrawRestartMenuText(RenderWindow& app, Text& restartText, Text& finalScoreText, Text& athScoreText, Text& topScoresText, Text& scoreHistoryText, int& finalScore, int& athScore, int scoreHistory[5], Font& font, int& count) {
    FloatRect rtBounds = restartText.getLocalBounds();
    FloatRect asBounds = athScoreText.getLocalBounds();
    FloatRect tsBounds = topScoresText.getLocalBounds();
    FloatRect fsBounds = finalScoreText.getLocalBounds();
    FloatRect shBounds = scoreHistoryText.getLocalBounds();
    RectangleShape finalScoreBox(Vector2f(fsBounds.width + 20, fsBounds.height + 20));
    RectangleShape topScoresBox(Vector2f(tsBounds.width + 40, tsBounds.height + shBounds.height + 30));

    app.clear(Color::White);
    int restartTextAlpha = count%85*3;

    restartText.setFont(font);
    restartText.setCharacterSize(33);
    restartText.setString("Press R to restart");
    restartText.setPosition(Vector2f(200, 155));
    restartText.setFillColor(Color(174,51,51,restartTextAlpha));
    restartText.setOrigin(rtBounds.left + rtBounds.width/2, rtBounds.top + rtBounds.height/2);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(40);
    finalScoreText.setFillColor(Color::Black);
    finalScoreText.setPosition(Vector2f(200, 38));
    finalScoreText.setString("FINAL SCORE:  " + std::to_string(finalScore));
    finalScoreText.setOrigin(fsBounds.left + fsBounds.width/2, fsBounds.top + fsBounds.height/2);

    athScoreText.setFont(font);
    athScoreText.setCharacterSize(27);
    athScoreText.setFillColor(Color::Black);
    athScoreText.setPosition(Vector2f(201, 85.5f));
    athScoreText.setString("ALL TIME BEST SCORE:  " + std::to_string(athScore));
    athScoreText.setOrigin(asBounds.left + asBounds.width/2, asBounds.top + asBounds.height/2);

    topScoresText.setFont(font);
    topScoresText.setCharacterSize(30);
    topScoresText.setFillColor(Color::Black);
    topScoresText.setString("BEST FIVE SCORES:");
    topScoresText.setPosition(Vector2f(200, 235));
    topScoresText.setOrigin(tsBounds.left + tsBounds.width/2, tsBounds.top + tsBounds.height/2);

    scoreHistoryText.setFont(font);
    scoreHistoryText.setCharacterSize(30);
    scoreHistoryText.setFillColor(Color::Black);
    scoreHistoryText.setPosition(140, 280);

    int numScoresToDisplay = 0;
    for (int i = 0; i < 5; i++) {
        if (scoreHistory[i] != -1) numScoresToDisplay += 1;
    }

    if (numScoresToDisplay == 1) {
        scoreHistoryText.setString("1.  " + std::to_string(scoreHistory[0]));
    }
    if (numScoresToDisplay == 2) {
        scoreHistoryText.setString("1.  " + std::to_string(scoreHistory[0]) +
                                   "\n\n2.  " + std::to_string(scoreHistory[1]));
    }
    if (numScoresToDisplay == 3) {
        scoreHistoryText.setString("1.  " + std::to_string(scoreHistory[0]) +
                                   "\n\n2.  " + std::to_string(scoreHistory[1]) +
                                   "\n\n3.  " + std::to_string(scoreHistory[2]));
    }
    if (numScoresToDisplay == 4) {
        scoreHistoryText.setString("1.  " + std::to_string(scoreHistory[0]) +
                                   "\n\n2.  " + std::to_string(scoreHistory[1]) +
                                   "\n\n3.  " + std::to_string(scoreHistory[2]) +
                                   "\n\n4.  " + std::to_string(scoreHistory[3]));
    }
    if (numScoresToDisplay == 5) {
        scoreHistoryText.setString("1.  " + std::to_string(scoreHistory[0]) +
                                   "\n\n2.  " + std::to_string(scoreHistory[1]) +
                                   "\n\n3.  " + std::to_string(scoreHistory[2]) +
                                   "\n\n4.  " + std::to_string(scoreHistory[3]) +
                                   "\n\n5.  " + std::to_string(scoreHistory[4]));
    }

    finalScoreBox.setOutlineColor(Color::Black);
    finalScoreBox.setOutlineThickness(5);
    finalScoreBox.setFillColor(Color::Transparent);
    finalScoreBox.setPosition(Vector2f(195, 41));
    finalScoreBox.setOrigin(fsBounds.left + fsBounds.width/2, fsBounds.top + fsBounds.height/2);

    topScoresBox.setOutlineColor(Color::Black);
    topScoresBox.setOutlineThickness(4);
    topScoresBox.setFillColor(Color::Transparent);
    topScoresBox.setPosition(Vector2f(182, 280));
    topScoresBox.setOrigin(tsBounds.left + tsBounds.width/2, tsBounds.top + tsBounds.height/2);

    app.draw(restartText);
    app.draw(athScoreText);
    app.draw(topScoresBox);
    app.draw(topScoresText);
    app.draw(finalScoreBox);
    app.draw(finalScoreText);
    app.draw(scoreHistoryText);

    app.display();
}
