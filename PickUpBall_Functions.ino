//*************************************************************
//All relevant definitions and thresholds
#define LIGHT_THRESHOLD 700                   //The threshold that determines whether or not a black line is beneath the sensor
                            //Will require further testing and adjustments
#define LIGHT_THRESHOLD2 700                  // THE this was what all sensors were beneath when neither were over the line
                                              //Turns until any sensor exceeds this threshold then proceeds

#define IR_THRESHOLD  300                    //The point at which the robot really needs slow down going to a wall (~5cm form wall)
#define IR_THRESHOLD2 750                    //When the robot's arm is the farthest it can go towards the wall (0cm from wall)

#define TURN_AJUSTMENT_RATION 0.65           // Ranges from 0.0 - 1.0. The lower the ration the fast the robot turns when it veers to
                                             // to correct its line tracking (ex 0.6 means that one motor speed is reduced to 60% of normal speed)


//*************************************************************
//*************************************************************

//Function Initalizations for the PICK UP BALL function

void pick_up_ball(int IR_sensor_val, int spd, int L, int C, int R)
void trackline(int L, int C, int R, int spd);
void forward_straight(int spd);
void veer(int spd, int dir);
void stop_robot();
void turn(int spd, int dir);
void print_sensor_vals(int sesn1_val, int sens2_val, int sens3_val);

//*************************************************************
//*************************************************************

//main loop variable initiaization
int left_line = 0;
int right_line = 0;
int center_line = 0; 
int IR_sensor_val = 0;
int wall_entry_speed = 0;

//*************************************************************

//*************************************************************
void pick_up_ball(int IR_sensor_val, int spd, int L, int C, int R){

  while(IR_sensor_val< IR_THRESHOLD){
        IR_sensor_val = analogRead(IR_sensor);
          Serial.println(IR_sensor_val);
        trackline(L, C, R, spd);
  }

  stop_robot();
  
  while(IR_sensor_val < IR_THRESHOLD2){
      IR_sensor_val = analogRead(IR_sensor);
        Serial.println(IR_sensor_val);
      trackline(L, C, R, 0.8*spd);
    }

   stop_robot();


   Serial.println("Doing arm pick up");
   delay(5000);
   turn(200, 1, C_line_sensor);
   //***MAYBE ADD WHILE LOOP HERE TO BEARK OUT OF FUNCTION ONCE IT REACH FIRST INTERECTION
   trackline(L, C, R, C);
}




void trackline(int L, int C, int R, int spd){
  R = analogRead(R_line_sensor);
  C = analogRead(C_line_sensor);
  L = analogRead(L_line_sensor);

  print_sensor_vals(R, C, L);

  if(L>LIGHT_THRESHOLD  && C>LIGHT_THRESHOLD && R>LIGHT_THRESHOLD){   //Stops the robot for a couple seconds when ever it crosses and intersection
    stop_robot();
    delay(3000);        
   
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
    analogWrite(E1, spd);
    analogWrite(E2, spd*);  // reduces speed of left motor to veer left
  }else{                       //only other direction to veer is right if dir is not 1
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, spd*0.7); // reduces speed of right motor to veer right
    analogWrite(E2, spd); 
  }
}

//**** Void turn was from one of the other groups code on their GIT respository
  void turn(int spd, int dir, int Csensor){//dir = 1 is left, dir = 0 is right
  int C = 0;
  
  delay(50);
  analogWrite(E2, 100);
  analogWrite(E1, 100);
  digitalWrite(M2, HIGH);
  digitalWrite(M1, HIGH);
  delay(700);
  if(dir == 1){
     analogWrite(E2, spd);
     analogWrite(E1, spd);
     digitalWrite(M2, LOW);
     digitalWrite(M1, HIGH);
  }
  else{
    analogWrite(E2, spd);
     analogWrite(E1, spd);
     digitalWrite(M2, HIGH);
     digitalWrite(M1, LOW);
  }

     C = analogRead(Csensor);
   while(C<LIGHT_THRESHOLD){
      C = analogRead(Csensor);
      analogWrite(E2, spd);
      analogWrite(E1, spd);
      if(dir = 1){
        digitalWrite(M1, LOW);
        digitalWrite(M2, HIGH);
      }else{
        digitalWrite(M2, HIGH);
        digitalWrite(M1, LOW);
      }
    
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

//Takes in the values of threes sensors and prints them legibly in Serial monitor
void print_sensor_vals(int sesn1_val, int sens2_val, int sens3_val){
  Serial.print(sesn1_val);
  Serial.print("  |  ");
  Serial.print(sens2_val);
  Serial.print("  |  ");
  Serial.print(sens3_val);
  Serial.println(" ");
}
//*************************************************************
  
