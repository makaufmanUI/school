/**********************************************************************************************************************
*                                                                                                                     *
*        Matt Kaufman      3/30/2022                                                                                  *
*                                                                                                                     *
*        main.cpp                                                                                                     *
*                                                                                                                     *
*        A recreation of the game Doodle Jump with sound effects, music, score keeping, and my own additions:         *
*             An opening main menu screen, a 'restart' screen offering several attempts per play session,             *
*             a 'best five' leaderboard for the current session as well as a persistent all-time high score,          *
*             discolored platforms with the inability to jump off of that offer extra points when hit,                *
*             a simple player jump ability with a 10 second cooldown for when times get tough, and                    *
*             a 'progression' mechanic that increases the difficulty of the game as the player progresses.            *
*                                                                                                                     *
***********************************************************************************************************************/

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "DoodleFunctions.h"
using namespace sf;




int main()
{
    Music backgroundMusic;
    LoadMusic(backgroundMusic);

    SoundBuffer jump, fall;
    Sound jumpSound, fallSound;
    LoadSounds(jump, fall, jumpSound, fallSound);

    Font font1, font2;
    LoadFonts(font1, font2);

    Text scoreText, mainMenuTitleText, gameDescriptionText, gameDescriptionTitleText, startText,
            restartText, finalScoreText, topScoresText, athScoreText, scoreHistoryText, cdText, jumpText;

    Texture t1, t2, t3, t4;
    LoadTextures(t1, t2, t3, t4);

    Sprite sBackground(t1), sPlat(t2), sPers(t3), sPlat_bad(t4);

    Point platform[18];
    Point bad_platform[6];

    RenderWindow app(VideoMode(400, 533), "Doodle Game!");
    app.setFramerateLimit(60);

    Event e { };


    // Initialize the game variables
    float dy;
    int x, y, h, score;
    int timeSinceStart = 1;
    bool jumpOffCooldown = true;
    int difficultyMultiplier = 0;
    int athScore = ReadHighScore();
    int scoreHistory[5] = {-1,-1,-1,-1,-1};
    int gameLoopCount = 0, jumpCount = 0, start = 0, jumpCooldownTimer = 0;


    while (app.isOpen())
    {
        gameLoopCount += 1;
        if (gameLoopCount % 60 == 0) {         // Every second
            timeSinceStart += 1;                  // Log one second of game time
            if (!jumpOffCooldown)
                jumpCooldownTimer -= 1;           // Decrement jump cooldown timer
            if (jumpCooldownTimer == 0)
                jumpOffCooldown = true;           // Reset jump cooldown
        }

        int loopCount = 0;
        while (start == 0) {
            loopCount += 1;
            while (app.pollEvent(e)) {
                if (e.type == Event::Closed) app.close();
                if (!app.isOpen()) return 0;
            }
            if (app.hasFocus()) {
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    // Start game (sets start to 1, breaks loop)
                    ResetGame(start, x, y, h, dy, score, platform, bad_platform,backgroundMusic,jumpOffCooldown, jumpCooldownTimer);
                }
            }
            SetAndDrawMainMenuText(app, mainMenuTitleText, gameDescriptionTitleText,gameDescriptionText, startText, font1, loopCount);
        }

        while (app.pollEvent(e)) {
            if (e.type == Event::Closed) app.close();
        }
        if (app.hasFocus()) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;
            if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3;
            if (Keyboard::isKeyPressed(Keyboard::Space) && jumpOffCooldown && jumpCount < 5) {
                Jump(dy, jumpCount, jumpCooldownTimer, jumpOffCooldown);
            }
        }

        if (gameLoopCount % 1800 == 0 && timeSinceStart >= 30)      // every 30 seconds after the first 30 seconds
            difficultyMultiplier += 1;                                  // increase difficulty (increase platform scroll speed)

        ScrollPlatforms(platform, bad_platform, difficultyMultiplier, score);

        dy += 0.2;
        y += dy;

        if (y > 500) {
            fallSound.play();
            backgroundMusic.stop();
            UpdateScoreHistory(scoreHistory, score);
            while (fallSound.getStatus() != 0) { }          // wait for sound to finish

            loopCount = 0;
            while (y > 500) {
                loopCount += 1;
                while (app.pollEvent(e)) {
                    if (e.type == Event::Closed) app.close();
                    if (!app.isOpen()) return 0;
                }
                if (score > athScore) {
                    athScore = score;
                    WriteHighScore(athScore);
                }
                SetAndDrawRestartMenuText(app, restartText, finalScoreText, athScoreText, topScoresText,
                                          scoreHistoryText, score, athScore, scoreHistory, font1, loopCount);
                if (app.hasFocus()) {
                    if (Keyboard::isKeyPressed(Keyboard::R)) {
                        // Sets y to 100, breaks loop
                        ResetGame(start, x, y, h, dy, score,
                                  platform, bad_platform, backgroundMusic, jumpOffCooldown, jumpCooldownTimer);
                    }
                }
            }
        }

        // enforce a hard barrier on screen-edge (too easy if you can wrap around)
        if (x + 50 > 400) x = 350;
        if (x + 35 < 0) x = -35;

        if (y < h) MoveAndAddPlatforms(platform, bad_platform, y, h, dy, score);

        CheckForPlatformCollisions(platform, bad_platform, jumpSound, x, y, dy, score);

        SetAndDrawSprites(app, platform, bad_platform, sPers, sBackground, sPlat, sPlat_bad, x, y);

        SetAndDrawScoreText(app, scoreText, score, font1);
        SetAndDrawJumpCooldownText(app, cdText, jumpText, jumpCount, jumpCooldownTimer, font1);

        app.display();
    }

    return 0;
}
