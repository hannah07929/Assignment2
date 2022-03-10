/*

*/
unsigned long Si; //start time
unsigned long Fi; //finish time
unsigned long Ci; //duration

//task 2 variables
byte SW1_GPIO = 22; //switch connected to pin 22
bool state;         //state of the switch

//task 4 variables
byte POT_GPIO = 33;
unsigned short int analg_val;

//task 5 variables
unsigned short int new_analg_val ;
unsigned short int filter_analg_val;

//task 7 variables
bool error_code;

//task 8 variables
byte LED_GPIO = 16;

void setup() {
  Serial.begin(9600);
  pinMode(SW1_GPIO, INPUT);  // Switch is an input
  pinMode(POT_GPIO, INPUT);  // Switch is an input
  pinMode(LED_GPIO, OUTPUT);  // Switch is an input
}

void loop() {

  //task2:
  state = digitalRead(SW1_GPIO);

  //task5:
  new_analg_val = 0 ;
  for (int i = 0; i <= 3; i++) {
    analg_val = analogRead(POT_GPIO);     //task4:
    //Serial.println(analg_val );
    new_analg_val = new_analg_val + analg_val;
  }
  filter_analg_val = new_analg_val / 4;

  //task6:
  for (int i = 0; i <= 999; i++) { //repeat 1000 times
    __asm__ __volatile__ ("nop");
  }

  //task7:
  if (filter_analg_val > (0.5 * 4095)) {
    error_code = 1;
  }
  else {
    error_code = 0;
  }
  //Serial.println(error_code);

  //task8:
  digitalWrite(LED_GPIO, LOW);
  if (error_code == 1) {
    digitalWrite(LED_GPIO, HIGH);
  }

  //Task9:
  Serial.print(state); Serial.print(","); Serial.print(error_code); Serial.print(","); Serial.println(filter_analg_val);




  
/* start measuring time 
  Si = micros(); //start timer
    for (int i = 0; i <= 999; i++) //repeat 1000 times
    {
      //start task here
  
  
      //endtask here
    }
  Fi = micros(); //end timer
  Ci = (Fi - Si); //calculate duration
  Serial.println(Ci); //display duration
end measuring time*/





}
/*
   Task1 = 50000
   Task2 = 221
   Task3 = ?
   Task4 = 59999
   Task5 = 239966
   Task6 = 25205
   Task7 = 381
   Task8 = 263
   Task9 = 7024
*/
