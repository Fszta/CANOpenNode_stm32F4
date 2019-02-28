/*!*****************************************************************************
 * @file        task.c
 * @brief       Simple 1ms task implementation.
 ******************************************************************************/

#define TMR_TASK_INTERVAL   (1000)  

/*-----------------------------------------------------------------------------
 * INCLUDE SECTION
 *----------------------------------------------------------------------------*/
#include "task.h"
#include "can.h"
#include "CO_SDO.h"
#include "CanOpen.h"

/*EEPROM driver is not the part of the demonstration code*/
//#define CAN_USE_EEPROM

/*-----------------------------------------------------------------------------
 * LOCAL (static) DEFINITIONS
 *----------------------------------------------------------------------------*/

static CO_NMT_reset_cmd_t reset;
#ifdef CAN_USE_EEPROM
static CO_EE_t                     CO_EEO;         /* Eeprom object */
#endif



void communicationReset(void) 
{
  CO_ReturnError_t err;
  err = CO_init((uint32_t)&hcan1, OD_CANNodeID, 250);
  CO_CANsetNormalMode(CO->CANmodule[0]);
  if(err != CO_ERROR_NO) 
  {
   CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR,
                  CO_EMC_SOFTWARE_INTERNAL, err);
  }
}


void programStart(void)
{
	
	/* Verify, if OD structures have proper alignment of initial values */
    if (CO_OD_RAM.FirstWord != CO_OD_RAM.LastWord)
        {
            while(1){}
        }
    if (CO_OD_EEPROM.FirstWord != CO_OD_EEPROM.LastWord)
        {
            while(1){}
        }
    if (CO_OD_ROM.FirstWord != CO_OD_ROM.LastWord)
        {
            while(1){}
        }
    /* increase variable each startup. Variable is stored in eeprom. */
    OD_powerOnCounter++;
}


void program1ms(void)
{
	
	 if(CO->CANmodule[0]->CANnormal) {
        bool_t syncWas;

        /* Process Sync and read inputs */
        syncWas = CO_process_SYNC_RPDO(CO, TMR_TASK_INTERVAL);

        /* Further I/O or nonblocking application code may go here. */

        /* Write outputs */
        CO_process_TPDO(CO, syncWas, TMR_TASK_INTERVAL);

        /* verify timer overflow */
        if(0) {
            CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, 
                           CO_EMC_SOFTWARE_INTERNAL, 0U);
        }
			}
}
