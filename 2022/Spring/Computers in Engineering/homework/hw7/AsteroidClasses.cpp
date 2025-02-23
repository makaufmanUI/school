/****************************************************************************
*                                                                           *
*        Matt Kaufman      4/12/2022                                        *
*                                                                           *
*        AsteroidClasses.cpp                                                *
*                                                                           *
*        Defines the classes declared in AsteroidClasses.h                  *
*                                                                           *
*****************************************************************************/

#include <cmath>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "AsteroidClasses.h"
using namespace sf;

const int H = 800;
const int W = 1200;
float DEGTORAD = 0.017453f;




/******************************
*                             *
*       Animation class       *
*                             *
*******************************/

/// Constructor
Animation::Animation() { }

/// Animation method
Animation::Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
{
    Frame = 0;
    speed = Speed;

    for (int i=0; i<count; i++)                         // for each frame in the sprite sheet:
        frames.push_back(IntRect(x + i * w, y, w, h));     // add it to the frames vector

    sprite.setTexture(t);                               // set the sprite's texture
    sprite.setOrigin(w/2,h/2);                          // set the sprite's origin
    sprite.setTextureRect(frames[0]);                   // set the sprite's texture rectangle
}

/// Update method
void Animation::update()
{
    Frame += speed;                               // increment the frame by animation speed
    int n = frames.size();                        // get the number of frames in the animation

    if (Frame >= n)                               // if the frame is greater than the number of frames in the animation:
        Frame -= n;                                  // decrement the frame by the number of frames in the animation

    if (n > 0)                                    // if there are frames in the animation:
        sprite.setTextureRect(frames[int(Frame)]);   // set the sprite's texture rectangle to the current frame
}

/// isEnd method
bool Animation::isEnd()
{
    return Frame+speed >= frames.size();          // returns true if the animation has ended
}





/****************************************
*                                       *
*       Entity class (base class)       *
*                                       *
*****************************************/

/// Constructor
Entity::Entity() {
    life = 1;       // set life to true/1 on instantiation
}

/// Destructor
Entity::~Entity() { }

/// Update Method
void Entity::update() { }

/// Settings Method
void Entity::settings(Animation& a, int X, int Y, float Angle, int radius) {
    x = X;
    y = Y;
    anim = a;
    R = radius;
    angle = Angle;
}

/// Draw Method
void Entity::draw(RenderWindow &app) {
    anim.sprite.setPosition(x,y);         // set the sprite's position
    anim.sprite.setRotation(angle+90);    // set the sprite's rotation
    app.draw(anim.sprite);                // draw the sprite animation
}

/// Split Method
void Entity::split(std::list<Entity*>& entities, Animation& ani) {
    for (int i = 0; i < 2; i++) {
        if (R == 15)                                 // if the asteroid is already small:
            continue;                                   // skip it (already been hit before)
        Entity* e = new asteroid();                  // create a new asteroid
        e->settings(ani, x, y, rand()%360, 15);      // give it properties
        entities.push_back(e);                       // add it to the entities list
    }
}

/// Explode Method
void Entity::explode(std::list<Entity*>& entities, Animation& ani) {
    Entity* e = new Entity();           // create a new entity
    e->settings(ani, x, y);             // give it an explosion animation at the entity's position
    e->name = "explosion";              // set its name
    entities.push_back(e);              // add it to the entities list
}

/// inRange Method
bool Entity::inRange(Entity* p) {
    return (p->x - x)*(p->x - x) +      // returns true if the magnitude of distance
           (p->y - y)*(p->y - y) <      // between UFO and player is less than 400 pixels
           (p->R + 400)*(p->R + 400);
}





/***************************************************
*                                                  *
*       Asteroid class (derived from Entity)       *
*                                                  *
****************************************************/

/// Constructor
asteroid::asteroid() {
    asteroid::count += 1;       // increment the asteroid count
    dx = rand()%8 - 4;          // initialize asteroid with a random x velocity (between -4 and +3)
    dy = rand()%8 - 4;          // initialize asteroid with a random y velocity (between -4 and +3)
    name = "asteroid";          // set the asteroid's name
}

/// Destructor
asteroid::~asteroid() {
    asteroid::count -= 1;       // decrement the asteroid count
}

/// Update Method
void asteroid::update() {
    x += dx;                    // update the asteroid's x position by its x velocity
    y += dy;                    // update the asteroid's y position by its y velocity

    if (x > W)  x = 0;          // if the asteroid is too far right: place on left side of screen
    if (x < 0)  x = W;          // if the asteroid is too far left: place on right side of screen
    if (y > H)  y = 0;          // if the asteroid is too far down: place on top of screen
    if (y < 0)  y = H;          // if the asteroid is too far up: place on bottom of screen
}




/*************************************************
*                                                *
*       Bullet class (derived from Entity)       *
*                                                *
**************************************************/

/// Constructor
bullet::bullet() {
    name = "bullet";                // Give the bullet an explicit name
    bullet::count += 1;             // increment bullet counter
}

/// Constructor
bullet::bullet(std::string name) {
    this->name = name;              // Give the bullet a defined name
}

/// Update Method
void bullet::update() {
    dx = cos(angle*DEGTORAD) * 6;            // set the bullet's x velocity (function of its angle)
    dy = sin(angle*DEGTORAD) * 6;            // set the bullet's y velocity (function of its angle)
    x += dx;                                 // update the bullet's x position by this x velocity
    y += dy;                                 // update the bullet's y position by this y velocity

    if (x > W || x < 0 || y > H || y < 0)    // if the bullet is off the screen:
        life = 0;                               // mark it for deletion
}





/*************************************************
*                                                *
*       Player class (derived from Entity)       *
*                                                *
**************************************************/

/// Constructor
player::player() {
    score = 0;              // set score to zero on instantiation
    lives = 5;              // set number of lives to 5 on instantiation
    distance = 0;
    name = "player";        // set the name
}

/// Reset Method
void player::reset(Animation& anim) {
    dx = 0;                               // set the player's x velocity to 0
    dy = 0;                               // set the player's y velocity to 0
    settings(anim, W/2, H/2, 0, 20);      // set the properties of the player
}

/// Update Method
void player::update() {
    if (thrust) {                         // if the player is thrusting:
        dx += cos(angle*DEGTORAD) * 0.2;     // add velocity to the player's x position
        dy += sin(angle*DEGTORAD) * 0.2;     // add velocity to the player's y position
    }
    else if (reverse) {                   // if the player is reversing:
        dx -= cos(angle*DEGTORAD) * 0.1;     // add velocity in negative direction to the player's x position
        dy -= sin(angle*DEGTORAD) * 0.1;     // add velocity in negative direction to the player's y position
    }
    else {                                // else if player is doing neither:
        dx *= 0.99;                          // decrease x velocity slightly
        dy *= 0.99;                          // decrease y velocity slightly
    }

    int maxSpeed = 10;                    // set the maximum speed the player can achieve
    float speed = sqrt(dx*dx + dy*dy);    // calculate the player's speed (length of velocity vector)

    if (speed > maxSpeed) {               // if the player's speed is too high:
        dx *= maxSpeed/speed;                // scale the x velocity back to the maximum speed
        dy *= maxSpeed/speed;                // scale the y velocity back to the maximum speed
    }

    x += dx;                // update the player's x position by their x velocity
    y += dy;                // update the player's y position by their y velocity

    if (y > H)  y = 0;      // if the player is too far down, place on top of screen
    if (y < 0)  y = H;      // if the player is too far up, place on bottom of screen
    if (x > W)  x = 0;      // if the player is too far right, place on left side of screen
    if (x < 0)  x = W;      // if the player is too far left, place on right side of screen
}





/**********************************************
*                                             *
*       UFO class (derived from Entity)       *
*                                             *
***********************************************/

/// Constructor
UFO::UFO() {
    name = "UFO";       // give the UFO a name
    alive = true;       // initialize UFO as alive on creation
    canShoot = true;    // initialize UFO as able to shoot on creation
}

/// Update Method
void UFO::update() {
    dx += cos(angle * DEGTORAD) * 0.8;      // add velocity to the ufo's x position
    dy += sin(angle * DEGTORAD) * 0.8;      // add velocity to the ufo's y position

    int maxSpeed = 3;                       // set the maximum speed the ufo can achieve
    float speed = sqrt(dx * dx + dy * dy);  // calculate the ufo's speed (length of velocity vector)

    if (speed > maxSpeed) {                 // if the ufo's speed is too high:
        dx *= maxSpeed / speed;                // scale the x velocity back to its maximum speed
        dy *= maxSpeed / speed;                // scale the y velocity back to its maximum speed
    }

    x += dx;                // update the ufo's x position by its x velocity
    y += dy;                // update the ufo's y position by its y velocity

    if (x > W) {                            // if the ufo went off the right side of the screen:
        x = 0;                                 // place it on the left side of the screen
        if (rand() % 2 == 0)                   // 50% chance it will either:
            angle = rand() % 45 + 15;            // change its angle toward -y direction
        else
            angle = 315 + rand() % 45 - 15;      // change its angle toward +y direction
    }
    if (x < 0) {                            // if the ufo went off the left side of the screen:
        x = W;                                 // place it on the right side of the screen
        if (rand() % 2 == 0)                   // 50% chance it will either:
            angle = 135 + rand() % 45 - 15;      // change its angle toward -y direction
        else
            angle = 180 + rand() % 45 + 20;      // change its angle toward +y direction
    }
    if (y > H) {                            // if the ufo went off the bottom of the screen:
        y = 0;                                 // place it on the top of the screen
        if (rand() % 2 == 0)                   // 50% chance it will either:
            angle = 45 + rand() % 45 - 15;       // change its angle toward +x direction
        else
            angle = 90 + rand() % 45 + 15;       // change its angle toward -x direction
    }
    if (y < 0) {                            // if the ufo went off the top of the screen:
        y = H;                                 // place it on the bottom of the screen
        if (rand() % 2 == 0)                   // 50% chance it will either:
            angle = 225 + rand() % 45 - 15;      // change its angle toward -x direction
        else
            angle = 270 + rand() % 45 + 15;      // change its angle toward +x direction
    }
}





/******************************
*                             *
*       GameClock class       *
*                             *
*******************************/

/// Constructor
GameClock::GameClock() {
    gameTime = 0;           // initialize game time to 0 on creation
    ufoDead = 0;            // initialize down time to 0 on creation
}

/// ++ Operator Overload
GameClock& GameClock::operator++() {
    gameTime++;             // increment game time by 1
    return *this;           // return the object
}

/// Clock Reset Method
void GameClock::reset() {
    gameTime = 0;           // reset game time to 0
    ufoDead = 0;            // reset down time to 0
}