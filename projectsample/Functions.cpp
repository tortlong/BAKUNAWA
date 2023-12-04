#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Header.h"


void move(node* head, node* tail, char direction) {

    node* sel = tail;

    if (direction == 'w')
        head->y -= 10;
    else if (direction == 'a')
        head->x -= 10;
    else if (direction == 's')
        head->y += 10;
    else if (direction == 'd')
        head->x += 10;

    do {
        sel->x = sel->front->x;
        sel->y = sel->front->y;

        sel = sel->front;
    } while (sel->front != nullptr);

}

void init_snake(node** hed, node** tel) {

    node* head = new(node);
    head->front = nullptr;

    node* sel = head;

    for (int i = 0; i < 10; i++) {
        sel->back = new(node);
        sel->back->front = sel;
        sel = sel->back;

        sel->x = 50;
        sel->y = 50;
    }

    sel->back = nullptr;

    head->x = 60;
    head->y = 50;

    *hed = head;
    *tel = sel;
}



void draw_snake(node* sel, sf::RenderWindow& window) {

    using namespace std;
    using namespace sf;
    window.setFramerateLimit(60);

    Texture snakeTex;
    if (!snakeTex.loadFromFile("scales.png"))
    {
        return;
    }

    //for rendering snake
    RectangleShape squares(Vector2f(20, 20));
    squares.setFillColor(Color::Green);
    squares.setPosition(600, 400);
    squares.setTexture(&snakeTex);

    while (sel != nullptr) {

        squares.setPosition(sel->x, sel->y);
        window.draw(squares);

        sel = sel->back;
    }

}

void setDirection(snake* snek) {
    using namespace std;
    using namespace sf;
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        if (snek->direction != 's')
            snek->direction = 'w';
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        if (snek->direction != 'd')
            snek->direction = 'a';
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        if (snek->direction != 'w')
            snek->direction = 's';
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        if (snek->direction != 'a')
            snek->direction = 'd';
    }
}

void grow(snake* snek) {

    for (int i = 0; i < 2; i++) {
        node* nyo = new(node);
        snek->tail->back = nyo;
        snek->tail->back->front = snek->tail;
        snek->tail->back->back = nullptr;

        snek->tail = nyo;
        snek->tail->x = snek->tail->front->x;
        snek->tail->y = snek->tail->front->y;
    }

}

bool isbitingSelf(snake* snek) {
    using namespace std;
    using namespace sf;

    RectangleShape haed(Vector2f(20, 20));
    RectangleShape body(Vector2f(20, 20));

    haed.setPosition(snek->head->x, snek->head->y);

    node* sel = snek->head->back->back->back->back;
    while (sel != nullptr) {

        body.setPosition(sel->x, sel->y);

        if (haed.getGlobalBounds().intersects(body.getGlobalBounds())) {
            return true;
        }
        sel = sel->back;
    }
    return false;
}

int appleError(object apol, node* sel) {

    using namespace std;
    using namespace sf;

    RectangleShape apple(Vector2f(apol.x, apol.y));

    while (sel->back != nullptr) {

        RectangleShape head(Vector2f(sel->x, sel->y));

        if (head.getGlobalBounds().intersects(apple.getGlobalBounds())) {
            return 1;
        }
        sel = sel->back;
    }

    return 0;
}

int gameEngine(sf::RenderWindow& window) {

    using namespace std;
    using namespace sf;

    int sc = 0;

    snake wemby;
    wemby.alive = true;
    wemby.direction = 'd';

    object apple;
    apple.x = rand() % 860;
    apple.y = rand() % 720;

    //Textures
    Texture appl;
    if (!appl.loadFromFile("apolreal.png"))
    {
        return -1;
    }
    Texture desert;
    if (!desert.loadFromFile("desert.png"))
    {
        return -1;
    }
    Texture logo;
    if (!logo.loadFromFile("1.png"))
    {
        return -1;
    }

    //SFX
    Music munch;
    if (!munch.openFromFile("apple.wav")) {
        return -1;
    }
    Music deadsound;
    if (!deadsound.openFromFile("over.wav")) {
        return -1;
    }

    //Fonts
    Font Arial;
    if (!Arial.loadFromFile("calibri-regular.ttf")) {
        return 1;
    }

    //Sprites
    Sprite background;
    background.setTexture(desert);
    background.setScale(1, 1);
    FloatRect titleBounds = background.getLocalBounds();

    //Score
    Text score("SCORE: " + to_string(sc), Arial, 20);
    score.setFillColor(Color::White);
    score.setStyle(Text::Bold);
    FloatRect scoreBounds = score.getLocalBounds();
    score.setPosition(690, 20);

    //Render Apple
    RectangleShape squares(Vector2f(20, 20));
    squares.setFillColor(Color::Red);
    squares.setPosition(apple.x, apple.y);
    squares.setTexture(&appl);


    //Setup SNAKE(wemby)
    init_snake(&wemby.head, &wemby.tail);

    while (wemby.alive) {

        //
        setDirection(&wemby);
        move(wemby.head, wemby.tail, wemby.direction);
        squares.setPosition(apple.x, apple.y);

        //Check if eat apple
        RectangleShape haed(Vector2f(20, 20));
        haed.setPosition(wemby.head->x, wemby.head->y);
        if (haed.getGlobalBounds().intersects(squares.getGlobalBounds())) {
            munch.stop();
            munch.play();
            grow(&wemby);

            do {
                apple.x = rand() % 860;
                apple.y = rand() % 720;
            } while (appleError(apple, wemby.head));

            sc++;
            score.setString("Score: " + to_string(sc));
        }

        //Check if dead
        Vector2f head_pos = haed.getPosition();
        Vector2f head_size = haed.getSize();
        Vector2u window_size = window.getSize();

        bool isEdged = (head_pos.x <= 0) ||
            (head_pos.x + head_size.x >= window_size.x) ||
            (head_pos.y <= 0) ||
            (head_pos.y + head_size.y >= window_size.y);

        if (isEdged || isbitingSelf(&wemby)) {
            wemby.alive = false;
        }

        //render
        window.clear();
        //window.draw(background);
        draw_snake(wemby.head, window);
        window.draw(squares);
        window.draw(score);

        Sleep(60);

        window.display();

    }
    deadsound.stop();
    deadsound.play();
    return sc;
}
