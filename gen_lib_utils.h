/* ========================================
 *  Project name: PSoc4_PWM_sound
 *  Author:       João Nuno Carvalho
 *  Date:         07.05.2015
 *  License:      MIT open source license.
 * ========================================
*/

#if !defined(GEN_LIB_UTILS_H)
#define GEN_LIB_UTILS_H

    // NOTE: To make the correct macro expansion of PWM_COMPO_NAME we must have 2 levels of FUNC_NAME_CONCAT,
    //       if we don't do this the PWM_COMPO_NAME will not be expanded.
    #define FUNC_NAME_CONCAT( func_start_name, func_end_name ) func_start_name ## func_end_name  
    #define FUNC_NAME_CONCAT_2( func_start_name, func_end_name ) FUNC_NAME_CONCAT(func_start_name, func_end_name)  

    // Example case:    
    /*    
    // Configuration of hardware component name.
    // PWM_PIZEO is the name of the PWM componente that you have givem when you added the component in the schematic design drawing, in PSoC Creator.
    #define PWM_COMPO_NAME PWM_PIEZO 
    #define PWM_COMPO( func_end_name ) FUNC_NAME_CONCAT_2( PWM_COMPO_NAME, func_end_name )
    */
    
#endif /* !defined(GEN_LIB_UTILS_H) */



/* [] END OF FILE */
