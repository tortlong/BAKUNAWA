#include "Header.h"

using namespace std;
using namespace sf;


node :: node (Vector2f pos) {
    this->setPosition(pos);
    this->setSize(Vector2f(20, 20));
    back = nullptr;
    front = nullptr;
}

void node :: draw() {
    RectangleShape dr(this->getPosition());
    dr.setSize(this->getSize());
    dr.setFillColor(Color::Green);
}

snake :: snake() {

    head = new node(Vector2f(50,60));
    node* sel = head;

    for (int i = 0; i < 10; i++) {

        sel->setFillColor(Color::Green);

        sel->back = new node(Vector2f(50,60));
        sel->back->front = sel;
        sel = sel->back;
    }

    head->setPosition(Vector2f(50, 70));
    tail = sel;
    alive = true;
    direction = 'd';
}

void snake :: move() {

    node* sel = tail;

    if (direction == 'w')
        head->move(Vector2f(0, -10));
    else if (direction == 'a')
        head->move(Vector2f(-10,0));
    else if (direction == 's')
        head->move(Vector2f(0, 10));
    else if (direction == 'd')
        head->move(Vector2f(10, 0));

    do {
        sel->setPosition(sel->front->getPosition());

        sel = sel->front;
    } while (sel->front != nullptr);

}

void snake :: render(sf::RenderWindow& window) {

    window.setFramerateLimit(90);

    Texture snakeTex;
    if (!snakeTex.loadFromFile("snaketex.png"))
    {
        return;
    }

    
    node* sel = head;
    
    while (sel != nullptr) {

        sel->draw();

        sel = sel->back;
    }

}

void snake :: grow() {

    for (int i = 0; i < 8; i++) {

        node* nyo = new node(tail->getPosition());
        tail->back = nyo;
        tail->back->front = tail;
        tail->back->back = nullptr;

        tail = nyo;
    }

}

void setDirection(snake* snek) {
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

bool snake :: isbitingSelf() {

    node* sel = head->back->back->back->back->back->back->back;
    while (sel != nullptr) {

        if (head->getGlobalBounds().intersects(sel->getGlobalBounds())) {
            return false;
        }
        sel = sel->back;
    }
    return false;
}

bool snake :: eats(RectangleShape apple){

    node* sel = head;

    while (sel != nullptr) {
        
        if (sel->getGlobalBounds().intersects(apple.getGlobalBounds())) {
            return 1;
        }
        sel = sel->back;
    }

    return 0;
}

int gameEngine(sf::RenderWindow& window, const std::string& playerName) {
    int sc = 0;

    snake wemby;

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

    float border_x = border.getPosition().x + rand() % int(border.getSize().x);
    float border_y = border.getPosition().y + rand() % int(border.getSize().y);
    Vector2f border_pos(border_x, border_y);
    RectangleShape apple (border_pos);
    apple.setFillColor(Color::Green);

    //Player Name
    Text playerNameText(playerName, Arial, 30);
    playerNameText.setPosition(100, 10);
    playerNameText.setStyle(Text::Bold);

    //Score
    Text score("SCORE: " + to_string(sc), Arial, 30);
    score.setFillColor(Color::White);
    score.setStyle(Text::Bold);
    FloatRect scoreBounds = score.getLocalBounds();
    score.setPosition(600, 10);


    //Setup SNAKE(wemby)

    while (wemby.alive) {

        //
        setDirection(&wemby);
        wemby.move();
 
        //grow
        if (wemby.head->getGlobalBounds().intersects(apple.getGlobalBounds())) {
            munch.stop();
            munch.play();
            wemby.grow();

            
            apple.setPosition(border.getPosition().x + rand() % int(border.getSize().x), border.getPosition().y + rand() % int(border.getSize().y));

            sc++;
            score.setString("SCORE: " + to_string(sc));
        }

        //Check if dead
        Vector2f head_pos = wemby.head->getPosition();
        Vector2f head_size = wemby.head->getSize();
        Vector2u window_size = window.getSize();

        bool isIntersect = false;

        // Check if the head intersects with the border
        if (head_pos.x < border.getPosition().x ||
            head_pos.x + head_size.x > border.getPosition().x + border.getSize().x ||
            head_pos.y < border.getPosition().y ||
            head_pos.y + head_size.y > border.getPosition().y + border.getSize().y) {
            isIntersect = true;
        }

        if (isIntersect || wemby.isbitingSelf()) {
            wemby.alive = false;
            deadsound.play();
            int result = gameOver(window);

            if (result == 1)
                return gameEngine(window, playerName);
            else if (result == 2)
                return sc;
        }

        //render
        window.clear();
        window.draw(background);
        window.draw(border);
        wemby.render(window);
        window.draw(apple);
        window.draw(playerNameText);
        window.draw(score);

        //Sleep(10);

        window.display();

    }
    return sc;
}

int gameOver(sf::RenderWindow& window) {

    Font font;
    if (!font.loadFromFile("pixeboy.ttf"))
    {
        return -1;
    }

    Text gameOverText("GAME OVER", font, 100);
    gameOverText.setFillColor(Color::Yellow);
    gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2, (window.getSize().y - gameOverText.getLocalBounds().height) / 2 - 100);

    Text restart("RESTART", font, 30);
    restart.setPosition((window.getSize().x - restart.getLocalBounds().width) / 2, (window.getSize().y - restart.getLocalBounds().height) / 2);

    Text returnmm("RETURN TO MAIN MENU", font, 30);
    returnmm.setPosition((window.getSize().x - returnmm.getLocalBounds().width) / 2, (window.getSize().y - returnmm.getLocalBounds().height) / 2 + 30);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (restart.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return 1;
                }
                if (returnmm.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return 2;
                }
            }
        }

        window.clear();
        window.draw(gameOverText);
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
    Font pix;
    if (!pix.loadFromFile("pixeboy.ttf")) {
        return;
    }

    Text back("BACK", pix, 20);
    back.setFillColor(Color::White);
    back.setStyle(Text::Bold);
    back.setPosition(30, 20);

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
        window.draw(contr);
        window.draw(howto);
        window.draw(back);
        window.display();
    }
}

string entername(sf::RenderWindow& window, int score) {
    sf::Color brownColor(139, 69, 19);
    RectangleShape enterNameBox(Vector2f(500.f, 100.f));
    enterNameBox.setFillColor(brownColor);
    enterNameBox.setPosition((window.getSize().x - enterNameBox.getSize().x) / 2, (window.getSize().y - enterNameBox.getSize().y) / 2);

    Font font;
    if (!font.loadFromFile("pixeboy.ttf"))
    {
        return "error loading font";
    }
    Text enterNameText("Enter Your Name:", font, 24);
    enterNameText.setPosition(enterNameBox.getPosition().x + 20, enterNameBox.getPosition().y + 10);

    Text playerName("", font, 24);
    playerName.setPosition(enterNameText.getPosition().x, enterNameText.getPosition().y + 40);
    
    Text back("CANCEL", font, 20);
    back.setFillColor(Color::White);
    back.setStyle(Text::Bold);
    back.setPosition(enterNameBox.getPosition().x + 430, enterNameBox.getPosition().y + 100);

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
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (back.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return "cancelled";
                }
            }
        }
        node* gr = new node(Vector2f(30,30));
        gr->setFillColor(Color::Red);
        window.clear();
        gr->draw();
        window.draw(enterNameBox);
        window.draw(enterNameText);
        window.draw(playerName);
        window.draw(back);
        window.display();
    }
}

void saveScore(const std::string & playerName, int score) {
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

            Text ldtitle("LEADERBOARD", font, 40);
            ldtitle.setFillColor(Color::Yellow);
            ldtitle.setStyle(Text::Bold);
            FloatRect ldtitleBounds = ldtitle.getLocalBounds();
            ldtitle.setPosition(window.getSize().x / 2 - ldtitleBounds.width / 2, window.getSize().y / 2 - 200);

            Text back("BACK", font, 20);
            back.setFillColor(Color::White);
            back.setStyle(Text::Bold);
            back.setPosition(30, 20);

            Text leaderboardText("", font, 30);
            FloatRect ldBounds = leaderboardText.getLocalBounds();
            leaderboardText.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 150);

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

                    if (back.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        return;
                    }
                }
                window.clear();
                window.draw(ldtitle);
                window.draw(back);
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
