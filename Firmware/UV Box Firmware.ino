#define red 0 //RED Led control
#define green 1 //GREEN Led control
#define buzzer 2 //Buzzer Control
#define relay 3 //Relay/UV Control
#define pushButton 4 //Pushbutton Input

unsigned int count = 0; //Global variable for counting PushButtons presses
int timeout_count = 0; //Power Saving Timeout counter

void setup()
{
   pinMode(red, OUTPUT); //Declare Red led signal output
   pinMode(green, OUTPUT); //Declare Green led signal output
   pinMode(buzzer, OUTPUT); //Declare Buzzer led signal output
   pinMode(relay, OUTPUT); //Declare Relay led signal output
   digitalWrite(relay,HIGH); //Default Relay to Off(The relay controls are inverted i.e HIGH=> NO!=COM and NC=COM and Vice Versa)
   pinMode(pushButton,INPUT); //Declare pushbutton as input
   digitalWrite(red,LOW); //Turn Off Red Led
}

//Function to count input button presses
int C()
{
   int reading = digitalRead(pushButton);
   if(reading == 1 )
   {
    delay(50);//Debounce Time
    reading = digitalRead(pushButton);
    if(reading)
      return 1;
   }
   else
    return 0;
}

//Function for Different Buzzer sounds
void buz(int mode)
{
  if(mode == 1)
  {
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(10);
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
  }
  else
  {
    digitalWrite(buzzer,HIGH);
    delay(500);
    digitalWrite(buzzer,LOW);
    
  }
}

//Function For UV light control and Timing 
void Sanitize()
{
  int count = 0; //variable to check the state of the pushbutton
  boolean ledstate = true; //Ledstate
  digitalWrite(green,LOW); // Turn off Green LED
  buz(1); //Give a turn on indication buzzer
  for(int T_count = 0;T_count < 5/0.005;T_count++) //for loop for layoff of 5 seconds
  {
    digitalWrite(red,HIGH);
    delay(5); 
    count = C();
    if(count)
    {
      buz(2);
      return;
    }
  }
  for(long int T_count = 0;T_count <= 2.8*60/0.005;T_count++)//for loop for 3 min exposure 
  {
    digitalWrite(relay,LOW);
    if(T_count%200 == 0)
    {
      if(ledstate)
      {
        digitalWrite(red,HIGH);
        ledstate = !ledstate;
      }
      else
      {
        digitalWrite(red,LOW);
        ledstate = !ledstate;}
      }
    delay(5);
    count = C();
    if(count)
    {
      buz(2);
      return;
    }
  }
  digitalWrite(relay,HIGH);
  buz(2);
}


void loop() 
{
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  digitalWrite(relay,HIGH);
    count = C();
    delay(5);
    if(count !=0)
    {
      Sanitize();
      timeout_count = 0;
    }
    timeout_count++;

  while(timeout_count >= 60/0.005) //loop for entering low power mode
  {
     digitalWrite(green,LOW);
      count = C();
      delay(10);
      if(count)
      {
      timeout_count = 0;
      break;
      }
   }
}
