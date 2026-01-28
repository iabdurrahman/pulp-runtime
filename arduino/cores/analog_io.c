#include "analog_io.h"

static int def_resolution = 255;

int analogRead(int pin){

}

void analogReadResolution(int bits){
    
}

void analogReference(uint8_t type){

}

void analogWrite(int pin, int val){
    int retval;

    /* initialize to invalid value */
    size_t   timer   = 0xffffffff;
	size_t   channel = 0xffffffff;

    /* register value */
	uint32_t config;
	uint32_t event_cfg = 0;
	uint32_t th_channel = 0;

	uint32_t padmux;
	int max_resolution = def_resolution;
	int min_resolution = 0;

	/* get timer and channel for current gpio */
	retval = adv_timer_get_timer_and_channel_from_io(GPIO_PWM, GPIO_PWM_MUX,
		&timer, &channel);
	if (retval != 0)
	{
		/* if we cannot get timer and channel then just return */
		return retval;
	}

	/* make sure we get correct result */
	if ((timer < 0) || (timer >= 4) || (channel < 0) || (channel >= 4))
	{
		return (-1);
	}

	hal_apb_soc_pad_set_function(GPIO_PWM, GPIO_PWM_MUX);

	adv_timer_stop_and_reset(timer);

	event_cfg = adv_timer_event_cfg_get(timer);
	event_cfg = ADV_TIMER_EVENT_CFG_ENA_SET(event_cfg, 0x00);
	adv_timer_event_cfg_set(timer, event_cfg);
	adv_timer_enable_clock(timer);
	adv_timer_config_frequency(timer, 4, max_resolution);
    th_channel = ADV_TIMER_TN_TH_CHANNELM_TH_SET(th_channel, val);

    if (val == 0)
	{
		th_channel = ADV_TIMER_TN_TH_CHANNELM_MODE_SET(th_channel, 0x4);
	}

    else if (val == 255)
	{
		th_channel = ADV_TIMER_TN_TH_CHANNELM_MODE_SET(th_channel, 0x0);
	}
    else
	{
		th_channel = ADV_TIMER_TN_TH_CHANNELM_MODE_SET(th_channel, 0x6);
	}
    adv_timer_tn_th_channelm_set(ARCHI_ADV_TIMER_ADDR, timer,
		channel, th_channel);

    adv_timer_update(timer);

    /* start timer */
	adv_timer_start(timer);

	printf("Entering test\n");

    /* let hardware run the pwm without processor involvement */
	while (true)
	{
		/* let cpu go sleep */
		__asm__ __volatile__ (
			"wfi\n"
			:
			:
			:
		);
	}
}

void analogWriteResolution(int bits){

}