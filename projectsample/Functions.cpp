#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

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
    window.setFramerateLimit(90);

    Texture snakeTex;
    if (!snakeTex.loadFromFile("snaketex.png"))
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

int gameEngine(sf::RenderWindow& window, const std::string& playerName) {

    using namespace std;
    using namespace sf;

    int sc = 0;

    snake wemby;
    wemby.alive = true;
    wemby.direction = 'd';

    object apple;
    apple.x = rand() % 100;
    apple.y = rand() % 100;


    //Textures
    Texture appl;
    if (!appl.loadFromFile("apolreal.png"))
    {
        return -1;
    }
    Texture desert;
    if (!desert.loadFromFile("desertterrain.png"))
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
    if (!Arial.loadFromFile("pixeboy.ttf")) {
        return 1;
    }

    //Sprites
    /*Sprite background;
    background.setTexture(desert);
    background.setScale(0.4, 0.4);
    FloatRect titleBounds = background.getLocalBounds();*/
    sf::Color brownColor(139, 69, 19);
    RectangleShape background(Vector2f(800, 600));
    background.setFillColor(brownColor);

    //border
    RectangleShape border(Vector2f(700, 500));
    border.setTexture(&desert);
    //border.setFillColor(Color::Black);
    border.setPosition((window.getSize().x - border.getSize().x) / 2, (window.getSize().y - border.getSize().y) / 2);

    //Player Name
    Text playerNameText(playerName, Arial, 30);
    playerNameText.setPosition(100,10);
    playerNameText.setStyle(Text::Bold);

    //Score
    Text score("SCORE: " + to_string(sc), Arial, 30);
    score.setFillColor(Color::White);
    score.setStyle(Text::Bold);
    FloatRect scoreBounds = score.getLocalBounds();
    score.setPosition(600, 10);

    //Render Apple
    RectangleShape squares(Vector2f(30, 30));
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
                apple.x = rand() % 700;
                apple.y = rand() % 500;
            } while (appleError(apple, wemby.head));

            sc++;
            score.setString("SCORE: " + to_string(sc));
        }

        //Check if dead
        Vector2f head_pos = haed.getPosition();
        Vector2f head_size = haed.getSize();
        Vector2u window_size = window.getSize();

        bool isEdged = (head_pos.x <= 0) || (head_pos.x + head_size.x >= window_size.x) || (head_pos.y <= 0) || (head_pos.y + head_size.y >= window_size.y);

        if (isEdged || isbitingSelf(&wemby)) {
            wemby.alive = false;
        }

        //render
        window.clear();
        window.draw(background);
        window.draw(border);
        draw_snake(wemby.head, window);
        window.draw(squares);
        window.draw(playerNameText);
        window.draw(score);

        Sleep(10);

        window.display();

    }
    deadsound.stop();
    deadsound.play();
    return sc;
}

void launchlogo(sf::RenderWindow& window) {
    using namespace std;
    using namespace sf;


    Music music;
    if (!music.openFromFile("start.wav"))
    {
        return;
    }
    Texture logo;
    if (!logo.loadFromFile("stellarlogo.png"))
    {
        return;
    }
    Sprite stellar;
    stellar.setTexture(logo);
    stellar.setScale(0.7, 0.7);
    stellar.setPosition(5, 50);
    
    window.draw(stellar);
    window.display();
    this_thread::sleep_for(std::chrono::seconds(2));
    music.stop();
    music.play();

    bool isFading = true;

    while (isFading) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        Color spriteColor = stellar.getColor();

        // Decrease the alpha value by a small amount
        spriteColor.a = static_cast<Uint8>(std::max(0, static_cast<int>(spriteColor.a - 1)));
        stellar.setColor(spriteColor);

        // Check if the sprite is fully transparent
        if (spriteColor.a == 0) {
            // End the fade effect
            isFading = false;
        }

        window.clear();
        window.draw(stellar);
        window.display();

        this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    window.clear();
    window.display();
}

void instructions(sf::RenderWindow& window) {
    using namespace std;
    using namespace sf;
    
    Font pix;
    if (!pix.loadFromFile("pixeboy.ttf")) {
        return;
    }

    Text back("BACK", pix, 20);
    back.setFillColor(Color::White);
    back.setStyle(Text::Bold);
    back.setPosition(30,20);

    Text contr("CONTROLS", pix, 40);
    contr.setFillColor(Color::Yellow);
    contr.setStyle(Text::Bold);
    FloatRect contrBounds = contr.getLocalBounds();
    contr.setPosition(window.getSize().x / 2 - contrBounds.width / 2, window.getSize().y / 2 -200);

    Text howto("HOW TO PLAY", pix, 40);
    howto.setFillColor(Color::Yellow);
    howto.setStyle(Text::Bold);
    FloatRect howtoBounds = howto.getLocalBounds();
    howto.setPosition(window.getSize().x / 2 - howtoBounds.width / 2, window.getSize().y / 2 - 10);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);

            if (back.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                return;
            }
        }
        window.clear();
        window.draw(back);
        window.draw(contr);
        window.draw(howto);
        window.display();
    }
}

std::string entername(sf::RenderWindow& window) {
    using namespace std;
    using namespace sf;

    RectangleShape enterNameBox(Vector2f(500.f, 100.f));
    enterNameBox.setFillColor(Color::Blue);
    enterNameBox.setPosition((window.getSize().x - enterNameBox.getSize().x) / 2, (window.getSize().y - enterNameBox.getSize().y) / 2);

    Font font;
    if (!font.loadFromFile("pixeboy.ttf"))
    {
        return "error loading";
    }
    Text enterNameText("Enter Your Name:", font, 24);
    enterNameText.setPosition(enterNameBox.getPosition().x + 10, enterNameBox.getPosition().y + 10);

    Text playerName("", font, 24);
    playerName.setPosition(enterNameText.getPosition().x, enterNameText.getPosition().y + 40);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 13) {  // Enter key
                        // Handle enter key press, e.g., start the game
                        return playerName.getString();
                    }
                    else if (event.text.unicode == 8 && playerName.getString().getSize() > 0) {  // Backspace key
                        // Handle backspace key press, remove the last character
                        playerName.setString(playerName.getString().substring(0, playerName.getString().getSize() - 1));
                    }
                    else {
                        // Append the entered character to the player's name
                        playerName.setString(playerName.getString() + static_cast<char>(event.text.unicode));
                    }
                }
            }
        }

        window.clear();
        window.draw(enterNameBox);
        window.draw(enterNameText);
        window.draw(playerName);
        window.display();
    }
}

void saveScore(const std::string& playerName, int score) {
    std::ofstream leaderboardFile("leaderboard.txt", std::ios::app);

    if (leaderboardFile.is_open()) {
        leaderboardFile << playerName << " " << score << "\n";
        leaderboardFile.close();
    }
    else {
        return;
    }
}

