


// gsensor
#include "accelerometer.h"

// platform support
#include "board.h"
#include "gpio_ctrl.h"
#include "system_clock.h"
#include "i2c.h"
#include "spi.h"
#include "debug_log.h"


// c
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// pah8011_Driver
#include "pah_driver.h"
#include "pah_comm.h"
#include "pah_driver_types.h"

// pah8011_Function
#include "pah_vp_function.h"
// pah

#include "pah_verify.h"
//Pixart Factory test 
#include "pah_factory_test.h"






/*============================================================================
STATIC VARIABLES
============================================================================*/


static volatile bool        has_interrupt_button = false;

static volatile bool        has_interrupt_pah = false;

static volatile uint64_t    interrupt_pah_timestamp = 0;





/*============================================================================
STATIC FUNCTION PROTOTYPES
============================================================================*/

void demo_ppg_dri_VP(void);
void demo_ppg_polling_VP(void);

static void demo_factory_mode(void);





static void gpio_in_interrupt_handler(GPIO_IN gpio);
static void gpio_btn_interrupt_handler(GPIO_BTN gpio);
static void Error_Handler(void);



int main(void)
{
 
    
    // bsp
    board_init();
    system_clock_init();
    gpio_init();
    gpio_in_set_interrupt_callback(gpio_in_interrupt_handler);
    gpio_btn_set_interrupt_callback(gpio_btn_interrupt_handler);
    debug_log_init();
    
    // pause
    debug_printf("\n");
    debug_printf("Wait for button to start... \n");
    while (!has_interrupt_button);
    has_interrupt_button = false;
    delay_ms(1000);
    
    // gsensor
    if (!accelerometer_init())
        Error_Handler();
    
    // i2c
    if (!i2c_init(pah_get_i2c_slave_addr()))
        Error_Handler();
    //if (!spi_init())
    //    Error_Handler();

    /*============================================================================
    For PPG_INT_MODE_VP
	============================================================================*/    
    //demo_ppg_dri_VP();
	/*============================================================================
	For PPG_POLLING_MODE_VP
	============================================================================*/    
    //demo_ppg_polling_VP();
    /*============================================================================
    For FACTORY_TEST_MODE
    ============================================================================*/
    //demo_factory_mode();

}


/////////////////////////8011ES DRIVER MAIN////////////////////////


void demo_ppg_dri_VP(void)
{
	//if sensor power up , please do pah8series_ppg_dri_VP_init()
	//This Function link register and set PPG sensor to power down
    pah8series_ppg_dri_VP_init();

    //Calibration Mode//
    //if customer want to start VP calibration, please call pah8series_ppg_dri_VP_Calibration_start(real_sbp,real_dbp)
    //This Function will enable the ppg sensor, and do calibration with real sbp/dbp(ex:from omron)
    pah8series_ppg_dri_VP_Calibration_start(120,60);

    //Physical Calibration Mode//
    //if customer want to start VP calibration, please call pah8series_ppg_dri_VP_Calibration_start(user_gender,user_age,user_height,user_weight)
    //This Function will enable the ppg sensor, and do calibration with physical information(gender,age,height,weight)), 1:male, 0:female
    //pah8series_ppg_dri_VP_Phy_Calibration_start(1,25,170,60);

    //Estimation Mode//
    //if customer want to start VP estimation, please call pah8series_ppg_dri_VP_Estimation_start();
    //This Function will enable the ppg sensor, and should be used after clibration is done
    //pah8series_ppg_dri_VP_Estimation_start();

    //Physical Estimation Mode//
    //if customer want to start VP estimation, please call pah8series_ppg_dri_VP_Estimation_start();
    //This Function will enable the ppg sensor, and should be used after physical clibration is done
    //pah8series_ppg_dri_VP_Phy_Estimation_start();
    
    //WithoutCal Estimation Mode//
    //pah8series_ppg_dri_VP_WithoutCal_Estimation_start();

    while (1)
    {      
        pah8series_ppg_dri_VP_task(&has_interrupt_pah,&has_interrupt_button ,&interrupt_pah_timestamp);
		//if customer want to stop PPG Sensor , please call pah8series_ppg_VP_stop()
		//This Function will disable the ppg sensor
		//pah8series_ppg_VP_stop();
		NRF_LOG_FLUSH();	//NRF platform
        
    }    // end of while
}

void demo_ppg_polling_VP(void)
{
    //if sensor power up , please do pah8series_ppg_polling_VP_init()
	//This Function link register and set PPG sensor to power down
    pah8series_ppg_polling_VP_init();

    //Calibration Mode//
    //if customer want to start VP calibration, please call pah8series_ppg_dri_VP_Calibration_start(real_sbp,real_dbp)
    //This Function will enable the ppg sensor, and do calibration with real sbp/dbp(ex:from omron)
	pah8series_ppg_dri_VP_Calibration_start(120,60); 

    //Physical Calibration Mode//
    //if customer want to start VP calibration, please call pah8series_ppg_dri_VP_Calibration_start(user_gender,user_age,user_height,user_weight)
    //This Function will enable the ppg sensor, and do calibration with physical information(gender,age,height,weight)), 1:male, 0:female
    //pah8series_ppg_dri_VP_Phy_Calibration_start(1,25,170,60);

	//Estimation Mode//
	//if customer want to start VP estimation, please call pah8series_ppg_polling_VP_Estimation_start();
	//This Function will enable the ppg sensor, and should be used after clibration is done
	//pah8series_ppg_dri_VP_Estimation_start();

    //Physical Estimation Mode//
    //if customer want to start VP estimation, please call pah8series_ppg_dri_VP_Estimation_start();
    //This Function will enable the ppg sensor, and should be used after physical clibration is done
    //pah8series_ppg_dri_VP_Phy_Estimation_start();
    
    //WithoutCal Estimation Mode//
    //pah8series_ppg_dri_VP_WithoutCal_Estimation_start();

    while (1)
    {
        pah8series_ppg_polling_VP_task(&has_interrupt_pah,&has_interrupt_button );
		//if customer want to stop PPG Sensor , please call pah8series_ppg_VP_stop()
		//This Function will disable the ppg sensor
		//pah8series_ppg_VP_stop();
        delay_ms(200);
    }    // end of while
}


static void demo_factory_mode(void)
{
#ifdef FACTORY_TEST_ES
	
		//factory_test_mode(factory_test_led_golden_only,Enable,expo_ch_b,expo_ch_c);
    //Customer can turn expo time to Get ADC_Data 2000
    factory_test_mode(factory_test_led_golden_only,1,0x42,0x42);

		//Get Correct value for the expo_ch_b,expo_ch_c
    //Please Modify the pah_verify_8011_reg.h pah8011_verify_led_brightness_array[][2] array
    factory_test_mode(factory_test_led_target_sample,0,0,0);
	
    factory_test_mode(factory_test_light_leak,0,0,0);
	
		//For golden sample only
	  factory_test_mode(factory_test_touch_value,0,0,0);	
	
		//Set Int Pin to LOW, Customer Check GPIO was low level
    factory_test_mode(factory_test_INT_low,0,0,0);
	
	delay_ms(50);
	uint32_t in_level1 = nrf_gpio_pin_read(GPIO_INPUT_PIN_NUMBER);
	debug_printf("in_level1 = %d   \n",in_level1);
	pah_enter_mode(pah_stop_mode);
	
	//Set Int Pin to HIGH, Customer Check GPIO was high level
	factory_test_mode(factory_test_INT_high,0,0,0);
	delay_ms(50);
	uint32_t in_level2 = nrf_gpio_pin_read(GPIO_INPUT_PIN_NUMBER);
	debug_printf(" in_level2 = %d \n",in_level2);
	pah_enter_mode(pah_stop_mode);
#endif

#ifdef FACTORY_TEST_ET

    //factory_test_mode(factory_test_led_golden_only,Enable,expo_ch_b,expo_ch_c);
    //Customer can turn expo time to Get ADC_Data 2000
    factory_test_mode(factory_test_led_golden_only,1,0x32,0x32);

    //Get Correct value for the expo_ch_b,expo_ch_c
    //Please Modify the pah_verify_8011_reg.h pah8011_verify_led_brightness_array[][2] array
    factory_test_mode(factory_test_led_target_sample,0,0,0);

    factory_test_mode(factory_test_light_leak,0,0,0);
		
		//For golden sample only
	factory_test_mode(factory_test_touch_value,0,0,0);
	
		//Set Int Pin to LOW, Customer Check GPIO was low level
    factory_test_mode(factory_test_INT_low,0,0,0);

	delay_ms(50);
	uint32_t in_level1 = nrf_gpio_pin_read(GPIO_INPUT_PIN_NUMBER);
	debug_printf("in_level1 = %d   \n",in_level1);
	pah_enter_mode(pah_stop_mode);
	
	//Set Int Pin to HIGH, Customer Check GPIO was high level
	factory_test_mode(factory_test_INT_high,0,0,0);
	delay_ms(50);
	uint32_t in_level2 = nrf_gpio_pin_read(GPIO_INPUT_PIN_NUMBER);
	debug_printf(" in_level2 = %d \n",in_level2);
	pah_enter_mode(pah_stop_mode);
#endif

}


#if 0
/**@brief Function for the Power Management. 
 */ 
static void power_manage(void) 
{ 
    // Use directly __WFE and __SEV macros since the SoftDevice is not available. 
    
    // Wait for event. 
    __WFE(); 

    // Clear Event Register. 
    __SEV(); 
    __WFE(); 
} 
#endif