//
// Created by chill on 2024/10/9.
//

#ifndef DBUS_LINEAR_MAPPING_H
#define DBUS_LINEAR_MAPPING_H

#include <cstdint>

float linearMapping(int16_t in, int16_t in_mid, int16_t in_max, float out_mid, float out_max){
  if (in_mid == in_max){
    return out_mid;
  }
  else{
    float out = out_mid + (out_max - out_mid) / (in_max - in_mid) * (in - in_mid);
    return out;
  }
}

#endif // DBUS_LINEAR_MAPPING_H
