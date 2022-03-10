/*
   B31DG Embedded Software 2022
   Assignment 2
   Hannah Mokhberi
   10/03/2022

   Code description:
   This code implements a simple Cyclic Executive for the ESP32 microcontroller using C++.
   The cyclic executive is used to cycle through a repeating sequence of activities, at a set frequency.
   The system must executes the following periodic tasks at the specified frequencies:

       Task1_period:40ms____ticks:2
       Task2_period:200ms___ticks:10
       Task3_period:1000ms__ticks:51
       Task4_period:0ms_____ticks:2
       Task5_period:40ms____ticks:2
       Task6_period:100ms___ticks:5
       Task7_period:320ms___ticks:16
       Task8_period:320ms___ticks:16
       Task9_period:5000ms__ticks:250

    The "ticks" indicate the number of clock cycles elapsed.
*/


unsigned int ticks; //number of clock cycles passed
byte cycle = 20;    //duration of a clock cycle

byte dog_GPIO = 15; //output watchdog waveform for task1

byte SW1_GPIO = 22; //digital input (i.e. switch)
bool state;         //state of the switch

byte POT_GPIO = 33;
unsigned short int analg_val;

unsigned short int new_analg_val ;
unsigned short int filter_analg_val;

bool error_code;

byte LED_GPIO = 16;

//task3 variables
byte square_GPIO = 34;
bool pulse;
float period;
int frequency;
unsigned long startTime;
unsigned long endTime;
unsigned long beginwave;


/*************************************************TASK FUNCTIONS*************************************************/

//Task1 - Output digital watchdog waveform
void Task1() {
  digitalWrite(dog_GPIO, HIGH);
  delayMicroseconds(50);        //pulse width
  digitalWrite(dog_GPIO, LOW);
}

//Task2 - Monitor switch state
void Task2() {
  state = digitalRead(SW1_GPIO);
}

//Task3 - Measure square wave's frequency within range 500-1000Hz
void Task3() {
  pulse = digitalRead(square_GPIO); //read pin
  beginwave = micros();
  if (pulse == 1)
    while (pulse == 1) {
      startTime = micros() - beginwave;
      pulse = digitalRead(square_GPIO);
    }

  while (pulse == 0) {
    endTime = micros() - beginwave;
    pulse = digitalRead(square_GPIO);
  }

  if (pulse == 0)
    while (pulse == 0) {
      startTime = micros() - beginwave;
      pulse = digitalRead(square_GPIO);
    }

  while (pulse == 1) {
    endTime = micros() - beginwave;
    pulse = digitalRead(square_GPIO);
  }

  period = (endTime - startTime) * 0.000001;
  //Serial.println(period);
  frequency = 1 / period;

}

//Task4 - Read analogue input using pot
void Task4() {
  analg_val = analogRead(POT_GPIO);
}

//Task5 - Compute filtered analogue value
void Task5() {
  new_analg_val = 0;
  for (int i = 0; i <= 3; i++) {
    Task4();
    new_analg_val = new_analg_val + analg_val;
  }
  filter_analg_val = new_analg_val / 4;
}

//Task6 - Execute instruction 1000 times
void Task6() {
  for (int i = 0; i <= 999; i++) { //repeat 1000 times
    __asm__ __volatile__ ("nop");
  }
}

//Task7 - Perform error check
void Task7() {
  if (filter_analg_val > (0.5 * 4095)) {
    error_code = 1;
  }
  else {
    error_code = 0;
  }
}

//Task8 - Visualise error_code using an LED
void Task8() {
  if (error_code == 1) {
    digitalWrite(LED_GPIO, HIGH);
  }
  else {
    digitalWrite(LED_GPIO, LOW);
  }
}

//Task9 - Log information to the serial port in CSV
void Task9() {
  Serial.print(state); Serial.print(","); Serial.print(frequency); Serial.print(","); Serial.println(filter_analg_val); //deactivate plausibility check
}

void setup() {
  Serial.begin(9600);
  pinMode(dog_GPIO, OUTPUT);
  pinMode(SW1_GPIO, INPUT);
  pinMode(square_GPIO, INPUT);
  pinMode(POT_GPIO, INPUT);
  pinMode(LED_GPIO, OUTPUT);
}
/*************************************************CYCLIC EXECUTIVE*************************************************/
void loop() {
  delay(cycle); ticks++;  //duration of 1 clock cycle is 20ms

  if (ticks % 2 == 0) { // if 2 clock cycles have been completed
    Task1(); //call Task1
    //delayMicroseconds(50);//plausibility check
    //Serial.print("task1_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }

  if (ticks % 10 == 0) {// if 10 clock cycles have been completed
    Task2(); //call Task2
    //delayMicroseconds(0.221);//plausibility check
    //Serial.print("task2_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }

  if (ticks % (50 + 1) == 0) { //Task3 has been shifted forward by 1 clock cycle, so if 51 clock cycles have been completed
    Task3(); //call task3
    //delay(15);//plausibility check
    //Serial.print("task3_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check

  }

  if (ticks % 2 == 0) {// if 2 clock cycles have been completed
    Task4(); //call Task4
    //delayMicroseconds(60);//plausibility check
    //Serial.print("task4_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }

  if (ticks % 2 == 0) { // if 2 clock cycles have been completed
    Task5(); //call Task5
    //delay(0.239966);//plausibility check
    //Serial.print("task5_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }

  if (ticks % 5 == 0) {
    Task6(); //call Task6
    //delayMicroseconds(25.205);//plausibility check
    //Serial.print("task6_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }
  if (ticks % 16 == 0) { // if 16 clock cycles have been completed
    Task7(); //call Task7
    //delayMicroseconds(0.381);//plausibility check
    //Serial.print("task7_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }

  if (ticks % 16 == 0) { // if 16 clock cycles have been completed
    Task8(); //call Task8
    //delayMicroseconds(0.263);//plausibility check
    //Serial.print("task8_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }

  if (ticks % 250 == 0) { // if 250 clock cycles have been completed
    Task9(); //call Task9
    //delay(7);//plausibility check
    //Serial.print("task9_"); Serial.print("time_"); Serial.print(millis()); Serial.print("tick_"); Serial.println(ticks); //plausibility check
  }
}
