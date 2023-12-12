#include "Header.h"

using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(sf::VideoMode(800, 600), "Bakunawa and the Infinite Moons");

	Event event;

	//Load window icon
	Image icon;
	if (icon.loadFromFile("bakulogo.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	launchlogo(window);

	//Pause
	bool isPaused = false;

	int score = 0;

	//Load Assets
	Font arial;
	if (!arial.loadFromFile("pixeboy.ttf")) {
		return -1;
	}

	Music bgmusic;
	if (!bgmusic.openFromFile("Bakunawa.wav")) {
		return -1;
	}

	Music lbmusic;
	if (!lbmusic.openFromFile("board.wav")) {
		return -1;
	}

	Music gameplayMusic;
	if (!gameplayMusic.openFromFile("gameplay.wav")) {
		return -1;
	}

	Music changetab;
	if (!changetab.openFromFile("retrochange.wav")) {
		return -1;
	}
	Music playsfx;
	if (!playsfx.openFromFile("start.wav")) {
		return -1;
	}

	Texture logo;
	if (!logo.loadFromFile("mainmenulogo.png")) {
		return -1;
	}
	Texture desert;
	if (!desert.loadFromFile("night.png"))
	{
		return -1;
	}
	Texture bgbutt;
	if (!bgbutt.loadFromFile("musicbutton.png"))
	{
		return -1;
	}

	//Loop BGMusic
	bgmusic.setLoop(true);
	bgmusic.play();

	//
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			//MENU - Title
			Sprite menulogo;
			menulogo.setTexture(logo);
			menulogo.setScale(1, 1);
			//menulogo.setPosition(Vector2f(220.f, 30.f));

			RectangleShape bg(Vector2f(800, 600));
			bg.setTexture(&desert);

			Sprite bgmusicbutton;
			bgmusicbutton.setTexture(bgbutt);
			bgmusicbutton.setScale(1, 1);
			bgmusicbutton.setPosition(Vector2f(730.f, 15.f));

			//MENU - Highscore
			Text hiscore("HI-SCORE: " + to_string(score), arial, 25);
			hiscore.setFillColor(Color::White);
			FloatRect hiscoreBounds = hiscore.getLocalBounds();
			hiscore.setPosition(Vector2f(30.f, 10.f));

			//MENU - Options
			Text play("PLAYER", arial, 40);
			play.setFillColor(Color::White);
			play.setStyle(Text::Bold);
			FloatRect playBounds = play.getLocalBounds();
			play.setPosition(window.getSize().x / 2 - playBounds.width / 2, window.getSize().y / 2 + 20);

			Text leaderbrd("LEADERBOARD", arial, 40);
			leaderbrd.setFillColor(Color::White);
			leaderbrd.setStyle(Text::Bold);
			FloatRect leaderbrdBounds = leaderbrd.getLocalBounds();
			leaderbrd.setPosition(window.getSize().x / 2 - leaderbrdBounds.width / 2, window.getSize().y / 2 + 60);

			Text howto("HOW TO PLAY", arial, 40);
			howto.setFillColor(Color::White);
			howto.setStyle(Text::Bold);
			FloatRect howtoBounds = howto.getLocalBounds();
			howto.setPosition(window.getSize().x / 2 - howtoBounds.width / 2, window.getSize().y / 2 + 100);

			Text quit("QUIT", arial, 40);
			quit.setFillColor(Color::White);
			quit.setStyle(Text::Bold);
			FloatRect quitBounds = quit.getLocalBounds();
			quit.setPosition(window.getSize().x / 2 - quitBounds.width / 2, window.getSize().y / 2 + 140);

			//ON-HOVER
			if (event.type == Event::MouseMoved) {

				Vector2i mousePos = Mouse::getPosition(window);

				// Check if the mouse position is within the bounds of a shape
				if (play.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					play.setFillColor(Color::Yellow);
					changetab.play();
				}
				else {
					play.setFillColor(Color::White);
					play.setScale(Vector2f(1.0f, 1.0f));
				}

				if (leaderbrd.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					leaderbrd.setFillColor(Color::Yellow);
					changetab.play();
				}
				else {
					leaderbrd.setFillColor(Color::White);
					leaderbrd.setScale(Vector2f(1.0f, 1.0f));
				}

				if (howto.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					howto.setFillColor(Color::Yellow);
					changetab.play();
				}
				else {
					howto.setFillColor(Color::White);
					howto.setScale(Vector2f(1.0f, 1.0f));
				}

				if (quit.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					quit.setFillColor(Color::Yellow);
					changetab.play();

				}
				else {
					quit.setFillColor(Color::White);
					quit.setScale(Vector2f(1.0f, 1.0f));
				}
			}
			//ON-CLICK
			else if (event.type == Event::MouseButtonPressed) {
				Vector2i mousePos = Mouse::getPosition(window);

				if (play.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					bgmusic.stop();
					
					string playerName = entername(window, score);
					if (playerName != "cancelled") {
						playsfx.play();
						introStory(window);
						gameplayMusic.play();
						score = gameEngine(window, playerName);
						gameplayMusic.stop();
						saveScore(playerName, score);
					}

					
				}

				if (leaderbrd.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					bgmusic.stop();
					displayLeaderboard(window,lbmusic);
				}
				if (howto.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					bgmusic.stop();
					instructions(window, lbmusic);
				}

				if (quit.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					window.close();
				}

				if (bgmusicbutton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					bgmusic.stop();
				}
			}

			if (bgmusic.getStatus() == Music::Stopped) {
				bgmusic.play();
			}


			window.clear();

			window.draw(bg);
			window.draw(menulogo);
			window.draw(play);
			window.draw(hiscore);
			window.draw(bgmusicbutton);
			window.draw(leaderbrd);
			window.draw(howto);
			window.draw(quit);

			window.display();
		}


	}

	return 0;
}