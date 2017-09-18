#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Pong.hpp>
#include <string>
#include <GameView.hpp>


Pong::Pong(): App(sf::VideoMode(800,600,32), "Pong!"){}

/* Main game loop */
void Pong::run(){
  GameView gameView = GameView(&App);
  gameView.init();
  while(App.isOpen())
  {

    float deltaTime = clock.restart().asSeconds();
    handleEvents();

    if(!gameView.isPlayerReady()){
      gameView.menuScreen();
    }

    else if(!gameView.getStatus()){
      gameView.update();
      gameView.updateLogic(deltaTime);
      gameView.updateScore();
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

/* Program start */
int main(int argc, char** argv){
  Pong pong;
  pong.run();
}
