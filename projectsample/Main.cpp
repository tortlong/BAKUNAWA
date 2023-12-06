#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Header.h"

using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(sf::VideoMode(800, 600), "SNEK: Ultimate Edition");

	Event event;
	
	launchlogo(window);

	//Pause
	bool isPaused = false;

	//Load window icon
	Image icon;
	if (icon.loadFromFile("logo.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	//Load Assets
	Font arial;
	if (!arial.loadFromFile("pixeboy.ttf")) {
		return -1;
	}
	Music bgmusic;
	if (!bgmusic.openFromFile("power.wav")) {
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
	if (!logo.loadFromFile("logo.png")) {
		return -1;
	}


	//
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			//Loop BGMusic
			bgmusic.setLoop(true);
			bgmusic.play();

			//MENU - Title
			Sprite menulogo;
			menulogo.setTexture(logo);
			menulogo.setScale(9,9);
			menulogo.setPosition(Vector2f(220.f, 30.f));

			//MENU - Highscore
			Text hiscore("HI-SCORE: ", arial, 20);
			hiscore.setFillColor(Color::White);
			FloatRect hiscoreBounds = hiscore.getLocalBounds();
			hiscore.setPosition(Vector2f(30.f, 10.f));

			//MENU - Options
			Text play("PLAY", arial, 40);
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
					playsfx.play();
					string playerName = entername(window);
					gameEngine(window, playerName);
				}

				if (howto.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					instructions(window);
				}

				if (quit.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
					window.close();
				}
			}


			window.clear();

			window.draw(menulogo);
			window.draw(play);
			window.draw(hiscore);
			window.draw(leaderbrd);
			window.draw(howto);
			window.draw(quit);

			window.display();
		}


	}

	return 0;
}