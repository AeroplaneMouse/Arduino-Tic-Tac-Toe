int board[3][3];
int screen[3][3];
int lastState[] = {0, 0, 0, 0, 0};
int curPos[] = {1, 1};
int player = 1;
int winnerPlayer = 0;
int pot = 50;
int input = 0;
bool blinkOn = false;
int test = 0;
// Counters
int counter = 0;
unsigned long blinkCounter = 0;
int winnerBlinkCounter = 0;
int lightsOn = 0;

void setup() {
  // Setting ouput pins.
  for (int i = 4; i < 14; i++){
    pinMode(i, OUTPUT);  
  }
  
  // Setting input pins.
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT);
  
  board[0][0] = 0;
  board[1][0] = 0;
  board[2][0] = 0;
  board[0][1] = 0;
  board[1][1] = 0;
  board[2][1] = 0;
  board[0][2] = 0;
  board[1][2] = 0;
  board[2][2] = 0;
  
  screen[0][0] = 1;
  screen[1][0] = 1;
  screen[2][0] = 1;
  screen[0][1] = 1;
  screen[1][1] = 1;
  screen[2][1] = 1;
  screen[0][2] = 1;
  screen[1][2] = 1;
  screen[2][2] = 1;
  //Serial.begin(9600);
}


// Start *******************************************************************************************

void loop() {

  //if (counter == 1000) Serial.println(analogRead(A5));
  // Checks if a button is pressed.
  int btn = 0;
  switch(counter) {
    case 0:
      btn = getPressedButton();
      buttonAction(btn);
      break;
  }

  if (blinkOn) {
    switch (blinkCounter) {
      case 50000:
        curBlink(true);
        //Serial.println("ON");
        break;
      case 100000:
        curBlink(false);
        //Serial.println("OFF");
        blinkCounter = 0;
        break;
    } 
  }
  
  // Updates the board/screen.
  // 666
  //1332
  //1998
  
  
  
  switch(counter){
    case 0:
      input = analogRead(A5);
      if (input > 1000) pot = 100;
      else pot = input/10;
      break;
    case 666:
      newDraw(1);
      break;
    case 1332:
      newDraw(2);
      break;
    case 1998:
      newDraw(3);
      counter = -1;
      break;
  }

  test = (pot * 664)/100;
  if (counter == (665 - test)) newDraw(4);
  else if (counter == (1331 - test)) newDraw(4);
  else if (counter == (1997 - test)) newDraw(4);
  
  counter++;
  blinkCounter++;
}

// End *******************************************************************************************
void buttonAction(int btn){
  int x;
  int y;
  //if (btn != 6) Serial.println(btn);
  switch(btn) {
    case 0:
      if (curPos[1] < 2) {
        x = curPos[0];
        y = curPos[1];
        updateScreen(x, y, board[x][y]);
        curPos[1]++;
        //Serial.println(0);
      }
      break;
    case 1:
      if (curPos[1] > 0) {
        x = curPos[0];
        y = curPos[1];
        updateScreen(x, y, board[x][y]);
        curPos[1]--;
        //Serial.println(1);
      }
      break;
    case 2:
      if (curPos[0] < 2) {
        x = curPos[0];
        y = curPos[1];
        updateScreen(x, y, board[x][y]);
        curPos[0]++;
        //Serial.println(2);
      }
      break;
    case 3:
      if (curPos[0] > 0) {
        x = curPos[0];
        y = curPos[1];
        updateScreen(x, y, board[x][y]);
        curPos[0]--;
        //Serial.println(3);
      }
      break;
    case 4:
      x = curPos[0];
      y = curPos[1];
      if (board[x][y] == 0 && lightsOn < 6){
        board[x][y] = player;
        updateScreen(x, y, player);
        curPos[0] = 1;
        curPos[1] = 1;
        lightsOn++;
        
        int winner = checkForWinner();
        /*
        if (winner == -1) {
          // Restarts the game.
          for (x = 0; x < 3; x++) {
            for (y = 0; y < 3; y++){
              board[x][y] = 0;
              screen[x][y] = 0;   
            }
          }
          lightsOn = 0;
        }
        */
        if (winner == 1) winnerPlayer = 1;
        else if (winner == 2) winnerPlayer = 2;
        
        if (player == 1) player = 2;
        else player = 1;
      }
      else if (lightsOn > 5 && board[x][y] == player) {
        board[x][y] = 0;
        updateScreen(x, y, 0);
        lightsOn--;
      }
      //Serial.println(4);
      break;
  }
}

void curBlink(bool state) {
  int x = curPos[0];
  int y = curPos[1];

  if (winnerPlayer == 0){
    if (state) updateScreen(x, y, player);
    else updateScreen(x, y, 0);
  }
  else if (winnerPlayer == 1){
    if (state) {
      for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
          updateScreen(x, y, 1);
        }
      }
    }
    else {
      for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
          updateScreen(x, y, 0);
        }
      }
    }
    winnerBlinkCounter++;  
  }
  else if (winnerPlayer == 2) {
    if (state) {
      for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
          updateScreen(x, y, 2);
        }
      }
    }
    else {
      for (x = 0; x < 3; x++) {
        for (y = 0; y < 3; y++) {
          updateScreen(x, y, 0);
        }
      }
    }
    winnerBlinkCounter++;
  }

  if (winnerBlinkCounter == 7) {
    winnerBlinkCounter = 0;
    winnerPlayer = 0;
    lightsOn = 0;
    // Restarts the game.
    for (x = 0; x < 3; x++) {
      for (y = 0; y < 3; y++){
        board[x][y] = 0;
        screen[x][y] = 0;   
      }
    }
  }
}

void updateScreen(int x, int y, int value){
  screen[x][y] = value;
}

int checkForWinner() {
  int x = 0;
  int y = 0;
  int _player;

  // Checking for winner in colums.
  for (_player = 1; _player < 3; _player++) {
    for (x = 0; x < 3; x++) {
      if (board[x][0] == _player && 
          board[x][1] == _player &&
          board[x][2] == _player) 
      {
        return _player;
      }
    }
  }

  // Checking for winner in rows.
  for (_player = 1; _player < 3; _player++) {
    for (y = 0; y < 3; y++) {
      if (board[0][y] == _player &&
          board[1][y] == _player &&
          board[2][y] == _player)
      {
        return _player;  
      }
    }
  }

  // Checking for winner diagonal.
  for (_player = 1; _player < 3; _player++) {
    if (board[0][0] == _player &&
        board[1][1] == _player &&
        board[2][2] == _player)
    {
      return _player;
    }

    if (board[0][2] == _player &&
        board[1][1] == _player &&
        board[2][0] == _player)
    {
      return _player; 
    }
  }

  /*
  if (lightsOn == 9) {
    return -1;
  }
  */
  return 0;
}

void newDraw(int row){
  int x;
  switch(row) {
    case 1: // Row 1
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      digitalWrite(11, HIGH);

      for (x = 0; x < 3; x++){
        if (screen[x][0] == 1) {
          digitalWrite(x + 5, HIGH);
        }
        else if (screen[x][0] == 2){
          digitalWrite(x + 8, HIGH);
        }
      }
      digitalWrite(11, HIGH);
      break;
    case 2: // Row 2
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      
      digitalWrite(11, LOW);
      digitalWrite(13, LOW);
      digitalWrite(12, HIGH);

      for (x = 0; x < 3; x++){
        if (screen[x][1] == 1) {
          digitalWrite(x + 5, HIGH);
        }
        else if (screen[x][1] == 2){
          digitalWrite(x + 8, HIGH);
        }
      }
      break;
    case 3: // Row 3
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);
      
      for (x = 0; x < 3; x++){
        if (screen[x][2] == 1) {
          digitalWrite(x + 5, HIGH);
        }
        else if (screen[x][2] == 2){
          digitalWrite(x + 8, HIGH);
        }
      }
      break;
    case 4: // Off
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      break;
  }
}

int getPressedButton(){
  int i = 0;
  int btn = 6;
  
  if (digitalRead(A0) == LOW && lastState[0] == 1) btn = 0;
  if (digitalRead(A1) == LOW && lastState[1] == 1) btn = 1; 
  if (digitalRead(A2) == LOW && lastState[2] == 1) btn = 2;
  if (digitalRead(A3) == LOW && lastState[3] == 1) btn = 3; 
  if (digitalRead(A4) == LOW && lastState[4] == 1) btn = 4;
  
  // Updates the buttons last state.
  if (digitalRead(A0) == HIGH ) lastState[0] = 1;
  else lastState[0] = 0;

  if (digitalRead(A1) == HIGH) lastState[1] = 1;
  else lastState[1] = 0;
  
  if (digitalRead(A2) == HIGH ) lastState[2] = 1;
  else lastState[2] = 0;

  if (digitalRead(A3) == HIGH) lastState[3] = 1;
  else lastState[3] = 0;

  if (digitalRead(A4) == HIGH) lastState[4] = 1;
  else lastState[4] = 0;
  
  return btn;
}
