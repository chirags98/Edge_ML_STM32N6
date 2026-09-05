 /**
 ******************************************************************************
 * @file    app_postprocess.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_POSTPROCESS_H
#define __APP_POSTPROCESS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stai.h"
#include "fd_blazeface_pp_if.h"
#include "fd_pp_output_if.h"

#define POSTPROCESS_FD_BLAZEFACE_UI  (500)
/* keep POSTPROCESS_WRAPPER_SECTION and the two app_postprocess_* prototypes */

#ifndef POSTPROCESS_WRAPPER_SECTION
  #define POSTPROCESS_WRAPPER_SECTION
#endif

/* Exported functions ------------------------------------------------------- */
int32_t app_postprocess_init(void *params_postprocess, stai_network_info *NN_Info);
int32_t app_postprocess_run(void *pInput[], int nb_input, void *pOutput, void *pInput_param);

#ifdef __cplusplus
}
#endif

#endif /*__APP_POSTPROCESS_H */
