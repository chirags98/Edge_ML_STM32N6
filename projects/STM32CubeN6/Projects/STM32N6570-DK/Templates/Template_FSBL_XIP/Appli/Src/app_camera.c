/**
  ******************************************************************************
  * @file    app_camera.c
  * @brief   Camera application layer.
  *
  *          Uses the CMW (Camera MiddleWare) abstraction to:
  *            - Initialise the IMX335 sensor via I2C (handled by BSP internally)
  *            - Configure DCMIPP PIPE1: full-sensor crop → 800x480 RGB565
  *            - Write frames directly to the AXISRAM3 frame buffer at
  *              CAMERA_FB_ADDRESS (non-cacheable, DMA-coherent)
  *            - Run the ISP AEC / AWB background algorithms
  *
  *          LTDC Layer 0 FBStartAddress must already point to CAMERA_FB_ADDRESS
  *          so the live feed appears on the display automatically.
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------------- */
#include "app_camera.h"
#include "cmw_camera.h"
#include "main.h"   /* Error_Handler, hltdc */

/* External LTDC handle from main.c ----------------------------------------- */
extern LTDC_HandleTypeDef hltdc;

/* Frame buffer ------------------------------------------------------------- */
/* Placed in .axisram3_bss section (NOLOAD, AXISRAM3 memory region).
   Must match CAMERA_FB_ADDRESS = 0x34200000.                                  */
__attribute__((section(".axisram3_bss")))
static uint8_t camera_frame_buffer[CAMERA_FB_SIZE];

/* Public state flag -------------------------------------------------------- */
volatile uint32_t camera_frame_received = 0U;


/* -------------------------------------------------------------------------- */
/*  Public API                                                                 */
/* -------------------------------------------------------------------------- */

/**
  * @brief  Initialise the camera pipeline and start continuous capture.
  */
void APP_CAMERA_Init(void)
{
  CMW_CameraInit_t  cam_conf  = {0};
  CMW_DCMIPP_Conf_t pipe_conf = {0};
  uint32_t          pitch     = 0U;

  /* ------------------------------------------------------------------
     1. Initialise sensor + DCMIPP via CMW
        - width / height = sensor native resolution (IMX335 2592x1944)
        - fps = 0  → CMW picks the sensor default
        - mirror_flip = CMW_MIRRORFLIP_NONE
     ------------------------------------------------------------------ */
  cam_conf.width       = 2592U;
  cam_conf.height      = 1944U;
  cam_conf.fps         = 30;
  cam_conf.mirror_flip = CMW_MIRRORFLIP_NONE;

  if (CMW_CAMERA_Init(&cam_conf, NULL) != CMW_ERROR_NONE)
  {
    Error_Handler();
  }

  /* ------------------------------------------------------------------
     2. Configure PIPE1 output: 800x480 RGB565
        - CMW_Aspect_ratio_crop: maintains aspect ratio, crops edges
        - enable_swap = 0
        - enable_gamma_conversion = 0 (ISP gamma applied internally)
        - mode = CMW_MODE_CONTINUOUS
     ------------------------------------------------------------------ */
  pipe_conf.output_width            = CAMERA_FRAME_WIDTH;
  pipe_conf.output_height           = CAMERA_FRAME_HEIGHT;
  pipe_conf.output_format           = DCMIPP_PIXEL_PACKER_FORMAT_RGB565_1;
  pipe_conf.output_bpp              = 2;
  pipe_conf.enable_swap             = 0;
  pipe_conf.enable_gamma_conversion = 0;
  pipe_conf.mode                    = CMW_Aspect_ratio_crop;

  if (CMW_CAMERA_SetPipeConfig(DCMIPP_PIPE1, &pipe_conf, &pitch) != CMW_ERROR_NONE)
  {
    Error_Handler();
  }

  /* ------------------------------------------------------------------
     3. Redirect LTDC Layer 0 to the AXISRAM3 frame buffer
     ------------------------------------------------------------------ */
  if (HAL_LTDC_SetAddress(&hltdc, CAMERA_FB_ADDRESS, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /* ------------------------------------------------------------------
     4. Start continuous capture into the frame buffer
     ------------------------------------------------------------------ */
  if (CMW_CAMERA_Start(DCMIPP_PIPE1,
                       (uint8_t *)CAMERA_FB_ADDRESS,
                       CMW_MODE_CONTINUOUS) != CMW_ERROR_NONE)
  {
    Error_Handler();
  }
}

/**
  * @brief  Background processing — must be called every main-loop iteration.
  *         Drives ISP AEC / AWB algorithms.
  */
void APP_CAMERA_Run(void)
{
  if (CMW_CAMERA_Run() != CMW_ERROR_NONE)
  {
    /* Non-fatal: ISP background step failed; camera capture continues */
  }
}

/* -------------------------------------------------------------------------- */
/*  CMW weak-callback overrides (called from CMW's HAL callback dispatcher)   */
/* -------------------------------------------------------------------------- */

/**
  * @brief  Override CMW's weak FrameEventCallback.
  *         CMW's HAL_DCMIPP_PIPE_FrameEventCallback calls this after its own
  *         internal processing, so we must NOT redefine the HAL callback.
  */
int CMW_CAMERA_PIPE_FrameEventCallback(uint32_t pipe)
{
  if (pipe == DCMIPP_PIPE1)
  {
    camera_frame_received = 1U;
  }
  return 0;
}
