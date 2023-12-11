#pragma once
#ifndef MYHEADER_H
#define MYHEADER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace sf;

class node : public RectangleShape{
public:
    node* back;
    node* front;

    node(Vector2f);
    void draw();
};

class snake {
public:
    node* head;
    node* tail;
    char direction;
    bool alive;

    snake();
    void render(sf::RenderWindow&);
    bool eats (RectangleShape);
    void move ();
    void grow ();
    bool isbitingSelf ();
};

class difficulty {
public:
    int speed;
    int growth;
};

struct scoreEntry {
    std::string name;
    int score;
};


void setDirection(snake*);

int gameEngine(sf::RenderWindow&, const std::string&);

int gameOver(sf::RenderWindow&);

void launchlogo(sf::RenderWindow&);

std::string entername(sf::RenderWindow&, int);

void instructions(sf::RenderWindow&);

void saveScore(const std::string&, int);

bool compareScores(const scoreEntry&, const scoreEntry&);

void displayLeaderboard(sf::RenderWindow&);

#endif