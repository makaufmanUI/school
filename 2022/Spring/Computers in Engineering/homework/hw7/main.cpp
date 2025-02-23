/************************************************************************************************************************
*                                                                                                                       *
*        Matt Kaufman      4/14/2022                                                                                    *
*                                                                                                                       *
*        main.cpp                                                                                                       *
*                                                                                                                       *
*        Asteroids game. My added features:                                                                             *
*        - Created my own sprite sheet and animation from the texture file I used                                       *
*        - Added player reverse thrust ability (down arrow)                                                             *
*        - Added player emergency stop/drift ability (left shift) (can combine with up arrow to move slow)              *
*        - Added UFO aggression: shoots at player if in certain range                                                   *
*        - Adding scoring system: +1 for asteroid kill, +20 for ufo kill                                                *
*        - Added a lives system, with a max number of lives of 5                                                        *
*        - Added main menu screen and end screen with instructions and game statistics                                  *
*        - Added an invulnerability window that lasts for 3 seconds after each death                                    *
*          as well as a 1 second invulnerability at the start of the game and each round                                *
*        - Added a few new game sounds                                                                                  *
*        - Slight progression mechanic: more asteroids spawn per level you reach, and the UFO gets better at shooting   *
*                                                                                                                       *
*************************************************************************************************************************/
// ufo.png texture file  -  made by Freepik (https://www.flaticon.com/authors/freepik) from https://www.flaticon.com/
// ConcertOne-Regular.ttf font file    - From https://fonts.google.com/specimen/Concert+One,  License: OFL, included in fonts folder as per license requirement
// ufo_spawn.ogg sound file  -  "Angry Boat 2" by anderz000, https://freesound.org/people/anderz000/,  License: CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/legalcode)
// ufo_ambience.ogg sound file  -  "Crop Circle-Ambivalence" by audiomirage, https://freesound.org/people/audiomirage/,  License: CC BY-NC 4.0 (https://creativecommons.org/licenses/by-nc/4.0/legalcode)
// player_death.ogg sound file  -  "Underground explosion impact echo" by Mixkit, https://mixkit.co/,  License: Free commercial and non-commercial use (I reduced pitch by 20%)
// ufo_explosion.ogg sound file -  "Underground explosion impact echo" by Mixkit, https://mixkit.co/,  License: Free commercial and non-commercial use (I increased pitch by 70%)
// applause.ogg sound file  -  "Medium size crowd applause" by Mixkit, https://mixkit.co/,  License: Free commercial and non-commercial use (Trimmed a little bit and added fade in/out)

#include <list>
#include <time.h>
#include <SFML/Graphics.hpp>
#include "AsteroidClasses.h"
#include "AsteroidFunctions.h"

using namespace sf;

const int H = 800;              // Height of window
const int W = 1200;             // Width of window
int GAME_LOOP = 0;              // Game loop counter
int MENU_LOOP = 0;              // Menu loop counter (used for flashing text)
int LAST_DEATH = -2;            // To keep track of last time player died
int LEVEL_COUNT = 1;            // Level counter
bool UFO::alive = false;        // Flag for whether UFO is alive
int asteroid::count = 0;        // Number of asteroids counter
bool UFO::canShoot = false;     // Flag for whether UFO can shoot
int bullet::count = 0;          // Counts the number of bullets fired



int main()
{
    WriteHighScore(0);
    srand(time(0));

    Music ufoAmbience;
    SoundBuffer ufoSpawn, explosionDeath, ufoKill, ufoExplosion;
    Sound ufoSpawnSound, explosionDeathSound, ufoKillSound, ufoExplosionSound;
    loadSounds(ufoSpawn,explosionDeath,ufoKill,ufoSpawnSound,explosionDeathSound,ufoKillSound,ufoAmbience,ufoExplosion,ufoExplosionSound);

    RenderWindow app(VideoMode(W,H), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1,t2,t3,t4,t5,t6,t7,t8;
    loadTextures(t1,t2,t3,t4,t5,t6,t7,t8);
    t1.setSmooth(true);
    t2.setSmooth(true);

    Font font;
    if (!font.loadFromFile("fonts/ConcertOne-Regular.ttf")) return EXIT_FAILURE;

    Text titleText, scoreText, livesText, finalScoreText, athScoreText, gameInfoText, startText, restartText, levelText;

    Sprite background(t2);

    // Instantiate the animation objects
    Animation sPlayer(t1,40,0,40,40,1,0);
    Animation sRock(t4,0,0,64,64,16,0.2);
    Animation sBullet(t5,0,0,32,64,16,0.8);
    Animation sUFO_go(t8,0,0,80,80,16,0.3);
    Animation sPlayer_go(t1,40,40,40,40,1,0);
    Animation sRock_small(t6,0,0,64,64,16,0.2);
    Animation sExplosion(t3,0,0,256,256,48,0.5);
    Animation sExplosion_ship(t7,0,0,192,192,64,0.5);


    std::list<Entity*> entities;

    spawnAsteroids(entities, sRock, 10);

    // Create the player
    player* p = new player();
    p->settings(sPlayer,W/2,H/2,0,20);
    entities.push_back(p);

    GameClock gameClock;

    Event event;
    bool gameStarted = false;

    int x = W/2;    // for storing x location each frame
    int y = H/2;    // for storing y location each frame
    int dx = 0;     // for caluclating x travel each frame
    int dy = 0;     // for calculating y travel each frame


    /*******************
    *  MAIN GAME LOOP  *
    ********************/
    while (app.isOpen())
    {

        while (!gameStarted) {                                              // Main menu loop
            while (app.pollEvent(event))
                if (event.type == Event::Closed) app.close();                   // close window if user closes
            if (!app.isOpen()) return 0;                                        // end program if window closed

            if (Keyboard::isKeyPressed(Keyboard::Enter)) gameStarted = true;    // start game on 'Enter' keypress

            ++MENU_LOOP;
            showMainMenu(app,font,titleText,gameInfoText,startText,MENU_LOOP);  // run main menu function
        }

        GAME_LOOP += 1;
        if (GAME_LOOP % 60 == 0) {                  // every second:
            ++gameClock;                                // increment gameClock counter
            if (!UFO::alive) ++gameClock.ufoDead;       // increment ufoDead counter if UFO is not alive
            else UFO::canShoot = true;                  // give the UFO the ability to shoot if it's not dead
        }

        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
            if (event.type == Event::KeyPressed)                   // if a key is pressed:
                if (event.key.code == Keyboard::Space) {              // if that key is space:
                    bullet* b = new bullet();                            // create a new bullet object
                    b->settings(sBullet,p->x,p->y,p->angle,10);          // give it properties
                    entities.push_back(b);                               // add it to entities list
                }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 3;      // if left arrow is pressed,  rotate left
        if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;      // if right arrow is pressed, rotate right

        if (Keyboard::isKeyPressed(Keyboard::Up))
             p->thrust = true;                                           // if up arrow is pressed: thrust forward
        else p->thrust = false;                                          // if up arrow is not pressed: stop thrust

        if (Keyboard::isKeyPressed(Keyboard::Down))
             p->reverse = true;                                          // if down arrow is pressed: reverse thrust
        else p->reverse = false;                                         // if down arrow is not pressed: stop reverse thrust

        if (Keyboard::isKeyPressed(Keyboard::LShift)) {                  // if left shift is pressed:
            p->dx *= 0.9;                                                   // reduce x velocity
            p->dy *= 0.9;                                                   // reduce y velocity
        }

        if (p->thrust || p->reverse)  p->anim = sPlayer_go;         // if the player is thrusting or reversing, play that animation
        else  p->anim = sPlayer;                                    // otherwise don't show thrusting animation

        for (auto a:entities)                                       //
        {                                                            //
            for (auto b:entities)                                     // for each pair of entities in the entities list:
            {
                if (a->name == "asteroid"  &&  b->name == "bullet")      // if entity 'a' is an asteroid and entity 'b' is a bullet:
                    if (isCollide(a,b)) {                                   // if they are colliding:
                        a->life = false;                                       // mark the asteroid for deletion
                        b->life = false;                                       // mark the bullet for deletion
                        a->explode(entities, sExplosion);                      // blow up the asteroid
                        a->split(entities, sRock_small);                       // split the asteroid if not already
                        ++p->score;
                    }

                if (a->name == "player"  &&  b->name == "asteroid")      // if entity 'a' is a player and entity 'b' is an asteroid:
                    if (isCollide(a,b)) {                                   // if they are colliding:
                        if (gameClock.gameTime - LAST_DEATH > 3) {
                            --p->lives;                                            // decrement player lives
                            b->life = false;                                       // mark the asteroid for deletion
                            a->explode(entities, sExplosion_ship);                 // blow up the player
                            p->reset(sPlayer);                                     // reset the player's position
                            explosionDeathSound.play();                            // play the explosion death sound
                            x = W/2;
                            y = H/2;
                            LAST_DEATH = gameClock.gameTime;
                        }
                    }

                if (a->name == "UFO"  &&  b->name == "bullet")           // if entity 'a' is a UFO and entity 'b' is a bullet:
                    if (isCollide(a,b)) {                                   // if they are colliding:
                        p->score += 20;                                        // give the player +20 score
                        a->life = false;                                       // mark the UFO for deletion
                        b->life = false;                                       // mark the bullet for deletion
                        a->explode(entities, sExplosion_ship);                 // blow up the UFO
                        ufoExplosionSound.play();                              // play the UFO explosion sound
                        ufoKillSound.play();                                   // play the UFO death/applause sound
                    }

                if (a->name == "player"  &&  b->name == "UFO")           // if entity 'a' is a player and entity 'b' is a UFO:
                    if (isCollide(a,b)) {                                   // if they are colliding:
                        --p->lives;                                            // decrement player lives
                        b->life = false;                                       // mark the UFO for deletion
                        a->explode(entities, sExplosion_ship);                 // blow up the player (and UFO)
                        p->reset(sPlayer);                                     // reset the player's position
                        explosionDeathSound.play();                            // play the explosion death sound
                        ufoKillSound.play();                                   // play the UFO death/applause sound
                    }

                if (a->name == "player"  &&  b->name == "UFObullet")     // if entity 'a' is a player and entity 'b' is a UFO bullet:
                    if (isCollide(a,b)) {                                   // if they are colliding:
                        --p->lives;                                            // decrement player lives
                        b->life = false;                                       // mark the UFO bullet for deletion
                        a->explode(entities, sExplosion_ship);                 // blow up the player
                        p->reset(sPlayer);                                     // reset the player's position
                        explosionDeathSound.play();                            // play the explosion death sound
                    }
            }
        }

        if (asteroid::count == 0) {                               // if all asteroids have been destroyed:
            LEVEL_COUNT += 1;                                        // increment the level count
            spawnAsteroids(entities,sRock,10+(5*LEVEL_COUNT));       // spawn 15 + 5*level new asteroids
        }
        if (gameClock.ufoDead > 20  &&  rand()%1000 == 0) {       // if the UFO has been dead for at least 20 seconds, 0.1% chance per frame to:
            gameClock.ufoDead = 0;                                   // reset the UFO clock
            spawnUFO(entities,sUFO_go,ufoSpawnSound,ufoAmbience);    // spawn a new UFO
        }

        for (auto e:entities)                                     // for each entity in entities list:
        {
            if (p->lives == 0) {                                     // if the player has lost all lives:
                if (e->name == "UFO")    e->life = false;               // if entity is a UFO, mark for deletion
                if (e->name == "bullet")  e->life = false;              // if entity is a bullet, mark for deletion
                if (e->name == "asteroid") e->life = false;             // if entity is an asteroid, mark for deletion
                if (e->name == "UFObullet") e->life = false;            // if entity is a UFO bullet, mark for deletion
            }

            if (e->name == "explosion") {                            // if entity is an explosion:
                if (e->anim.isEnd())                                    // if the animation is done:
                    e->life = 0;                                        // mark the animation for deletion
            }

            if (e->name == "UFO") {                                  // if entity is a UFO:
                if (!e->life) {                                         // if the UFO is dead:
                    UFO::alive = false;                                    // flag the UFO as dead
                    ufoAmbience.stop();                                    // stop the UFO ambience music
                    gameClock.ufoDead = 0;                                 // reset the UFO clock
                }
                else if (e->inRange(p)) {                            // if the UFO is in range of the player:
                    if (UFO::canShoot) {                                // if the UFO can shoot:
                        bullet* b = new bullet("UFObullet");               // create a new bullet object
                        b->settings(sBullet,e->x,e->y,theta(p,e)+          // at the UFO's current position,
                                 rand()%(25-(5*LEVEL_COUNT)),10);          // with velocity in direction of player, with some random offset to angle (dependent on level)
                        entities.push_back(b);                             // add the bullet to entities list
                        UFO::canShoot = false;                             // flag the UFO as not able to shoot
                    }
                }
            }
        }

        for (auto i=entities.begin();  i!=entities.end();)     // for each entity in the entities list:
        {
            Entity* e = *i;                                       // get the entity
            e->update();                                          // update the entity
            e->anim.update();                                     // update the entity's animation

            if (!e->life) {                                       // if the entity has been marked for deletion:
                i = entities.erase(i);                               // remove it from the entities list
                delete e;                                            // delete it
            }
            else  i++;                                            // if the entity is not dead, go to the next entity
        }

        dx = x - p->x;
        dy = y - p->y;
        p->distance += std::sqrt(dx*dx + dy*dy);     // calculate distance traveled since last time
        x = p->x;
        y = p->y;

        if (p->lives == 0)
            if (p->score > ReadHighScore())
                WriteHighScore(p->score);

        while (p->lives == 0) {                                // while the player has no lives:
            while (app.pollEvent(event))                          //
                if (event.type == Event::Closed) app.close();     // listen for app close event
            if (!app.isOpen()) return 0;                          //

            ++MENU_LOOP;                                          // increment loop counter
            showEndMenu(app,font,titleText,gameInfoText,          // show the end menu screen
                        finalScoreText,athScoreText,restartText,
                        p->score,MENU_LOOP,gameClock.gameTime,bullet::count,p->distance,LEVEL_COUNT);

            if (Keyboard::isKeyPressed(Keyboard::R)) {            // if the R key is pressed:
                gameClock.reset();                                   // reset game clock
                gameStarted = false;                                 // flag gameStarted as false
                p->settings(sPlayer,W/2,H/2,0,20);                   // reset player settings to initial value
                x = W/2;
                y = H/2;
                p->distance = 0;
                LAST_DEATH = -2;
                LEVEL_COUNT = 0;
                p->lives = 5;                                        // reset player lives to 5, breaking loop
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) app.close();  // if escape key is pressed, close the window
        }

        ////// Draw //////
        app.draw(background);                           // draw the background
        drawScore(app, scoreText, p->score, font);      // draw the player's score
        drawLives(app, livesText, p->lives, font);      // draw the player's lives
        drawLevel(app, levelText, LEVEL_COUNT, font);   // draw the current level

        for (auto i:entities)        // for each entity in the entities list:
            i->draw(app);               // draw the entity

        app.display();               // display the window
    }

    return 0;
}
