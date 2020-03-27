/* ========================================
 *  Project name: PSoc4_PWM_sound
 *  Author:       João Nuno Carvalho
 *  Date:         12.05.2015
 *  License:      MIT open source license.
 * ========================================
*/
#include <project.h>

#include <PWM_sound_freq.h>

#define MUSIC_PLAY_MODE (0u)
#define MOVEMENT_CONFIGURATION_MODE (1u)

/* Attribute CY_NOINIT puts SRAM variable in memory section which is retained in low power modes like Hibernate, bur not stop mode */
CY_NOINIT uint8 application_mode;


// The Interrupt Service Routine for LPComp.
CY_ISR(Comparator_ISR_Handler)
{
    isr_LPComparator_ClearPending();
    LPComp_movement_detector_ClearInterrupt(LPComp_movement_detector_INTR);
}

int main()
{

    /* Start Low Power Comparator */
    LPComp_movement_detector_Start();
    isr_LPComparator_StartEx(Comparator_ISR_Handler);
    
    LPComp_movement_detector_ClearInterrupt(LPComp_movement_detector_INTR);
    isr_LPComparator_ClearPending();
    
    /* Enable all interrupts */
    CyGlobalIntEnable;
    
    // Sets Piezo pin to output Strong.
    Pin_1_1_Piezo_SetDriveMode(Pin_1_1_Piezo_DM_STRONG);
    
    
    if (CySysPmGetResetReason() == CY_PM_RESET_REASON_WAKEUP_HIB){
        // Wakeup from hibernation.
        
        int i;
        uint32 freq_clock;
        switch(application_mode){
        
            case MUSIC_PLAY_MODE:    
                // 5 Minutes = 60*5 = 300 seconds
                // 300s /25 s = 12 times.  
                for(i = 0; i < 12; i++ ){
                    // Music duration 25 seconds of one play.
                    PWM_SOUND_FREQ_TEST_Music_Brahms_Lullaby();
                }            
                break;

            case MOVEMENT_CONFIGURATION_MODE:
                freq_clock = 1200000; // 12MHZ This gives the best resolution for the range, but can have other values.
                PWM_SOUND_FREQ * sound_obj = PWM_SOUND_FREQ_CONSTRUCT( freq_clock );
                uint32 freq_sound = 0;
                uint32 duration_ms = 0; // 300;
                // Tests one tone(1000Hz) frequency for 2 seconds at full sound power(max volume), it auto terminates the sound.
                freq_sound     = 1000; // 1KHz
                duration_ms    = 500; // 60 seconds
                PWM_SOUND_FREQ_PLAY(sound_obj, freq_sound, duration_ms);
                // Pauses for 1 second.
                CyDelay(500); 
                PWM_SOUND_FREQ_STOP(sound_obj);
                PWM_SOUND_FREQ_DESTROY( sound_obj );       
                break;
                
        }
        
        // Puts Piezo Pin in high Z (impedance) so that the piezo positive is free from ground.
        Pin_1_1_Piezo_SetDriveMode( Pin_1_1_Piezo_DM_ALG_HIZ );

        // We are going to make the pin high Z (impedance) so that it will not 
        // consume current.
        Pin_P0_7_switch_SetDriveMode( Pin_P0_7_switch_DM_ALG_HIZ );
        
        /* Start Low Power Comparator */
//        LPComp_movement_detector_Start();
        
        // Hibernate.
        CySysPmHibernate();
        
    }else{
        // Normal reset.

        if (Pin_P0_7_switch_Read() == 1u){
            // Switch not pressed.
            
            // This is the normal mode that plays the Lullaby tune for 5 minuts then
            // it goes into hibernation.
            // When a movment is detected it plays for onother 5 minuts and goes
            // into hibernation.
            
            application_mode = MUSIC_PLAY_MODE;

            // 5 Minutes = 60*5 = 300 seconds
            // 300s /25 s = 12 times.
            int i;
            for(i = 0; i < 12; i++ ){
                // Music duration 25 seconds of one play.
                // PWM_SOUND_FREQ_TEST_Music();
                PWM_SOUND_FREQ_TEST_Music_Brahms_Lullaby();
            }
            
        }else{
            // Pin_P0_7_switch_Read() == 0u // Switch pressed
            
            // The user enters this mode if the button on the board 049-KIT is
            // pressed while the power is turned on.
            // This mode allows to set the sensitivity of movement detection in
            // the potenciometer.
            
            application_mode = MOVEMENT_CONFIGURATION_MODE;
        
            // Enters directly to hybernation for movment configuration mode.
            // The program will beep with movment, so that the user can configure
            // the sensitivity of the movement detection.
            // To exit this mode the user has to make a power down cycle.
        }
            
        // Puts Piezo Pin in high Z (impedance) so that the piezo positive is free from groun.
        Pin_1_1_Piezo_SetDriveMode( Pin_1_1_Piezo_DM_ALG_HIZ );
        
        // We are going to make the pin high Z (impedance) so that it will not 
        // consume current.
        Pin_P0_7_switch_SetDriveMode( Pin_P0_7_switch_DM_ALG_HIZ );
        
        /* Start Low Power Comparator */
 //       LPComp_movement_detector_Start();
        
        // Hibernate.
        CySysPmHibernate();
    }
    
    /*
    for(;;)
    {
        // PWM_SOUND_FREQ_TEST_01();
        // PWM_SOUND_FREQ_TEST_02();

        // Portuguese national anthem.
        // PWM_SOUND_FREQ_TEST_Music();
        
        // Brahms lullaby
        PWM_SOUND_FREQ_TEST_Music_Brahms_Lullaby();
    }
    */
}

/* [] END OF FILE */
