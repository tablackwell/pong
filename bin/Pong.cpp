/*****************************
  Thomas Blackwell
  2017
  CSCI 420 Pong Project
  tablackwell@email.wm.edu
  cs username: tablackw

*****************************/


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Pong.hpp>
#include <string>
#include <GameView.hpp>


// Constructor initializes the render window via member initialization list
Pong::Pong(): App(sf::VideoMode(800,600,32), "Pong!",sf::Style::Titlebar | sf::Style::Close){}

/* Main game loop */
void Pong::run(){
  //Construct and initialize our game view class
  GameView gameView = GameView(&App);
  gameView.init();
  while(App.isOpen())
  {

    //Start our clock for frame rate management
    float deltaTime = clock.restart().asSeconds();
    handleEvents();

    // Menu screen, runs until appropriate player input (enter)
    if(!gameView.isPlayerReady()){
      gameView.menuScreen();
    }

    // Run the game loop until its done
    else if(!gameView.getStatus()){
      gameView.update();
      gameView.updateLogic(deltaTime);
    }

    else{
      gameView.endScreen();
    }

  }
}

/* Event Handler*/
void Pong::handleEvents(){
  sf::Event Event;
  while(App.pollEvent(Event))
  {
    // Exit
    if(Event.type == sf::Event::Closed)
      App.close();
  }
}


int main(int argc, char** argv){
  Pong pong;
  pong.run();
}
