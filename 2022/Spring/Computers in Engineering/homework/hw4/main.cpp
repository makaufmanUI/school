#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

struct point { int x, y; };



int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(400, 533), "Doodle Game!");  // create window
    app.setFramerateLimit(60);      // set limit to framerate

    Texture t1, t2, t3;

    t1.loadFromFile("images/background.png");           // load background texture
    if (!t1.loadFromFile("images/background.png")) {
        return EXIT_FAILURE;
    }
    t2.loadFromFile("images/platform.png");             // load platform texture
    if (!t2.loadFromFile("images/platform.png")) {
        return EXIT_FAILURE;
    }
    t3.loadFromFile("images/doodle.png");               // load doodle texture
    if (!t3.loadFromFile("images/doodle.png")) {
        return EXIT_FAILURE;
    }


    Sprite sBackground(t1), sPlat(t2), sPers(t3);   // initialize sprites with loaded textures

    point plat[20];

    for (int i = 0; i < 10; i++) {
       plat[i].x = rand() % 400;        //
       plat[i].y = rand() % 533;        // place platforms in random places
    }

	int x = 100, y = 100, h = 200;
    float dx = 0, dy = 0;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed) {      // close game if close event detected by player
                app.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {      // movement in right direction
            x += 3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {       // movement in left direction
            x -= 3;
        }

        dy += 0.2;      //
        y  += dy;       //
        if (y>500) {    // appears to control gravity
            dy = -10;   //
        }               //

	    if (y<h) {
            for (int i = 0; i < 10; i++) {
                y = h;
                plat[i].y = plat[i].y - dy;     // shift platforms down

                if (plat[i].y > 533) {          //
                    plat[i].y = 0;              // adds new platforms from the top
                    plat[i].x = rand() % 400;   //
                }
            }
        }

	    for (int i = 0; i < 10; i++) {
            if ( (x+50 > plat[i].x) && (x+20 < plat[i].x+68) && (y+70 > plat[i].y) && (y+70 < plat[i].y+14) && (dy > 0) ) {
                dy = -10;
            }
	    }

	    sPers.setPosition(x, y);    // character position

        app.draw(sBackground);      // draw background
        app.draw(sPers);            // draw character

        for (int i = 0; i < 10; i++) {
            sPlat.setPosition(plat[i].x, plat[i].y);    // set platform position
            app.draw(sPlat);        // draw platform
        }

        app.display();              // display the app
    }

    return 0;
}
