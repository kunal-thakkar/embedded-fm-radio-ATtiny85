#ifndef __RADIO_h__
#define __RADIO_h__

#include <Arduino.h>
#include <Wire.h>

// address of the RDA5807 on two wire bus
#define RDA5807M_ADDRESS  0b0010000 // 0x10

#define BOOT_CONFIG_LEN 12
#define TUNE_CONFIG_LEN 4
#define MAX_VOLUME 15

// array channels of the desired frequency
// assuming band starts at 87.0MHz (per settings below)
// and channel spacing of 100kHz (0.1MHz) (per settings below)
// then channel can be derived as follows:
//
// channel = (<desired freq in MHz> - 87.0) / 0.1 
//
// which is the same as: <10 x desired freq in MHz> - 870
const uint16_t channels[] = {
  41, //91.1 MHz
  65, //93.5
  73, //94.3
  80, //95.0
  113,//98.3
  140,//101.0,
  186,//105.6
  208//107.8
};
const int max_idx = 7;
int ch_idx = 0;
int vol = 15;

// These bytes set our initial configuration
// We don't bother to tune to a channel at this stage.
// But instead initiate a reset.
uint8_t boot_config[] = {
  /* register 0x02 */
  0b11000000,
  0b00000011,
  /* register 0x03 */
  channels[ch_idx] >> 2, 
  ((channels[ch_idx] & 0b11) << 6 ) | 0b00010000,
  /* register 0x04 */
  0b00001010, 
  0b00000000, 
  /* register 0x05 */
  0b10001000, 
  0b00001111, 
  /* register 0x06 */
  0b00000000, 
  0b00000000, 
  /* register 0x07 */
  0b01000010, 
  0b00000010, 
};


/// Library to control radio chips in general. This library acts as a base library for the chip specific implementations.
class RADIO {
  public:
    void init() {
      Wire.begin(); // join i2c bus (address optional for master)
      write();
      boot_config[1] &= 1;
    }

    void next_channel() {
      if (ch_idx >= max_idx) {
        ch_idx = 0;
      }
      else {
        ch_idx++;
      }
      write();
    }

    void pre_channel(){
      if (ch_idx == 0) {
        ch_idx = max_idx;
      }
      else {
        ch_idx--;
      }
      write();
    }

    void vol_up() {
      if(vol < MAX_VOLUME) {
        vol++;
        write();
      }
    }

    void vol_down() {
      if(vol > 0) {
        vol--;
        write();
      }
    }

    private:
      void write() {
        boot_config[2] = channels[ch_idx] >> 2;
        boot_config[3] = ((channels[ch_idx] & 0b11) << 6 ) | 0b00010000;
        boot_config[7] = 0b00000000 | vol;
        Wire.beginTransmission(RDA5807M_ADDRESS);
        Wire.write(boot_config, BOOT_CONFIG_LEN);
        Wire.endTransmission();    // stop transmitting 
      }
};

#endif
