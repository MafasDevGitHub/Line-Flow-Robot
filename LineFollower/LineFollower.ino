#include <AFMotor.h>

//assign pins for the motors
AF_DCMotor motor1(3); //1
AF_DCMotor motor2(4); //2
AF_DCMotor motor3(2); //3
AF_DCMotor motor4(1); //4

//ultraSonic Sensor
const int trig = A2;
const int echo = A3;
float duration, distance;

//IR Sensors
const int ir1 = A0;
const int ir2 = A1;
const int ir3 = A4;
const int ir4 = A5;

int right;
int left;
int midRight;
int midLeft;

//buzzer
int buz = 22;
int led =09;

void setup() {
  // put your setup code here, to run once:

  //motor speed
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor3.setSpeed(130);
  motor4.setSpeed(130);

  //ultraSonic
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //IR Sensor
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);

  //buzzer
  pinMode(buz, OUTPUT);

}//void setup end

void loop() {
  // put your main code here, to run repeatedly:

  right = IrValue(ir1);
  midRight = IrValue(ir2);
  midLeft = IrValue(ir3);
  left = IrValue(ir4);
  
  
  if(getObstacle() == LOW){

    if(left == LOW && midLeft == HIGH && midRight == HIGH && right == LOW){
      // 0 1 1 0
      moveForward();
      // light();
    }else if(left == LOW  && midLeft == LOW && midRight == HIGH && right == HIGH){
      // 0 0 1 1
      turnRight();
    }else if(left == HIGH && midLeft == HIGH && midRight == LOW && right == LOW){
      // 1 1 0 0
      turnLeft();
    }else if(left == LOW  && midLeft == HIGH && midRight == HIGH && right == HIGH){
      // 0 1 1 1    *sharp right turn
      sharpRight();
    }else if(left == HIGH  && midLeft == HIGH && midRight == HIGH && right == LOW){
      // 1 1 1 0    *sharp left turn
      sharpLeft();
    }else if(left == LOW  && midLeft == LOW && midRight == LOW && right == HIGH){
      // 0 0 0 1
      turnRight();
    }else if(left == HIGH  && midLeft == LOW && midRight == LOW && right == LOW){
      // 1 0 0 0 
      turnLeft();
    }else if(left == LOW  && midLeft == LOW && midRight == HIGH && right == LOW){
      // 0 0 1 0 
      turnRight();
    }else if(left == LOW  && midLeft == HIGH && midRight == LOW && right == LOW){
      // 0 1 0 0 
      turnLeft();
    }else if(left == HIGH  && midLeft == HIGH && midRight == HIGH && right == HIGH){
      // 1 1 1 1    *loop/turnBack/end
      stop();
      delay(600);

      //stop and check conditions

      //check sensor again
      right = IrValue(ir1);
      midRight = IrValue(ir2);
      midLeft = IrValue(ir3);
      left = IrValue(ir4);

      if(left == LOW  && midLeft == LOW && midRight == LOW && right == LOW ) {
        // 0 0 0 0  *end
        stop();
      }
      else if(left == HIGH  && midLeft == HIGH && midRight == HIGH && right == HIGH ){
        // 1 1 1 1  *return
          
        //move little forward and check conditions again
        moveForward();
        digitalWrite(buz, HIGH);
        delay(230); 

        //check sensor again
        right = IrValue(ir1);
        midRight = IrValue(ir2);
        midLeft = IrValue(ir3);
        left = IrValue(ir4);

        //if 1 1 1 1 turn 180
        if(left == HIGH  && midLeft == HIGH && midRight == HIGH && right == HIGH){
          // 1 1 1 1  *return
          turnBack();
        }else if(left == LOW  && midLeft == LOW && midRight == LOW && right == LOW){
          // 0 0 0 0  *end
          stop();
        }

      }else{
        stop();
      }
    }
    else{
      stop();
    }

  }else if(getObstacle() == HIGH){
    // if detect an obstacle
    digitalWrite(buz, HIGH);
    stop();
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(buz, LOW);

      //check sensor again
      if(getObstacle() == HIGH){
        passObs();
      }

  }

}//void loop end.




//methods to move the robot

// forward method
void moveForward(){
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}
// turn right method
void turnRight(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}
// // turn left method
void turnLeft(){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
//stop method
void stop(){
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
void sharpRight(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(600);
}
void sharpLeft(){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(600);
}
// turn 180
void turnBack(){
  // moveForward();
  // delay(1000);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(1600);  
  // moveForward();
  // delay(1000);
}

//pass obstacle
void passObs(){
  turnLeft();
  delay(550);
  moveForward();
  delay(500);
  turnRight();
  delay(550);

  moveForward();
  delay(900);

  turnRight();
  delay(600);
  moveForward();
  delay(500);
  turnLeft();
  delay(600);
}




//Additional Method Section

int getObstacle(){  //start
  //this method will return the 1 if obstacle is detected

  //trigger the sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  //get the distance
  duration = pulseIn(echo, HIGH);
  distance = (duration*0.0343)/2;

  int obs;
  if(distance <= 25){
    obs = 1;
  }else{
    obs = 0;
  }

  return obs;
} //end


int IrValue(int ir){  //start

  //method will return 0/1 from IR analog signals

  int red = analogRead(ir);
  int value;

  if(red <1000){
    value = 0;
  }else if(red > 1000){
    value = 1;
  }

  return value;
} //end



