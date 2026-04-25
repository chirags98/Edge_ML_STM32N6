/**
  ******************************************************************************
  * @file    cmw_camera_conf.h
  * @brief   Camera middleware configuration - selects IMX335 sensor and
  *          provides required HAL / BSP includes for the STM32N6570-DK board.
  ******************************************************************************
  */

#ifndef CMW_CAMERA_CONF_H
#define CMW_CAMERA_CONF_H

/* Sensor selection --------------------------------------------------------- */
#define USE_IMX335_SENSOR

/* Required includes for CMW (HAL types, BSP I2C bus functions) ------------- */
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery_bus.h"

#endif /* CMW_CAMERA_CONF_H */
