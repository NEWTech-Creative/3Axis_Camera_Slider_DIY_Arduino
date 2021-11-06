// Sketch for CamSlider
// 06/04/2019 by RZtronics <raj.shinde004@gmail.com>
// 03/11/2021 Adapted by NewTech Creative <newtechcreative@gmail.com>
// original Project homepage: http://RZtronics.com/
// Adapted 3 axis Project: NewTech Creative YoutTube channel: https://www.youtube.com/channel/UCROCWhGcdRSwRNmuOSTrljg
///////////////////////////////////////////////////////////////////////////////////////
//Terms of use
///////////////////////////////////////////////////////////////////////////////////////
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h> //check what version you have purchased and ensure the correct library linked
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "bitmapNEW.h"

#define limitSwitch 31
#define PinSW 2
#define PinCLK 3
#define PinDT 10
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

AccelStepper stepper3(1, 9, 8); // (Type:driver, STEP, DIR)
AccelStepper stepper2(1, 7, 6);
AccelStepper stepper1(1, 5, 4);

MultiStepper StepperControl; 

long gotoposition[3]; 

volatile long XInPoint=0;
volatile long YInPoint=0;
volatile long ZInPoint=0;
volatile long XMidPoint=0;
volatile long YMidPoint=0;
volatile long ZMidPoint=0;
volatile long XOutPoint=0;
volatile long YOutPoint=0;  
volatile long ZOutPoint=0;  
volatile long totaldistance=0;
int flag=0; 
int temp=0;
int i,j;
unsigned long switch0=0;
unsigned long rotary0=0;
float setspeed=200; 
//setspeed was 200 (times microsteps = 32*200)
float motorspeed;
float timeinsec;
float timeinmins;
volatile boolean TurnDetected;  
volatile boolean rotationdirection;  

const int stepsPerRevolution =200;
const int maxspeed = 9000;


void Switch()  
{
 if(millis()-switch0>500)
 {
 flag=flag+1;
 }
 switch0=millis();
}

void Rotary()  
{
delay(75);
if (digitalRead(PinCLK))
rotationdirection= digitalRead(PinDT);
else
rotationdirection= !digitalRead(PinDT);
TurnDetected = true; 
delay(75);
}

void setup() 
{
  Serial.begin(115200);
  stepper1.setMaxSpeed(maxspeed);
  stepper1.setSpeed(stepsPerRevolution);
  stepper2.setMaxSpeed(maxspeed);
  stepper2.setSpeed(stepsPerRevolution);
  stepper3.setMaxSpeed(maxspeed);
  stepper3.setSpeed(stepsPerRevolution);
  
  pinMode(limitSwitch, INPUT_PULLUP);
  pinMode(PinSW,INPUT_PULLUP); 
  pinMode(PinCLK,INPUT_PULLUP); 
  pinMode(PinDT,INPUT_PULLUP); 

  //Uncomment if you require processor enabled pins
  //pinMode (38, HIGH); //ENABLE X Motor
  //pinMode (A2, HIGH); //ENABLE Y Motor
  //pinMode (A8, HIGH); //ENABLE Z Motor
  
  display.begin(SH1106_SWITCHCAPVCC, 0x3c); 
  display.clearDisplay();
  
  // Create instances for MultiStepper - Adding the 3 steppers to the StepperControl instance for multi control
  StepperControl.addStepper(stepper1);
  StepperControl.addStepper(stepper2);
  StepperControl.addStepper(stepper3);

  attachInterrupt (digitalPinToInterrupt(2),Switch,RISING); // SW connected to D2
  attachInterrupt (digitalPinToInterrupt(3),Rotary,RISING); // CLK Connected to D3

  //display Boot animation

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation1,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation2,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation3,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation4,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation5,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation6,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation7,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation8,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation9,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation10,128,64, 1);
  display.display();
  delay(40);

  display.clearDisplay();
  display.drawBitmap(0, 0,OpenAnimation11,128,64, 1);
  display.display();
  delay(40);
  
  // display Boot logo
  display.drawBitmap(0, 0, OpenAnimation11, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay(); 
  
  Home(); // Move the slider to the initial position - homingg
  
}

void Home()
{
  stepper1.setMaxSpeed(maxspeed);
  stepper1.setSpeed(stepsPerRevolution);
  stepper2.setMaxSpeed(maxspeed);
  stepper2.setSpeed(stepsPerRevolution);
  stepper3.setMaxSpeed(maxspeed);
  stepper3.setSpeed(stepsPerRevolution);
  if(digitalRead(limitSwitch)==1)
  {

    display.clearDisplay();
    display.drawBitmap(0, 0, Homing4,128,64, 1);
    display.display();
   
  }
  
  while (digitalRead(limitSwitch)== 1) 
  {
    stepper1.setSpeed(-maxspeed);
    stepper1.runSpeed();
    
 }
  delay(20);
  stepper1.setCurrentPosition(0);
    stepper1.moveTo(200);
    while(stepper1.distanceToGo() != 0)
    {
      stepper1.setSpeed(maxspeed);
      stepper1.runSpeed();
    }
    stepper1.setCurrentPosition(0);
    display.clearDisplay();
}

void SetSpeed()
{
  display.clearDisplay();
  while( flag==12)
  {
  if (TurnDetected)  
  {
        TurnDetected = false;  // do NOT repeat IF loop until new rotation detected
        if (rotationdirection) 
        { 
         setspeed = setspeed + 20;
        }
        if (!rotationdirection) 
        { 
         setspeed = setspeed - 20;        
         if (setspeed < 0) 
        { 
         setspeed = 0;
        }
        }

          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(30,0);
          display.print("Speed");
          motorspeed=setspeed/80;
          display.setCursor(5,16);
          display.print(motorspeed);
          display.print(" mm/s");   
           ///////////////////// replaced -  totaldistance=max(XOutPoint,XMidPoint)-XInPoint;
              if (XMidPoint>XOutPoint)
                {
                    totaldistance=(XMidPoint+(XMidPoint-XOutPoint));
                }
                    else
                {
                   totaldistance=XOutPoint-XInPoint;
                }
           if(totaldistance<0)
            {
              totaldistance=totaldistance*(-1);
            }

            //////////////////////////////////////
          if(totaldistance<0)
            {
              totaldistance=totaldistance*(-1);
            }
            else
            {
              
            }
          timeinsec=(totaldistance/setspeed);
          timeinmins=timeinsec/60;
          display.setCursor(35,32);
          display.print("Time");
          display.setCursor(8,48);
          if(timeinmins>1)
          {
          display.print(timeinmins);
          display.print(" min");
          }
          else
          { 
          display.setCursor(30,50);//Center the time in seconds
          display.print(timeinsec,0);
          display.print(" sec");
          }
          display.display();
          }
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(30,0);
          display.print("Speed");
          motorspeed=setspeed/80;
          display.setCursor(5,16);
          display.print(motorspeed);
          display.print(" mm/s");  
          ///////////////////// replaced -  totaldistance=max(XOutPoint,XMidPoint)-XInPoint;
              if (XMidPoint>XOutPoint)
                {
                    totaldistance=(XMidPoint+(XMidPoint-XOutPoint));
                }
                    else
                {
                   totaldistance=XOutPoint-XInPoint;
                }
           if(totaldistance<0)
            {
              totaldistance=totaldistance*(-1);
            }

            //////////////////////////////////////
            else
            {
              
            }
          timeinsec=(totaldistance/setspeed);
          timeinmins=timeinsec/60;
          display.setCursor(35,32);
          display.print("Time");
          display.setCursor(8,48);
          if(timeinmins>1)
          {
          display.print(timeinmins);
          display.print(" min");
          }
          else
          { 
          display.setCursor(30,50);//Center the time in seconds
          display.print(timeinsec,0);
          display.print(" sec");
          }
          display.display();
  }
 
}

void stepperposition(int n)
{
  stepper1.setMaxSpeed(maxspeed);
  stepper1.setSpeed(stepsPerRevolution);
  stepper2.setMaxSpeed(maxspeed);
  stepper2.setSpeed(stepsPerRevolution);
  stepper3.setMaxSpeed(maxspeed);
  stepper3.setSpeed(stepsPerRevolution);
  if (TurnDetected)  
  {
        TurnDetected = false;  // do NOT repeat IF loop until new rotation detected
     if(n==1)
     {
        if (!rotationdirection) 
        { 
          if( stepper1.currentPosition()-500>0 )
          {
          stepper1.move(-500);
          while(stepper1.distanceToGo() != 0)
            {
              stepper1.setSpeed(-maxspeed);
              stepper1.runSpeed();
            }
          }
            else
            {
                while (stepper1.currentPosition()!=0) 
                 {
                      stepper1.setSpeed(-maxspeed);
                      stepper1.runSpeed();
                 }
            }
        }

        if (rotationdirection) 
        { 
          if( stepper1.currentPosition()+500<149000 )
          {
          stepper1.move(500);
          while(stepper1.distanceToGo() != 0)
            {
              stepper1.setSpeed(maxspeed);
              stepper1.runSpeed();
            }
          }
          else
          {
            while (stepper1.currentPosition()!= 149000) 
             {
                  stepper1.setSpeed(maxspeed);
                  stepper1.runSpeed();
    
             } 
          }
        }
     }
     if(n==2)
     {
       if (rotationdirection) 
       { 
         stepper2.move(-20);
         while(stepper2.distanceToGo() != 0)
           {
             stepper2.setSpeed(-maxspeed);
             stepper2.runSpeed();
           }      
       }
        if (!rotationdirection) 
       { 
         stepper2.move(20);
         while(stepper2.distanceToGo() != 0)
           {
             stepper2.setSpeed(maxspeed);
             stepper2.runSpeed();
           } 
       }
     }
      
     if(n==3)
     {
        if (rotationdirection) 
       { 
         stepper3.move(-20);
         while(stepper3.distanceToGo() != 0)
           {
             stepper3.setSpeed(-maxspeed);
             stepper3.runSpeed();
           }      
       }
        if (!rotationdirection) 
       { 
         stepper3.move(20);
         while(stepper3.distanceToGo() != 0)
           {
             stepper3.setSpeed(maxspeed);
             stepper3.runSpeed();
           } 
       }
     }
     }
} 


void loop()
{ 
  //Begin Setup
  if(flag==0)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, BeginSetup, 128, 64, 1);
    display.display(); 
    setspeed=200;
  }
  
  //SetXin
  if(flag==1)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn3,128,64, 1);     
    display.display();
         delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideIn3,128,64, 1);     
    display.display();
    
    while(flag==1)
    {
    stepperposition(1); 
    }
    XInPoint=stepper1.currentPosition();
  }
  
  //SetYin
  if(flag==2)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanIn3,128,64, 1);     
    display.display();

    
    while(flag==2)
    {
    stepperposition(2);
    }
    stepper2.setCurrentPosition(0);
    YInPoint=stepper2.currentPosition();
    
  }
    //SetZin
  if(flag==3)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn2,128,64, 1);     
    display.display();
        delay(80);    
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltIn3,128,64, 1);     
    display.display();       
    while(flag==3)
    {
    stepperposition(3); 
    }
    stepper3.setCurrentPosition(0);
    ZInPoint=stepper3.currentPosition();
    
  }

  //////////////////

    //SetXmid
  if(flag==4)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid3,128,64, 1);     
    display.display();
         delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideMid3,128,64, 1);     
    display.display();
    
    while(flag==4)
    {
    stepperposition(1); 
    }
    XMidPoint=stepper1.currentPosition();
  }
  
  //SetYmid
  if(flag==5)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanMid3,128,64, 1);     
    display.display();

    
    while(flag==5)
    {
    stepperposition(2);
    }
    //stepper2.setCurrentPosition(0);//////////////////////////////
    YMidPoint=stepper2.currentPosition();
    
  }
    //SetZmid
  if(flag==6)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid2,128,64, 1);     
    display.display();
        delay(80);    
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltMid3,128,64, 1);     
    display.display(); 
          
    while(flag==6)
    {
    stepperposition(3); 
    }
    //stepper3.setCurrentPosition(0);////////////////////////////////////
    ZMidPoint=stepper3.currentPosition();
    
  }

  //////////////////////////////////////////End Mid point

  
  //SetXout
  if(flag==7)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,SlideOut3,128,64, 1);     
    display.display();
    
    while(flag==7)
    {
    stepperposition(1);
    //Serial.println(stepper1.currentPosition()); //////////////////////////
    }
    XOutPoint=stepper1.currentPosition();
    
  }
  //SetYout
  if(flag==8)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,PanOut3,128,64, 1);     
    display.display();
    
    while(flag==8)
    {
    stepperposition(2);
    }
    YOutPoint=stepper2.currentPosition();
    
  }
  //SetZout
  if(flag==9)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut3,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut1,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut2,128,64, 1);     
    display.display();
        delay(80);
    display.clearDisplay();
    display.drawBitmap(0, 0,TiltOut3,128,64, 1);     
    display.display();

    
    while(flag==9)
    {
    stepperposition(3);
    }
    ZOutPoint=stepper3.currentPosition();
    display.clearDisplay();
       { 
    // Go to IN position
    gotoposition[0]=XMidPoint;
    gotoposition[1]=YMidPoint;
    gotoposition[2]=ZMidPoint; 

    display.clearDisplay();
    display.drawBitmap(0, 0,Rewinding4,128,64, 1);     
    display.display();
      
    stepper1.setMaxSpeed(maxspeed);
    StepperControl.moveTo(gotoposition);
    StepperControl.runSpeedToPosition();
       }
       flag=10;
       {
    
    gotoposition[0]=XInPoint;
    gotoposition[1]=YInPoint;
    gotoposition[2]=ZInPoint;      

    display.clearDisplay();
    display.drawBitmap(0, 0,Rewinding4,128,64, 1);     
    display.display();
    
    stepper1.setMaxSpeed(maxspeed);
    StepperControl.moveTo(gotoposition);
    StepperControl.runSpeedToPosition();
       }
       flag=11;
  }

  //Display Set Speed
  if(flag==11)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,SetSpeedd,128,64, 1);     
    display.display();
  }
  //Change Speed
  if(flag==12)
  {
    display.clearDisplay();
    SetSpeed();
  }
  //DisplayStart
  if(flag==13)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0,Start,128,64, 1);     
    display.display();
  }
  //Start
  if(flag==14)
  { 
    display.clearDisplay();
    display.drawBitmap(0, 0,Running1,128,64, 1);     
    display.display();
            delay(1000);
    display.clearDisplay();
    display.drawBitmap(0, 0,Running2,128,64, 1);     
    display.display();
            delay(1000);
    display.clearDisplay();
    display.drawBitmap(0, 0,Running3,128,64, 1);     
    display.display();
            delay(1000);
    display.clearDisplay();
    display.drawBitmap(0, 0,Running4,128,64, 1);     
    display.display();
    
    Serial.println(XInPoint);
    Serial.println(XMidPoint);
    Serial.println(XOutPoint);
    Serial.println(YInPoint);
    Serial.println(YMidPoint);
    Serial.println(YOutPoint);
    Serial.println(ZInPoint);
    Serial.println(ZMidPoint);
    Serial.println(ZOutPoint);
      {
    gotoposition[0]=XMidPoint;
    gotoposition[1]=YMidPoint;
    gotoposition[2]=ZMidPoint;

    stepper1.setMaxSpeed(setspeed);
    StepperControl.moveTo(gotoposition);
    StepperControl.runSpeedToPosition();

      }
      flag=15;
      {
    
       gotoposition[0]=XOutPoint;
       gotoposition[1]=YOutPoint;
       gotoposition[2]=ZOutPoint;
       stepper1.setMaxSpeed(setspeed);
        StepperControl.moveTo(gotoposition);
        StepperControl.runSpeedToPosition();
      }
    flag=flag+1;
  }
        //Return to start
        if(flag==16)
        {
        {
      gotoposition[0]=XMidPoint;
      gotoposition[1]=YMidPoint;
      gotoposition[2]=ZMidPoint; 


      

      display.clearDisplay();
      display.drawBitmap(0, 0,Rewinding4,128,64, 1);   
      display.display();

            
      stepper1.setMaxSpeed(setspeed);
      StepperControl.moveTo(gotoposition);
      StepperControl.runSpeedToPosition();
        }
        flag=17;
        {
      gotoposition[0]=XInPoint;
      gotoposition[1]=YInPoint;
      gotoposition[2]=ZInPoint;

            
      stepper1.setMaxSpeed(setspeed);
      StepperControl.moveTo(gotoposition);
      StepperControl.runSpeedToPosition();
        }
      flag=11;
    //SetSpeed();
        }

}
