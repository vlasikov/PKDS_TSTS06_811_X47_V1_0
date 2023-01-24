/*
 * @file bus_io.h
 * @date 2021-01-08
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * BUS_IO v4.0.4 - BUS_IO APP is used to configure/control several GPIO pins as one entity.
 *
 * Copyright (c) 2015-2020, Infineon Technologies AG
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
 * 2016-04-01
 *     - Initial version.<br>
 *
 * 2021-01-08:
 *     - Modified check for minimum XMCLib version
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

#ifndef BUS_IO_H
#define BUS_IO_H

#include "xmc_gpio.h"
#include "DAVE_Common.h"
#include "bus_io_conf.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#define BUS_IO_XMC_LIB_MAJOR_VERSION 2
#define BUS_IO_XMC_LIB_MINOR_VERSION 0
#define BUS_IO_XMC_LIB_PATCH_VERSION 0

#if !((XMC_LIB_MAJOR_VERSION > BUS_IO_XMC_LIB_MAJOR_VERSION) ||\
      ((XMC_LIB_MAJOR_VERSION == BUS_IO_XMC_LIB_MAJOR_VERSION) && (XMC_LIB_MINOR_VERSION > BUS_IO_XMC_LIB_MINOR_VERSION)) ||\
      ((XMC_LIB_MAJOR_VERSION == BUS_IO_XMC_LIB_MAJOR_VERSION) && (XMC_LIB_MINOR_VERSION == BUS_IO_XMC_LIB_MINOR_VERSION) && (XMC_LIB_PATCH_VERSION >= BUS_IO_XMC_LIB_PATCH_VERSION)))
#error "BUS_IO requires XMC Peripheral Library v2.0.0 or higher"
#endif

 /**********************************************************************************************************************
 * ENUMS
 **********************************************************************************************************************/

/**
* @ingroup BUS_IO_enumerations
* @{
*/

/**
* @brief Initialization status of BUS_IO APP.
*/
typedef enum BUS_IO_STATUS
{
  BUS_IO_STATUS_OK = 0U,/**< 0=Status OK */
  BUS_IO_STATUS_FAILURE = 1U/**< 1=Status Failed */
} BUS_IO_STATUS_t;

/**
* @}
*/

/**********************************************************************************************************************
 * DATA STRUCTURES
 **********************************************************************************************************************/
/**
* @ingroup BUS_IO_datastructures
* @{
*/

typedef struct BUS_IO_PORT_PIN
{
  XMC_GPIO_PORT_t *const gpio_port;  /**< port number */
  uint8_t gpio_pin;  /**< pin number */
} BUS_IO_PORT_PIN_t;

/**
* @brief Initialization data structure of BUS_IO APP
*/
typedef struct BUS_IO
{
  const BUS_IO_PORT_PIN_t *const pin_array;  /**< pointer to pin configuration */
  XMC_GPIO_CONFIG_t gpio_config;  /**< mode, initial output level and pad driver strength / hysteresis */
  uint8_t number_of_pins;  /**< total number of pins required */
  bool initialized;  /**< APP initialization status */
} BUS_IO_t;

typedef XMC_GPIO_MODE_t BUS_IO_MODE_t;
/**
* @}
*/


/***********************************************************************************************************************
 * API Prototypes
 **********************************************************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/**
* @ingroup BUS_IO_apidoc
* @{
*/

/**
* @brief Get BUS_IO APP version
* @return DAVE_APP_VERSION_t APP version information (major, minor and patch number)
*
* \par<b>Description: </b><br>
* The function can be used to check application software compatibility with a
* specific version of the APP.
*
* Example Usage:
*
* @code
* #include "DAVE.h"
*
* int main(void)
* {
*   DAVE_STATUS_t init_status;
*   DAVE_APP_VERSION_t version;
*
*   // Initialize BUS_IO APP:
*   // BUS_IO_Init() is called from within DAVE_Init().
*   init_status = DAVE_Init();
*   if(init_status == DAVE_STATUS_SUCCESS)
*   {
*     version = BUS_IO_GetAppVersion();
*     if (version.major != 4U) {
*     // Probably, not the right version.
*     }
*   }
*
*     // More code here
*     while(1) {
*
*     }
*     return (1);
*  }
* @endcode<BR>
*/
DAVE_APP_VERSION_t BUS_IO_GetAppVersion(void);

/**
*
* @brief Function to initialize the port pin as per UI settings.
* @param handler Constant pointer to APP data structure. Refer @ref BUS_IO_t for details.
* @return BUS_IO_STATUS_t BUS_IO APP status. Refer @ref BUS_IO_STATUS_t structure for details.
*
* \par<b>Description:</b><br>
* This function initializes GPIO port registers IOCR, PDISC, OMR, PDR/PHCR to configure pin direction,
* initial output level and pad driver strength/hysteresis.
*
*
* Example Usage:
* @code
* #include "DAVE.h"//Declarations from DAVE Code Generation (includes SFR declaration)
* int main(void)
* {
*   DAVE_STATUS_t status;
*   status = DAVE_Init();  //(DAVE_STATUS_t)BUS_IO_Init(&BUS_IO_0) is called within DAVE_Init()
*   if(status == DAVE_STATUS_SUCCESS)
*   {
*     XMC_DEBUG("DAVE Apps initialization success\n");
*   }
*   else
*   {
*     XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
*     while(1U)
*     {
*     }
*   }
*   //Placeholder for user application code. The while loop below can be replaced with user application code.
*   while(1U)
*   {
*   }
*   return 1U;
*  }
*  @endcode
*/
BUS_IO_STATUS_t BUS_IO_Init(BUS_IO_t *const handle_ptr);

/**
*
* @brief Function to set the mode of the bus.
* @param handler Constant pointer to APP data structure. Refer @ref BUS_IO_t and @ref BUS_IO_MODE_t for details.
* @return None
*
* \par<b>Description:</b><br>
* This function configures bus as input or output by setting port input output control register Pn_IOCR,
* to make port pins as input or output.
*
*
* Example Usage:
* @code
* #include "DAVE.h"//Declarations from DAVE Code Generation (includes SFR declaration)
* int main(void)
* {
*   DAVE_STATUS_t status;
*   status = DAVE_Init();  //(DAVE_STATUS_t)BUS_IO_Init(&BUS_IO_0) is called within DAVE_Init()
*   if(status == DAVE_STATUS_SUCCESS)
*   {
*     XMC_DEBUG("DAVE Apps initialization success\n");
*   }
*   else
*   {
*     XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
*     while(1U)
*     {
*     }
*   }
*   //Placeholder for user application code. The while loop below can be replaced with user application code.
*   BUS_IO_SetMode(&BUS_IO_0, XMC_GPIO_MODE_INPUT_TRISTATE);
*   while(1U)
*   {
*     // Add application code here
*   }
*
*   return (1);
* }
*  @endcode
*/
void  BUS_IO_SetMode(BUS_IO_t *const handle_ptr, const BUS_IO_MODE_t mode);

/**
* @brief Function to read the bus pins state.
* @param handler Constant pointer to APP data structure. Refer @ref BUS_IO_t for details.
* @return uint16_t, reading the pin state and packing the result according to the pin position in
*                   the list, from LSB to MSB
*
* \par<b>Description:</b><br>
* This function reads the Pn_IN register and returns the current logical value at the GPIO pin.
*
* \par<b>Related APIs:</b><BR>
* BUS_IO_Write(), BUS_IO_Toggle()
*
* Example Usage:
* @code
*  #include "DAVE.h"//Declarations from DAVE Code Generation (includes SFR declaration)
*  int main(void)
*  {
*    DAVE_STATUS_t status;
*    uint16_t count = 0;
*
*    status = DAVE_Init();  //(DAVE_STATUS_t)BUS_IO_Init(&BUS_IO_0) is called within DAVE_Init()
*    if(status == DAVE_STATUS_SUCCESS)
*    {
*      XMC_DEBUG("DAVE Apps initialization success\n");
*    }
*    else
*    {
*      XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
*      while(1U)
*      {
*      }
*    }
*    //Placeholder for user application code. The while loop below can be replaced with user application code.
*    count = BUS_IO_Read(&BUS_IO_0);
*    // Use count for validation purpose
*    while(1U)
*    {
*      // Add application code here
*    }
*
*   return (1);
* }
*  @endcode
*/
uint16_t BUS_IO_Read(BUS_IO_t *const handle_ptr);

/**
* @brief Function to program the bus state.
* @param handler Constant pointer to APP data structure. Refer @ref BUS_IO_t for details.
* @return None
*
* \par<b>Description:</b><br>
* This function setting the pin state according the pin position in the list, from LSB to MSB.
*
* \par<b>Related APIs:</b><BR>
* BUS_IO_Read(), BUS_IO_Toggle()
*
* Example Usage:
* @code
*  #include "DAVE.h"//Declarations from DAVE Code Generation (includes SFR declaration)
*  int main(void)
*  {
*    DAVE_STATUS_t status;
*    uint16_t data = 1;
*    status = DAVE_Init();  //(DAVE_STATUS_t)BUS_IO_Init(&BUS_IO_0) is called within DAVE_Init()
*    if(status == DAVE_STATUS_SUCCESS)
*    {
*      XMC_DEBUG("DAVE Apps initialization success\n");
*    }
*    else
*    {
*      XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
*      while(1U)
*      {
*      }
*    }
*    //Placeholder for user application code. The while loop below can be replaced with user application code.
*    BUS_IO_Write(&BUS_IO_0, data);
*    while(1U)
*    {
*      // Add application code here
*    }
*
*   return (1);
* }
*  @endcode
*/
void BUS_IO_Write(BUS_IO_t *const handle_ptr, const uint16_t data);

/**
* @brief Toggles the complete pins associated with the bus.
* @param handler Constant pointer to APP data structure. Refer @ref BUS_IO_t for details.
* @return None
*
* \par<b>Description:</b><br>
* This function toggles the bus pins state.
*
* \par<b>Related APIs:</b><BR>
* BUS_IO_Read(), BUS_IO_Write()
*
* Example Usage:
*
* @code
* #include "DAVE.h"//Declarations from DAVE Code Generation (includes SFR declaration)
* int main(void)
* {
*   DAVE_STATUS_t status;
*   volatile uint32_t delay_count;
*   status = DAVE_Init();  //(DAVE_STATUS_t)BUS_IO_Init(&BUS_IO_0) is called within DAVE_Init()
*   if(status == DAVE_STATUS_SUCCESS)
*   {
*     XMC_DEBUG("DAVE Apps initialization success\n");
*   }
*   else
*   {
*      XMC_DEBUG(("DAVE Apps initialization failed with status %d\n", status));
*      while(1U)
*      {
*      }
*   }
*   //Placeholder for user application code. The while loop below can be replaced with user application code.
*   while(1U)
*   {
*      BUS_IO_Toggle(&BUS_IO_0); //toggles : 1 -> 0 (if initial output level is logic 1)
*      //Add application code here
*      for(delay_count = 0;delay_count<0xfffff;delay_count++);
*      BUS_IO_Toggle(&BUS_IO_0); //toggles : 0 -> 1
*      //Add application code here
*      for(delay_count = 0;delay_count<0xfffff;delay_count++);
*   }
*   return (1);
* }
*  @endcode
*/
void BUS_IO_Toggle(BUS_IO_t *const handle_ptr);

/**
*@}
*/

#ifdef __cplusplus
}
#endif

/* Include APP extern file */
#include "bus_io_extern.h"


#endif /* BUS_IO_H */
