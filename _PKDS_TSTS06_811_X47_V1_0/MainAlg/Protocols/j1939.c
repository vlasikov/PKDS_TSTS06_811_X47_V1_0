/*
 * j1939.c
 *
 *  Created on: Jan 22, 2023
 *      Author: kendor
 *
 */
#include <DAVE.h>
#include "j1939.h"
#include <xmc_can.h>
#include "MainAlg/presets.h"


// CAN BASE_IDs
#define DSC1_ID_BASE 0x18B6CC27
#define DCS2_ID_BASE 0x18B7CC27
#define DCS3_ID_BASE 0x18EFCC27
// CAN GLOBAL_IDs
#define CCVS_ID 0x18FEF100
#define TD_ID 0x18FEE600


// CAN DSC1_MASKS
#define DSC1_OPEN_CMD_Msk (1UL<<0)
#define DSC1_CLOSE_CMD_Msk (1UL<<1)

#define DSC1_INTERN_PASSENGER_BUTTON_ON_Msk (1UL<<2)
#define DSC1_INTERN_PASSENGER_BUTTON_NON_Msk (1UL<<3)

#define DSC1_EXTERN_PASSENGER_BUTTON_ON_Msk (1UL<<4)
#define DSC1_EXTERN_PASSENGER_BUTTON_NON_Msk (1UL<<5)

#define DSC1_OPEN_DRIVER_DOOR_Msk (1UL<<6)
#define DSC1_CLOSE_DRIVER_DOOR_Msk (1UL<<7)

#define DSC1_IGNITION_ON_Msk (1UL<<8)
#define DSC1_IGNITION_NON_Msk (1UL<<9)

#define DSC1_LINK_ON_Msk (1UL<<10)
#define DSC1_LINK_NON_Msk (1UL<<11)

#define DSC1_PARKING_LIGHTS_ON_Msk (1UL<<12)
#define DSC1_PARKING_LIGHTS_NON_Msk (1UL<<13)

// CAN DSC2_MASKS
	// 1 byte
#define DSC2_RUBBER_LIGHTS_IN_Msk 		(31UL<<0)
#define DSC2_RUBBER_LIGHTS_OUT_Msk 		(31UL<<4)
	// 2 byte
#define DSC2_HEAT_SCREEN_Msk 			(3UL<<0)
#define DSC2_BUZZER_Msk 				(31UL<<2)
#define DSC2_NEON_LIGHT_Msk 			(3UL<<6)
	// 3 byte
#define DSC2_STEP_LIGHT_Msk 			(3UL<<0)
#define DSC2_RED_LIGHT_Msk 				(3UL<<2
#define DSC2_PASSENGER_BUTTON_LIGHT_IN 	(3UL<<4)
#define DSC2_PASSENGER_BUTTON_LIGHT_OUT (3UL<<6)
	// 4 byte
#define DSC2_INVALID_BUTTON_LIGHT_IN 	(3UL<<0)
#define DSC2_INVALID_BUTTON_LIGHT_OUT 	(3UL<<2)
#define DSC2_STOP_INDICATOR 			(3UL<<4)
#define DSC2_GREEN_INDICATOR 			(3UL<<6)
	// 5 byte
#define DSC2_RED_INDICATOR 				(3UL<<0)

// etc..............................


// Public vars
CAN_CMD_t CanCmd;
CAN_Periph_ctrl_t CanPeriphControl;
CAN_Time_t CanTime;


// private vars
uint16_t VehicleSpeed;
uint16_t VehicleSpeedThreshold;

void DSC1IsrHandler(void)
{
	XMC_CAN_MO_t* Msg;
	Msg = CAN_EXT_LMO_01_Config.mo_ptr;

	uint32_t data[2] = {0,0};

	if(Msg->can_identifier == (DSC1_ID_BASE + ((Presets.DOOR_NUMBER - 1) * 0x0100)) )
	{
		data[0] = Msg->can_data[0];
		//data[1] = Msg->can_data[1];
	}
	//Parce to CanCmd struct


	if((data[0] & DSC1_CLOSE_CMD_Msk) && !(data[0] & DSC1_OPEN_CMD_Msk))
	{
		CanCmd.close = 1;
	}
	else
	{
		CanCmd.close = 0;
	}

	if((data[0] & DSC1_OPEN_CMD_Msk) && !(data[0] & DSC1_CLOSE_CMD_Msk))
	{
		CanCmd.open = 1;
	}
	else
	{
		CanCmd.open = 0;
	}

	if( ((data[0] & DSC1_EXTERN_PASSENGER_BUTTON_ON_Msk) && !(data[0] & DSC1_EXTERN_PASSENGER_BUTTON_NON_Msk)) ||
		((data[0] & DSC1_INTERN_PASSENGER_BUTTON_ON_Msk) && !(data[0] & DSC1_INTERN_PASSENGER_BUTTON_NON_Msk)) )
	{
		CanCmd.passengersButtonEn = 1;
	}
	else
	{
		CanCmd.passengersButtonEn = 0;
	}

	if((data[0] & DSC1_OPEN_DRIVER_DOOR_Msk) && !(data[0] & DSC1_CLOSE_DRIVER_DOOR_Msk))
	{
		CanCmd.openDriverDoor = 1;
	}
	else
	{
		CanCmd.openDriverDoor = 0;
	}

	if((data[0] & DSC1_CLOSE_DRIVER_DOOR_Msk) && !(data[0] & DSC1_OPEN_DRIVER_DOOR_Msk))
	{
		CanCmd.closeDriverDoor = 1;
	}
	else
	{
		CanCmd.closeDriverDoor = 0;
	}

	if((data[0] & DSC1_IGNITION_ON_Msk) && !(data[0] & DSC1_IGNITION_NON_Msk))
	{
		CanCmd.ignitionOn = 1;
	}
	else
	{
		CanCmd.ignitionOn = 0;
	}

	if((data[0] & DSC1_LINK_ON_Msk) && !(data[0] & DSC1_LINK_NON_Msk))
	{
		CanCmd.linkOn = 1;
	}
	else
	{
		CanCmd.linkOn = 0;
	}

	if((data[0] & DSC1_PARKING_LIGHTS_ON_Msk) && !(data[0] & DSC1_PARKING_LIGHTS_NON_Msk))
	{
		CanCmd.parkingLightsEn = 1;
	}
	else
	{
		CanCmd.parkingLightsEn = 0;
	}
}

void DSC2IsrHandler (void)
{
	XMC_CAN_MO_t* Msg;
	Msg = CAN_EXT_LMO_02_Config.mo_ptr;

	uint32_t data[5] = {0,0,0,0,0};

	if(Msg->can_identifier == (DCS2_ID_BASE + ((Presets.DOOR_NUMBER - 1) * 0x0100)) )
	{
		data[0] = Msg->can_data[0];
		data[1] = Msg->can_data[1];
		data[2] = Msg->can_data[2];
		data[3] = Msg->can_data[3];
		data[4] = Msg->can_data[4];


		CanPeriphControl.rubberLightsIn 			= (data[0] & DSC2_RUBBER_LIGHTS_IN_Msk) >> 0;
		CanPeriphControl.rubberLightsOut 			= (data[0] & DSC2_RUBBER_LIGHTS_OUT_Msk) >> 4;

		CanPeriphControl.heatScreen 				= (data[1] & DSC2_HEAT_SCREEN_Msk) >> 0;
		CanPeriphControl.buzzer 					= (data[1] & DSC2_BUZZER_Msk) >> 2;
		CanPeriphControl.neonLight 					= (data[1] & DSC2_NEON_LIGHT_Msk) >> 6;

		CanPeriphControl.stepLight 					= (data[2] & DSC2_STEP_LIGHT_Msk) >> 0;
		CanPeriphControl.redLight 					= (data[2] & DSC2_RED_LIGHT_Msk) >> 2;
		CanPeriphControl.passengerButtonLightIn 	= (data[2] & DSC2_PASSENGER_BUTTON_LIGHT_IN) >> 4;
		CanPeriphControl.passengerButtonLightOut 	= (data[2] & DSC2_PASSENGER_BUTTON_LIGHT_OUT) >> 6;

		CanPeriphControl.disabledButtonLightIn		= (data[3] & DSC2_INVALID_BUTTON_LIGHT_IN) >> 0;
		CanPeriphControl.disabledButtonLightOut 	= (data[3] & DSC2_INVALID_BUTTON_LIGHT_OUT) >> 2;
		CanPeriphControl.stopIndicator 				= (data[3] & DSC2_STOP_INDICATOR) >> 4;
		CanPeriphControl.greenIndicator				= (data[3] & DSC2_GREEN_INDICATOR) >> 6;

		CanPeriphControl.redIndicator				= (data[4] & DSC2_RED_INDICATOR) >> 0;
	}
}

void DSC3IsrHandler (void)
{
	XMC_CAN_MO_t* Msg;
	Msg = CAN_EXT_LMO_03_Config.mo_ptr;

	uint32_t data[2] = {0,0};

	if(Msg->can_identifier == (DCS3_ID_BASE + ((Presets.DOOR_NUMBER - 1) * 0x0100)) )
	{
		data[0] = Msg->can_data[0];
		data[1] = Msg->can_data[1];
	}

	// don't parce yet. Need to think how it can be doing
}

void CCVSIsrHandler(void)
{
	XMC_CAN_MO_t* Msg;
	Msg = CAN_EXT_LMO_04_Config.mo_ptr;

	uint32_t data[2] = {0,0};
	int32_t  speed_wagon=0;

	if(Msg->can_identifier == CCVS_ID)
	{
		data[0] = Msg->can_data[0];
		data[1] = Msg->can_data[1];

		speed_wagon = (data[1]<<8) + data[0];
	}
	// Parce to CanCmd.speedIsZero  throw compare with (uint16_t)Presets.VEHICLE_SPEED_THRESHOLD
}

void TDIsrHandler (void)
{
	XMC_CAN_MO_t* Msg;
	Msg = CAN_EXT_LMO_05_Config.mo_ptr;

	uint32_t data[6] = {0,0,0,0,0,0};

	if(Msg->can_identifier == TD_ID)
	{
		data[0] = Msg->can_data[0];
		data[1] = Msg->can_data[1];
		data[2] = Msg->can_data[2];
		data[3] = Msg->can_data[3];
		data[4] = Msg->can_data[4];
		data[5] = Msg->can_data[5];

		CanTime.sec		= data[0];
		CanTime.min		= data[1];
		CanTime.hour	= data[2];
		CanTime.day		= data[3];
		CanTime.month	= data[4];
		CanTime.year	= data[5];
	}

	//Parce to CanTime
}
