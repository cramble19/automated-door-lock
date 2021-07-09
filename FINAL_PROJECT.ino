#include <Keypad.h>
#include <Servo.h>
#include<String.h>
int pos = 0,i=0,apin,button;
 String pass="66";
 int mag_value;
Servo myservo;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
String temp_pass="";
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {10, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
int servo_state=0;
//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  pinMode(12,INPUT);//button for opening door
  Serial.begin(9600);
   myservo.attach(9);
   myservo.write(180);//servo_state=0 signifies 180 degree position aka Lock is open
   pinMode(11,OUTPUT);//relay
   digitalWrite(11,LOW);
}
  
void loop(){
 
  delay(50);
 i=0;
char key;
 temp_pass="";
  

  //Mechanism to operate door as per inputs

  
while(i!=2){
  delay(50);
  //If arduino operated with battery then goto servo operation else continue
  
    checkPower:{
      apin=analogRead(A1);
      Serial.print("Value of voltage from Vin: ");
      Serial.println(apin);
  if(apin>=500)
  {
    delay(50);
    
    
    apin=analogRead(A1);
    if(apin>=500){
    servo_operation();
    goto checkPower;
    delay(3000);}
  }
  }

//Using button at back

  button=digitalRead(12);
  Serial.print("Button: ");
Serial.println(button);


  if(button==1){
     but_func();
   
  }
  // Checking for the mc38 Sensors
  
  mag_value=analogRead(A0);
  Serial.print("magnetic value analog: ");
  Serial.println(mag_value);
  if(mag_value>1010 && servo_state==0){
    
    mag_door();
    
  }
  if(temp_pass==pass){
    Serial.println("Magnetic door detetcted");
    break;
  }

  //Checking inputs from keypad
  
   key = keypad.getKey();
   if(key=='A'){
    Serial.println("Reset!");
    break;
   }
  if(key){
     Serial.println("Button Pressed.");
    temp_pass+=key;
  i++;
  }
}

Serial.print("Password Till Now: ");
Serial.println(temp_pass);
  if(pass==temp_pass){
    Serial.println("Pass macthed");
    servo_operation();
    }
  else{
    Serial.println("Wrong Password, Get lost you Theif!");
  }
  
}

// Function to check for mc38 response

void mag_door(){
  Serial.println("Enter into magnetic door function");
 int k=0;
 while(k!=5){
    mag_value=analogRead(A0);

  if(mag_value>1010){
    k++;
    delay(20);
  }
  else{
    k=0;
    break;
  }
 }
 if(k==5){
   Serial.print("Mag Door Detected: ");
   if(servo_state==0){
    delay(150);
    servo_operation();
   }

 }
}
void but_func(){
  
   Serial.println("Enter into button door function");
 int k=0;
 while(k!=5){
    button=digitalRead(12);

  if(button==1){
    k++;
    delay(10);
  }
  else{
    k=0;
    break;
  }
 }
 if(k==5){
  Serial.println("BUTTON DETECTED");
   
   
 servo_operation();
 delay(200);
 }
}
void servo_operation()
{
  Serial.println("Entered servo operation");
  apin=analogRead(A1);
  if((servo_state==000)&& (apin<=500)){// the lock will close only when it is powered by 5V DC Micro USB and not when powered by battery
      
       for (pos = 180; pos >= 85; pos -= 2) { // goes from 180 degrees to 85 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15ms for the servo to reach the position
                        // to delay the loop so that the magnetic door sensor does not sense the door and close the lock
  }
  delay(100);
  servo_state=1;
  Serial.println("servo state changed to 1 in so");
  delay(3000); 
    }
        else if(servo_state==1){
       for (pos = 85; pos <= 180; pos += 2) { // goes from 85 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15ms for the servo to reach the position
  }
  delay(100);
  servo_state=0;
  Serial.println("servo state changed to 0 in sMo");
 delay(3000);
    }
}
