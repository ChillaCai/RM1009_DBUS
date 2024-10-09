//
// Created by chill on 2024/10/9.
//
#include "RC.h"
#include "usart.h"
#include "linear_mapping.h"

#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)

void RC::ReadDBUSData() {
  HAL_UART_Receive_DMA(&huart1, this->rx_buf_, RC_RX_BUF_SIZE);
  for (int i = 0; i < sizeof(this->rx_buf_); i++){
    this->rx_data_[i] = this->rx_buf_[i];
  }
  ParseDBUSData();
}

void RC::ParseDBUSData() {
  // RCChannel
  uint16_t r_row_raw = (((int16_t)rx_data_[1] << 8) | ((int16_t)rx_data_[0])) & 0x07FF;
  uint16_t r_col_raw = (((int16_t)rx_data_[2] << 5 | (int16_t)rx_data_[1] >> 3)) & 0x07FF;
  uint16_t l_row_raw = (((int16_t)rx_data_[4] << 10) | ((int16_t)rx_data_[3] << 2) | ((int16_t)rx_data_[2] >> 6)) & 0x07FF;
  uint16_t l_col_raw = (((int16_t)rx_data_[5] << 7) | ((int16_t)rx_data_[4] >> 1) | ((int16_t)rx_data_[3] >> 3)) & 0x07FF;
  this->channel_.r_row = linearMapping((int16_t)r_row_raw, 1024, 1684, 0.0f, 1.0f);
  this->channel_.r_col = linearMapping((int16_t)r_col_raw, 1024, 1684, 0.0f, 1.0f);
  this->channel_.l_row = linearMapping((int16_t)l_row_raw, 1024, 1684, 0.0f, 1.0f);
  this->channel_.l_col = linearMapping((int16_t)l_col_raw, 1024, 1684, 0.0f, 1.0f);
  // RCSwitch
  uint8_t s1_raw = (rx_data_[5] >> 6) & 0x0003;
  uint8_t s2_raw = (rx_data_[5] >> 4) & 0x0003;
  switch (s2_raw){
  case 1:
    this->switch_.r = UP;
    break;
  case 2:
    this->switch_.r = DOWN;
    break;
  case 3:
    this->switch_.r = MID;
    break;
  }
  switch (s1_raw){
  case 1:
    this->switch_.l = UP;
    break;
  case 2:
    this->switch_.l = DOWN;
    break;
  case 3:
    this->switch_.l = MID;
    break;
  }
  // mouse_move_
  int16_t x_raw = (((int16_t)rx_data_[7] << 8) | ((int16_t)rx_data_[6]));
  int16_t y_raw = (((int16_t)rx_data_[9] << 8) | ((int16_t)rx_data_[8]));
  int16_t z_raw = (((int16_t)rx_data_[11] << 8) | ((int16_t)rx_data_[10]));
  this->mouse_move_.x = linearMapping(x_raw, 0, 32767, 0.0f, 1.0f);
  this->mouse_move_.y = linearMapping(y_raw, 0, 32767, 0.0f, 1.0f);
  this->mouse_move_.z = linearMapping(z_raw, 0, 32767, 0.0f, 1.0f);
  // mouse_press_
  uint8_t l_press_raw = rx_data_[12];
  uint8_t r_press_raw = rx_data_[13];
  this->mouse_press_.l = (PressState_e)l_press_raw;
  this->mouse_press_.r = (PressState_e)r_press_raw;
  // key_
  uint16_t key_raw = (((int16_t)rx_data_[15] << 8) | ((int16_t)rx_data_[14]));
  if ((KEY_PRESSED_OFFSET_W & key_raw) == KEY_PRESSED_OFFSET_W) key_press_.w = PRESS;
  else key_press_.w = RELEASE;
  if ((KEY_PRESSED_OFFSET_S & key_raw) == KEY_PRESSED_OFFSET_S) key_press_.s = PRESS;
  else key_press_.s = RELEASE;
  if ((KEY_PRESSED_OFFSET_A & key_raw) == KEY_PRESSED_OFFSET_A) key_press_.a = PRESS;
  else key_press_.a = RELEASE;
  if ((KEY_PRESSED_OFFSET_D & key_raw) == KEY_PRESSED_OFFSET_D) key_press_.d = PRESS;
  else key_press_.d = RELEASE;
  if ((KEY_PRESSED_OFFSET_Q & key_raw) == KEY_PRESSED_OFFSET_Q) key_press_.q = PRESS;
  else key_press_.q = RELEASE;
  if ((KEY_PRESSED_OFFSET_E & key_raw) == KEY_PRESSED_OFFSET_E) key_press_.e = PRESS;
  else key_press_.e = RELEASE;
  if ((KEY_PRESSED_OFFSET_SHIFT & key_raw) == KEY_PRESSED_OFFSET_SHIFT) key_press_.shift = PRESS;
  else key_press_.shift = RELEASE;
  if ((KEY_PRESSED_OFFSET_CTRL & key_raw) == KEY_PRESSED_OFFSET_CTRL) key_press_.ctrl = PRESS;
  else key_press_.ctrl = RELEASE;
}