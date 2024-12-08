/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

#include "picoprobe_config.h"
#include "get_serial.h"
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"
//#include "hardware/structs/systick.h"
#include "hardware/pwm.h"

#include "carr.pio.h"
#include <string.h>

#define GP0 0
#define GP1 1
#define GP2 2
#define GP3 3
#define GP4 4
#define GP5 5
#define GP6 6
#define GP7 7
#define GP8 8
#define GP9 9
#define GP10 10
#define GP11 11
#define GP12 12
#define GP13 13
#define GP14 14
#define GP15 15
#define GP16 16
#define GP17 17
#define GP18 18
#define GP19 19
#define GP20 20
#define GP21 21
#define GP22 22
#define GP26 26
#define GP27 27
#define GP28 28


#define MAX_UART_PKT 64
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

static uint32_t wp1,wp2,wp3,wp4;
static uint32_t wp5,wp6,wp7,wp8;
static uint32_t rp1,rp2,rp3,rp4;
static uint32_t rp5,rp6,rp7,rp8;

//static uint32_t tik, tik0;

uint8_t buf0[1024];
uint8_t buf1[1024];
uint8_t buf2[1024];
uint8_t buf3[1024];
uint8_t buf4[1024];
uint8_t buf5[1024];
uint8_t buf6[1024];
uint8_t buf7[1024];

uint8_t tx_buf[MAX_UART_PKT];

uint slice_num0;
uint slice_num1;
uint slice_num2;
uint slice_num3;
uint slice_num4;
uint slice_num5;
uint slice_num6;
uint slice_num7;

/*
void init_systick() {
  systick_hw->csr = M0PLUS_SYST_CSR_CLKSOURCE_BITS | M0PLUS_SYST_CSR_ENABLE_BITS;
  systick_hw->rvr = 5669;
}

void init_systickeh() {
  systick_hw->csr = M0PLUS_SYST_CSR_CLKSOURCE_BITS | M0PLUS_SYST_CSR_ENABLE_BITS;
  systick_hw->rvr = 0x00FFFFFF;
}

// returns 0 - 5668
int get_systick() {
  return systick_hw->cvr;
}
*/

void carrier(PIO pio, uint8_t gp, uint16_t f_kHz) {
    double pio_freq = f_kHz*2.0;
    double div = 125000.0/pio_freq;
    uint16_t divi = (uint16_t) div;
    double rest = div - (double) divi;
    uint16_t fract16 = (uint16_t) (256*rest + 0.5);
    if (fract16 > 255) fract16 = 255;
    uint8_t fract = fract16;
    
    // Get first free state machine in pio
    uint sm = pio_claim_unused_sm(pio, true);

    // Add PIO program to PIO instruction memory. SDK will find location and
    // return with the memory offset of the program.
    uint offset = pio_add_program(pio, &carr_program);

    // Initialize the program using the helper function in our .pio file
    carr_program_init(pio, sm, offset, gp, divi, fract);

    // Start running our PIO program in the state machine
    pio_sm_set_enabled(pio, sm, true);

}


void set_freq2(PIO pio, int ch, int GPgo, int GPpo) {
  uint16_t freq, f1, f2;
  f1 = (uint16_t) tx_buf[6 + 2*ch]; 
  f2 = (uint16_t) tx_buf[7 + 2*ch];
  freq = (f1 << 8) + f2;
  
  if((freq > 100) && (freq < 310)) {
     carrier(pio, GPgo, freq); 
     gpio_init(GPpo);
     gpio_set_dir(GPpo, GPIO_OUT);
     gpio_put(GPpo,0);  
     };
  if((freq > 309) && (freq < 1610)) {
     carrier(pio, GPpo, freq); 
     gpio_init(GPgo);
     gpio_set_dir(GPgo, GPIO_OUT);
     gpio_put(GPgo,0);  
     };  
  }  

// Configure the eight carriers pins
void set_freq() {
// void set_freq2(PIO pio, int ch, int GPgo, int GPpo)
  set_freq2(pio0, 0, 26, 27);  
  set_freq2(pio0, 1, 1, 0);
  set_freq2(pio0, 2, 18, 19);
  set_freq2(pio0, 3, 7, 5);
  set_freq2(pio1, 4, 4, 3);  
  set_freq2(pio1, 5, 11, 9);
  set_freq2(pio1, 6, 17, 15);
  set_freq2(pio1, 7, 13, 12);
  sleep_ms(100);
  }  
  
void init_outp() {
  gpio_init(GP0); // mod2
  gpio_set_dir(GP0, GPIO_OUT);
  gpio_put(GP0,0);  
  gpio_init(GP1);
  gpio_set_dir(GP1, GPIO_OUT);
  gpio_put(GP1,0);  
  
  gpio_init(GP5); // mod4
  gpio_set_dir(GP5, GPIO_OUT);
  gpio_put(GP5,0);  
  gpio_init(GP7);
  gpio_set_dir(GP7, GPIO_OUT);
  gpio_put(GP7,0);
  
  gpio_init(GP3); // mod5
  gpio_set_dir(GP3, GPIO_OUT);
  gpio_put(GP3,0);  
  gpio_init(GP4);
  gpio_set_dir(GP4, GPIO_OUT);
  gpio_put(GP4,0);  
  
  gpio_init(GP9); // mod6
  gpio_set_dir(GP9, GPIO_OUT);
  gpio_put(GP9,0);  
  gpio_init(GP11);
  gpio_set_dir(GP11, GPIO_OUT);
  gpio_put(GP11,0);   
  
  gpio_init(GP27); // mod7
  gpio_set_dir(GP27, GPIO_OUT);
  gpio_put(GP27,0);  
  gpio_init(GP26);
  gpio_set_dir(GP26, GPIO_OUT);
  gpio_put(GP26,0);  
  
  gpio_init(GP19); // mod3
  gpio_set_dir(GP19, GPIO_OUT);
  gpio_put(GP19,0);  
  gpio_init(GP18);
  gpio_set_dir(GP18, GPIO_OUT);
  gpio_put(GP18,0);
  
  gpio_init(GP15); // mod1
  gpio_set_dir(GP15, GPIO_OUT);
  gpio_put(GP15,0);  
  gpio_init(GP17);
  gpio_set_dir(GP17, GPIO_OUT);
  gpio_put(GP17,0);  
  
  gpio_init(GP12); // mod8
  gpio_set_dir(GP12, GPIO_OUT);
  gpio_put(GP12,0);  
  gpio_init(GP13);
  gpio_set_dir(GP13, GPIO_OUT);
  gpio_put(GP13,0);      
  }   

int main() {
    static char c,ch;
    uint16_t wpp1, wpp2, wpp3, wpp4;
    uint16_t wpp5, wpp6, wpp7, wpp8;
    uint16_t rpp1, rpp2, rpp3, rpp4;
    uint16_t rpp5, rpp6, rpp7, rpp8;
    uint16_t fsm1,fsm2,cnt,cnt0, fc , bc;
    int tik, tik0;
    bool gp22, gp22_old, eh16;
    
    stdio_init_all();  
    clock_gpio_init(GP21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 5669);
   
    tusb_init();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(GP22);
    gpio_set_dir(GP22, GPIO_IN);
    init_outp();

        // GP28 = audio ch0 / MOD7
    gpio_set_function(GP28, GPIO_FUNC_PWM); // GP28 = pwm
    slice_num0 = pwm_gpio_to_slice_num(GP28);
    pwm_set_wrap(slice_num0, 256);
    pwm_set_enabled(slice_num0, true);
    
        // GP2 = audio ch1 / MOD2
    gpio_set_function(GP2, GPIO_FUNC_PWM); // GP2 = pwm
    slice_num1= pwm_gpio_to_slice_num(GP2);
    pwm_set_wrap(slice_num1,256);
    pwm_set_enabled(slice_num1, true);
    
        // GP20= audio ch2 / MOD3
    gpio_set_function(GP20, GPIO_FUNC_PWM); // GP20 = pwm
    slice_num2 = pwm_gpio_to_slice_num(GP20);
    pwm_set_wrap(slice_num2, 256);
    pwm_set_enabled(slice_num2, true);
     
        // GP6 = audio ch3 / MOD4
    gpio_set_function(GP6, GPIO_FUNC_PWM); // GP6 = pwm
    slice_num3= pwm_gpio_to_slice_num(GP6);
    pwm_set_wrap(slice_num3,256);
    pwm_set_enabled(slice_num3, true);
    
       // GP8 = audio ch4 / MOD5
    gpio_set_function(GP8, GPIO_FUNC_PWM); // GP8 = pwm
    slice_num4= pwm_gpio_to_slice_num(GP8);
    pwm_set_wrap(slice_num4,256);
    pwm_set_enabled(slice_num4, true);
    
       // GP10 = audio ch5 / MOD6
    gpio_set_function(GP10, GPIO_FUNC_PWM); // GP10 = pwm
    slice_num5 = pwm_gpio_to_slice_num(GP10);
    pwm_set_wrap(slice_num5, 256);
    pwm_set_enabled(slice_num5, true);
    
      // GP16 = audio ch6 / MOD1
    gpio_set_function(GP16, GPIO_FUNC_PWM); // GP16 = pwm
    slice_num6= pwm_gpio_to_slice_num(GP16);
    pwm_set_wrap(slice_num6,256);
    pwm_set_enabled(slice_num6, true);
     
      // GP14 = audio ch7 / MOD8
    gpio_set_function(GP14, GPIO_FUNC_PWM); // GP14 = pwm
    slice_num7= pwm_gpio_to_slice_num(GP14);
    pwm_set_wrap(slice_num7,256);
    pwm_set_enabled(slice_num7, true);
        
    
    fsm1 = 0;
    gp22_old = false;
    fsm2 = 0;
    while(1) {
         uint tx_len = 0;
         switch(fsm1) {
          case 0:  
            tud_task(); // tinyusb device task
            if (tud_cdc_connected() && tud_cdc_available()) fsm1 = 20;
            break; 
          case 20:  
            tx_len = tud_cdc_read(tx_buf, sizeof(tx_buf));
            fsm1 = 40;
            break;  
          case 40:  
            bc=5; 
            if (tx_buf[0]!=31) fsm1 = 0;
            if (tx_buf[1]!=97) fsm1 = 0;
            if (tx_buf[2]!=52) fsm1 = 0;
            if (tx_buf[3]!=73) fsm1 = 0;
            if (fsm1==40) fsm1=60;
            break;   
          case 60: 
            ch = tx_buf[4];  
            
            if(ch==99) {set_freq(); fsm1 = 0;};
            if(ch==1) {
              if((wp1-rp1)<1023){
                 wpp1 = wp1 & 0x3FF;
                 buf0[wpp1] = tx_buf[bc++];
                 wp1++;
                };    // endif 
              if (bc>63) fsm1 = 0;
            }; // endif  
            
            if(ch==2) {
              if((wp2-rp2)<1023){
                 wpp2 = wp2 & 0x3FF;
                 buf1[wpp2] = tx_buf[bc++];
                 wp2++;
                };    // endif 
              if (bc>63) fsm1 = 0;
            }; // endif 
            
            if(ch==3) {
              if((wp3-rp3)<1023){
                 wpp3 = wp3 & 0x3FF;
                 buf2[wpp3] = tx_buf[bc++];
                 wp3++;
                };    // endif 
             if (bc>63) fsm1 = 0;
            }; // endif 
            
            if(ch==4) {
              if((wp4-rp4)<1023){
                 wpp4 = wp4 & 0x3FF;
                 buf3[wpp4] = tx_buf[bc++];
                 wp4++;
                };    // endif 
             if (bc>63) fsm1 = 0;
            }; // endif  
            
            if(ch==5) {
              if((wp5-rp5)<1023){
                 wpp5 = wp5 & 0x3FF;
                 buf4[wpp5] = tx_buf[bc++];
                 wp5++;
                };    // endif 
             if (bc>63) fsm1 = 0;
            }; // endif  
            
            if(ch==6) {
              if((wp6-rp6)<1023){
                 wpp6 = wp6 & 0x3FF;
                 buf5[wpp6] = tx_buf[bc++];
                 wp6++;
                };    // endif 
             if (bc>63) fsm1 = 0;
            }; // endif 
            
            if(ch==7) {
              if((wp7-rp7)<1023){
                 wpp7 = wp7 & 0x3FF;
                 buf6[wpp7] = tx_buf[bc++];
                 wp7++;
                };    // endif 
             if (bc>63) fsm1 = 0;
            }; // endif  
            
            if(ch==8) {
              if((wp8-rp8)<1023){
                 wpp8 = wp8 & 0x3FF;
                 buf7[wpp8] = tx_buf[bc++];
                 wp8++;
                };    // endif 
             if (bc>63) fsm1 = 0;
            }; // endif  
            break;   
          default:
          fsm1 = 0;  
         }; // end switch    
         
         
          
         switch(fsm2) {
          case 0:  
            gp22 = gpio_get(GP22);
            if ((gp22 == true)&&(gp22_old == false)) fsm2 = 20;
            gp22_old = gp22;
            break;  
         
          case 20:  
            if (rp1 < wp1){
               rpp1 = rp1 & 0x3FF; 
               pwm_set_chan_level(slice_num0, PWM_CHAN_A, buf0[rpp1]);
               rp1++;
               };  // endif
               
            if (rp2 < wp2){
               rpp2 = rp2 & 0x3FF; 
               pwm_set_chan_level(slice_num1, PWM_CHAN_A, buf1[rpp2]);
               rp2++;
               };  // endif  
               
            if (rp3 < wp3){
               rpp3 = rp3 & 0x3FF; 
               pwm_set_chan_level(slice_num2, PWM_CHAN_A, buf2[rpp3]);
               rp3++;
               };  // endif    
               
            if (rp4 < wp4){
               rpp4 = rp4 & 0x3FF; 
               pwm_set_chan_level(slice_num3, PWM_CHAN_A, buf3[rpp4]);
               rp4++;
               };  // endif      
               
            if (rp5 < wp5){
               rpp5= rp5 & 0x3FF; 
               pwm_set_chan_level(slice_num4, PWM_CHAN_A, buf4[rpp5]);
               rp5++;
               };  // endif 
               
            if (rp6 < wp6){
               rpp6= rp6 & 0x3FF; 
               pwm_set_chan_level(slice_num5, PWM_CHAN_A, buf5[rpp6]);
               rp6++;
               };  // endif   
               
            if (rp7 < wp7){
               rpp7= rp7 & 0x3FF; 
               pwm_set_chan_level(slice_num6, PWM_CHAN_A, buf6[rpp7]);
               rp7++;
               };  // endif 
               
            if (rp8 < wp8){
               rpp8= rp8 & 0x3FF; 
               pwm_set_chan_level(slice_num7, PWM_CHAN_A, buf7[rpp8]);
               rp8++;
               };  // endif        
                  
            fsm2 = 0;
            break; 
          default:
            fsm2=0;     
          }; // end switch
        
  } // end while
return (0);
}


