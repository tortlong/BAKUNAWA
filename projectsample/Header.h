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

class node {
public:
    int x, y;
    node* back;
    node* front;
};

struct object {
public:
    int x;
    int y;
};

class snake {
public:
    node* head;
    node* tail;
    char direction;
    int size;
    bool alive;
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

void move(node*, node*, char);

void init_snake(node**, node**);

void draw_snake(node*, sf::RenderWindow&);

void setDirection(snake*);

void appleCheck(snake*, object*);

void grow(snake*);

bool isbitingSelf(snake*);

int gameEngine(sf::RenderWindow&, const std::string&);

int gameOver(sf::RenderWindow&);

void launchlogo(sf::RenderWindow&);

std::string entername(sf::RenderWindow&, int);

void instructions(sf::RenderWindow&);

void saveScore(const std::string&, int);

bool compareScores(const scoreEntry&, const scoreEntry&);

void displayLeaderboard(sf::RenderWindow& window, sf::Music& bgmusic);

void introStory(sf::RenderWindow&);

#endif
