//
// Created by chill on 2024/10/9.
//

#ifndef DBUS_RC_H
#define DBUS_RC_H

#include <cstdint>

#define RC_RX_BUF_SIZE 18
#define RC_RX_DATA_SIZE 18

class RC {
private:
  uint8_t rx_buf_[RC_RX_BUF_SIZE];
  uint8_t rx_data_[RC_RX_DATA_SIZE];

public:
  enum RCSwitchState_e{
    UP=0, MID, DOWN
  };
  enum PressState_e{
    RELEASE=0, PRESS
  };

  struct RCChannel{
    float r_row;
    float r_col;
    float l_row;
    float l_col;
  } channel_;
  struct RCSwitch {
    RCSwitchState_e l;
    RCSwitchState_e r;
  } switch_;
  struct MouseMove {
    float x;
    float y;
    float z;
  } mouse_move_;
  struct MousePress {
    PressState_e l;
    PressState_e r;
  } mouse_press_;
  struct KeyState {
    PressState_e w;
    PressState_e s;
    PressState_e a;
    PressState_e d;
    PressState_e q;
    PressState_e e;
    PressState_e shift;
    PressState_e ctrl;
  } key_press_;

  void ReadDBUSData();
  void ParseDBUSData();
};


#endif // DBUS_RC_H
