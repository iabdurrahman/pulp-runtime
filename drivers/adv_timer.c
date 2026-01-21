/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdint.h>
#include <stddef.h>

#include <pulp.h>


/**
	* from given gpio pin index/number and mux_index,
	* check which timer and channel connected to it
	*
	* arg:
	*          input  gpio:      index of gpio
	*          input  mux_index: index of mux value
	*          output timer:     index of adv timer unit (0-3)
	*          output channel:   index of channel of the adv timer unit (0-3)
	*
	* return:
	*          0: success
	*          non-zero: fail / given gpio pin don't have timer channel
	*/
int adv_timer_get_timer_and_channel_from_io(
	unsigned int gpio, unsigned int mux_index,
	size_t *timer, size_t *channel)
{
	/* initialize retval to always return fail */
	int retval = (-1);

	/**
		* check at given gpio pin, which one has timer channel
		* if given gpio pin don't have timer output, then just return fail
		*/
	switch (mux_index)
	{
	case (0): /* no timer channel in any gpio for mux 0 */
	case (1): /* no timer channel in any gpio for mux 1 */
		break; /* from switch (mux_index) */
	case (2):
		/* some gpio pin can be act as timer output */
		switch (gpio)
		{
		case ( 0):
		case ( 1):
		case ( 2):
		case ( 3):
		case ( 4):
		case ( 5):
		case ( 6):
		case ( 7):
		case ( 8):
			break; /* from switch (gpio) */
		case ( 9): /* timer1_i[0] */
			*timer = 1;
			*channel = 0;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (10): /* timer1_i[1] */
			*timer = 1;
			*channel = 1;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (11): /* timer1_i[2] */
			*timer = 1;
			*channel = 2;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (12): /* timer1_i[3] */
			*timer = 1;
			*channel = 3;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (13): /* timer2_i[0] */
			*timer = 2;
			*channel = 0;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (14): /* timer2_i[1] */
			*timer = 2;
			*channel = 1;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (15): /* timer2_i[2] */
			*timer = 2;
			*channel = 2;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (16): /* timer2_i[3] */
			*timer = 2;
			*channel = 3;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (17): /* timer3_i[0] */
			*timer = 3;
			*channel = 0;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (18): /* timer3_i[1] */
			*timer = 3;
			*channel = 1;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (19): /* timer3_i[2] */
			*timer = 3;
			*channel = 2;
			retval = 0; /* set retval to success */
			break; /* from switch (gpio) */
		case (20):
		case (21):
		case (22):
		case (23):
		case (24):
		case (25):
		case (26):
		case (27):
		case (28):
		case (29):
		case (30):
		case (31):
		case (32):
		case (33):
		case (34):
		case (35):
		case (36):
		case (37):
		case (38):
		case (39):
		case (40):
		case (41):
		case (42):
		case (43):
		case (44):
		case (45):
		case (46):
		case (47):
		case (48):
		case (49):
		case (50):
		case (51):
		case (52):
		case (53):
		case (54):
		case (55):
		case (56):
		case (57):
		case (58):
		case (59):
		case (60):
		case (61):
		case (62):
		case (63):
			break;
		/*default:*/ /* of switch (gpio) */
			/* error: label at end of compound statement */
		}
		break; /* from switch (mux_index) */
	case (3): /* no timer channel in any gpio for mux 3 */
		break;
	/*default:*/ /* of switch (mux_index) */
		/* error: label at end of compound statement */
	}

	return retval;
}



/**
	* start timer by send start command to timer and enabling timer clock
	*
	* arg:
	*          input  timer:     index timer (0-3)
	*
	* return:
	*          (none)
	*/
void adv_timer_start(size_t timer)
{
	uint32_t tn_cmd = 0;

	adv_timer_enable_clock(timer);

	/* start timer then start clock */
	tn_cmd = ADV_TIMER_TN_CMD_START_SET(tn_cmd, 0x01);
	adv_timer_tn_cmd_set(ARCHI_ADV_TIMER_ADDR, timer, tn_cmd);
}

/**
	* stop timer and disable clock to timer
	*
	* arg:
	*          input  timer:     index timer (0-3)
	*
	* return:
	*          (none)
	*/
void adv_timer_stop(size_t timer)
{
	uint32_t tn_cmd = 0;

	/* stop timer then stop clock */
	tn_cmd = ADV_TIMER_TN_CMD_STOP_SET(tn_cmd, 0x01);
	adv_timer_tn_cmd_set(ARCHI_ADV_TIMER_ADDR, timer, tn_cmd);

	adv_timer_disable_clock(timer);
}

/**
	* stop timer, disable clock to timer,
	* and reset timer (reset counter, pwm state, etc.)
	*
	* arg:
	*          input  timer:     index timer (0-3)
	*
	* return:
	*          (none)
	*/
void adv_timer_stop_and_reset(size_t timer)
{
	uint32_t tn_cmd = 0;

	/* stop timer */
	adv_timer_stop(timer);

	/* reset timer */
	tn_cmd = ADV_TIMER_TN_CMD_RESET_SET(tn_cmd, 0x01);
	adv_timer_tn_cmd_set(ARCHI_ADV_TIMER_ADDR, timer, tn_cmd);
}


/**
	* update timer setting (useful when timer is already started)
	*
	* arg:
	*          input  timer:     index timer (0-3)
	*
	* return:
	*          (none)
	*/
void adv_timer_update(size_t timer)
{
	uint32_t tn_cmd = 0;

	/* reset timer */
	tn_cmd = ADV_TIMER_TN_CMD_UPDATE_SET(tn_cmd, 0x01);
	adv_timer_tn_cmd_set(ARCHI_ADV_TIMER_ADDR, timer, tn_cmd);
}


/**
	* enable timer clock
	*
	* arg:
	*          input  timer:     index timer (0-3)
	*
	* return:
	*          (none)
	*/
void adv_timer_enable_clock(size_t timer)
{
	uint32_t cg = adv_timer_cg_get(ARCHI_ADV_TIMER_ADDR);
	switch (timer)
	{
	case (0):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg | (1u << (ADV_TIMER_CG_ENA_BIT + 0u)));
		break;
	case (1):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg | (1u << (ADV_TIMER_CG_ENA_BIT + 1u)));
		break;
	case (2):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg | (1u << (ADV_TIMER_CG_ENA_BIT + 2u)));
		break;
	case (3):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg | (1u << (ADV_TIMER_CG_ENA_BIT + 3u)));
		break;
	/*default:*/ /* error: label at end of compound statement */
	}
}

/**
	* disable timer clock
	*
	* arg:
	*          input  timer:     index timer (0-3)
	*
	* return:
	*          (none)
	*/
void adv_timer_disable_clock(size_t timer)
{
	uint32_t cg = adv_timer_cg_get(ARCHI_ADV_TIMER_ADDR);
	switch (timer)
	{
	case (0):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg & (~(((uint32_t) 1u) << (ADV_TIMER_CG_ENA_BIT + 0u))));
		break;
	case (1):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg & (~(((uint32_t) 1u) << (ADV_TIMER_CG_ENA_BIT + 1u))));
		break;
	case (2):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg & (~(((uint32_t) 1u) << (ADV_TIMER_CG_ENA_BIT + 2u))));
		break;
	case (3):
		adv_timer_cg_set(ARCHI_ADV_TIMER_ADDR,
			cg & (~(((uint32_t) 1u) << (ADV_TIMER_CG_ENA_BIT + 3u))));
		break;
	/*default:*/ /* error: label at end of compound statement */
	}
}

/**
	* set timer frequency by setting clock source, prescaler,
	* and counter threshold
	*
	* arg:
	*          input  timer:             index timer (0-3)
	*          input  clock_prescaler:   clock divider to counter
	*          input  counter_threshold: counter threshold when counter restart
	*
	* return:
	*          (none)
	*/
void adv_timer_config_frequency(size_t timer,
	uint8_t clock_prescaler, uint16_t counter_threshold)
{
	uint32_t   tn_config;
	uint32_t   tn_threshold = 0;

	/**
		* set timer clock source
		* for now, supported timer clock source (clock for counting)
		* is only FLL clock
		*/
	/*(void) clock_src;*/

	/* read current configuration */
	tn_config = adv_timer_tn_config_get(ARCHI_ADV_TIMER_ADDR, timer);

	/**
		* set clock source
		* FIXME: for now we only support FLL clock as counter clock
		*/

	/* ignore insel */
	/*ADV_TIMER_TN_CONFIG_INSEL_SET(tn_config, );*/

	/* trigger counting every clock */
	tn_config = ADV_TIMER_TN_CONFIG_MODE_SET(tn_config, 0x00);

	/* use FLL as source clock */
	tn_config = ADV_TIMER_TN_CONFIG_CLKSEL_SET(tn_config, 0x00);

	/* counting in one direction
	* (FIXME: we should support 2 direction counting) */
	tn_config = ADV_TIMER_TN_CONFIG_UPDOWNSEL_SET(tn_config, 0x01);

	/* set prescaler for slower counting */
	tn_config = ADV_TIMER_TN_CONFIG_PRESC_SET(tn_config, clock_prescaler);


	/* set threshold */

	/* counter start from 0 */
	tn_threshold = ADV_TIMER_TN_THRESHOLD_TH_LO_SET(tn_threshold, 0);

	/* counting stop at this value */
	tn_threshold = ADV_TIMER_TN_THRESHOLD_TH_HI_SET(tn_threshold, counter_threshold);


	/* write register */

	/* set config and threshold */
	adv_timer_tn_config_set(ARCHI_ADV_TIMER_ADDR, timer, tn_config);
	adv_timer_tn_threshold_set(ARCHI_ADV_TIMER_ADDR, timer, tn_threshold);

	/* then set update */

	adv_timer_update(timer);
}
