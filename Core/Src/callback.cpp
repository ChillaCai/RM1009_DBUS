//
// Created by chilla on 2024/10/9.
//

#include "main.h"
#include "usart.h"
#include "RC.h"

extern RC rc;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if (huart == &huart1){
    rc.ReadDBUSData();
  }
}