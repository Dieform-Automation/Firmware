//------------------------------------------------------------------------------
#include <STM32FreeRTOS.h>                                             //include
#include <CMSIS_DSP.h>

const uint8_t PinSerial1RX  = PA3;                                        //Pins
const uint8_t PinSerial1TX  = PA2;
const uint8_t PinWire1SDA   = PB7;     
const uint8_t PinWire1SCL   = PB6;   
const uint8_t PinButton     = PC13;     
const uint8_t PinLed        = PA5;  

HardwareSerial  Serial1(PinSerial1RX, PinSerial1TX);

int      count          = 0;                                          //Varables
uint8_t  buttonState    = 0; 

double kp = 2.0;                                                 //PID constants
double ki = 5.0;
double kd = 1.0;
 
uint32_t currentTime, previousTime;
uint32_t elapsedTime, error,lastError, out;
uint32_t input, output, setpoint, cumError, rateError;

osSemaphoreId_t semCount;                                       //Initialize Sem
osSemaphoreId_t semDec; 

osThreadId_t tid_led;                                             //Threads init
osThreadId_t tid_counterInc;
osThreadId_t tid_counterDec;
osThreadId_t tid_reset;

const osThreadAttr_t led_attr = {                                  //Thread attr
  .priority = osPriorityNormal
}; 
const osThreadAttr_t counterInc_attr = {
  .priority = osPriorityNormal
}; 
const osThreadAttr_t counterDec_attr = {
  .priority = osPriorityNormal
}; 
const osThreadAttr_t reset_attr = {
  .priority = osPriorityNormal
};


//------------------------------------------------------------------------------
void reset(void *) {                                                  //Thread 1
  while(1) {
    attachInterrupt(digitalPinToInterrupt(PinButton), button_ISR, FALLING);
    if(buttonState == 1){
      osSemaphoreAcquire(semCount, 0);
      osSemaphoreRelease(semDec);
      Serial1.println("");
      Serial1.println("START COUNTING DOWN");
      Serial1.println("");
      buttonState = 2;
    }
    if(buttonState == 3){
      osSemaphoreAcquire(semDec, 0); 
      osSemaphoreRelease(semCount);
      Serial1.println("");
      Serial1.println("START COUNTING UP");
      Serial1.println("");
      buttonState = 4;
    }
  }
}

void led(void *) {                                                    //Thread 2
  while(1) {
    digitalToggle(PinLed);
    osDelay(500);
  }
}

void counterInc(void *) {                                             //Thread 3
  while(1) {
    osSemaphoreAcquire(semCount, osWaitForever);
    count++;
    Serial1.print("COUNTER: ");
    Serial1.println(count);
    osSemaphoreRelease(semCount);
    osDelay(100);
  }
}

void counterDec(void *) {                                             //Thread 4
  while(1) {
    osSemaphoreAcquire(semDec, osWaitForever);
    count--;
    Serial1.print("COUNTER: ");
    Serial1.println(count);
    osSemaphoreRelease(semDec);
    osDelay(100);
  }
}


//------------------------------------------------------------------------------
void button_ISR() {                                                  //Interrupt
  buttonState++;
  if(buttonState >= 5){
    buttonState=1;
  }
}


//------------------------------------------------------------------------------
void setup() {                                                      //Initialize
  pinMode(PinLed, OUTPUT);
  pinMode(PinButton, INPUT);
  
  Serial1.begin(115200); 
  while(!Serial1) {}

  tid_led         = osThreadNew(led, NULL, &led_attr);      //Initialize Threads
  tid_reset       = osThreadNew(reset, NULL, &reset_attr);
  tid_counterInc  = osThreadNew(counterInc, NULL, &counterInc_attr);
  tid_counterDec  = osThreadNew(counterDec, NULL, &counterDec_attr);

  semCount = osSemaphoreNew(1, 1, NULL); 
  semDec   = osSemaphoreNew(1, 0, NULL);

  osKernelInitialize();                                 //Initialize CMSIS-RTOS

  if (osKernelGetState() == osKernelReady) {
    osKernelStart();                                   //Start thread execution
  }
  while(1);
}


//------------------------------------------------------------------------------
double motorPID(double inp){     
  currentTime = millis();                                     //get current time
  elapsedTime = (double)(currentTime - previousTime);     //compute time elapsed

  error       = setpoint - inp;                                //determine error
  cumError   += error * elapsedTime;                          //compute integral
  rateError   = (error - lastError)/elapsedTime;            //compute derivative

  out = kp*error + ki*cumError + kd*rateError;                      //PID output

  lastError = error;                                    //remember current error
  previousTime = currentTime;                            //remember current time

  return out;                                            //return the PID output
}


//------------------------------------------------------------------------------
void loop() {                                                       //main loop
}
