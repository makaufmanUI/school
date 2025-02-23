/****************************************************************************
*                                                                           *
*        Matt Kaufman      4/12/2022                                        *
*                                                                           *
*        AsteroidClasses.h                                                  *
*                                                                           *
*        Declaration of all functions used in main.cpp                      *
*                                                                           *
*****************************************************************************/

#ifndef ASTEROIDS_ASTEROIDFUNCTIONS_H
#define ASTEROIDS_ASTEROIDFUNCTIONS_H

#include <list>
#include <random>
#include <fstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;



// Reading all time high score from file
int ReadHighScore();


// Writing new all time high score to file
void WriteHighScore(int high_score);


// Collision detection
bool isCollide(Entity* a, Entity* b);


// Angle between two entities
double theta(Entity* a, Entity* b);


// Texture loading
int loadTextures(Texture& t1,Texture& t2,Texture& t3,Texture& t4,Texture& t5,Texture& t6,Texture& t7,Texture& t8);


// Sound loading
int loadSounds(SoundBuffer& ufoSpawn,SoundBuffer& explosionDeath,SoundBuffer& ufoKill,Sound& ufoSpawnSound,
               Sound& explosionDeathSound,Sound& ufoKillSound,Music& ufoAmbience,SoundBuffer& ufoExplosion, Sound& ufoExplosionSound);


// UFO spawning
void spawnUFO(std::list<Entity*>& entities, Animation& anim, Sound& ufoSpawnSound, Music& ufoAmbience);


// Asteroid spawning
void spawnAsteroids(std::list<Entity*>& entities, Animation& anim, int numToSpawn);


// Main menu handler
void showMainMenu(RenderWindow& app, Font& font, Text& titleText, Text& gameInfoText, Text& startText, int& loopCounter);


// Game over menu handler
void showEndMenu(RenderWindow& app, Font& font, Text& titleText, Text& gameInfoText, Text& finalScoreText,
                 Text& athScoreText, Text& restartText, int& finalScore, int& loopCounter, int& gameTime, int& bulletCount, int& distance, int& level);


// Draw score text to screen function
void drawScore(RenderWindow& app, Text& scoreText, int& score, Font& font);


// Draw lives to screen function
void drawLives(RenderWindow& app, Text& livesText, int& lives, Font& font);


// Draw level to screen function
void drawLevel(RenderWindow& app, Text& levelText, int& level, Font& font);



#endif //ASTEROIDS_ASTEROIDFUNCTIONS_H
