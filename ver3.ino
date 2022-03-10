/*
  Note:
  1. high level description (e.g. at top) describing overall behaviour of the code.
  2. add task 3
  3. chnage task 9 to contain task 3
*/

/*
  Task 1 every 40ms
  Task 2 every 200ms
  Task 3 every 1000ms
  Task 4 every 40ms
  Task 5 every 40ms
  Task 6 every 100ms
  Task 7 every 320ms
  Task 8 every 320ms
  Task 9 every 5000ms
*/

unsigned int ticks = 0;

byte dog_GPIO = 15; //pin 15 is outputting watchdog waveform

byte SW1_GPIO = 22; //switch connected to pin 22
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
float frequency;
unsigned long startTime;
unsigned long endTime;
unsigned long beginwave;


//Task FUNCTIONS

//Task1 - Output digital watchdog waveform
void Task1() {
  digitalWrite(dog_GPIO, HIGH);
  delayMicroseconds(50);
  digitalWrite(dog_GPIO, LOW);
}

//Task2 - Monitor switch state
void Task2() {
  state = digitalRead(SW1_GPIO);
}

//Task3 - square wave
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
  //Serial.print(state); Serial.print(","); Serial.print(frequency); Serial.print(","); Serial.println(filter_analg_val); //remeber to change this accourding to task 3
}

void setup() {
  Serial.begin(9600);
  pinMode(dog_GPIO, OUTPUT);
  pinMode(SW1_GPIO, INPUT);
  pinMode(square_GPIO, INPUT);
  pinMode(POT_GPIO, INPUT);
  pinMode(LED_GPIO, OUTPUT);
}

void loop() {
  delay(20); ticks++;  //duration of 1 clock cycle is 20ms

  if (ticks % 2 == 0) { // if 2 clock cycles have been completed
    Task1(); //call Task1
    
    
  }

  if (ticks % 10 == 0) {
    Task2(); //call Task2
  }
  if (ticks % 50 == 1) {//shift task3 forward by 1 clock cycle
    Task3(); //call task3
    Serial.print("task1");Serial.print(millis());Serial.println(ticks);
  }

  if (ticks % 2 == 0) {
    Task4(); //call Task4
  }

  if (ticks % 2 == 0) {
    Task5(); //call Task5
  }

  if (ticks % 5 == 0) {
    Task6(); //call Task6
  }
  if (ticks % 16 == 0) {
    Task7(); //call Task7
  }

  if (ticks % 16 == 0) {
    Task8(); //call Task8
  }

  if (ticks % 250 == 0) {
    Task9(); //call Task9
  }

}
