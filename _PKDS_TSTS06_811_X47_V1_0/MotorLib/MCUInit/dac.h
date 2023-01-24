/**
 * @file dac.h
 * @brief DAC configuration
 * -# DAC is optionally used to
 * provide 1.65V offset for current measurement.
 * @date 2016-08-31

 ***********************************************************************************************************************
 * BLDC_SCALAR_CONTROL v1.0.2 - BLDC motor control using block commutation
 * Copyright (c) 2015-2016, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2016-08-31:
 *     - Initial version
 *
 * @endcond
 *
 */
/**
 * @addtogroup BLDC_SCALAR BLDC Motor Control
 * @{
 */

/**
 * @addtogroup MCUInit
 * @brief  MCU peripheral initialization <br>
 * @{
 */
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#ifndef MCUINIT_DAC_H_
#define MCUINIT_DAC_H_


#include "../Configuration/bldc_scalar_derived_parameter.h"
#include "../Configuration/bldc_scalar_mcuhw_config.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#if (MOTOR0_BLDC_SCALAR_OFFSET_DAC == 1U)
/** DAC settlement delay time */
#define BLDC_SCALAR_DAC_SETTLEMENT_DELAY   (0xFU)
/** DAC output voltage set 1.2V */
#define BLDC_SCALAR_DAC_VOLTAGE_COUNT      ((uint16_t)1660U)
#endif
/**********************************************************************************************************************
* DATA STRUCTURES
**********************************************************************************************************************/


#ifdef __cplusplus
   extern "C" {
#endif
/***********************************************************************************************************************
 * API Prototypes
 **********************************************************************************************************************/


/**
 * @return None <br>
 *
 * \par<b>Description:</b><br>
 * This function initializes DAC with configured value.
 */
#if (MOTOR0_BLDC_SCALAR_OFFSET_DAC == 1U)
void Motor0_BLDC_SCALAR_DAC_Init(void);
#endif
/***********************************************************************************************************************
 * EXTERN
 **********************************************************************************************************************/

/**
 * @}
 */

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
#endif /* MCUINIT_DAC_H_ */
