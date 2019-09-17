// PathfindingVisualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include <iostream>
#include "Gameboard.cpp"
#include "Gui.cpp"
#include <chrono>
#include "Game.cpp"

int main(void) {
	gm::Game* g = new gm::Game();
	g->loop();
}