/****************************************************************************
*                                                                           *
*        Matt Kaufman      4/12/2022                                        *
*                                                                           *
*        AsteroidClasses.h                                                  *
*                                                                           *
*        Declaration of all classes used in main.cpp                        *
*                                                                           *
*****************************************************************************/

#ifndef ASTEROIDS_ASTEROIDCLASSES_H
#define ASTEROIDS_ASTEROIDCLASSES_H

#include <list>
#include <SFML/Graphics.hpp>
using namespace sf;



/******************************
*                             *
*       Animation class       *
*                             *
*******************************/
class Animation {
public:
    Animation();                                // constructor
    Animation(Texture& t,int x,int y,int w,     // constructor
              int h,int count,float Speed);

    bool isEnd();                   // end of animation method
    void update();                  // update animation method

    Sprite sprite;                  // sprite object
    float Frame, speed;             // frame and speed
    std::vector<IntRect> frames;    // vector of frames
};


/****************************************
*                                       *
*       Entity class (base class)       *
*                                       *
*****************************************/
class Entity {
public:
    Entity();                       // constructor
    virtual ~Entity();              // destructor

    virtual void update();                      // entity update method
    bool inRange(Entity* p);                    // inRange method (for UFO determining proximity to player)
    virtual void draw(RenderWindow& app);       // entity draw method
    void split(std::list<Entity*>& entities,    // entity split method (for asteroids)
               Animation& ani);
    void explode(std::list<Entity*>& entities,  // entity explode method
                 Animation& ani);
    void settings(Animation& a,int X,int Y,     // entity settings method
                  float Angle=0,int radius=1);

    Animation anim;                 // entity animation object

    bool life;                      // life state of entity
    float angle, R;                 // angle and radius of entity
    std::string name;               // name of the entity
    float x, y, dx, dy;             // position and velocity of entity
};


/***************************************************
*                                                  *
*       Asteroid class (derived from Entity)       *
*                                                  *
****************************************************/
class asteroid : public Entity {
public:
    asteroid();                     // constructor
    ~asteroid() override;           // destructor
    void update() override;         // update method

    static int count;               // asteroid counter
};


/*************************************************
*                                                *
*       Bullet class (derived from Entity)       *
*                                                *
**************************************************/
class bullet : public Entity {
public:
    bullet();                               // constructor
    explicit bullet(std::string name);      // constructor
    void update() override;                 // update method

    static int count;               // bullet counter
};


/*************************************************
*                                                *
*       Player class (derived from Entity)       *
*                                                *
**************************************************/
class player : public Entity {
public:
    player();                       // constructor
    void update() override;         // update method
    void reset(Animation& anim);    // reset method

    int score;                      // player score keeper
    int lives;                      // player lives keeper
    bool thrust;                    // thrust state of player
    bool reverse;                   // reverse movement state
    int distance;                   // distance traveled by player
};


/**********************************************
*                                             *
*       UFO class (derived from Entity)       *
*                                             *
***********************************************/
class UFO : public Entity {
public:
    UFO();                          // constructor
    void update() override;         // update method

    static bool alive;              // life state of UFO (needed in order to operate the ufo clock)
    static bool canShoot;           // canShoot state of UFO
};


/******************************
*                             *
*       GameClock class       *
*                             *
*******************************/
class GameClock {
public:
    GameClock();                    // constructor

    void reset();                   // reset method
    GameClock& operator++();        // prefix increment operator

    int gameTime;                   // keeps track of game time
    int ufoDead;                    // keeps track of down time
};




#endif //ASTEROIDS_ASTEROIDCLASSES_H
