#define LIGHT_THRESHOLD 700

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
int IR_sesnor = A5;


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
  pinMode(IR_sesnor, INPUT);
  pinMode(IR_receiver, INPUT);
  
  pinMode( , OUTPUT);
  pinMode( , OUTPUT);
  pinMode( , OUTPUT);
  pinMode( , OUTPUT);
  pinMode(TILT_servo , OUTPUT);
  pinMode(GRIP_servo , OUTPUT);
  pinMode(PAN_servo , OUTPUT);

  Serial.mode(9600);


}

void loop() {
  // put your main code here, to run repeatedly:

}
