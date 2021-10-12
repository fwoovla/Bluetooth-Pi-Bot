//nano i2c wheel motor controller


#include <Wire.h>


const int16_t I2C_MASTER = 0x42;
//const int16_t MOTOR_ADDR = 0x08; // 0x08 LEFT   0x09 RIGHT;
const int16_t MOTOR_ADDR = 0x08;


#define L_OUT_A    7
#define L_OUT_B    8
#define L_OUT_PWM  9
#define R_OUT_A    4
#define R_OUT_B    5
#define R_OUT_PWM  6

//typedef struct wheel_message { //4 bytes each
//    float dir;
//    float pwm;
//    float rpm;
//    
//} wheel_message;
//
//wheel_message wheel_data;

int l_pwm = 0;
int l_dir = 0;   //-1,0,1
int r_pwm = 0;
int r_dir = 0;


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  Wire.begin(8);        // join i2c bus with address #8
  Wire.onRequest(on_request); // register event  
  Wire.onReceive(on_receive);

  init_pins();

}

long last_check = 0;
int check_time = 50;

int dir = 0;

bool stick_left = false;
bool stick_right = false;
bool stick_up = false;
bool stick_down = false;
bool stick_centered = true;

int button = 0;
int tmp_value = 0;
int y_value = 0;
int x_value = 0;
const int VEC_MAX = 5;
float data_msg[VEC_MAX] = {0,0,0,0,0};

void loop() {

  if(millis() - last_check > check_time) {
    last_check = millis();
    read_bt_input();
  }
}


void read_bt_input() {

  
  stick_left = false;
  stick_right = false;
  stick_up = false;
  stick_down = false;


  l_dir = 0;
  l_pwm = 0;
  r_dir = 0;
  r_pwm = 0;

  if(button == 0) {
    y_value = map(tmp_value,0, 255, 255, -255);
    //Serial.println(y_value);
  }
  if(y_value > 5) {
    stick_up = true;
    //Serial.println("stick up");
  }
  if(y_value < -5) {
    stick_down = true; 
  }
  
  if(button == 1) {
    x_value = map(tmp_value,0, 255, 255, -255);
    //Serial.println(x_value);
  }
  if(x_value > 5) {
    stick_right = true;
  }
  if(x_value < -5) {
    stick_left = true;
  }

  if(stick_up) {
    l_dir = 1;
    r_dir = 1;
    //Serial.println("stick up");
    if(stick_right) {
      //Serial.println("stick right");
      l_pwm = y_value + x_value;
      r_pwm = y_value - x_value;
    }
    if(stick_left) {
      //Serial.println("stick left");
      l_pwm = y_value + x_value;
      r_pwm = y_value - x_value;
    }
    else {
      l_pwm = y_value;
      r_pwm = y_value;
    }
  }
  else if(stick_down) {
    l_dir = -1;
    r_dir = -1;
    //Serial.println("stick down");
    if(stick_right) {
      //Serial.println("stick right");
      l_pwm = abs(y_value) + x_value;
      r_pwm = abs(y_value) - x_value;      
    }
    if(stick_left) {
      //Serial.println("stick left");
      l_pwm = abs(y_value) + x_value;
      r_pwm = abs(y_value) - x_value;        
    }
    else {
      l_pwm = abs(y_value);
      l_pwm = abs(y_value);
    }
  }

  else if (stick_left) {
    l_dir = -1;
    r_dir = 1;
    l_pwm = abs(x_value);
    r_pwm = abs(x_value);
  }
  else if (stick_right) {
    l_dir = 1;
    r_dir = -1;
    l_pwm = abs(x_value);
    r_pwm = abs(x_value);
  }

  l_pwm = map(l_pwm, 0, 255, 130, 255);
  if(l_pwm < 195) {
    l_pwm = 0;
  }
  
  r_pwm = map(r_pwm, 0, 255, 130, 255);
  if(r_pwm < 195) {
    r_pwm = 0;
  }    
  Serial.println(l_pwm);
  Serial.println(r_pwm);
//  Serial.println(l_dir);
//  Serial.println(r_dir);
  cmd_motors();
}



void cmd_motors() {

  
  analogWrite(L_OUT_PWM, l_pwm);
  analogWrite(R_OUT_PWM, r_pwm);

  if(l_dir == 1) {
    digitalWrite(L_OUT_A, HIGH);
    digitalWrite(L_OUT_B, LOW);
  }
  if(l_dir == -1) {
    digitalWrite(L_OUT_A, LOW);
    digitalWrite(L_OUT_B, HIGH);
  }
  if(l_dir == 0) {
    digitalWrite(L_OUT_A, LOW);
    digitalWrite(L_OUT_B, LOW);
  }

  if(r_dir == 1) {
    digitalWrite(R_OUT_A, HIGH);
    digitalWrite(R_OUT_B, LOW);
  }
  if(r_dir == -1) {
    digitalWrite(R_OUT_A, LOW);
    digitalWrite(R_OUT_B, HIGH);
  }
  if(r_dir == 0) {
    digitalWrite(R_OUT_A, LOW);
    digitalWrite(R_OUT_B, LOW);
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()

void on_request() {
  Wire.write((uint8_t*) data_msg, sizeof(data_msg));
}


void on_receive(size_t howMany) {
  
  (void) howMany;
//  while (1 < Wire.available()) { // loop through all but the last
//    char c = Wire.read(); // receive byte as a character
//    Serial.print(c);         // print the character
//  }
  button = Wire.read();    // receive byte as an integer
  tmp_value = Wire.read();
  //Serial.println(x);         // print the integer
  //Serial.println(_button);
  //Serial.println(_value);
}  


void init_pins() {
  pinMode(L_OUT_A, OUTPUT);
  pinMode(L_OUT_B, OUTPUT);
  pinMode(L_OUT_PWM, OUTPUT);
  pinMode(R_OUT_A, OUTPUT);
  pinMode(R_OUT_B, OUTPUT);
  pinMode(R_OUT_PWM, OUTPUT);
}
