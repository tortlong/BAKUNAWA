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
    if (!snakeTex.loadFromFile("bakunawatex.png"))
    {
        return;
    }

    //for rendering snake
    RectangleShape squares(Vector2f(20, 20));
    squares.setPosition(600, 300);
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

    for (int i = 0; i < 10; i++) {
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

void introStory(sf::RenderWindow& window) {
    Texture introbg;
    if (!introbg.loadFromFile("intro.png")) {
        return;
    }

    Sprite bg;
    bg.setTexture(introbg);
    bg.setScale(1, 1);

    window.clear();
    window.draw(bg);
    window.display();
    sleep(seconds(10));
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
    if (!appl.loadFromFile("MOON.png"))
    {
        return -1;
    }
    Texture desert;
    if (!desert.loadFromFile("night.png"))
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
    background.setFillColor(Color::Black);

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

        Sleep(50);

        window.display();

    }
    deadsound.stop();
    deadsound.play();
    return sc;
}

    Texture gobg;
    if (!gobg.loadFromFile("gameoverbg.png")) {
        return -1;
    }

    Sprite bg;
    bg.setTexture(gobg);
    bg.setScale(1, 1);

    /*Text gameOverText("GAME OVER", font, 100);
    gameOverText.setFillColor(Color::Yellow);
    gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2, (window.getSize().y - gameOverText.getLocalBounds().height) / 2 - 100);*/

    Text restart("RETRY", font, 40);
    restart.setStyle(Text::Bold);
    restart.setPosition((window.getSize().x - restart.getLocalBounds().width) / 2, (window.getSize().y - restart.getLocalBounds().height) / 2 + 60);

    Text returnmm("QUIT", font, 40);
    returnmm.setStyle(Text::Bold);
    returnmm.setPosition((window.getSize().x - returnmm.getLocalBounds().width) / 2, (window.getSize().y - returnmm.getLocalBounds().height) / 2 + 110);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);


        window.clear();
        window.draw(bg);
        window.draw(restart);
        window.draw(returnmm);
        window.display();
    }
}

void launchlogo(sf::RenderWindow& window) {
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

    Texture backbutton;
    if (!backbutton.loadFromFile("back.png")) {
        return;
    }

    Texture htbg;
    if (!htbg.loadFromFile("howtobg.png")) {
        return;
    }

    Sprite bg;
    bg.setTexture(htbg);
    bg.setScale(1, 1);

    Sprite goBack;
    goBack.setTexture(backbutton);
    goBack.setScale(2, 2);
    goBack.setPosition(30, 10);

    /*Text back("BACK", pix, 20);
    back.setFillColor(Color::White);
    back.setStyle(Text::Bold);
    back.setPosition(30, 20);*/

    /*Text contr("CONTROLS", pix, 40);
    contr.setFillColor(Color::Yellow);
    contr.setStyle(Text::Bold);
    FloatRect contrBounds = contr.getLocalBounds();
    contr.setPosition(window.getSize().x / 2 - contrBounds.width / 2, window.getSize().y / 2 -200);

    Text howto("HOW TO PLAY", pix, 40);
    howto.setFillColor(Color::Yellow);
    howto.setStyle(Text::Bold);
    FloatRect howtoBounds = howto.getLocalBounds();
    howto.setPosition(window.getSize().x / 2 - howtoBounds.width / 2, window.getSize().y / 2 - 10);*/

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);

            if (goBack.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                return;
            }
        }
        window.clear();
        window.draw(bg);
        window.draw(goBack);
        window.display();
    }
}

string entername(sf::RenderWindow& window, int score) {

    Font font;
    if (!font.loadFromFile("pixeboy.ttf"))
    {
        return "error loading";
    }

    Color gray(128, 128, 128);
    RectangleShape enterNameBox(Vector2f(500.f, 80.f));
    enterNameBox.setFillColor(gray);
    enterNameBox.setPosition((window.getSize().x - enterNameBox.getSize().x) / 2, (window.getSize().y - enterNameBox.getSize().y) / 2);

    Texture enternamebg;
    if (!enternamebg.loadFromFile("enternamebg.png")) {
        return "error loading texture";
    }

    Sprite bg;
    bg.setTexture(enternamebg);
    bg.setScale(1, 1);

    Text enterNameText("Enter Your Name:", font, 24);
    enterNameText.setPosition(enterNameBox.getPosition().x + 10, enterNameBox.getPosition().y + 10);

    Text playerName("", font, 45);
    playerName.setFillColor(Color::Black);
    playerName.setPosition(enterNameText.getPosition().x+80, enterNameText.getPosition().y-5);
    
    Text back("CANCEL", font, 20);
    back.setFillColor(Color::White);
    back.setStyle(Text::Bold);
    back.setPosition(enterNameBox.getPosition().x + 430, enterNameBox.getPosition().y + 80);

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
        window.draw(bg);
        window.draw(enterNameBox);
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

bool compareScores(const scoreEntry& a, const scoreEntry& b) {
    return a.score > b.score;
}

void displayLeaderboard(sf::RenderWindow& window) {
    std::ifstream leaderboardFile("leaderboard.txt");

    if (leaderboardFile.is_open()) {
        std::vector<scoreEntry> leaderboard;

        std::string name;
        int score;

        while (leaderboardFile >> name >> score) {
            leaderboard.push_back({ name, score });
        }

        leaderboardFile.close();

        // Sort the leaderboard entries by score (descending order)
        std::sort(leaderboard.begin(), leaderboard.end(), compareScores);

        // Display the leaderboard entries
        Font font;
        if (font.loadFromFile("pixeboy.ttf")) {

            /*Text ldtitle("LEADERBOARD", font, 40);
            ldtitle.setFillColor(Color::Yellow);
            ldtitle.setStyle(Text::Bold);
            FloatRect ldtitleBounds = ldtitle.getLocalBounds();
            ldtitle.setPosition(window.getSize().x / 2 - ldtitleBounds.width / 2, window.getSize().y / 2 - 200);*/

            Texture backbutton;
            if (!backbutton.loadFromFile("back.png")) {
                return;
            }

            Texture lbbg;
            if (!lbbg.loadFromFile("leadbbg.png")) {
                return;
            }

            Sprite bg;
            bg.setTexture(lbbg);
            bg.setScale(1, 1);

            Sprite goBack;
            goBack.setTexture(backbutton);
            goBack.setScale(2, 2);
            goBack.setPosition(30, 10);

            Text leaderboardText("", font, 30);
            FloatRect ldBounds = leaderboardText.getLocalBounds();
            leaderboardText.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 120);

            for (const auto& entry : leaderboard) {
                leaderboardText.setString(leaderboardText.getString() + entry.name + ": " + to_string(entry.score) + "\n");
            }

            while (window.isOpen()) {
                Event event;
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                    }
                }
                if (event.type == Event::MouseButtonPressed) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    if (goBack.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        return;
                    }
                }
                window.clear();
                window.draw(bg);
                window.draw(goBack);
                window.draw(leaderboardText);
                window.display();
            }
        }
        else {
            return;
        }
    }
    else {
        return;
    }
}