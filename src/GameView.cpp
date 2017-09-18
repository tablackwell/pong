#include <GameView.hpp>

GameView::GameView(sf::RenderWindow* app):

playerOne(sf::Vector2f(15,100)),playerTwo(sf::Vector2f(15,100)), ball(10){
    window = app;
}

void GameView::init(){

  //Paddles and stuff
  playerOne.setFillColor(sf::Color(151,209,216));
  playerTwo.setFillColor(sf::Color(221,177,221));
  playerOne.setPosition(785,200);
  playerTwo.setPosition(0,200);
  ball.setPosition(375,275);
  //init scores
  p1Score = 0;
  p2Score = 0;
  cpuScore = 0;

  //Scoreboard setup
  font.loadFromFile("../src/fonts/arial.ttf");
  scoreboard.setFont(font);
  scoreboard.setPosition(250,10);
  scoreboard.setString(scoreString);
  scoreboard.setCharacterSize(15);
  scoreboard.setFillColor(sf::Color::White);

  menuString = "           Welcome to Pong!\n\n"
               "Press 1 for single player mode\n"
               "Press 2 for two player mode\n\n"
               "           Controls:\n\n"
               "P1 = UP/DOWN\n"
               "P2 = W/S\n\n\n"
               "(Press ESC to quit)";
  bulletin.setFont(font);
  bulletin.setPosition(250,100);
  bulletin.setString(menuString);
  bulletin.setCharacterSize(20);
  bulletin.setFillColor(sf::Color::White);

  //Sound setup
  buffer.loadFromFile("../src/sfx/blip.wav");
  buffer2.loadFromFile("../src/sfx/blip2.wav");
  scoreBuffer.loadFromFile("../src/sfx/score.wav");
  cpuScoreBuffer.loadFromFile("../src/sfx/cpuscore.wav");
  blip.setBuffer(buffer);
  blip2.setBuffer(buffer2);
  scoreSound.setBuffer(scoreBuffer);
  cpuScoreSound.setBuffer(cpuScoreBuffer);

  // Computer and ball velocity variables
  cdx = -1;
  bdx = 1;
  bdy = 1;

  sf::Clock clock;
  twoPlayerMode = false;
  ballWaiting = false;
  done = false;
  menuActive = true;
}

void GameView::update(){
  window->clear(sf::Color::Black);
  window->draw(playerOne);
  window->draw(playerTwo);
  window->draw(ball);
  window->draw(scoreboard);
  window->display();
}

bool GameView::getStatus(){
  return done;
}

bool GameView::getMenuActive(){
  return menuActive;
}

void GameView::updateScore(){
  //Switch to a case statement maybe
  if (cpuScore >= 11){
    done = true;
    resultsString = "\n\nGAME OVER\n\n";
    resultsString += "\nPress ENTER to restart \nor ESCAPE to quit";
  }
  else if (p1Score >= 11){
    done = true;
    resultsString = "\n\nPlayer One Wins!\n\n";
    resultsString += "\n\nPress ENTER to restart \nor ESCAPE to quit";
  }
  else if (p2Score >= 11){
    done = true;
    resultsString = "\n\nPlayer Two Wins!\n\n";
    resultsString += "\n\nPress ENTER to restart \nor ESCAPE to quit";
  }

  if(!twoPlayerMode){
    scoreString = "Player One Score: " + std::to_string(p1Score) + "     " +
                  "CP Score: " + std::to_string(cpuScore);
  }
  else{
    scoreString = "Player One Score: " + std::to_string(p1Score) + "     " +
                  "Player Two Score: " + std::to_string(p2Score);
  }
  scoreboard.setString(scoreString);
}

/*
  Handling of movement of balls and paddles
  Here we are moving the entities in proportion to the time
  elapsed per cycle of the update method. I'm keeping track of time in terms
  of seconds rather than milliseconds, thus the use of float rather than
  int.
*/
void GameView::updateLogic(float deltaTime){
  //Player paddles
  if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) &&
  (playerOne.getPosition().y >= 0) ){
      playerOne.move(0.0,-500.0 * deltaTime);
  }
  else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) &&
  (playerOne.getPosition().y <= 500) ){
    playerOne.move(0.0, 500.0 * deltaTime);
  }

  //AI
  if(!twoPlayerMode){
    if(ball.getPosition().y > playerTwo.getPosition().y){
      cdx = 1;
    }
    else if (ball.getPosition().y < playerTwo.getPosition().y){
      cdx = -1;
    }
    playerTwo.move(0.0,cdx * 300.0 * deltaTime); //Seems to be a good speed I guess
  }
  else{
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) &&
    (playerTwo.getPosition().y >= 0) ){
        playerTwo.move(0.0,-500.0 * deltaTime);
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) &&
    (playerTwo.getPosition().y <= 500) ){
      playerTwo.move(0.0, 500.0 * deltaTime);
    }
  }

  // ball collision with walls
  if(!ballWaiting){
    ball.move(400 * bdx * deltaTime, 400 * bdy * deltaTime);
    if((ball.getPosition().y <= 0) or (ball.getPosition().y >= 591)){
      bdy = - bdy;
      blip.play();
    }

    // Collisions with paddles
    if(ball.getGlobalBounds().intersects(playerOne.getGlobalBounds())){
      bdx = -bdx;
      blip2.play();
    }
    if(ball.getGlobalBounds().intersects(playerTwo.getGlobalBounds())){
      bdx = - bdx;
      blip2.play();
    }

    // Scoring stuff
    if(ball.getPosition().x <= 0){ //If p1 scores
      p1Score += 1;
      ball.setPosition(375,275);
      ballWaiting = true;
      ballTimer = globalClock.getElapsedTime().asSeconds();
      scoreSound.play();
    }
    else if (ball.getPosition().x >= 780){ //if p2 / cpu scores
      if(!twoPlayerMode){
        cpuScore += 1;
        cpuScoreSound.play();
      }
      else{
        p2Score += 1;
        scoreSound.play();
      }
      ball.setPosition(375,275);
      ballWaiting = true;
      ballTimer = globalClock.getElapsedTime().asSeconds();
    }
  }
  else if((globalClock.getElapsedTime().asSeconds() - ballTimer) > 2){ //Wait 2 seconds
    ballWaiting = false;
    globalClock.restart();
  }
}

void GameView::endScreen(){
  //Close if user hits escape
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    window->close();
  }
  //Restart if they hit enter
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
    playerOne.setPosition(785,200);
    playerTwo.setPosition(0,200);
    ball.setPosition(375,275);
    p1Score = 0;
    p2Score = 0;
    cpuScore = 0;
    cdx = -1;
    bdx = 1;
    bdy = 1;
    done = false;
    menuActive = true;
  }
  scoreboard.setString(resultsString);
  window->clear(sf::Color::Black);
  window->draw(scoreboard);
  window->display();
}

void GameView::menuScreen(){
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
    menuActive = false;
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
    menuActive = false;
    twoPlayerMode = true;
  }
  //Close if user hits escape
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    window->close();
  }
  window->clear(sf::Color::Blue);
  window->draw(bulletin);
  window->display();
}
