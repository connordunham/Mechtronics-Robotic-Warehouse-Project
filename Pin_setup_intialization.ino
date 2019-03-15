#define LIGHT_THRESHOLD 700 //The threshold that determines whether or not a black line is beneath the sensor
                            //Will require further testing and adjustments
                                              //Turns until any sensor exceeds this threshold then proceeds

#define IR_THRESHOLD  300                    //The point at which the robot really needs slow down going to a wall (~5cm form wall)
#define IR_THRESHOLD2 750                    //When the robot's arm is the farthest it can go towards the wall (0cm from wall)

#define TURN_AJUSTMENT_RATION 0.65           // Ranges from 0.0 - 1.0. The lower the ration the fast the robot turns when it veers to
                                             // to correct its line tracking (ex 0.6 means that one motor speed is reduced to 60% of normal speed)

//Digital inputs/outputs
int Lbumper = 0;
int Rbumper = 1;
int M2 = 4;                //Left motor
int E2 = 5;
int E1 = 6;               //Right motor
int M1 = 7;
int Rencoder = 8;
int Lencoder = 9;
int PAN_servo = 10;
int TILT_servo = 11;
int GRIP_servo = 12;
int IR_receiver = 13;

//Analog inputs/outputs
int R_line_sensor = A0;
int C_line_sensor = A1;
int L_line_sensor = A2;
int force_sensor = A3;
int IR_sensor = A5;


//Initialze all functions here when puttting together all the code
//------------------------------------------------------------------------vv

//------------------------------------------------------------------------^^


void setup() {
  // put your setup code here, to run once:
  pinMode(Lbumper, INPUT);
  pinMode(Rbumper, INPUT);
  pinMode(Lencoder, INPUT);
  pinMode(Rencoder, INPUT);
  pinMode(R_line_sensor, INPUT);
  pinMode(C_line_sensor, INPUT);
  pinMode(L_line_sensor, INPUT);  
  pinMode(R_line_sensor, INPUT);
  pinMode(force_sensor, INPUT);
  pinMode(IR_sensor, INPUT);
  pinMode(IR_receiver, INPUT);
  
  pinMode(M1 , OUTPUT);
  pinMode(M2 , OUTPUT);
  pinMode(E1 , OUTPUT);
  pinMode(E2 , OUTPUT);
  pinMode(TILT_servo , OUTPUT);
  pinMode(GRIP_servo , OUTPUT);
  pinMode(PAN_servo , OUTPUT);

  Serial.mode(9600);


}


//main loop variable initiaizations
//Pickup Ball Variables
//--------------------------
int left_line = 0;
int right_line = 0;
int center_line = 0; 
int IR_sensor_val = 0;
int wall_entry_speed = 0;
//--------------------------



void loop() {
  // put your main code here, to run repeatedly:

}
