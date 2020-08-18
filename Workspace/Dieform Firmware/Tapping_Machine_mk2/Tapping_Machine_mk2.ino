// Program for Dieform Tapping machine for dynamig
// Gagan: start aug 17 2018  end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Relays
const int up = 6;        
const int side =  7;     
const int clamp =  3;   
const int unclamp = 2;     
const int switch1 =  11;

const int extra1 = 8;
const int extra2 =  12;
const int extra3 =  13;  

//Sensors
const int p_up_out =  A9; 
const int p_up_in =  A8;  
const int p_side_out =  A1;
const int p_side_in =  A0;

const int partInput =  A6;     
const int p_clamp =  A3;  
const int p_unclamp =  A4;
const int p_head_out =  A7;  
const int p_head_in =  A5;   
   

//variables for sensors
int part = 0;
int Pclamp = 0;
int Punclamp = 0;
int PupOut = 0;
int PupIn = 0;
int PsideOut = 0;
int PsideIn = 0;
int PheadOut = 0;
int PheadIn = 0;
int state = 0;
       


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  //initialize inut
  pinMode(p_up_out, INPUT);
  pinMode(p_up_in, INPUT);
  pinMode(p_side_out, INPUT);
  pinMode(p_side_in, INPUT);
  pinMode(p_clamp, INPUT);
  pinMode(p_unclamp, INPUT);
  pinMode(p_head_out, INPUT);
  pinMode(p_head_in, INPUT);
  pinMode(part, INPUT);
  
  //initialize output
  pinMode(up, OUTPUT);
  pinMode(side, OUTPUT);
  pinMode(extra1, OUTPUT);
  pinMode(clamp, OUTPUT);
  pinMode(unclamp, OUTPUT);
  pinMode(switch1, OUTPUT);

  //turn all relays off
  relaysOff();

  //turn on comunicaiton
  Serial.begin(9600);

  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  readSensors();

  
  if (part>700 && PsideOut>100 && PupOut>100 && PheadIn>700 && state==0){                    //check if part is loaded 
    delay(500);
    readSensors();
    if (part>700){
      state = 1;
      Serial.println(state);
    }
  }


  if(Punclamp>700 && PheadIn>700 && PupOut>100  && state==1 ){              //birng part to clamp
    digitalWrite(side,LOW);
    state = 2;
    Serial.println(state);
  }

  if (PupOut>100 && PsideIn>100 && Punclamp>700 && state==2){             //clamp condition 
    delay(300);
    digitalWrite(unclamp,HIGH);
    digitalWrite(clamp,LOW);
    state = 3;
    Serial.println(state);
  }


//make return2home func
  if(Punclamp<700 && PheadIn>700 && state==3){                                          //return to home Pclamp==1 &&
    delay(300);
    digitalWrite(up,LOW);     //brin up low
    digitalWrite(switch1,LOW);//bring switch down
    state = 4;
    Serial.println(state);
  }
  if(PupIn>100 && state==4){                  //return to home
    digitalWrite(side,HIGH);    //bring side over
    state = 5;
    Serial.println(state);
  }
  if (PsideOut>100 && state==5){                 //piston up 
    digitalWrite(up,HIGH);      //bring side up 
    state = 6;
    Serial.println(state);
  }



  if(PheadOut>700 && Punclamp<700 && state ==6){
    digitalWrite(switch1,HIGH);
    state = 7;
    Serial.println(state);
  }

  if(PheadIn>700 && state==7){                   //Unclamp condition
    digitalWrite(unclamp,LOW);
    digitalWrite(clamp,HIGH);
    state=0;
    Serial.println(state);
  }


  
  delay(1);
}

void readSensors(){
  part = analogRead(partInput);
  Pclamp = analogRead(p_clamp);
  Punclamp = analogRead(p_unclamp);
  PupOut = analogRead(p_up_out);
  PupIn = analogRead(p_up_in);
  PsideOut = analogRead(p_side_out);
  PsideIn = analogRead(p_side_in);
  PheadOut = analogRead(p_head_out);
  PheadIn = analogRead(p_head_in);
  Serial.print("Part: ");
  Serial.print(part);
  Serial.print(" Pclamp: ");
  Serial.print(Pclamp);
  Serial.print(" Punclamp: ");
  Serial.print(Punclamp);
  Serial.print(" PupOut: ");
  Serial.print(PupOut);
  Serial.print(" PupIn: ");
  Serial.print(PupIn);
  Serial.print(" PsideOut: ");
  Serial.print(PsideOut);
  Serial.print(" PsideIn: ");
  Serial.print(PsideIn);
  Serial.print(" PheadOut: ");
  Serial.print(PheadOut);
  Serial.print(" PheadIn: ");
  Serial.print(PheadIn);
  Serial.println();
  delay(1);
}

void relaysOff(){
    digitalWrite(up ,HIGH);
    digitalWrite(side ,HIGH);
    digitalWrite(clamp ,HIGH);
    digitalWrite(unclamp ,LOW);
    digitalWrite(switch1 ,HIGH);
    digitalWrite(extra1 ,HIGH);
    digitalWrite(extra2 ,HIGH);
    digitalWrite(extra3 ,HIGH);
}


