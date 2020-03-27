/* ========================================
 * Autor: João Nuno Carvalho
 * Date: 07.05.2015
 *
 * License: MIT license.
 *
 * Note: To use this lib file you have to change the value name PWM_PIEZO of the macro PWM_COMPO_NAME,
 *       to the name of the PWM component that you choose to include in your design. 
 * ========================================
*/

#if !defined(PWM_SOUND_FREQ_H)
#define PWM_SOUND_FREQ_H

    #include <cytypes.h>

    typedef struct PWM_SOUND_FREQ
    {
        uint32 freq_clock;    
        
    }PWM_SOUND_FREQ; 
    
    typedef PWM_SOUND_FREQ* PWM_SOUND_FREQ_PTR;
    
    typedef struct ELEMENT_NOTE{
        uint16 note;
        uint16 duration;
    }element_note;
    
    
    // Function Prototypes
    PWM_SOUND_FREQ * PWM_SOUND_FREQ_CONSTRUCT(uint32 freq_clock);
    void PWM_SOUND_FREQ_DESTROY(PWM_SOUND_FREQ * self);

    void PWM_SOUND_FREQ_PLAY(PWM_SOUND_FREQ * self, uint32 freq_sound, uint32 duration_ms);
    void PWM_SOUND_FREQ_STOP(PWM_SOUND_FREQ * self);
    
    void PWM_SOUND_FREQ_PLAY_SEQUENCE(PWM_SOUND_FREQ * self, element_note* music_sequence, uint32 num_of_notes, float32 freq_scale_factor);
    
    
    // Test/example cases.
    void PWM_SOUND_FREQ_TEST_01(void);
    void PWM_SOUND_FREQ_TEST_02(void);
    //Portuguese National Anthem.
    void PWM_SOUND_FREQ_TEST_Music(void);
    void PWM_SOUND_FREQ_TEST_Music_Brahms_Lullaby(void);
    
    
#endif /* !defined(PWM_SOUND_FREQ_H) */

/* [] END OF FILE */
