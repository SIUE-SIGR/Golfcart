/*******************************************************************************
* File Name: QuadDecSteer_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "QuadDecSteer_Cnt16.h"

static QuadDecSteer_Cnt16_backupStruct QuadDecSteer_Cnt16_backup;


/*******************************************************************************
* Function Name: QuadDecSteer_Cnt16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  QuadDecSteer_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void QuadDecSteer_Cnt16_SaveConfig(void) 
{
    #if (!QuadDecSteer_Cnt16_UsingFixedFunction)

        QuadDecSteer_Cnt16_backup.CounterUdb = QuadDecSteer_Cnt16_ReadCounter();

        #if(!QuadDecSteer_Cnt16_ControlRegRemoved)
            QuadDecSteer_Cnt16_backup.CounterControlRegister = QuadDecSteer_Cnt16_ReadControlRegister();
        #endif /* (!QuadDecSteer_Cnt16_ControlRegRemoved) */

    #endif /* (!QuadDecSteer_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: QuadDecSteer_Cnt16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  QuadDecSteer_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void QuadDecSteer_Cnt16_RestoreConfig(void) 
{      
    #if (!QuadDecSteer_Cnt16_UsingFixedFunction)

       QuadDecSteer_Cnt16_WriteCounter(QuadDecSteer_Cnt16_backup.CounterUdb);

        #if(!QuadDecSteer_Cnt16_ControlRegRemoved)
            QuadDecSteer_Cnt16_WriteControlRegister(QuadDecSteer_Cnt16_backup.CounterControlRegister);
        #endif /* (!QuadDecSteer_Cnt16_ControlRegRemoved) */

    #endif /* (!QuadDecSteer_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: QuadDecSteer_Cnt16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  QuadDecSteer_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void QuadDecSteer_Cnt16_Sleep(void) 
{
    #if(!QuadDecSteer_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(QuadDecSteer_Cnt16_CTRL_ENABLE == (QuadDecSteer_Cnt16_CONTROL & QuadDecSteer_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            QuadDecSteer_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            QuadDecSteer_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        QuadDecSteer_Cnt16_backup.CounterEnableState = 1u;
        if(QuadDecSteer_Cnt16_backup.CounterEnableState != 0u)
        {
            QuadDecSteer_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!QuadDecSteer_Cnt16_ControlRegRemoved) */
    
    QuadDecSteer_Cnt16_Stop();
    QuadDecSteer_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: QuadDecSteer_Cnt16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  QuadDecSteer_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void QuadDecSteer_Cnt16_Wakeup(void) 
{
    QuadDecSteer_Cnt16_RestoreConfig();
    #if(!QuadDecSteer_Cnt16_ControlRegRemoved)
        if(QuadDecSteer_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            QuadDecSteer_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!QuadDecSteer_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
