#include <Servo.h>

//All relevant definitions and thresholds
#define LIGHT_THRESHOLD 1000                   //The threshold that determines whether or not a black line is beneath the sensor
                            //Will require further testing and adjustments
#define LIGHT_THRESHOLD2 900                  // THE this was what all sensors were beneath when neither were over the line
                                              //Turns until any sensor exceeds this threshold then proceeds

#define IR_THRESHOLD  300                    //The point at which the robot really needs slow down going to a wall (~5cm form wall)
#define IR_THRESHOLD2 750                    //When the robot's arm is the farthest it can go towards the wall (0cm from wall)

#define TURN_AJUSTMENT_RATION 0.65           // Ranges from 0.0 - 1.0. The lower the ration the fast the robot turns when it veers to
                                             // to correct its line tracking (ex 0.6 means that one motor speed is reduced to 60% of normal speed)

#define REV_FROM_WALL_DIST  3                // Number of encoder ticks before we are the right distance from the vall to pick up dice
#define REV_FROM_WALL_SPD   90                // A slow enough speed to ensure encoders are being properly read
#define LEFT_ADJUST   1
#define RIGHT_ADJUST  1.1


Servo panS, tiltS, gripS; //Create the 3 servo motor objects

//Digital inputs/outputs
int Lbumper = 11;
int Rbumper = 0;          //For some reason it only works with pin 11 for team 10s robot
int M2 = 4;                //Left motor
int E2 = 5;
int E1 = 6;               //Right motor
int M1 = 7;
int Rencoder = 3;         //Removed from robot for now
int Lencoder = 2;         //Removed from robot for now
int PAN_servo = 8;  
int TILT_servo = 9;
int GRIP_servo = 10;
int IR_receiver = 12;
int LED = 13;

//Analog inputs/outputs
int R_line_sensor = A0;
int C_line_sensor = A2;
int L_line_sensor = A1;
int fsrPin = A3;
int IR_sensor = A5;


//Initialze all functions here when puttting together all the code
//------------------------------------------------------------------------vv
void pick_up_ball(int IR_sensor_val, int spd, int L, int C, int R, int ACTION);
void trackline(int L, int C, int R, int spd);
void forward_straight(int spd);
void veer(int spd, int dir);
void stop_robot();
void turn(int spd, int dir);
void print_sensor_vals(int sesn1_val, int sens2_val, int sens3_val);
int check_bumper(int left_bumperPin, int right_bumperPin);
//------------------------------------------------------------------------^^


void setup() {
  // put your setup code here, to run once:
  pinMode(Lbumper, INPUT);
  pinMode(Rbumper, INPUT);
  pinMode(Lencoder, INPUT);
  pinMode(Rencoder, INPUT);
  pinMode(C_line_sensor, INPUT);
  pinMode(L_line_sensor, INPUT);  
  pinMode(R_line_sensor, INPUT);
  pinMode(fsrPin, INPUT);
  pinMode(IR_sensor, INPUT);
  pinMode(IR_receiver, INPUT);
  
  pinMode(M1 , OUTPUT);
  pinMode(M2 , OUTPUT);
  pinMode(E1 , OUTPUT);
  pinMode(E2 , OUTPUT);
  pinMode(TILT_servo , OUTPUT);
  pinMode(GRIP_servo , OUTPUT);
  pinMode(PAN_servo , OUTPUT);

  panS.attach(8);
  tiltS.attach(9);
  gripS.attach(10);
  Serial.begin(9600); 
  pinMode(LED, OUTPUT);

  Serial.begin(9600);


}


//main loop variable initiaizations
//Pickup Ball Variables
//--------------------------
int left_line = 0;
int right_line = 0;
int center_line = 0; 
int IR_sensor_val = 0;
int wall_entry_speed = 100;
int fsrReading = 0;         //force sensor reading
int Lbumper_val = 0;
int Rbumper_val = 0;
int encoder_state = 0;
int encoder_state_prev = digitalRead(Rencoder);
int action_pickup = 0;      //Tell robot to drop ball (0) or pick up ball (1)
//--------------------------

//Main loop
//****************************************************************************
//****************************************************************************

void loop() {
  action_pickup = 1;
  
  pick_up_ball(IR_sensor, wall_entry_speed, L_line_sensor, C_line_sensor, R_line_sensor, action_pickup);


  delay(5000);
  
  action_pickup = 0;

  pick_up_ball(IR_sensor, wall_entry_speed, L_line_sensor, C_line_sensor, R_line_sensor, action_pickup);
}
//****************************************************************************
//****************************************************************************


//ACTION: 1 is pick up ball and 0 is drop off ball
void pick_up_ball(int IR_sensor_val, int spd, int L, int C, int R, int ACTION){

  int R_sensor_val = analogRead(R);
  int C_sensor_val = analogRead(C);
  int L_sensor_val = analogRead(L);

   //Uses bumpers to detect once robot hits wall
  //Using bumpers to detect wall
  Lbumper_val = digitalRead(Lbumper);
  Rbumper_val = digitalRead(Rbumper);

  while(1){

    Serial.print(Lbumper_val);
    Serial.print("  |  ");
    Serial.print(Rbumper_val);
    Serial.println("");
  
    Lbumper_val = digitalRead(Lbumper);
    Rbumper_val - digitalRead(Rbumper);
    trackline(L, C, R, spd);
    delay(50);
    //Bumpers are hit once at wall and allow robot to enter pick up manuever
    if( Lbumper_val == 0  || Rbumper_val == 0){
    break;
  }
}
  reverse_dist(REV_FROM_WALL_DIST, REV_FROM_WALL_SPD);
    // We are at the wall
   if(ACTION == 1){
   stop_robot();
 
   PickUpBall();
   }else{
      dropBall(1);
   }
   delay(1000);
   turn(100, 1, C_line_sensor);
   //***MAYBE ADD WHILE LOOP HERE TO BEARK OUT OF FUNCTION ONCE IT REACH FIRST INTERECTION
   while(1){
    trackline(L, C, R, spd);
    
    L_sensor_val = analogRead(L);
    C_sensor_val = analogRead(C);
    R_sensor_val = analogRead(R);
    
    if( L_sensor_val > LIGHT_THRESHOLD && C_sensor_val > LIGHT_THRESHOLD && R_sensor_val > LIGHT_THRESHOLD){
        break;
    }
  }
}

void trackline(int L, int C, int R, int spd){
  int R_line_val = analogRead(R);
  int C_line_val = analogRead(C);
  int L_line_val = analogRead(L);
  
   print_sensor_vals(L_line_val, C_line_val, R_line_val);
  

  if(L>LIGHT_THRESHOLD  && C>LIGHT_THRESHOLD && R>LIGHT_THRESHOLD){   //Stops the robot for a couple seconds when ever it crosses and intersection
    stop_robot();
    delay(1000);  
    forward_straight(spd);      
   
  }
  else if(C>R && C>L && C>LIGHT_THRESHOLD*0.7){   //If Center sesnor has the highest value and its large enough to means its over a light go straight
    forward_straight(spd);
  }
  else if(L>C && L>R){          //If --> Left Sensor has the highest value turn left
    veer(spd, 0);   //Veer Left
  }
  else if(R>C && R>L){         ////If --> right Sensor has the highest value turn left
    veer(spd, 1);   //Veer Right
  }
  }


//Sets robot to drive in a straight direction at a designated speed
void forward_straight(int spd){
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, spd);
    analogWrite(E2, spd);
}

//Makes minor adjustments to direction by reducing left or right motor speeds
void veer(int spd, int dir){
  //For direction 1 = left and 0 = right

  //Veering left if dir == 1
  if(dir == 1){
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, spd*RIGHT_ADJUST);
    analogWrite(E2, spd*TURN_AJUSTMENT_RATION*LEFT_ADJUST);  // reduces speed of left motor to veer left
  }else{                       //only other direction to veer is right if dir is not 1
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, spd*TURN_AJUSTMENT_RATION*RIGHT_ADJUST); // reduces speed of right motor to veer right
    analogWrite(E2, spd*LEFT_ADJUST); 
  }
}

//**** Void turn was from one of the other groups code on their GIT respository
  void turn(int spd, int dir, int Csensor){//dir = 1 is left, dir = 0 is right
  int C = 0;
  
  //Goes past the the insersection a little bit so it can performa a 90 degree turn
  // Required adjust based of differing speed
  
  // Not needed for pick up ball -------------------------------------------
  /*delay(50);
  analogWrite(E2, 100);
  analogWrite(E1, 100);
  digitalWrite(M2, HIGH);
  digitalWrite(M1, HIGH);
  delay(700);*/
   //-------------------------------------------------------------------------
  if(dir == 1){
     analogWrite(E2, spd);
     analogWrite(E1, spd);
     digitalWrite(M2, LOW);
     digitalWrite(M1, HIGH);
     delay(700);
  }
  else{
    analogWrite(E2, spd);
     analogWrite(E1, spd);
     digitalWrite(M2, HIGH);
     digitalWrite(M1, LOW);
     delay(700);
  }

     C = analogRead(Csensor);
   while(C<LIGHT_THRESHOLD){
      Serial.println("HERE");
      Serial.println(C);
      
      analogWrite(E2, spd);
      analogWrite(E1, spd);
      if(dir = 1){
        digitalWrite(M2, LOW);
        digitalWrite(M1, HIGH);
      }else{
        digitalWrite(M2, HIGH);
        digitalWrite(M1, LOW);
      }
     C = analogRead(Csensor);
    
    }
    analogWrite(E2, 0);
    analogWrite(E1, 0);

}

//stop the robot motors immidiately
void stop_robot(){
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, 0);
  analogWrite(E2, 0);
}

//Reveses the robot after hitting the wall using the encoders
//Allows robot to reach a percise distance from the wall
void reverse_dist(int dist, int spd){
    int count = 0;
    
    while(count < dist){
        encoder_state_prev = encoder_state;
        encoder_state = digitalRead(Rencoder);
        
        if( encoder_state != encoder_state_prev){
          count++;
        }
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        analogWrite(E1, spd);
        analogWrite(E2, spd);
    }
}
    

//Takes in the values of threes sensors and prints them legibly in Serial monitor
void print_sensor_vals(int sesn1_val, int sens2_val, int sens3_val){
  Serial.print(sesn1_val);
  Serial.print("  |  ");
  Serial.print(sens2_val);
  Serial.print("  |  ");
  Serial.print(sens3_val);
  Serial.println(" ");
}


//Check state of both bumpers and returns:
//3 --> Both bumpers hit    2--> Right bumper hit    1--> Left Bumper hit   0-->no bumper hit
/*
int check_bumper(int left_bumperPin, int right_bumperPin){
  int bumper_state = 0;
    
  L_bumperState = digitalRead(left_bumperPin);
  R_bumperState = digitalRead(right_bumperPin);
  
  if(L_bumperState || R_bumperState){             //Delays for a a moment to see if both bumpers were actually be meant to hit
    delay(500);
    if(L_bumperState && R_bumperState){
      bumper_state = 3;
    }else if(L_bumperState){
      bumper_state = 2;
    }else if(R_bumperState){
      bumper_state = 1;
    }else{
      bumper_state = 0;
    }
    return bumper_state;
  }
  return bumper_state;        //Bumper was not triggeed and never entered if statements so it should be a zero
}
}
*/

void center(){
  panS.write(110);
  gripS.write(0);
  for(int i = 160; i> 10; i--){
    tiltS.write(i);
    delay(35);
  }

}

void PickUpBall(){
  int grippingBOOL = 0;
  
  center();
  while (!grippingBOOL){
    grippingBOOL = beginGripping(); //1 griped 0 not gripped
  }
}

int beginGripping(){
  //int fsrReading = 0;
  //center(); // i moved rgus
  for(int i = 0; i<90; i++){
    gripS.write(i);
    fsrReading = analogRead(fsrPin);  
    delay(60);
    

    if(fsrReading>850){
      tiltS.write(50);
      return 1;
    }
    if (i >= 89){
      i = 0;
      return 0;
    }
  }
}

int dropBall(int doItNow){
  if (doItNow == 1){
    
      
      gripS.write(0);
      delay(20);
    
    return 0;
  }
}

