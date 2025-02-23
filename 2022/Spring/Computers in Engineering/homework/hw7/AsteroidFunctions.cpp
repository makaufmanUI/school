/****************************************************************************
*                                                                           *
*        Matt Kaufman      4/12/2022                                        *
*                                                                           *
*        AsteroidClasses.cpp                                                *
*                                                                           *
*        Defines the functions declared in AsteroidFunctions.h              *
*                                                                           *
*****************************************************************************/

#include <SFML/Graphics.hpp>
#include "AsteroidClasses.h"
#include "AsteroidFunctions.h"
using namespace sf;

const int H = 800;          // Height of window
const int W = 1200;         // Width of window




/*********************************************************************
*                                                                    *
*   Function for reading the all-time-high score from file           *
*     Input: None                                                    *
*     Outputs: the player's highest score across agames (type int)   *
*                                                                    *
**********************************************************************/
int ReadHighScore() {
    std::ifstream file("ath_score.txt");
    if (!file) return EXIT_FAILURE;
    int high_score;
    file >> high_score;
    file.close();
    return high_score;
}



/***************************************************************
*                                                              *
*   Function for writing a new all-time-high score to file     *
*     Input: the score to be written to the file (type int),   *
*     Outputs: None                                            *
*                                                              *
****************************************************************/
void WriteHighScore(int high_score) {
    std::ofstream file("ath_score.txt");
    if (!file) return;
    file << high_score;
    file.close();
}



/*************************************************************************************
*                                                                                    *
*   Function for drawing the player's current score to the screen                    *
*     Inputs: references to the window (type RenderWindow),                          *
*             the text object to use (type Text), the current score (type int),      *
*             and the font object to use for the text (type Font)                    *
*     Outputs: None                                                                  *
*                                                                                    *
**************************************************************************************/
void drawScore(RenderWindow& app, Text& scoreText, int& score, Font& font) {
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(12.5f,3.7f);
    scoreText.setString("SCORE: " + std::to_string(score));
    app.draw(scoreText);
}



/*************************************************************************************
*                                                                                    *
*   Function for drawing the player's current lives to the screen                    *
*     Inputs: references to the window (type RenderWindow),                          *
*             the text object to use (type Text), the number of lives (type int),    *
*             and the font object to use for the text (type Font)                    *
*     Outputs: None                                                                  *
*                                                                                    *
**************************************************************************************/
void drawLives(RenderWindow& app, Text& livesText, int& lives, Font& font) {
    livesText.setFont(font);
    livesText.setCharacterSize(30);
    livesText.setFillColor(Color::White);
    livesText.setPosition(11.5f,30.7f);
    livesText.setString("LIVES: " + std::to_string(lives));
    app.draw(livesText);
}



/*************************************************************************************
*                                                                                    *
*   Function for drawing the player's current level to the screen                    *
*     Inputs: references to the window (type RenderWindow),                          *
*             the text object to use (type Text), the current level (type int),      *
*             and the font object to use for the text (type Font)                    *
*     Outputs: None                                                                  *
*                                                                                    *
**************************************************************************************/
void drawLevel(RenderWindow& app, Text& levelText, int& level, Font& font) {
    levelText.setFont(font);
    levelText.setCharacterSize(30);
    levelText.setFillColor(Color::White);
    levelText.setPosition(1080.5f,3.7f);
    levelText.setString("LEVEL: " + std::to_string(level));
    app.draw(levelText);
}



/*****************************************************************************
*                                                                            *
*   Function for checking if two entities are colliding                      *
*     Inputs: pointers to two Entity objects, a and b                        *
*     Outputs: true if their circular bounding regions overlap, else false   *
*                                                                            *
******************************************************************************/
bool isCollide(Entity* a, Entity* b)
{
    return (b->x - a->x)*(b->x - a->x)+
           (b->y - a->y)*(b->y - a->y)<
           (a->R + b->R)*(a->R + b->R);
}



/************************************************************************
*                                                                       *
*   Function for calculating the angle between two entities             *
*   (for UFO -> player shooting)                                        *
*     Inputs: pointers to two Entity objects, p and u                   *
*     Outputs: angle in degrees, in the range [-pi, +pi] (type double)  *
*                                                                       *
*************************************************************************/
double theta(Entity* p, Entity* u)
{
    float dx = p->x - u->x;
    float dy = p->y - u->y;
    return std::atan2(dy, dx)*57.296;
}



/**********************************************************************************
*                                                                                 *
*   Function for loading the game's texture files                                 *
*     Inputs: references to each texture object (type sf::Texture) to be loaded   *
*     Outputs: EXIT_SUCCESS by default, EXIT_FAILURE if any file failed to load   *
*                                                                                 *
***********************************************************************************/
int loadTextures(Texture& t1,Texture& t2,Texture& t3,Texture& t4,Texture& t5,Texture& t6,Texture& t7,Texture& t8)
{
    if (!t1.loadFromFile("images/spaceship.png"))  return EXIT_FAILURE;
    if (!t2.loadFromFile("images/background.jpg")) return EXIT_FAILURE;
    if (!t3.loadFromFile("images/explosions/type_C.png")) return EXIT_FAILURE;
    if (!t4.loadFromFile("images/rock.png")) return EXIT_FAILURE;
    if (!t5.loadFromFile("images/fire_blue.png"))  return EXIT_FAILURE;
    if (!t6.loadFromFile("images/rock_small.png")) return EXIT_FAILURE;
    if (!t7.loadFromFile("images/explosions/type_B.png")) return EXIT_FAILURE;
    if (!t8.loadFromFile("images/ufosheet.png")) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}



/**************************************************************************************************
*                                                                                                 *
*   Function for loading the game's sound files & music file                                      *
*     Inputs: references to each sound (type sf::Sound) and buffer (type sf::SoundBuffer) pair,   *
*             and a reference to the music stream object (type sf::Music)                         *
*     Outputs: EXIT_SUCCESS by default, EXIT_FAILURE if any file failed to load                   *
*                                                                                                 *
***************************************************************************************************/
int loadSounds(SoundBuffer& ufoSpawn,SoundBuffer& explosionDeath,SoundBuffer& ufoKill,Sound& ufoSpawnSound,
               Sound& explosionDeathSound,Sound& ufoKillSound,Music& ufoAmbience,SoundBuffer& ufoExplosion,Sound& ufoExplosionSound)
{
    if (!ufoSpawn.loadFromFile("sounds/ufo_spawn.ogg")) return EXIT_FAILURE;
    else {
        ufoSpawnSound.setBuffer(ufoSpawn);
        ufoSpawnSound.setVolume(50);
    }
    if (!ufoAmbience.openFromFile("sounds/ufo_ambience.ogg")) return EXIT_FAILURE;
    else {
        ufoAmbience.setLoop(true);
        ufoAmbience.setVolume(10);
    }
    if (!explosionDeath.loadFromFile("sounds/player_death.ogg")) return EXIT_FAILURE;
    else {
        explosionDeathSound.setBuffer(explosionDeath);
        explosionDeathSound.setVolume(75);
    }
    if (!ufoKill.loadFromFile("sounds/applause2.ogg")) return EXIT_FAILURE;
    else {
        ufoKillSound.setBuffer(ufoKill);
        ufoKillSound.setVolume(25);
    }
    if (!ufoExplosion.loadFromFile("sounds/ufo_explosion.ogg")) return EXIT_FAILURE;
    else {
        ufoExplosionSound.setBuffer(ufoExplosion);
        ufoExplosionSound.setVolume(50);
    }
    return EXIT_SUCCESS;
}



/*********************************************************************************
*                                                                                *
*   Function for spawning a UFO when needed                                      *
*     Inputs: references to the Entity-pointer list (type std::list<Entity*>),   *
*             the UFO's Animation object (type Animation),                       *
*             the UFO's spawn sound (type sf::Sound),                            *
*             and the UFO's ambient sound (type sf::Music)                       *
*     Outputs: None                                                              *
*                                                                                *
**********************************************************************************/
void spawnUFO(std::list<Entity*>& entities, Animation& anim, Sound& spawnSound, Music& ambience)
{
    UFO* u = new UFO();                                   // Create a new UFO
    u->settings(anim,W,H,rand()%360,20);                  // Set its position, velocity, angle, and size
    entities.push_back(u);                                // Add it to the entities list
    ambience.play();                                      // Start playing the ambient sound
    spawnSound.play();                                    // Play the spawn sound
}



/*********************************************************************************
*                                                                                *
*   Function for spawning sets of asteroids when needed                          *
*     Inputs: references to the Entity-pointer list (type std::list<Entity*>),   *
*             the asteroid's Animation object (type Animation),                  *
*             and the number of asteroids to be spawned (type int)               *
*     Outputs: None                                                              *
*                                                                                *
**********************************************************************************/
void spawnAsteroids(std::list<Entity*>& entities, Animation& anim, int numToSpawn)
{
    for (int i=0; i<numToSpawn; i++) {
        auto* a = new asteroid();                             // Create a new asteroid
        a->settings(anim,rand()%W,rand()%H,rand()%360,25);    // Set its position, velocity, angle, and size
        entities.push_back(a);                                // Add it to the entities list
    }
}



/********************************************************************
*                                                                   *
*   Function for drawing and displaying the main menu               *
*     Inputs: references to the window (type RenderWindow),         *
*            the font for text (type Font),                         *
*            the three Text objects to be used (type Text),         *
*            and the current menu loop count (type int)             *
*     Outputs: None                                                 *
*                                                                   *
*********************************************************************/
void showMainMenu(RenderWindow& app, Font& font, Text& titleText, Text& gameInfoText, Text& startText, int& loopCounter) {
    app.clear(Color::White);
    int startTextAlpha = loopCounter%85*3;

    FloatRect startBounds = startText.getLocalBounds();
    FloatRect titleBounds = titleText.getLocalBounds();
    FloatRect gameInfoBounds = gameInfoText.getLocalBounds();
    RectangleShape titleBox(Vector2f(titleBounds.width + 80, titleBounds.height + 20));
    RectangleShape gameInfoBox(Vector2f(gameInfoBounds.width + 24.5f, gameInfoBounds.height + 20));

    titleText.setFont(font);
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color::Black);
    titleText.setString("ASTEROIDS!");
    titleText.setPosition(Vector2f(600, 86.5f));
    titleText.setOrigin(titleBounds.left + titleBounds.width/2, titleBounds.top + titleBounds.height/2);

    startText.setFont(font);
    startText.setCharacterSize(35);
    startText.setString("Press Enter to start");
    startText.setPosition(Vector2f(600, 175));
    startText.setFillColor(Color(174,51,51,startTextAlpha));
    startText.setOrigin(startBounds.left + startBounds.width/2, startBounds.top + startBounds.height/2);

    gameInfoText.setFont(font);
    gameInfoText.setCharacterSize(25);
    gameInfoText.setFillColor(Color::Black);
    gameInfoText.setPosition(Vector2f(600, 504));
    gameInfoText.setString(" Don't get hit by the asteroids!  More asteroids spawn per level.\n\n      Each asteroid kill gives +1 point, get as far as you can!\n\n\n\n\n\nControls:\n\n"
                           "   - Press spacebar to shoot\n\n   - Press up arrow to move forward\n\n   - Press down arrow to move in reverse\n\n"
                           "   - Press left and right arrows to rotate your ship\n\n   - Press left shift to engage the emergency brake/drift\n\n\n\n"
                           "Every so often, a UFO will spawn that becomes aggressive...\n\nIf you are within range, it will shoot at you. Stay away from it!\n\nKilling the UFO grants you +20 points, but it's a pretty good shot\n\n          and only gets better with each level you reach!");
    gameInfoText.setOrigin(gameInfoBounds.left + gameInfoBounds.width/2, gameInfoBounds.top + gameInfoBounds.height/2);

    titleBox.setOutlineColor(Color::Black);
    titleBox.setOutlineThickness(5);
    titleBox.setFillColor(Color::Transparent);
    titleBox.setPosition(Vector2f(565, 89.5f));
    titleBox.setOrigin(titleBounds.left + titleBounds.width/2, titleBounds.top + titleBounds.height/2);

    gameInfoBox.setOutlineColor(Color::Black);
    gameInfoBox.setOutlineThickness(3);
    gameInfoBox.setFillColor(Color::Transparent);
    gameInfoBox.setPosition(Vector2f(590, 500));
    gameInfoBox.setOrigin(gameInfoBounds.left + gameInfoBounds.width/2, gameInfoBounds.top + gameInfoBounds.height/2);

    app.draw(titleBox);
    app.draw(titleText);
    app.draw(startText);
    app.draw(gameInfoBox);
    app.draw(gameInfoText);

    app.display();
}



/*****************************************************************************
*                                                                            *
*   Function for drawing and displaying the end menu                         *
*     Inputs: references to the window (type RenderWindow),                  *
*            the font for text (type Font),                                  *
*            the five Text objects to be used (type Text),                   *
*            the final score the player achieved this game (type int),       *
*            and the current menu loop count (type int),                     *
*            the game time at the end of the game (type int),                *
*            the number of bullets fired this game (type int),               *
*            and the distance traveled by the player this game (type int)    *
*     Outputs: None                                                          *
*                                                                            *
******************************************************************************/
void showEndMenu(RenderWindow& app,Font& font,Text& titleText,Text& gameInfoText,Text& finalScoreText,Text& athScoreText,Text& restartText,int& finalScore,int& loopCounter,int& gameTime,int& bulletCount,int& distance,int& level) {
    app.clear(Color::White);
    int restartTextAlpha = loopCounter%85*3;

    FloatRect athScoreBounds = athScoreText.getLocalBounds();
    FloatRect gameInfoBounds = gameInfoText.getLocalBounds();
    FloatRect restartTextBounds = restartText.getLocalBounds();
    FloatRect finalScoreBounds = finalScoreText.getLocalBounds();
    RectangleShape finalScoreBox(Vector2f(finalScoreBounds.width + 20, finalScoreBounds.height + 20));

    restartText.setFont(font);
    restartText.setCharacterSize(33);
    restartText.setString("Press R to restart or ESC to quit");
    restartText.setPosition(Vector2f(600, 235));
    restartText.setFillColor(Color(174,51,51,restartTextAlpha));
    restartText.setOrigin(restartTextBounds.left + restartTextBounds.width/2, restartTextBounds.top + restartTextBounds.height/2);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(50);
    finalScoreText.setFillColor(Color::Black);
    finalScoreText.setPosition(Vector2f(600, 100));
    finalScoreText.setString("FINAL SCORE:  " + std::to_string(finalScore));
    finalScoreText.setOrigin(finalScoreBounds.left + finalScoreBounds.width/2, finalScoreBounds.top + finalScoreBounds.height/2);

    athScoreText.setFont(font);
    athScoreText.setCharacterSize(35);
    athScoreText.setFillColor(Color::Black);
    athScoreText.setPosition(Vector2f(600, 165.5f));
    athScoreText.setString("ALL TIME BEST SCORE:  " + std::to_string(ReadHighScore()));
    athScoreText.setOrigin(athScoreBounds.left + athScoreBounds.width/2, athScoreBounds.top + athScoreBounds.height/2);

    gameInfoText.setFont(font);
    gameInfoText.setCharacterSize(32);
    gameInfoText.setFillColor(Color::Black);
    gameInfoText.setPosition(Vector2f(600, 475));
    gameInfoText.setString("Some info/statistics about your game:\n\n\n"
                           "- You got to level " + std::to_string(level) + ".\n\n"
                           "- The game lasted " + std::to_string(gameTime) + " seconds.\n\n"
                           "- You fired a total of " + std::to_string(bulletCount) + " bullets.\n\n"
                           "- You traveled a total distance of " + std::to_string(distance) + " pixels.\n\n"
                           "- Your average speed was " + std::to_string(distance/gameTime/12) + "% of max speed.");
    gameInfoText.setOrigin(gameInfoBounds.left + gameInfoBounds.width/2, gameInfoBounds.top + gameInfoBounds.height/2);

    finalScoreBox.setOutlineColor(Color::Black);
    finalScoreBox.setOutlineThickness(5);
    finalScoreBox.setFillColor(Color::Transparent);
    finalScoreBox.setPosition(Vector2f(595, 103));
    finalScoreBox.setOrigin(finalScoreBounds.left + finalScoreBounds.width/2, finalScoreBounds.top + finalScoreBounds.height/2);

    app.draw(restartText);
    app.draw(gameInfoText);
    app.draw(athScoreText);
    app.draw(finalScoreBox);
    app.draw(finalScoreText);

    app.display();
}