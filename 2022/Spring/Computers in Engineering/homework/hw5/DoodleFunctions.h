/********************************************************************************************
*                                                                                           *
*        Matt Kaufman      3/30/2022                                                        *
*                                                                                           *
*        DoodleFunctions.h                                                                  *
*                                                                                           *
*        Declaration of all functions, as well as the Point class, used in main.cpp         *
*                                                                                           *
*********************************************************************************************/

#ifndef DOODLEJUMP_DOODLEFUNCTIONS_H
#define DOODLEJUMP_DOODLEFUNCTIONS_H

#include <random>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;




class Point {
public:
    explicit Point();                   // explicit constructor
    unsigned int GetX() const;          // x variable getter method
    unsigned int GetY() const;          // y variable getter method
    void SetX(unsigned int new_x);      // x variable setter method
    void SetY(unsigned int new_y);      // y variable setter method
private:
    unsigned int x;
    unsigned int y;
};


std::default_random_engine& Engine();
unsigned int Rand(unsigned int min, unsigned int max);

int ReadHighScore();
void WriteHighScore(int high_score);

int LoadMusic(Music& bgMusic);
int LoadFonts(Font& font1, Font& font2);
int LoadTextures(Texture& t1, Texture& t2, Texture& t3, Texture& t4);
int LoadSounds(SoundBuffer& jump, SoundBuffer& fall, Sound& jumpSound, Sound& fallSound);

void Jump(float& dy, int& jumpCount, int& jumpCooldownTimer, bool& jumpOffCooldown);
void ScrollPlatforms(Point platform[20], Point bad_platform[4], int& difficultyMultiplier, int& score);
void MoveAndAddPlatforms(Point platform[20], Point bad_platform[4], int& y, int& h, float& dy, int& score);
void CheckForPlatformCollisions(Point platform[20], Point bad_platform[4], Sound& jumpSound, int& x, int& y, float& dy, int& score);
void SetAndDrawSprites(RenderWindow& app, Point platform[20], Point bad_platform[4], Sprite& sPers, Sprite& sBackground, Sprite& sPlat, Sprite& sPlat_bad, int& x, int& y);
void ResetGame(int& start, int& x, int& y, int& h, float& dy, int& score, Point platform[20], Point bad_platform[4], Music& bgMusic, bool& jumpOffCooldown, int& jumpCooldownTimer);

void UpdateScoreHistory(int scores[3], int& finalScore);
void SetAndDrawScoreText(RenderWindow& app, Text& text, int& score, Font& font);
void SetAndDrawJumpCooldownText(RenderWindow& app, Text& cdText, Text& jumpText, int& jumpCount, int& jumpCooldownTimer, Font& font);
void SetAndDrawMainMenuText(RenderWindow& app, Text& titleText, Text& descriptionTitleText, Text& descriptionText, Text& startText, Font& font, int& count);
void SetAndDrawRestartMenuText(RenderWindow& app, Text& restartText, Text& finalScoreText, Text& athScoreText, Text& topScoresText, Text& scoreHistoryText, int& finalScore, int& athScore, int scoreHistory[3], Font& font, int& count);



#endif //DOODLEJUMP_DOODLEFUNCTIONS_H
