// Program for Dieform Tapping machine for dynamig V3 fully auto
// Gagan: start jun 30 2020
///////////////////////////////////////////////////////////////////////////////////////

// Relays
const int pnch        =   30; //SOL4    
const int SOL6        =   32; //SOL6
const int stpr        =   34; //SOL3
const int SOL5        =   36; //SOL5
const int pogo        =   38; //SOL1
const int side        =   40; //SOL2
const int switch1     =   42;
const int HYD         =   44; 
const int open        =   7;
const int close       =   5;
const int safe        =   6;

    
//Sensors
const int p_unclamp   =  A0; 
const int p_partMag   =  A1;
const int p_headIn    =  A2;
const int p_headOut   =  A3;
const int p_partInput =  A4;

const int c_side_out  =  A10;     
const int c_side_in   =  A11;  
const int c_stpr_out  =  A9;  
const int c_stpr_in   =  A12; 
const int c_pnch_out  =  A15;
const int c_pnch_mid  =  A13; 
const int c_pnch_in   =  A14;   

//variables for sensors
float state             =  0;

int Punclamp          =  0;
int PpartMag          =  0;
int PheadIn           =  0;
int PheadOut          =  0;
int PpartInput        =  0;

int CsideOut          =  0;     
int CsideIn           =  0;  
int CstprOut          =  0;  
int CstprIn           =  0; 
int CpnchOut          =  0;
int CpnchMid          =  0;
int CpnchIn           =  0;

////////////////////////////////////////////////////////////////////////////////////
void setup() {
  //initialize input
  pinMode(p_unclamp,   INPUT);
  pinMode(p_partMag,   INPUT);
  pinMode(p_headIn,    INPUT);
  pinMode(p_headOut,   INPUT);
  pinMode(p_partInput, INPUT);

  pinMode(c_side_out,  INPUT);     
  pinMode(c_side_in,   INPUT);  
  pinMode(c_stpr_out,  INPUT); 
  pinMode(c_stpr_in,   INPUT); 
  pinMode(c_pnch_out,  INPUT);
  pinMode(c_pnch_mid,  INPUT); 
  pinMode(c_pnch_in,   INPUT); 

  //initialize output
  pinMode(pnch,        OUTPUT);
  pinMode(SOL6,        OUTPUT);
  pinMode(stpr,        OUTPUT);
  pinMode(SOL5,        OUTPUT);
  pinMode(pogo,        OUTPUT);
  pinMode(side,        OUTPUT);
  pinMode(switch1,     OUTPUT);
  pinMode(HYD,         OUTPUT);
  pinMode(open,        OUTPUT);
  pinMode(close,       OUTPUT);
  pinMode(safe,        OUTPUT);

  //turn all relays off
  relaysOff();

  //turn on coms
  Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////////
void loop() {
  readSensors();

  //index a part into the intermediate positioning device
  if (state==0 && PpartInput<500 && PpartMag>500 && CsideOut>500){
    kaChuck();
    state = 1;
    Serial.println(state);
  }
    if (state==0 && PpartInput>500 ){
    state = 1;
    Serial.println(state);
  }
  //extend the pogo, stopper extend
  if(state==1 && PpartInput>500){ 
    digitalWrite(pogo, LOW);
    digitalWrite(stpr,HIGH);
    state = 1.1;
    Serial.println(state);
  }
  
  //extend the pogo, stopper extend
  if(state==1.1 && PheadIn>500){ 
    digitalWrite(open, LOW);   //open the jaws
    digitalWrite(close, HIGH);
    delay(200);
    state = 2;
    Serial.println(state);
  }
  //withdraw the side cylinder, pincher retract
  if (state==2 && PpartInput>500 && PheadIn>500 && Punclamp>500){   
    digitalWrite(side,LOW);
    digitalWrite(pnch,HIGH);
    delay(100);
    state = 3;
    Serial.println(state);
  }
  //clamp the hydraulic cylinder
  if (state==3 && PheadIn>500 && Punclamp>500 && CsideIn>500){   
    digitalWrite(open, HIGH);
    digitalWrite(close, LOW);     //close the jaws
    state = 4;
    Serial.println(state);
  }
  //withdraw the pogo
  if (state==4 && PheadIn>500 && Punclamp<500){   
    kaChuck();
    digitalWrite(pogo,HIGH);
    delay(200);
    state = 5;
    Serial.println(state);
  }
  //start tapping and extend the linear cylinder
  if (state==5 && PheadIn>500 && Punclamp<500){   
    digitalWrite(side,HIGH);
    digitalWrite(switch1,LOW);
    state = 6;
    Serial.println(state);
  }
  //when the head is fully extended withdraw it
  if (state==6 && PheadOut>500){   
    digitalWrite(switch1,HIGH);
    state = 0;
    Serial.println(state);
  }  
  delay(1); 
}

void readSensors(){
  //get sensor values
  Punclamp  =    analogRead(p_unclamp);
  PpartMag  =    analogRead(p_partMag);
  PheadIn   =    analogRead(p_headIn);
  PheadOut  =    analogRead(p_headOut);
  PpartInput=    analogRead(p_partInput);

  CsideOut  =    analogRead(c_side_out);
  CsideIn   =    analogRead(c_side_in);
  CstprOut  =    analogRead(c_stpr_out);
  CstprIn   =    analogRead(c_stpr_in);
  CpnchOut  =    analogRead(c_pnch_out);
  CpnchMid  =    analogRead(c_pnch_mid);
  CpnchIn   =    analogRead(c_pnch_in);

  Serial.print(" Punclamp: ");
  Serial.print(Punclamp);
  Serial.print(" PpartMag: ");
  Serial.print(PpartMag);
  Serial.print(" PheadIn: ");
  Serial.print(PheadIn);
  Serial.print(" PheadOut: ");
  Serial.print(PheadOut);  
  Serial.print(" Part: ");
  Serial.print(PpartInput);

  Serial.print(" CsideOut: ");
  Serial.print(CsideOut);
  Serial.print(" CsideIn: ");
  Serial.print(CsideIn);
  Serial.print(" CstprOut: ");
  Serial.print(CstprOut);
  Serial.print(" CstprIn: ");
  Serial.print(CstprIn);  
  Serial.print("CpnchOut: ");
  Serial.print(CpnchOut);
  Serial.print(" CpnchMid: ");
  Serial.print(CpnchMid);
  Serial.print(" CpnchIn: ");
  Serial.print(CpnchIn);  

  Serial.println();
  delay(1);
}

void relaysOff(){
  //set relays off
  digitalWrite(pnch,    HIGH); //retracted 
  digitalWrite(SOL6,    HIGH);
  digitalWrite(stpr,    HIGH); // extended
  digitalWrite(SOL5,    HIGH);
  digitalWrite(pogo,    HIGH);
  digitalWrite(side,    HIGH);
  digitalWrite(switch1, HIGH);
  digitalWrite(HYD,     HIGH);
  digitalWrite(open,    LOW);
  digitalWrite(close,   HIGH);
  digitalWrite(safe,    HIGH);
}
void kaChuck(){
  if (PpartInput<500 && PpartMag>500){
    digitalWrite(pnch,LOW);
    digitalWrite(stpr,LOW);     //open the stoper shoot
  }
}
