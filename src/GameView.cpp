#include <GameView.hpp>
#include <cstdlib>
#include <ctime>

GameView::GameView(sf::RenderWindow* app):
playerOne(sf::Vector2f(15,100)),playerTwo(sf::Vector2f(15,100)), ball(10){
    window = app;
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

bool GameView::isPlayerReady(){
  return playerReady;
}

void GameView::updateScore(){
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
    scoreString = "Player One Score: "
                  + std::to_string(p1Score)+ "                  "
                  "CP Score: " + std::to_string(cpuScore);
  }
  else{
    scoreString = "Player One Score: "
                  +std::to_string(p1Score)+"                  "
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
    if((ball.getPosition().y > playerTwo.getPosition().y) &&
    playerTwo.getPosition().y <= 500){
      cdx = 1;
    }
    else if ((ball.getPosition().y < playerTwo.getPosition().y) &&
    playerTwo.getPosition().y >= 0){
      cdx = -1;
    }
    else{
      cdx = 0;
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
    ball.move(400 * bdx * deltaTime * ballSpeed, 400 * bdy * deltaTime * ballSpeed);
    if(ball.getPosition().y <= 0){
      bdy = - bdy;
      blip.play();
      ball.setPosition(ball.getPosition().x, ball.getPosition().y + 1);
      perturb();
    }
    if (ball.getPosition().y >= 580){
      bdy = - bdy;
      blip.play();
      ball.setPosition(ball.getPosition().x, ball.getPosition().y - 1);
      perturb();
    }

    // Collisions with paddles
    if(ball.getGlobalBounds().intersects(playerOne.getGlobalBounds())){
      bdx = -bdx;
      blip2.play();
      ball.setPosition(ball.getPosition().x - 1.0, ball.getPosition().y);
      if(speedupToggled){
        ballSpeed = ballSpeed +.05;
      }
    }
    if(ball.getGlobalBounds().intersects(playerTwo.getGlobalBounds())){
      bdx = - bdx;
      blip2.play();
      ball.setPosition(ball.getPosition().x + 1.0, ball.getPosition().y);
      if(speedupToggled){
        ballSpeed = ballSpeed +.05;
      }
    }

    // Scoring stuff
    if(ball.getPosition().x <= 0){ //If p1 scores
      p1Score += 1;
      ball.setPosition(375,275);
      ballWaiting = true;
      ballTimer = globalClock.getElapsedTime().asSeconds();
      scoreSound.play();
      ballSpeed = 1.0;
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
      ballSpeed = 1.0;
    }
  }
  else if((globalClock.getElapsedTime().asSeconds() - ballTimer) > 2){ //Wait 2 seconds
    ballWaiting = false;
    globalClock.restart();
  }

  updateScore();
}

void GameView::perturb(){
  srand (time(NULL));
  int i = rand() % 999;
  int coinflip = rand() % 1;
  float f = (i / 1000.0) / 2.5;

  if (coinflip == 0) {
    f *= -1.0;
  }

  if(bdx > 0){
    bdx = ballSpeed + f;
  }
  else{
    bdx = -ballSpeed - f;
  }

  srand (time(NULL));
  i = rand() % 999;
  coinflip = rand() % 1;
  f = (i / 1000.0) / 2.5;

  if (coinflip == 0) {
    f *= -ballSpeed;
  }

  if(bdy > 0){
    bdy = ballSpeed + f;
  }
  else{
    bdy = -ballSpeed - f;
  }

}

void GameView::reset(){
  playerOne.setPosition(765,200);
  playerTwo.setPosition(20,200);
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

void GameView::menuScreen(){
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
    twoPlayerMode = false;
    onePlayerText.setFillColor(sf::Color(247, 132, 131));
    twoPlayerText.setFillColor(sf::Color(247, 233, 131));
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
    twoPlayerMode = true;
    onePlayerText.setFillColor(sf::Color(247, 233, 131));
    twoPlayerText.setFillColor(sf::Color(247, 132, 131));
  }
  //Close if user hits escape
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    window->close();
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
    playerReady = true;
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
    speedupToggled = true;
    speedupText.setFillColor(sf::Color(247, 132, 131));
    speedupText.setString("Speedup Enabled\n(hit D to disable)");

  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
    speedupToggled = false;
    speedupText.setFillColor(sf::Color(247, 233, 131));
    speedupText.setString("Speedup Disabled\n(hit S to enable)");
  }


  window->clear(sf::Color(72, 85, 97));
  window->draw(bulletin);
  window->draw(onePlayerText);
  window->draw(twoPlayerText);
  window->draw(speedupText);
  window->display();
}

void GameView::endScreen(){
  //Close if user hits escape
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    window->close();
  }
  //Restart if they hit enter
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
    reset();
  }
  scoreboard.setString(resultsString);
  window->clear(sf::Color::Black);
  window->draw(scoreboard);
  window->display();
}

void GameView::init(){
  //Paddles and stuff
  playerOne.setFillColor(sf::Color(151,209,216));
  playerTwo.setFillColor(sf::Color(221,177,221));
  playerOne.setPosition(765,200);
  playerTwo.setPosition(20,200);
  ball.setPosition(375,275);
  ball.setFillColor(sf::Color(247, 132, 131));
  //init scores
  p1Score = 0;
  p2Score = 0;
  cpuScore = 0;

  //Scoreboard setup
  font.loadFromFile("../src/fonts/DTM-Sans.otf");
  scoreboard.setFont(font);
  scoreboard.setPosition(200,10);
  scoreboard.setString(scoreString);
  scoreboard.setCharacterSize(20);
  scoreboard.setFillColor(sf::Color(247,233,131));

  menuString = "Welcome to Pong!\n\n"
               "P1 = UP/DOWN \n"
               "P2 = W/S\n\n"
               "Press ESC to quit\n"
               "Press ENTER to start\n"
               "UP/DOWN to switch mode\n";
  bulletin.setFont(font);
  bulletin.setPosition(250,100);
  bulletin.setString(menuString);
  bulletin.setCharacterSize(20);
  bulletin.setFillColor(sf::Color(247, 233, 131));

  onePlayerText.setFont(font);
  onePlayerText.setCharacterSize(20);
  onePlayerText.setPosition(250,350);
  onePlayerText.setFillColor(sf::Color(247, 132, 131));
  onePlayerText.setString("Single player mode\n");

  twoPlayerText.setFillColor(sf::Color(247,233,131));
  twoPlayerText.setPosition(250,400);
  twoPlayerText.setCharacterSize(20);
  twoPlayerText.setFont(font);
  twoPlayerText.setString("Two player mode\n\n");

  speedupText.setFont(font);
  speedupText.setCharacterSize(20);
  speedupText.setPosition(250,450);
  speedupText.setFillColor(sf::Color(247,233,131));
  speedupText.setString("Speedup Disabled\n(hit S to enable)");

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
  bdx = 1.0;
  bdy = 1.0;
  ballSpeed = 1.0;

  sf::Clock clock;
  twoPlayerMode = false;
  ballWaiting = false;
  done = false;
  playerReady = false;
  speedupToggled = false;
}
