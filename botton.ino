#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

const int playerOneButton = 2;  
const int playerOneLed = 11;    

const int playerTwoButton = 3;  
const int playerTwoLed = 12;    

const int optionA_Button = 6;    
const int optionB_Button = 7;    
const int optionC_Button = 8;    
const int optionD_Button = 9;    

const int nextButton = 4; 
int nextButtonState = 0; 
const int resetButton = 5;    

typedef struct {
    char question[32];
    char options[4][10];
    int correctOption;
} QuizQuestion;

QuizQuestion questions[6] = {
    {"What is A ---------------------?", {"A", "B", "C", "D"}, 0},
    {"What is B ---------------------?", {"A", "B", "C", "D"}, 1},
    {"What is C ---------------------?", {"A", "B", "C", "D"}, 2},
    {"What is D ---------------------?", {"A", "B", "C", "D"}, 3},
    {"What is E ---------------------?", {"A", "B", "C", "D"}, 4},
    {"What is F ---------------------?", {"A", "B", "C", "D"}, 5}
};

int scoreOne = 0;
int scoreTwo = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void(* resetFunc) (void) = 0;
void ansQuestion();
void resetSystem();
void selectOption(int player,int option);


void setup() {
  
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(playerOneButton, INPUT);
  pinMode(playerOneLed, OUTPUT);
  
  pinMode(playerTwoButton, INPUT);
  pinMode(playerTwoLed, OUTPUT);

  pinMode(nextButton, INPUT);
  pinMode(resetButton, INPUT);
  
}

int currentQ;

void loop() {

  int count = 0;
  
  while(count <3){

    lcd.clear();
    

    currentQ = rand() % (5 + 1 - 0) + 0;


    for(int x=0;x<strlen(questions[currentQ].question);x++){
      if(x==16){
        lcd.setCursor(0,1);
      }
      lcd.write(questions[currentQ].question[x]);
      
    }
    

    while(true){
      nextButtonState = digitalRead(nextButton);

      if (digitalRead(playerOneButton) == HIGH) {  
          
          turnOnLed(1);
          lcd.clear();
          lcd.print("Player 1");
          ansQuestion(1);
        
      } else if (digitalRead(playerTwoButton) == HIGH ) {
          
          turnOnLed(2);
          lcd.clear();
          lcd.print("Player 2");
          ansQuestion(2);
      } 
      if (nextButtonState == HIGH) {
          lcd.clear();
          turnOffLed();
          
          lcd.print("Loading");
          count +=1;
          delay(1000);
          break;
      }
      resetSystem();
      
    }
  }
  lcd.clear();
  lcd.print("Loading Results");
  delay(2000);
  lcd.clear();

  lcd.print(" Player 1: ");
  lcd.print(scoreOne);
  lcd.setCursor(0, 1);
  lcd.print(" Player 2: ");
  lcd.print(scoreTwo);
  while(true){
    resetSystem();
  }
  
}

void ansQuestion(int player){
  delay(1000);
  lcd.clear();

    for (int x=0;x<4;x++){
    
      lcd.print(x+1);
      lcd.print(": ");
      for(int y = 0;y<strlen(questions[currentQ].options[x]);y++){
          lcd.print(questions[currentQ].options[x][y]);

        }
        lcd.print("  ");
    }

  while(true){
    if (digitalRead(optionA_Button) == HIGH) {  
          selectOption(player,0);     
          break;
      } else if (digitalRead(optionB_Button) == HIGH) {
         selectOption(player,1);     
          break;
      } else if (digitalRead(optionC_Button) == HIGH) {
          selectOption(player,2);     
          break;
      }else if (digitalRead(optionD_Button) == HIGH) {
          selectOption(player,3);     
          break;
      }
      resetSystem();
  }
}

void selectOption(int player,int option){

  lcd.clear();
  lcd.print("Option: ");
  lcd.print(option+1);
  lcd.print(" : ");
  lcd.print(questions[currentQ].options[option]);
  if (questions[currentQ].correctOption == option){
    if(player == 1 ){
      scoreOne +=5;
    }else{
      scoreTwo +=5;
    }
  }else{
    if(player == 1 ){
      scoreTwo +=3;
    }else{
      scoreOne +=3;
    }
  }
  delay(3000);
  nextButtonState = 1;
}
void resetSystem(){
  if (digitalRead(resetButton) == HIGH) {
          turnOffLed();
          resetFunc();
  }
}
void turnOnLed(int led) {
    if (led == 1) {
      digitalWrite(playerOneLed, HIGH);
    } else if (led == 2) {
      digitalWrite(playerTwoLed, HIGH);
    }
}

void turnOffLed() {
    digitalWrite(playerOneLed, LOW);
    digitalWrite(playerTwoLed, LOW);
}
