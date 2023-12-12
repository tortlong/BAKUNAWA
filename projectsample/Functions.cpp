#include "Header.h"

using namespace std;
using namespace sf;

void move(node* head, node* tail, char direction){

    node* sel = tail;

    if (direction == 'w')
        head->y -= 10;
    else if (direction == 'a')
        head->x -= 10;
    else if (direction == 's')
        head->y += 10;
    else if (direction == 'd') 
        head->x += 10;

    do{
        sel->x = sel->front->x;
        sel->y = sel->front->y;

        sel = sel->front;
    } while(sel->front != nullptr);

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

void setDirection(snake* snek){
    if(Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)){
        if (snek->direction != 's')
            snek->direction = 'w';
    }
    if(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){
        if (snek->direction != 'd')
            snek->direction = 'a';
    }
    if(Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)){
        if (snek->direction != 'w')
            snek->direction = 's';
    }
    if(Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
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


int moonError(object apol, node* sel) {
    RectangleShape apple(Vector2f(apol.x, apol.y));

    while (sel != nullptr) {

        RectangleShape head(Vector2f(sel->x, sel->y));

        if (head.getGlobalBounds().intersects(apple.getGlobalBounds())) { // mag loop
            return 1;
        }
        sel = sel->back;
    }

    return 0;
}

void introStory(sf::RenderWindow& window) {
    sf::Texture introbg1, introbg2;
    if (!introbg1.loadFromFile("story1.png") || !introbg2.loadFromFile("story2.png")) {
        return;
    }

    sf::Music music;
    if (!music.openFromFile("story.wav")) {
        return;
    }

    sf::Sprite bg1, bg2;
    bg1.setTexture(introbg1);
    bg1.setScale(1, 1);

    bg2.setTexture(introbg2);
    bg2.setScale(1, 1);

    window.clear();
    window.draw(bg1);
    window.display();

    sf::Event event;

    bool showSecondPicture = false;
    bool spacePressed = false;

    // Play the intro music
    music.play();

    // Get the current time for the first picture
    auto startTime1 = std::chrono::steady_clock::now();

    // Get the current time for the second picture
    auto startTime2 = std::chrono::steady_clock::now();

    while (window.isOpen() && (music.getStatus() == sf::Music::Playing) && !spacePressed) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (showSecondPicture) {
                    music.stop();  // Stop the music
                    spacePressed = true;
                    return;        // Skip the intro on the second space bar press
                }
                else {
                    showSecondPicture = true;
                    window.clear();
                    window.draw(bg2);
                    window.display();
                    // Get the current time for the second picture
                    startTime2 = std::chrono::steady_clock::now();
                }
            }
        }

        // Get the current time
        auto currentTime = std::chrono::steady_clock::now();

        if (showSecondPicture) {
            // Calculate the elapsed time in seconds for the second picture
            auto duration2 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime2).count();

            if (duration2 >= 1) {
                return;  // Exit the function after 3 seconds of showing the second picture
            }
        }
        else {
            // Calculate the elapsed time in seconds for the first picture
            auto duration1 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime1).count();

            if (duration1 >= 1) { 
                showSecondPicture = true;
                window.clear();
                window.draw(bg2);
                window.display();
                // Get the current time for the second picture
                startTime2 = std::chrono::steady_clock::now();
            }
        }
    }
}

int gameEngine(sf::RenderWindow& window, const std::string& playerName) {
    int sc = 0;

    snake wemby;
    wemby.alive = true;
    wemby.direction = 'd';

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

    object apple;
    apple.x = border.getPosition().x + 20 + rand() % int(border.getSize().x - 40);
    apple.y = border.getPosition().y + 20 + rand() % int(border.getSize().y - 40);

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


    //Render Apple
    RectangleShape squares(Vector2f(20, 20));
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
                apple.x = border.getPosition().x + 20 + rand() % int(border.getSize().x - 40);
                apple.y = border.getPosition().y + 20 + rand() % int(border.getSize().y - 40);
            } while (!moonError(apple, wemby.head));

            sc++;
            score.setString("SCORE: " + to_string(sc));
        }

        //Check if dead
        Vector2f head_pos = haed.getPosition();
        Vector2f head_size = haed.getSize();
        Vector2u window_size = window.getSize();

        bool isIntersect = false;

        // Check if the head intersects with the border
        if (head_pos.x < border.getPosition().x ||
            head_pos.x + head_size.x > border.getPosition().x + border.getSize().x ||
            head_pos.y < border.getPosition().y ||
            head_pos.y + head_size.y > border.getPosition().y + border.getSize().y) {
            isIntersect = true;
        }

        if (isIntersect || isbitingSelf(&wemby)) {
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
        draw_snake(wemby.head, window);
        window.draw(squares);
        window.draw(playerNameText);
        window.draw(score);

        Sleep(50);

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

                if (restart.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return 1;
                }
                if (returnmm.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return 2;
                }
            }
        }

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
    stellar.setScale(1, 1);

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

void instructions(sf::RenderWindow& window, sf::Music& bgmusic) {
    
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

    while (window.isOpen()) {
        bgmusic.setLoop(true);
        bgmusic.play();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        if (event.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);

            if (goBack.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                bgmusic.stop();
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
        return "error loading font";
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
    enterNameText.setPosition(enterNameBox.getPosition().x + 20, enterNameBox.getPosition().y + 10);

    Text playerName("", font, 45);
    playerName.setFillColor(Color::Black);
    playerName.setPosition(enterNameText.getPosition().x + 80, enterNameText.getPosition().y - 5);

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
            if (event.type == Event::MouseButtonPressed) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (back.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return "cancelled";
                }
            }
        }

        window.clear();
        window.draw(bg);
        window.draw(enterNameBox);
        window.draw(playerName);
        window.draw(back);
        window.display();
    }
}


void saveScore(const std::string& playerName, int score) {
    // Read existing entries
    std::ifstream leaderboardFileIn("leaderboard.txt");
    std::vector<scoreEntry> leaderboard;

    std::string name;
    int existingScore;

    while (leaderboardFileIn >> name >> existingScore) {
        leaderboard.push_back({ name, existingScore });
    }

    leaderboardFileIn.close();

    // Add the new score
    leaderboard.push_back({ playerName, score });

    // Sort the leaderboard entries by score in descending order
    std::sort(leaderboard.begin(), leaderboard.end(), [](const scoreEntry& a, const scoreEntry& b) {
        return a.score > b.score;
        });

    // Keep only the top 10 entries
    leaderboard.resize(std::min(static_cast<int>(leaderboard.size()), 10));

    // Write the updated leaderboard to the file
    std::ofstream leaderboardFileOut("leaderboard.txt");

    if (leaderboardFileOut.is_open()) {
        for (const auto& entry : leaderboard) {
            leaderboardFileOut << entry.name << " " << entry.score << "\n";
        }
        leaderboardFileOut.close();
    }
    else {
        return;
    }
}



bool compareScores(const scoreEntry& a, const scoreEntry& b) {
    return a.score > b.score;
}

void displayLeaderboard(sf::RenderWindow& window, sf::Music& bgmusic) {
    std::ifstream leaderboardFile("leaderboard.txt");

    if (leaderboardFile.is_open()) {
        bgmusic.setLoop(true);
        bgmusic.play();

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
                        bgmusic.stop();
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