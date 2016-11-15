// Sri Vadrevu
// ECE 367 Spring 2016

// Lab 9  - This lab does ADC conversions for transducers thermistor and the photocell.

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


// ____        _          ____  _      _   _
//|  _ \  __ _| |_ __ _  |  _ \(_) ___| |_(_) ___  _ __   __ _ _ __ _   _
//| | | |/ _` | __/ _` | | | | | |/ __| __| |/ _ \| '_ \ / _` | '__| | | |
//| |_| | (_| | || (_| | | |_| | | (__| |_| | (_) | | | | (_| | |  | |_| |
//|____/ \__,_|\__\__,_| |____/|_|\___|\__|_|\___/|_| |_|\__,_|_|   \__, |
//                                                                  |___/

//-----------------
//    OUR DATA
//-----------------
unsigned char LUT[10] =
// These are the data values for displaying each number in the LCD
    {0x30, // 0
    0x31,  // 1
    0x32,  // 2
    0x33,  // 3
    0x34,  // 4
    0x35,  // 5
    0x36,  // 6
    0x37,  // 7
    0x38,  // 8
    0x39}; // 9

volatile char toggling = 1;       // Flag for resetting
unsigned int tempRead;
unsigned int lightRead;

//-----------------------
//  FUNCTION PROTOTYPES
//-----------------------

void Delay(int k);       
  // This is a delay function using the timer module
  // if int k is 1, delay is roughly ~0.005 sec
  
void SystemInitialize(void);                
  // Initialize system
  
void SPIsetup(void);                        
  // Set up the SPI with using SIPO IC

void InitLCD(void);                         
  // LCD initialize
  
void Commander(unsigned char command);      
  // Send Commands to LCD using SPI
  
void PressPerson(unsigned char printThis);  
  // Sends Data to LCD using SPI
  
void PrintsStrings(char string[]);
  // Prints Strings to display
  
void DisplayLight(unsigned int lightReading);
  // 

void DisplayTemp(unsigned int tempReading);

//  __  __       _          ____          _      
// |  \/  | __ _(_)_ __    / ___|___   __| | ___ 
// | |\/| |/ _` | | '_ \  | |   / _ \ / _` |/ _ \
// | |  | | (_| | | | | | | |__| (_) | (_| |  __/
// |_|  |_|\__,_|_|_| |_|  \____\___/ \__,_|\___|                                               

void main(void) {
    
//--------------
//  Setup MCU
//--------------
    SystemInitialize(); 
    SPIsetup();
    EnableInterrupts; // "cli"
    INTCR = 0xC0;
    DDRT = 0x00;   // setting up proper pins to input and to output.
    // Upper 4 pins of Port T will fire up rows in keypad, while
    // lower 4 pins will act as inputs to the columns of keypad
    
    
//--------------
//  Setup LCD
//--------------
  //  PTM_PTM3 = 1;   // LCD Setup sample code had this
  //  PTM_PTM1 = 0;   // LCD Setup sample code had this
    InitLCD();      // Let's setup the LCD before using it

//-----------------------
// Initialize ATD Module
//-----------------------
    
    ATDCTL2 = 0b11000000;
    Delay(1500);
    ATDCTL3 = 0b00010000;
    ATDCTL4 = 0b01100101;
    
    while(1) {
        PTM_PTM0 = 0;
        PTM_PTM1 = 0;
        ATDCTL5 = 0b10010000;
        while(!(ATDSTAT0 & 0b10000000));
        lightRead = ATDDR0;
        DisplayLight(lightRead);
     //   tempRead = ATDDR1;
    //    DisplayTemp(tempRead);
        tempRead = PTT;
        DisplayTemp(tempRead);
       
    }
    
} 




//  _____                 _   _                 
// |  ___|   _ _ __   ___| |_(_) ___  _ __  ___ 
// | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

//----------------------------
//    LIGHT DATA TO LCD
//----------------------------
void DisplayLight(unsigned int lightReading) {
    
    if ( (lightReading <= 200) && (lightReading > 0) ) {
        Commander(0xC0);
        PrintsStrings("Light: Dark!      ");
        Delay(50);
    } 
      else if ((lightReading <= 400) && (lightReading > 200)) {
        Commander(0xC0);
        PrintsStrings("Light: Med Low   ");
        Delay(50);
    } 
     else if ((lightReading <= 600) && (lightReading > 400)) {
        Commander(0xC0);
        PrintsStrings("Light: Med     ");
        Delay(50);
    } 
    else if ((lightReading <= 800) && (lightReading > 600)) {
        Commander(0xC0);
        PrintsStrings("Light: Med High    ");
        Delay(50);
    } else {
        Commander(0xC0);
        PrintsStrings("Light: Bright!     ");
        Delay(50);
    }
}


//----------------------------
//     TEMP DATA TO LCD
//----------------------------
void DisplayTemp(unsigned int tempReading) {
    unsigned int curTemp;
    unsigned int onesPlace;
    unsigned int tensPlace;
    unsigned int decPlace;
    curTemp = tempReading/10;
    curTemp = curTemp + 28;
    
    if (toggling == 1) {  
        Commander(0x02);
        PrintsStrings("Temp: ");
        onesPlace = LUT[curTemp%10];
        tensPlace = LUT[curTemp/10];
        decPlace = LUT[tempReading%10];
        PressPerson(tensPlace);
        PressPerson(onesPlace);
        PressPerson(0x2E);
        PressPerson(decPlace);
        PrintsStrings(" F");
        Delay(400);
    } 
    else {
        Commander(0x02);
        PrintsStrings("Temp: ");
        curTemp = curTemp - 32;
        curTemp = curTemp/2;
        onesPlace = LUT[curTemp%10];
        tensPlace = LUT[curTemp/10];
        decPlace = LUT[tempReading%10];
        PressPerson(tensPlace);
        PressPerson(onesPlace);
        PressPerson(0x2E);
        PressPerson(decPlace);
        PrintsStrings(" C");
        Delay(400);
    }
      
  
} 


//----------------------------
//     INTERRUPT VECTORS
//----------------------------
interrupt VectorNumber_Virq void IRQHandler() {
    if (toggling == 1) {
        toggling = 0;
    }
    else if (toggling == 0) {
        toggling = 1;
    }
}
                                     
//----------------------------
//       TIMER DELAY
//----------------------------
void Delay(int k)
{
    int ix;
    TSCR1 = 0x90;      /* enable TCNT and fast timer flag clear */
    TSCR2 = 0x06;      /* disable timer interrupt, set prescaler to 64 */
    TIOS  |= 0x01;     /* enable OC0 */
    TFLG1 &= 0x01;     /* clear timer flag OC0F*/
    TC0 = TCNT + 50;
    
    for(ix = 0; ix < k; ix++) {
        while(!(TFLG1 & 0x01));
        TC0 += 50;
    }
    
    TIOS  &= (~0x01);  /* disable OC0 */
}


//----------------------------
//    PHRASE DISPLAYER
//----------------------------
void PrintsStrings (char string[]) 
// Simple function, basically take first character of array
// (Writing phrase/string in function call, we can automatically treat 
// and refer to this as a character array)
// Then display each character in string individually left to right
// the loop is exited when X (points to array/string) reaches NULL or 0
{
    char x;
    int j = 0;
    x = string[0];
    while (x != 0) {
        PressPerson(x);
        j++;
        x = string[j];
    }
}



//----------------------------
//    COMMANDER FUNCTION
//----------------------------
void Commander(unsigned char command) {
    
    while( !(SPISR & 0x20));  // Wait for register empty flag (SPIEF)
    SPIDR = command;          // Output command via SPI to SIPO IC
    while(!(SPISR & 0x80));   // Wait for SPI Flag (all bits to go through)
    command = SPIDR;          // Equate command with SPIDR    
    asm(nop);
    PTM = PTM & ~0x08;        // Pulse RCK pin
    asm(nop);
    asm(nop);
    PTM = PTM | 0x08;         // Command ready for LCD
    PTM = PTM & ~0x01;        // Clear PTM 0 pin (set to 0) for sending commands
    asm(nop);
    asm(nop);
    asm(nop);
    PTM = PTM | 0x02;         // Fire ENABLE pin
    asm(nop);
    asm(nop);
    PTM = PTM & ~0x02;        // Turn off the Enable pin
    asm(nop);
    Delay(4);                 // We need to wait a little bit (don't know why)
    return;
}

//----------------------------
//       PRINT DATA
//----------------------------
void PressPerson(unsigned int printThis) {
    // Virtually same as Commander function
    while( !(SPISR & 0x20));
    SPIDR = printThis;
    while(!(SPISR & 0x80)); 
    printThis = SPIDR;
    asm(nop);
    PTM = PTM & ~0x08;
    asm(nop);
    asm(nop);
    PTM = PTM | 0x08;
    PTM = PTM | 0x01;  // Set PTM 0 pin (set to 1) for sending data 
    asm(nop);
    asm(nop);
    asm(nop);
    PTM = PTM | 0x02;
    asm(nop);
    asm(nop);
    PTM = PTM & ~0x02;
    asm(nop);
    Delay(4);
    return;
}



//----------------------------
//   SYSTEM INITIALIZATION
//----------------------------
void SystemInitialize(void)
{
    asm(sei);          // turn of interrupts
    CLKSEL  &= 0x80; // disengage PLL from system
    SYNR    = 0x01;    // set SYSCLK to 24 MHz from a 4-MHz oscillator
    REFDV   = 0;       //
    PLLCTL  = 0x40;    // turn on PLL, set automatic
    while(!(CRGFLG & 0x08)); // wait for HIGN on LOCK bit at address CRGFLG
    asm(nop);          // very short delays
    asm(nop);
    CLKSEL  |= 0x80; // clock derived from PLL
    asm(cli);          // turn on interrups
}


//----------------------------
//         SPI SETUP
//----------------------------
void SPIsetup(void) {
    while(!(CRGFLG & 0x08));
    CLKSEL = CLKSEL | 0x80;
    asm(ANDCC #$BF);
    
    SPICR1 = 0x50;
    SPIBR = 0x22;
    SPICR2 = 0x00;
    DDRM = 0x3B;
}


//----------------------------
//      INITIALIZE LCD
//----------------------------                                                       
void InitLCD ()
{  
    Delay(10);
    Commander(0x30);  // Could be 0x38 too
    Delay(10);        // need extra delay at startup
    Commander(0x30);  // LCD Sample code had this
    Delay(10);
    Commander(0x30);
    Commander(0x38);  // Use 8 - words (command or data) and both lines of LCD
    Commander(0x0C);  // Turn on the Display
    Commander(0x01);  // Clear the display and put the cursor in home position
                      // home position at DD RAM address 00
    Delay(150);       // Clear command needs more time to execute
}


