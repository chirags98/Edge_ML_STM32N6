/**
  ******************************************************************************
  * @file    isp_param_conf.h
  * @brief   Thin wrapper that pulls in the ISP IQ parameters for the IMX335
  *          sensor on the STM32N6570-DK board.
  *
  *          cmw_camera.h (which includes isp_api.h) must be visible before
  *          this header is included so that ISP_IQParamTypeDef is defined.
  ******************************************************************************
  */

#ifndef __ISP_PARAM_CONF_H
#define __ISP_PARAM_CONF_H

#include "cmw_camera.h"
#include "imx335_isp_param_conf.h"

/* Array of IQ parameter pointers expected by cmw_imx335.c:
 *   (void) ISP_IQParamCacheInit;   <- suppress unused warning
 *   ISP_Init(..., &ISP_IQParamCacheInit_IMX335);
 * The array itself is only referenced via (void)-cast; the actual init
 * call passes &ISP_IQParamCacheInit_IMX335 directly.                   */
static const ISP_IQParamTypeDef * const ISP_IQParamCacheInit[] = {
    &ISP_IQParamCacheInit_IMX335
};

#endif /* __ISP_PARAM_CONF_H */
