/**
  ******************************************************************************
  * @file    app_camera.h
  * @brief   Camera application layer public interface.
  *
  *          The camera captures 800x480 RGB565 frames directly into a
  *          frame buffer located in AXISRAM3 (non-cacheable).  LTDC Layer 0
  *          is programmed to display that buffer, giving a live viewfinder.
  ******************************************************************************
  */

#ifndef APP_CAMERA_H
#define APP_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Frame dimensions --------------------------------------------------------- */
#define CAMERA_FRAME_WIDTH    800U
#define CAMERA_FRAME_HEIGHT   480U
/* RGB565 = 2 bytes/pixel */
#define CAMERA_FB_SIZE        (CAMERA_FRAME_WIDTH * CAMERA_FRAME_HEIGHT * 2U)

/* Frame buffer base address in AXISRAM3 (non-cacheable region) ------------- */
#define CAMERA_FB_ADDRESS     0x34200000UL

/* Public state flag -------------------------------------------------------- */
/* Set to 1 by HAL_DCMIPP_PIPE_FrameEventCallback each time a new frame
   has been written to CAMERA_FB_ADDRESS.  Clear it after consuming the frame. */
extern volatile uint32_t camera_frame_received;

/* Public API --------------------------------------------------------------- */
/**
  * @brief  Initialize CMW, configure DCMIPP PIPE1, and start the camera.
  *         Must be called after MX_RAMCFG_Init, SystemIsolation_Config, and
  *         MX_LTDC_Init.  Calls Error_Handler() on any failure.
  */
void APP_CAMERA_Init(void);

/**
  * @brief  Background processing: call once per main-loop iteration.
  *         Drives the ISP auto-exposure / auto-white-balance algorithms.
  */
void APP_CAMERA_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_CAMERA_H */
