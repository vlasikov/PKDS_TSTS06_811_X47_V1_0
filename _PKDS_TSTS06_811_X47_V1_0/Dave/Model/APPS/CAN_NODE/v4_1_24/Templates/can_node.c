/**
 * @file can_node.c
 * @date 2019-06-24
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * CAN_NODE v4.1.24 - Configures a Node and MO registers of MultiCAN module. It provides a run-time APIs to change the
 *                   node baud rate and to enable/disable Node and MO events.
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
 * 2015-02-16:
 *     - Initial version<br>
 *
 * 2015-05-12:
 *     - CAN_NODE_EnableEvent() and CAN_NODE_ConfigBaudrate() APIs are included with additional arguments.<br>
 *     - New APIs are added: CAN_NODE_MO_ReceiveData(), CAN_NODE_MO_ClearStatus(), CAN_NODE_ClearStatus().<br>
 *
 * 2018-06-21:
 *     - Changed CAN_NODE_Init() to use  XMC_CAN_NODE_NominalBitTimeConfigureEx() to fix issues when using XMC43/47/48 and bit rates below 100Kbit/s
 * 
 * 2019-06-24
 *     - Changed CAN_NODE_Init() to reset analyzer mode
 * 
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "can_node.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#define CAN_NODE_MIN_BAUDRATE (100000U)
#define CAN_NODE_MAX_BAUDRATE (1000000U)
#define CAN_NODE_MAX_SAMPLE_POINT (100U)
#define CAN_NODE_MAX_SJW    (3U)
#define CAN_NODE_MO_CLEAR_MASK (0x0fffU)

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/
static void CAN_NODE_SetNodeEvents(const CAN_NODE_t *handle);
static void CAN_NODE_SetNodePointer(const CAN_NODE_t *handle);

/**
  * @brief  Function to enable the can node events like node alert,LEC,
  *         Transfer OK and Frame counter overflow based on GUI configuration.
  * @param  handle is a pointer pointing to APP data structure.
  * @return None
  */

static void CAN_NODE_SetNodeEvents(const CAN_NODE_t *handle)
{
  uint32_t lnode_event = 0U;

  XMC_ASSERT("CAN_NODE_lInit: handle null", handle != NULL);

  CAN_NODE_SetNodePointer(handle); /* set node service pointer */

  if (handle->txok_event_enable == true)
  {
    lnode_event |= XMC_CAN_NODE_EVENT_TX_INT;
  }
  if (handle->lec_event_enable == true)
  {
    lnode_event |= XMC_CAN_NODE_EVENT_LEC;
  }
  if (handle->alert_event_enable == true)
  {
    lnode_event |= XMC_CAN_NODE_EVENT_ALERT;
  }
  XMC_CAN_NODE_EnableEvent(handle->node_ptr, (XMC_CAN_NODE_EVENT_t)lnode_event);

  if (handle->framecount_event_enable == true)
  {
    XMC_CAN_NODE_EnableEvent(handle->node_ptr, XMC_CAN_NODE_EVENT_CFCIE);
  }

}

/**
  * @brief  Function to set the Node event pointer
  *
  */

static void CAN_NODE_SetNodePointer(const CAN_NODE_t *handle)
{

  XMC_ASSERT("CAN_NODE_SetNodePointer: handle null", handle != NULL);

  /* Configured the node event pointer for the alert event with the service request number */
  XMC_CAN_NODE_SetEventNodePointer(handle->node_ptr, XMC_CAN_NODE_POINTER_EVENT_ALERT,
                                                        handle->node_sr_ptr->alert_event_sr);
  /* Configured the node event pointer for the LEC event with the service request number */
  XMC_CAN_NODE_SetEventNodePointer(handle->node_ptr, XMC_CAN_NODE_POINTER_EVENT_LEC,
                                                         handle->node_sr_ptr->lec_event_sr);
  /* Configured the node event pointer for the TXOK event with the service request number */
  XMC_CAN_NODE_SetEventNodePointer(handle->node_ptr, XMC_CAN_NODE_POINTER_EVENT_TRANSFER_OK,
                                                               handle->node_sr_ptr->txok_event_sr);
  /* Configured the node event pointer for the Frame counter event with the service request number */
  XMC_CAN_NODE_SetEventNodePointer(handle->node_ptr, XMC_CAN_NODE_POINTER_EVENT_FRAME_COUNTER,
                                                             handle->node_sr_ptr->framecount_event_sr);

}

/**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/*  API to retrieve the version of the CAN_NODE APP */
DAVE_APP_VERSION_t CAN_NODE_GetAppVersion()
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t)CAN_NODE_MAJOR_VERSION;
  version.minor = (uint8_t)CAN_NODE_MINOR_VERSION;
  version.patch = (uint8_t)CAN_NODE_PATCH_VERSION;

  return (version);
}

/* API to enable the can node events like node alert,LEC,Transfer OK and Frame counter overflow. */
void CAN_NODE_EnableEvent(const CAN_NODE_t *handle, const XMC_CAN_NODE_EVENT_t node_event)
{
  XMC_ASSERT("CAN_NODE_EnableEvent: handle null", handle != NULL);

  XMC_CAN_NODE_t *const can_node = handle->node_ptr;

  XMC_CAN_NODE_EnableEvent(can_node,node_event);

}

/*  Function to configure the baud rate based on UI configuration. */
void CAN_NODE_ConfigBaudrate(const CAN_NODE_t *handle, uint32_t baudrate, uint16_t sample_point, uint8_t sjw)
{
  XMC_CAN_NODE_t *const can_node = handle->node_ptr;
  XMC_CAN_NODE_NOMINAL_BIT_TIME_CONFIG_t *const can_bit_time = handle->baudrate_config;

  XMC_ASSERT("CAN_NODE_ConfigBaudrate: handle null", handle != NULL);
  XMC_ASSERT("CAN_NODE_ConfigBaudrate: baud rate invalid", (baudrate >= CAN_NODE_MIN_BAUDRATE)
                                                                     && (baudrate <= CAN_NODE_MAX_BAUDRATE));
  XMC_ASSERT("CAN_NODE_ConfigBaudrate: sample_point invalid", (sample_point >= 0U) &&
                                                                        (sample_point <= CAN_NODE_MAX_SAMPLE_POINT));
  XMC_ASSERT("CAN_NODE_ConfigBaudrate: Synchronization jump width invalid", (sjw >= 0U) && (sjw <= CAN_NODE_MAX_SJW));

  can_bit_time->baudrate = baudrate;
  can_bit_time->sample_point = (sample_point * CAN_NODE_MAX_SAMPLE_POINT);
  can_bit_time->sjw = (uint16_t)sjw;

  XMC_CAN_NODE_NominalBitTimeConfigure(can_node, can_bit_time);
}

/*  Function to initialize the CAN MO based on UI configuration. */
void CAN_NODE_MO_Init(const CAN_NODE_LMO_t *lmo_ptr)
{
  XMC_ASSERT("CAN_NODE_MO_Init: lmo_ptr null", lmo_ptr != NULL);

  XMC_CAN_MO_Config(lmo_ptr->mo_ptr);

  if (lmo_ptr->tx_event_enable == true)
  {
    XMC_CAN_MO_SetEventNodePointer(lmo_ptr->mo_ptr, XMC_CAN_MO_POINTER_EVENT_TRANSMIT, lmo_ptr->tx_sr);
    CAN_NODE_MO_EnableTxEvent(lmo_ptr);
  }
  if (lmo_ptr->rx_event_enable == true)
  {
    XMC_CAN_MO_SetEventNodePointer(lmo_ptr->mo_ptr, XMC_CAN_MO_POINTER_EVENT_RECEIVE, lmo_ptr->rx_sr);
    CAN_NODE_MO_EnableRxEvent(lmo_ptr);
  }

}

/* Function to transmit the can MO frame.  */
CAN_NODE_STATUS_t CAN_NODE_MO_Transmit(const CAN_NODE_LMO_t *lmo_ptr)
{
  CAN_NODE_STATUS_t status = CAN_NODE_STATUS_SUCCESS;
  XMC_ASSERT("CAN_NODE_MO_Transmit: lmo_ptr null", lmo_ptr != NULL);

  status = (CAN_NODE_STATUS_t)XMC_CAN_MO_Transmit(lmo_ptr->mo_ptr);
  return (status);
}

/* Function to read the received CAN message from the selected message object. */
CAN_NODE_STATUS_t CAN_NODE_MO_Receive( CAN_NODE_LMO_t *lmo_ptr)
{
  CAN_NODE_STATUS_t status = CAN_NODE_STATUS_SUCCESS;
  XMC_ASSERT("CAN_NODE_MO_Receive: lmo_ptr null", lmo_ptr != NULL);

  status = (CAN_NODE_STATUS_t)XMC_CAN_MO_Receive(lmo_ptr->mo_ptr);
  return (status);
}

/* Function to read the received CAN message data from the selected message object excluding other parameters. */
CAN_NODE_STATUS_t CAN_NODE_MO_ReceiveData( CAN_NODE_LMO_t *lmo_ptr)
{
  CAN_NODE_STATUS_t status = CAN_NODE_STATUS_SUCCESS;
  XMC_ASSERT("CAN_NODE_MO_ReceiveData: lmo_ptr null", lmo_ptr != NULL);

  status = (CAN_NODE_STATUS_t)XMC_CAN_MO_ReceiveData(lmo_ptr->mo_ptr);
  return (status);
}

/* Function to updates the data for the CAN Message Object. */
CAN_NODE_STATUS_t CAN_NODE_MO_UpdateData(const CAN_NODE_LMO_t *const lmo_ptr, uint8_t *array_data)
{
  CAN_NODE_STATUS_t status = CAN_NODE_STATUS_SUCCESS;
  uint32_t *data_pointer = (uint32_t*) array_data;

  XMC_ASSERT("CAN_NODE_MO_UpdateData: lmo_ptr null", lmo_ptr != NULL);

  lmo_ptr->mo_ptr->can_data[0U] = *data_pointer;
  lmo_ptr->mo_ptr->can_data[1U] = *(data_pointer + 1U);

  status = (CAN_NODE_STATUS_t)XMC_CAN_MO_UpdateData(lmo_ptr->mo_ptr);
  return (status);
}

/*  Function to reads the status of the CAN Message Object.  */
uint32_t CAN_NODE_MO_GetStatus(const CAN_NODE_LMO_t *lmo_ptr)
{
  uint32_t status;

  XMC_ASSERT("CAN_NODE_MO_GetStatus: lmo_ptr null", lmo_ptr != NULL);

  status = XMC_CAN_MO_GetStatus(lmo_ptr->mo_ptr);

  return (status);
}

/*  Function to clear/ reset the status of the CAN Message Object  */
void CAN_NODE_MO_ClearStatus(const CAN_NODE_LMO_t *lmo_ptr, const uint32_t mask)
{

  XMC_ASSERT("CAN_NODE_MO_ClearStatus: lmo_ptr null", lmo_ptr != NULL);
  XMC_ASSERT("CAN_NODE_MO_ClearStatus: mask invalid", (mask <= CAN_NODE_MO_CLEAR_MASK));

  XMC_CAN_MO_ResetStatus(lmo_ptr->mo_ptr, mask);

}

/*  Function to disable the node event  */
void CAN_NODE_DisableEvent(const CAN_NODE_t *handle, const XMC_CAN_NODE_EVENT_t node_event)
{
  XMC_CAN_NODE_t *const can_node = handle->node_ptr;

  XMC_ASSERT("CAN_NODE_DisableEvent: handle null", handle != NULL);

  XMC_CAN_NODE_DisableEvent(can_node,node_event);

}

/* Function to read the current status of the CAN node  */
uint32_t CAN_NODE_GetStatus(const CAN_NODE_t* handle)
{
  uint32_t  status;

  XMC_ASSERT("CAN_NODE_GetStatus: handle null", handle != NULL);

  XMC_CAN_NODE_t *const can_node = handle->node_ptr;

  status = XMC_CAN_NODE_GetStatus(can_node);
  return (status);
}

/* Function to clear the current status of the CAN node  */
void CAN_NODE_ClearStatus(const CAN_NODE_t *handle, XMC_CAN_NODE_STATUS_t can_node_status)
{
  XMC_ASSERT("CAN_NODE_ClearStatus: handle null", handle != NULL);

  XMC_CAN_NODE_t *const can_node = handle->node_ptr;

  XMC_CAN_NODE_ClearStatus(can_node, can_node_status);
}

/*  Function to initialize the CAN node by configuring the baud rate, can bus type (External or internal)
 *  and message objects.
 */

CAN_NODE_STATUS_t CAN_NODE_Init(const CAN_NODE_t* handle)
{
  CAN_NODE_STATUS_t status = CAN_NODE_STATUS_SUCCESS;
  uint32_t loop_count;
  const CAN_NODE_LMO_t *lmo_ptr;

  XMC_ASSERT("CAN_NODE_Init: handle null", handle != NULL);

  const CAN_NODE_GPIO_t *const lgpio_ptr = handle->gpio_out;

  /* Initialize the GLOBAL_CAN APP */
  status  = (CAN_NODE_STATUS_t)GLOBAL_CAN_Init(handle->global_ptr);

  if (status == CAN_NODE_STATUS_SUCCESS)
  {
    /* Initialize the GUI configured values for baud rate to NBTR Reg */
    if (XMC_CAN_NODE_NominalBitTimeConfigureEx(handle->node_ptr, handle->baudrate_config) == XMC_CAN_STATUS_SUCCESS)
    {
      /* set CCE and INIT bit NCR for node configuration */

      XMC_CAN_NODE_EnableConfigurationChange(handle->node_ptr);
      XMC_CAN_NODE_SetInitBit(handle->node_ptr);
      XMC_CAN_NODE_ReSetAnalyzerMode(handle->node_ptr);

      if (handle->loopback_enable == true) /* Loop back mode enabled */
      {
        XMC_CAN_NODE_EnableLoopBack(handle->node_ptr);
      }
      else
      {
        XMC_GPIO_Init(handle->gpio_in->port, handle->gpio_in->pin, handle->gpio_in_config);
        XMC_CAN_NODE_SetReceiveInput(handle->node_ptr, handle->rx_signal);
      }
      CAN_NODE_SetNodeEvents(handle); /* API to enable node events */

      for (loop_count = 0U; loop_count < handle->mo_count; loop_count++)
      {
        lmo_ptr = handle->lmobj_ptr[loop_count];
        XMC_CAN_AllocateMOtoNodeList(handle->global_ptr->canglobal_ptr, handle->node_num, lmo_ptr->number);
        CAN_NODE_MO_Init(lmo_ptr);
      }
      /* reset CCE and INIT bit NCR for node configuration */
      XMC_CAN_NODE_DisableConfigurationChange(handle->node_ptr);
      XMC_CAN_NODE_ResetInitBit(handle->node_ptr);
      if (handle->loopback_enable == false)
      {
        /* CAN transmit pin configuration */
        XMC_GPIO_Init(lgpio_ptr->port, lgpio_ptr->pin, handle->gpio_out_config);
      }
    }
    else
    {
      status = CAN_NODE_STATUS_FAILURE;
    }
  }
  else
  {
    status = CAN_NODE_STATUS_FAILURE;
  }

  return (status);
}
