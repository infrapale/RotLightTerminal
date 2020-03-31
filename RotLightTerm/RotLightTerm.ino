/*
 * Rotary encoder Example Program
 * HW:  using 3 digital pins of which 2 must support interrupts
 *  dpInEncoderA rotaty encoder pin, interrupt
 *  dpInEncoderB rotaty encoder pin
 *  dpInEncoderC push button pin
 *  Tom Höglund 2020
 *  https://github.com/infrapale/Rotary_Encoder.git
 */
 
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "rotenc.h"

// Digital pin definitions
enum enDigitalPins
{
  // Rotary encoder input lines
  dpInEncoderA=5,
  dpInEncoderB=6,
  dpInEncoderC=9
};

typedef void (*pMenuFunc)(int);

struct MenuEntryStruct {
  char tag[4];
  char dive[4];
  char name[10];
  pMenuFunc func;
  int param;
};

struct MenuPtrStruct {
    uint16_t first;
    uint16_t last;
    uint16_t nbr;
    
} menu_handle;

void nop(int x)
{
    Serial.println(x);
}

struct MenuEntryStruct menu[] = {
    {"top","MH1","MH1      ", &nop,69},
    {"top","MH2","MH2      ", &nop,42},
    {"top","TUP","Tupa     ", &nop,42},
    {"---","---","         ", &nop, 0},
    {"MH1","---","MH1 1    ", &nop,69},
    {"MH1","---","MH1 2    ", &nop,69},
    {"MH1","---","MH1 3    ", &nop,69},
    {"---","---","         ", &nop, 0},
    {"MH2","---","MH2 1    ", &nop,69},
    {"MH2","---","MH2 2    ", &nop,69},
    {"---","---","         ", &nop, 0},
    {"TUP","---","Tupa 1   ", &nop,69},
    {"TUP","---","Tupa 2   ", &nop,69},
    {"---","---","         ", &nop, 0},
    {"!!!","---","         ", &nop, 0}    
};



int position;

uint16_t find_menu_tag( MenuEntryStruct *menu, MenuPtrStruct *m_handle, char *tag){
    uint8_t ph = 0;  // find_fist, find_last
    uint16_t i = 0;
    bool  done = false;
    char tags[3][4] = {"123", "---", "!!!"};
    strcpy(tags[0],tag);
    Serial.println(tag);
    m_handle->first = -1;
    m_handle->last = -1;
    m_handle->nbr = 0;
    while (! done){
        Serial.print("i="); Serial.println(i);
        uint8_t tag_indx = 0;
        Serial.print("menu="); Serial.println(menu[i].tag);
        for (tag_indx = 0; tag_indx < 3; tag_indx++){
            if ( strcmp(tags[tag_indx],menu[i].tag) ==0) {
                break; 
            } 
        }
        Serial.print("tag_indx="); Serial.println(tag_indx);
        Serial.print("Ph=");Serial.println(ph);
        switch(ph){
        case 0:  // not yet found
            switch(tag_indx){
                case 0:  // == tag
                    ph++; 
                    m_handle->first = i;
                    break;
                case 1:  // == "---"
                    break;
                case 2:  // == "!!!"
                    done = true;
                     m_handle->nbr = 0;
                    break;
            }
            break;
        case 1:  // reading till end
            switch(tag_indx){
                case 0:  // == tag
                    break;
                case 1:  // == "---"
                    if (m_handle->first >= 0){
                        m_handle->last = i;
                        m_handle->nbr = m_handle->last - m_handle->first ;  
                    }    
                    else {
                        m_handle->nbr = 0;  
                    }    
                    done = true;
                    break;
                case 2:  // == "!!!"
                    m_handle->nbr = 0;  
                    done = true;
                    break;
            }
        }
        i++;
                        
    } 
    
    return(m_handle->nbr);
}

void setup()
{
       
    rotenc_init(dpInEncoderA, dpInEncoderB, dpInEncoderC);
    rotenc_set_step(0, 0, 10, 1);
    // init serial communication
    delay(2000);
    while (!Serial); // wait until serial console is open, remove if not tethered to computer
    Serial.begin(115200); 
    Serial.println("Ready to begin");
    Serial.println(menu[0].name);
    menu[0].func(menu[0].param);
    uint16_t n = find_menu_tag(menu,&menu_handle,"MH2");
    Serial.println(menu_handle.first);
    Serial.println(menu_handle.last);
    Serial.println(menu_handle.nbr);
    
}


void loop()
{
  int last_pos = 0;
  while(true){
    position = get_pos();
    if(position != last_pos){
      Serial.print(last_pos);
      Serial.print(" ");
      Serial.print(position);
      Serial.print(" ");
      if(position > last_pos){
	        Serial.println("+");
      }
      else{
	        Serial.println("-");
      }
      last_pos = position;
    }
    uint8_t pressed = rd_pressed();
    if(pressed != 0){
        Serial.println(pressed);
    }
  }
}
