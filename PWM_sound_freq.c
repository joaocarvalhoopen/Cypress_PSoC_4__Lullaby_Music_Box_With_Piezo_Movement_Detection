/* ========================================
 * Autor: João Nuno Carvalho
 * Date: 07.05.2015
 *
 * License: MIT license.
 *
 * Note: To use this lib file you have to change in the header file the value name PWM_PIEZO of the macro PWM_COMPO_NAME,
 *       to the name of the PWM component that you choose to include in your design. 
 * ========================================
*/

#include <project.h>

#include <PWM_sound_freq.h>

#include <gen_lib_utils.h>  // Token concatenation macros.

// Configuration of hardware component name.
// PWM_PIZEO is the name of the PWM componente that you have givem when you added the component in the schematic design drawing, in PSoC Creator.
#define PWM_COMPO_NAME PWM_PIEZO 
#define PWM_COMPO( func_end_name ) FUNC_NAME_CONCAT_2( PWM_COMPO_NAME, func_end_name )
#define CLOCK_PWM_COMPO_NAME Clock_PWM_PIEZO 
#define CLOCK_PWM_COMPO( func_end_name ) FUNC_NAME_CONCAT_2( CLOCK_PWM_COMPO_NAME, func_end_name )
// NOTE: The preceding defines are this file and not in a header file to minimize the numbers of files and the name colisions,
//       they could also be in a separate header file, that would be only included in this C file.

#include <stdlib.h>  // For malloc and free functions.


    // Constructor
    // freq_clock: For best range in PWM 16bit counter use 1.2MHz
    PWM_SOUND_FREQ * PWM_SOUND_FREQ_CONSTRUCT(uint32 freq_clock){
        PWM_SOUND_FREQ * tmp_self = (PWM_SOUND_FREQ *) malloc( sizeof(PWM_SOUND_FREQ) );
        tmp_self->freq_clock = freq_clock;
        return tmp_self;
    }

    void PWM_SOUND_FREQ_DESTROY(PWM_SOUND_FREQ * self){
        // PWM_PIEZO_Stop();
        PWM_COMPO(_Stop) ();
        // Clock_PWM_PIEZO_Stop();
        CLOCK_PWM_COMPO(_Stop) ();
        free(self);    
    }

    /*
    freq_sound: 20Hz to 20KHz
    duration_ms: If is zero then starts the playing the sound until a PWM_SOUND_FREQ_STOP(void) is called.
    */
    void PWM_SOUND_FREQ_PLAY(PWM_SOUND_FREQ * self, uint32 freq_sound, uint32 duration_ms){
        // Parameter validation.
        if (freq_sound < 20)
            freq_sound = 20;
        else if (freq_sound > 20000)
            freq_sound = 20000;
        // Clock_PWM_PIEZO_Start();
        CLOCK_PWM_COMPO(_Start) ();
        //PWM_PIEZO_Start();
        PWM_COMPO(_Start) ();
        uint32 period = self->freq_clock / freq_sound;    
        // PWM_PIEZO_WritePeriod(period);
        PWM_COMPO(_WritePeriod) (period);
        uint32 comparator = period * 0.5;
        // PWM_PIEZO_WriteCompare(comparator);
        PWM_COMPO(_WriteCompare) (comparator);
        if (duration_ms != 0)
        {
          CyDelay(duration_ms);
          // PWM_PIEZO_Stop();
          PWM_COMPO(_Stop) ();
          // Clock_PWM_PIEZO_Stop();
          CLOCK_PWM_COMPO(_Stop) ();
        }
    }
    
    void PWM_SOUND_FREQ_STOP(PWM_SOUND_FREQ * self){
        // PWM_PIEZO_Stop();
        PWM_COMPO(_Stop) ();
        // Clock_PWM_PIEZO_Stop();
        CLOCK_PWM_COMPO(_Stop) ();       
    }
   
    // The sequence to be played can be from flash program memory, or can be from ram,
    // but beware that RAM size is small, and songs should be pushed to a array
    // initialization in flash program memory.
    // freq_scale_factor: Normaly should be 1.0, but for same piezo disks can be greater to adjust to their responce curve. 
    void PWM_SOUND_FREQ_PLAY_SEQUENCE(PWM_SOUND_FREQ * self, element_note* music_sequence, uint32 num_of_notes, float32 freq_scale_factor){
        if (freq_scale_factor == 0.0 || freq_scale_factor == 1.0)
            freq_scale_factor = 1.0;
        uint32 note_freq;
        uint32 duration_ms;
        const uint32 zero_duration = 0;
        uint32 i;
        for(i=0; i < num_of_notes; i++){
            note_freq   = music_sequence[i].note * freq_scale_factor;
            duration_ms = music_sequence[i].duration;
            if (note_freq != 0){
                PWM_SOUND_FREQ_PLAY( self, note_freq, zero_duration );
            }else{
                PWM_SOUND_FREQ_STOP( self );
            }
            CyDelay( duration_ms );
        }
        PWM_SOUND_FREQ_STOP( self ); 
    }        
        
     
    
    
    
//Test cases and examples. 

    
void PWM_SOUND_FREQ_TEST_01(void){

    uint32 freq_clock = 1200000; // 12MHZ This gives the best resolution for the range, but can have other values.
    
    PWM_SOUND_FREQ * sound_obj = PWM_SOUND_FREQ_CONSTRUCT( freq_clock );
    
    uint32 freq_sound = 0;
    uint32 duration_ms = 0; // 300;
    
    // Testes one tone(1000Hz) frequency for 2 seconds at full sound power(max volume), it auto terminates the sound.
    freq_sound     = 1000; // 1KHz
    duration_ms    = 60000; // 60 seconds
    PWM_SOUND_FREQ_PLAY(sound_obj, freq_sound, duration_ms);
    // Pauses for 1 second.
    CyDelay(1000); 
    PWM_SOUND_FREQ_STOP(sound_obj);
    PWM_SOUND_FREQ_DESTROY( sound_obj );

} // END PWM_SOUND_FREQ_TEST_01(void)

    
void PWM_SOUND_FREQ_TEST_02(void){

    uint32 freq_clock = 1200000; // 12MHZ This gives the best resolution for the range, but can have other values.
    
    PWM_SOUND_FREQ * sound_obj = PWM_SOUND_FREQ_CONSTRUCT( freq_clock );
    
    uint32 freq_sound = 0;
    uint32 duration_ms = 0; // 300;
    
    // Testes one tone(1000Hz) frequency for 2 seconds at full sound power(max volume), it auto terminates the sound.
    freq_sound     = 1000; // 1KHz
    duration_ms    = 2000; // 2 seconds
    PWM_SOUND_FREQ_PLAY(sound_obj, freq_sound, duration_ms);
    
    // Pauses for 1 second.
    CyDelay(1000);
    
    // Play the same tone for 2 seconds with method play and method stop.
    freq_sound     = 1000; // 1KHz
    duration_ms    = 0;    // 0 miliseconds
    PWM_SOUND_FREQ_PLAY(sound_obj, freq_sound, duration_ms);
    CyDelay(2000); // 2 seconds
    PWM_SOUND_FREQ_STOP(sound_obj);
    
    // Pauses for 1 second.
    CyDelay(1000);
    
    // Tests the linear change from 20Hz to 20KHz in frequency for 5 seconds.
    freq_sound     = 0; // 0KHz it transforms into 20Hz
    duration_ms    = 0;    // 0 miliseconds
    uint32 i;
    for(i = 0; i <= 20; i++){
        PWM_SOUND_FREQ_PLAY(sound_obj, freq_sound, duration_ms);
        CyDelay(250); // 250 ms
        freq_sound += 500;
    }
    PWM_SOUND_FREQ_STOP(sound_obj);

    // Pauses for 1 second.
    CyDelay(1000);
    
    // Tests the logaritmic change in frequency.

    PWM_SOUND_FREQ_DESTROY( sound_obj );

} // END PWM_SOUND_FREQ_TEST_01(void)


//Test cases play Portuguese national anthem "A Portuguesa". 
void PWM_SOUND_FREQ_TEST_Music(void){
    
  // Note: Table obtained from site: http://www.phy.mtu.edu/~suits/notefreqs.html

  // Frequency of each musical note.
    const uint16 NULA = 0;

    const uint16 sol_A3 = 220;
    const uint16 B3 = 247;


    const uint16 C4  = 261;
    const uint16 Cs4 = 277;
    const uint16 Db4 = 277;
    const uint16 D4  = 294;
    const uint16 Ds4 = 311;
    const uint16 Eb4 = 311;
    const uint16 E4  = 330;
    const uint16 F4  = 349;
    const uint16 Fs4 = 370;
    const uint16 Gb4 = 370;
    const uint16 G4  = 392;
    const uint16 Gs4 = 415;
    const uint16 Ab4 = 415;
    const uint16 sol_A4  = 440;
    const uint16 As4 = 466;
    const uint16 Bb4 = 466;
    const uint16 B4  = 494;

    const uint16 C5 = 523;
    const uint16 D5 = 587;
    const uint16 E5 = 659;

    const uint16 E7 = 2637;
    const uint16 G7 = 3136;

     
    // Duracao de cada nota.
    const uint16 factor = 2; // 1.5
    const uint16 T3   = (500+250)*factor;
    const uint16 T2   = 500*factor;
    const uint16 T15  = (250 + 125)*factor;
    const uint16 T1   = 250*factor;
    const uint16 T05  = 125*factor;
    const uint16 T025 = 62*factor;

    
    /////////////////////////////////////////////////
    // "A Portuguesa" Hino de Portugal.
    //
    // Note: o Hino foi convertido da pauta proveniente do site:
    //       http://www.heroisdomar.com/hino/partitura.html
    const element_note notes_portuguse_anthem[] = {
            { E4, T2},
            { G4, T2},

            { F4, T2},
            { E4, T1},
            { sol_A4, T1},
            
            { G4, T1},
            { C4, T05},
            { D4, T05},
            { E4, T1},
            { D4, T1},
            
            { C4, T2},
            { C4, T05},
            { NULA, T025},
            { G4, T025},
            { E4, T05},
            { G4, T025},

            ////////////////////////////
            
            { sol_A4, T2},
            { G4, T1},
            { G4, T025},
            { sol_A4, T025},
            
            { G4, T1},
            { F4, T1},
            { E4, T05},
            { C4, T025},
            { B3, T05},     ///
            { C4, T025},
            
            { E4, T2},
            { F4, T05},
            { G4, T025},
            { sol_A4, T05},
            { B4, T025},

            ///////////////////////////
            { G4, T2},
            { NULA, T025},
            { G4, T05},
            { B4, T025},
            
            { D5, T2},  ////
            { B4, T1},
            { G4, T05},
            { F4, T05},
            
            { F4, T2},
            { E4, T1},
            { G4, T05},
            { E4, T025},

            ///////////////////////////
            { C4, T2},
            { C4, T05},
            { D4, T05},
            { E4, T05},
            { G4, T05},
            
            { D4, T2},
            { D4, T05},
            { NULA, T1},
            { NULA, T05},
            
            { D4, T15},
            { E4, T05},
            { C4, T1},
            { C4, T1},

            ///////////////////
            { F4, T15},
            { G4, T05},
            { E4, T1},
            { E4, T05},
            { G4, T05},
            
            { C5, T15},   ///
            { G4, T05},
            { G4, T05},
            { B4, T1},
            { sol_A4, T05},

            { G4, T1},
            { D4, T2},
            { NULA, T05},
            { F4, T05},

            //////////////////
            { E4, T1},
            { G4, T1},
            { G4, T1},
            { F4, T05},
            { E4, T05},
            
            { D4, T3},
            { NULA, T05},
            { F4, T05},
            
            { E4, T1},
            { G4, T05},
            { G4, T025},
            { G4, T1},
            { F4, T05},
            { E4, T05},

            ////////////////////      
            { G4, T1},
            { G4, T025},
            { G4, T1},
            { NULA, T1},
            { G4, T025},
            
            { C5, T2}, ///
            { B4, T1},
            { NULA, T025},
            { F4, T025},

            { sol_A4, T2},
            { G4, T1},
            { C4, T05},
            { D4, T05},

            /////////////////////
            { E4, T1},
            { E4, T1},
            { G4, T1},
            { E4, T1},

            { D4, T2},
            { D4, T05},
            { NULA, T05},
            { NULA, T025},
            { G4, T025},
            
            { C5, T2}, ///
            { B4, T1},
            { NULA, T025},
            { F4, T025},

            //////////////////////
            { sol_A4, T2},
            { G4, T1},
            { NULA, T025},
            { G4, T05},
            { C5, T05},  ///

            { E5, T2},   ///
            { D5, T15},  ///
            { C5, T025}, ///

            { C5, T2},   ///
            { C5, T05},  ///
            { E4, T05},
            { F4, T05},
            { G4, T05},

            //////////////////////
            { sol_A4, T1},
            { sol_A4, T1},
            { G4, T15},
            { G4, T05},
            
            { C5, T2},    ///
            { C5, T05},   ///
            { NULA, T05},
            { NULA, T1},
            
            };

    // End of "A Portuguesa".
    /////////////////////////////////////////////////

    uint32 freq_clock = 1200000; // 1.2MHz
    PWM_SOUND_FREQ * sound_obj = PWM_SOUND_FREQ_CONSTRUCT( freq_clock );
    // NOTE: The cast is present in the following line so that we can use constant array or a allocated array.
    uint32 num_music_notes = sizeof(notes_portuguse_anthem)/sizeof(notes_portuguse_anthem[0]);
    float32 freq_scale_factor = 1.0;
    PWM_SOUND_FREQ_PLAY_SEQUENCE( sound_obj, 
                                  (element_note *) &notes_portuguse_anthem, 
                                  num_music_notes,
                                  freq_scale_factor );    
    PWM_SOUND_FREQ_DESTROY( sound_obj );
    CyDelay(2000); // 2 seconds.
    
} // END PWM_SOUND_FREQ_TEST_Music(void)

//////////////////////
// Brahms Lullaby
//////////////////////

//Test cases play Brahms Lullaby. 
void PWM_SOUND_FREQ_TEST_Music_Brahms_Lullaby(void){
    
  // Note: Table obtained from site: http://www.phy.mtu.edu/~suits/notefreqs.html

  // Frequency of each musical note.
    const uint16 NULA = 0;

    const uint16 sol_A3 = 220;
    const uint16 B3 = 247;


    const uint16 C4  = 261;
    const uint16 Cs4 = 277;
    const uint16 Db4 = 277;
    const uint16 D4  = 294;
    const uint16 Ds4 = 311;
    const uint16 Eb4 = 311;
    const uint16 E4  = 330;
    const uint16 F4  = 349;
    const uint16 Fs4 = 370;
    const uint16 Gb4 = 370;
    const uint16 G4  = 392;
    const uint16 Gs4 = 415;
    const uint16 Ab4 = 415;
    const uint16 sol_A4  = 440;
    const uint16 As4 = 466;
    const uint16 Bb4 = 466;
    const uint16 B4  = 494;

     
    const uint16 C5 = 523;
    const uint16 D5 = 587;
    const uint16 E5 = 659;

    const uint16 E7 = 2637;
    const uint16 G7 = 3136;

     
    // Duracao de cada nota.
    const uint16 factor = 2; // 1.5
    const uint16 T3   = (500+250)*factor;
    const uint16 T2   = 500*factor;
    const uint16 T15  = (250 + 125)*factor;
    const uint16 T1   = 250*factor;
    const uint16 T05  = 125*factor;
    const uint16 T025 = 62*factor;

    
    /////////////////////////////////////////////////
    // Brahms Lullaby
    // Music converted converted from score in:
    // http://www.true-piano-lessons.com/brahms-lullaby.html
    const element_note notes_brahms_lullaby[] = {
    
                {E4, T05},
                {E4, T05},

                {G4, T15},
                { NULA, T025}, ////
                {E4, T05},
                {E4, T1},

                {G4, T15}, //{G4, T2},
                { NULA, T025}, ////
                {E4, T05},
                {G4, T05},

                {C5, T1},
                {B4, T1},
                {sol_A4, T1},

                {sol_A4, T1},
                {G4, T1},
                {D4, T05},
                {E4, T05},

                //////////////

                {F4, T1},
                {D4, T1},
                {D4, T05},
                {E4, T05},

                {F4, T2},
                {D4, T05},
                {F4, T05},

                {B4, T05},
                {sol_A4, T05},
                {G4, T1},
                {B4, T1},

                {C5, T2},
                {C4, T05},
                {C4, T05},

                /////////////

                {C5, T2},
                {sol_A4, T05},
                {F4, T05},

                {G4, T2},
                {E4, T05},
                {C4, T05},

                {F4, T1},
                {G4, T1},
                {sol_A4, T1},

                {E4, T05},
                {G4, T15},
                {C4, T05},
                {C4, T05},

                /////////////

                {C5, T2},
                {sol_A4, T05},
                {F4, T05},

                {G4, T2},
                {E4, T05},
                {C4, T05},

                {F4, T05},
                {G4, T025},
                {F4, T025},
                {E4, T1},
                {D4, T1},

                {C4, T3},
        
          //////////////////////////////////////
                { NULA, T1},
            
                };

    // End of "Brahms Lullaby".
    /////////////////////////////////////////////////

    uint32 freq_clock = 1200000; // 1.2MHz
    PWM_SOUND_FREQ * sound_obj = PWM_SOUND_FREQ_CONSTRUCT( freq_clock );
    // NOTE: The cast is present in the following line so that we can use constant array or a allocated array.
    uint32 num_music_notes = sizeof(notes_brahms_lullaby)/sizeof(notes_brahms_lullaby[0]);
    float32 freq_scale_factor = 1.0;
    PWM_SOUND_FREQ_PLAY_SEQUENCE( sound_obj, 
                                  (element_note *) &notes_brahms_lullaby, 
                                  num_music_notes,
                                  freq_scale_factor );    
    PWM_SOUND_FREQ_DESTROY( sound_obj );
    CyDelay(2000); // 2 seconds.
    
} // END PWM_SOUND_FREQ_TEST_Music_Brahms_Lullaby(void)

    
/* [] END OF FILE */
