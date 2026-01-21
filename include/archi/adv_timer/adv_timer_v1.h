/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#ifndef __ARCHI_ADV_TIMER_ADV_TIMER_V1_H__
#define __ARCHI_ADV_TIMER_ADV_TIMER_V1_H__

#ifndef LANGUAGE_ASSEMBLY

#include <stdint.h> /* for fixed-size integer types */
#include <stddef.h> /* for size_t types (integer type for array indexing) */

#include "archi/utils.h" /* for bit manipulation operations */

#endif /* ! defined(LANGUAGE_ASSEMBLY) */



/**
	* **************************************************
	* REGISTER OFFSETS
	* **************************************************
	*/

/**
	* offset started at:
	* T0: 0x00
	* T1: 0x40
	* T2: 0x80
	* T3: 0xc0
	*/

/**
	* NOTE: don't directly use following __ prefixed macro in user code
	*       (e.g. in runtime-example), instead use non __ prefixed one!
	*/
#define __ADV_TIMER_TN_REGISTER_START(N)                ( (N) * 0x40u         )
#define __ADV_TIMER_TN_REGISTER_START_AFTER_CHANNEL(N)  (((N) * 0x40u) + 0x2cu)


/* Register address based on timer index (N) and channel index (M) */
#define ADV_TIMER_TN_CMD(N)            \
	(__ADV_TIMER_TN_REGISTER_START(N)               + 0x00u)

#define ADV_TIMER_TN_CONFIG(N)         \
	(__ADV_TIMER_TN_REGISTER_START(N)               + 0x04u)

#define ADV_TIMER_TN_THRESHOLD(N)      \
	(__ADV_TIMER_TN_REGISTER_START(N)               + 0x08u)

#define ADV_TIMER_TN_TH_CHANNELM(N, M) \
	(__ADV_TIMER_TN_REGISTER_START(N)               + 0x0cu + ((M) * 0x04u))

#define ADV_TIMER_TN_COUNTER(N)        \
	(__ADV_TIMER_TN_REGISTER_START_AFTER_CHANNEL(N) + 0x00u)

/* Register address for fixed channel index */
#define ADV_TIMER_TN_TH_CHANNEL0(N)    ADV_TIMER_TN_TH_CHANNELM(N, 0u)
#define ADV_TIMER_TN_TH_CHANNEL1(N)    ADV_TIMER_TN_TH_CHANNELM(N, 1u)
#define ADV_TIMER_TN_TH_CHANNEL2(N)    ADV_TIMER_TN_TH_CHANNELM(N, 2u)
#define ADV_TIMER_TN_TH_CHANNEL3(N)    ADV_TIMER_TN_TH_CHANNELM(N, 3u)



/* TIMER 0 */
#define ADV_TIMER_T0_CMD               ADV_TIMER_TN_CMD(0u)
#define ADV_TIMER_T0_CONFIG            ADV_TIMER_TN_CONFIG(0u)
#define ADV_TIMER_T0_THRESHOLD         ADV_TIMER_TN_THRESHOLD(0u)
#define ADV_TIMER_T0_TH_CHANNEL0       ADV_TIMER_TN_TH_CHANNEL0(0u)
#define ADV_TIMER_T0_TH_CHANNEL1       ADV_TIMER_TN_TH_CHANNEL1(0u)
#define ADV_TIMER_T0_TH_CHANNEL2       ADV_TIMER_TN_TH_CHANNEL2(0u)
#define ADV_TIMER_T0_TH_CHANNEL3       ADV_TIMER_TN_TH_CHANNEL3(0u)
#define ADV_TIMER_T0_COUNTER           ADV_TIMER_TN_COUNTER(0u)

/* TIMER 1 */
#define ADV_TIMER_T1_CMD               ADV_TIMER_TN_CMD(1u)
#define ADV_TIMER_T1_CONFIG            ADV_TIMER_TN_CONFIG(1u)
#define ADV_TIMER_T1_THRESHOLD         ADV_TIMER_TN_THRESHOLD(1u)
#define ADV_TIMER_T1_TH_CHANNEL0       ADV_TIMER_TN_TH_CHANNEL0(1u)
#define ADV_TIMER_T1_TH_CHANNEL1       ADV_TIMER_TN_TH_CHANNEL1(1u)
#define ADV_TIMER_T1_TH_CHANNEL2       ADV_TIMER_TN_TH_CHANNEL2(1u)
#define ADV_TIMER_T1_TH_CHANNEL3       ADV_TIMER_TN_TH_CHANNEL3(1u)
#define ADV_TIMER_T1_COUNTER           ADV_TIMER_TN_COUNTER(1u)

/* TIMER 2 */
#define ADV_TIMER_T2_CMD               ADV_TIMER_TN_CMD(2u)
#define ADV_TIMER_T2_CONFIG            ADV_TIMER_TN_CONFIG(2u)
#define ADV_TIMER_T2_THRESHOLD         ADV_TIMER_TN_THRESHOLD(2u)
#define ADV_TIMER_T2_TH_CHANNEL0       ADV_TIMER_TN_TH_CHANNEL0(2u)
#define ADV_TIMER_T2_TH_CHANNEL1       ADV_TIMER_TN_TH_CHANNEL1(2u)
#define ADV_TIMER_T2_TH_CHANNEL2       ADV_TIMER_TN_TH_CHANNEL2(2u)
#define ADV_TIMER_T2_TH_CHANNEL3       ADV_TIMER_TN_TH_CHANNEL3(2u)
#define ADV_TIMER_T2_COUNTER           ADV_TIMER_TN_COUNTER(2u)

/* TIMER 3 */
#define ADV_TIMER_T3_CMD               ADV_TIMER_TN_CMD(3u)
#define ADV_TIMER_T3_CONFIG            ADV_TIMER_TN_CONFIG(3u)
#define ADV_TIMER_T3_THRESHOLD         ADV_TIMER_TN_THRESHOLD(3u)
#define ADV_TIMER_T3_TH_CHANNEL0       ADV_TIMER_TN_TH_CHANNEL0(3u)
#define ADV_TIMER_T3_TH_CHANNEL1       ADV_TIMER_TN_TH_CHANNEL1(3u)
#define ADV_TIMER_T3_TH_CHANNEL2       ADV_TIMER_TN_TH_CHANNEL2(3u)
#define ADV_TIMER_T3_TH_CHANNEL3       ADV_TIMER_TN_TH_CHANNEL3(3u)
#define ADV_TIMER_T3_COUNTER           ADV_TIMER_TN_COUNTER(3u)

/* EVENT CFG */
#define ADV_TIMER_EVENT_CFG            0x100u

/* CLOCK GATING */
#define ADV_TIMER_CG                   0x104u



/**
	* **************************************************
	* REGISTERS FIELDS
	* **************************************************
	*/

/**
	* TIMER(N) CMD
	*/
#define ADV_TIMER_TN_CMD_START_BIT            0u
#define ADV_TIMER_TN_CMD_START_WIDTH          1u
#define ADV_TIMER_TN_CMD_START_MASK           (0x1u << (ADV_TIMER_TN_CMD_START_BIT))

#define ADV_TIMER_TN_CMD_STOP_BIT             1u
#define ADV_TIMER_TN_CMD_STOP_WIDTH           1u
#define ADV_TIMER_TN_CMD_STOP_MASK            (0x1u << (ADV_TIMER_TN_CMD_STOP_BIT))

#define ADV_TIMER_TN_CMD_UPDATE_BIT           2u
#define ADV_TIMER_TN_CMD_UPDATE_WIDTH         1u
#define ADV_TIMER_TN_CMD_UPDATE_MASK          (0x1u << (ADV_TIMER_TN_CMD_UPDATE_BIT))

#define ADV_TIMER_TN_CMD_RESET_BIT            3u
#define ADV_TIMER_TN_CMD_RESET_WIDTH          1u
#define ADV_TIMER_TN_CMD_RESET_MASK           (0x1u << (ADV_TIMER_TN_CMD_RESET_BIT))

#define ADV_TIMER_TN_CMD_ARM_BIT              4u
#define ADV_TIMER_TN_CMD_ARM_WIDTH            1u
#define ADV_TIMER_TN_CMD_ARM_MASK             (0x1u << (ADV_TIMER_TN_CMD_ARM_BIT))

/**
	* TIMER(N) CONFIG
	*/
#define ADV_TIMER_TN_CONFIG_INSEL_BIT         0u
#define ADV_TIMER_TN_CONFIG_INSEL_WIDTH       8u
#define ADV_TIMER_TN_CONFIG_INSEL_MASK        (0xffu << (ADV_TIMER_TN_CONFIG_INSEL_BIT))

#define ADV_TIMER_TN_CONFIG_MODE_BIT          8u
#define ADV_TIMER_TN_CONFIG_MODE_WIDTH        3u
#define ADV_TIMER_TN_CONFIG_MODE_MASK         (0x7u << (ADV_TIMER_TN_CONFIG_MODE_BIT))

#define ADV_TIMER_TN_CONFIG_CLKSEL_BIT        11u
#define ADV_TIMER_TN_CONFIG_CLKSEL_WIDTH      1u
#define ADV_TIMER_TN_CONFIG_CLKSEL_MASK       (0x1u << (ADV_TIMER_TN_CONFIG_CLKSEL_BIT))

#define ADV_TIMER_TN_CONFIG_UPDOWNSEL_BIT     12u
#define ADV_TIMER_TN_CONFIG_UPDOWNSEL_WIDTH   1u
#define ADV_TIMER_TN_CONFIG_UPDOWNSEL_MASK    (0x1u << (ADV_TIMER_TN_CONFIG_UPDOWNSEL_BIT))

#define ADV_TIMER_TN_CONFIG_PRESC_BIT         16u
#define ADV_TIMER_TN_CONFIG_PRESC_WIDTH       8u
#define ADV_TIMER_TN_CONFIG_PRESC_MASK        (0xffu << (ADV_TIMER_TN_CONFIG_PRESC_BIT))

/**
	* TIMER(N) THRESHOLD
	*/
#define ADV_TIMER_TN_THRESHOLD_TH_LO_BIT      0u
#define ADV_TIMER_TN_THRESHOLD_TH_LO_WIDTH    16u
#define ADV_TIMER_TN_THRESHOLD_TH_LO_MASK     (0xffffu << (ADV_TIMER_TN_THRESHOLD_TH_LO_BIT))

#define ADV_TIMER_TN_THRESHOLD_TH_HI_BIT      16u
#define ADV_TIMER_TN_THRESHOLD_TH_HI_WIDTH    16u
#define ADV_TIMER_TN_THRESHOLD_TH_HI_MASK     (0xffffu << (ADV_TIMER_TN_THRESHOLD_TH_HI_BIT))

/**
	* TIMER(N) CHANNEL(M)
	*/
#define ADV_TIMER_TN_TH_CHANNELM_TH_BIT       0u
#define ADV_TIMER_TN_TH_CHANNELM_TH_WIDTH     16u
#define ADV_TIMER_TN_TH_CHANNELM_TH_MASK      (0xffffu << (ADV_TIMER_TN_TH_CHANNELM_TH_BIT))

#define ADV_TIMER_TN_TH_CHANNELM_MODE_BIT     16u
#define ADV_TIMER_TN_TH_CHANNELM_MODE_WIDTH   3u
#define ADV_TIMER_TN_TH_CHANNELM_MODE_MASK    (0x7u << (ADV_TIMER_TN_TH_CHANNELM_MODE_BIT))

/**
	* TIMER(N) COUNTER
	*/
#define ADV_TIMER_TN_COUNTER_COUNTER_BIT      0u
#define ADV_TIMER_TN_COUNTER_COUNTER_WIDTH    16u
#define ADV_TIMER_TN_COUNTER_COUNTER_MASK     (0xffffu << (ADV_TIMER_TN_COUNTER_COUNTER_BIT))

/**
	* EVENT CFG
	*/
#define ADV_TIMER_EVENT_CFG_SEL0_BIT          0u
#define ADV_TIMER_EVENT_CFG_SEL0_WIDTH        4u
#define ADV_TIMER_EVENT_CFG_SEL0_MASK         (0xfu << (ADV_TIMER_EVENT_CFG_SEL0_BIT))

#define ADV_TIMER_EVENT_CFG_SEL1_BIT          4u
#define ADV_TIMER_EVENT_CFG_SEL1_WIDTH        4u
#define ADV_TIMER_EVENT_CFG_SEL1_MASK         (0xfu << (ADV_TIMER_EVENT_CFG_SEL1_BIT))

#define ADV_TIMER_EVENT_CFG_SEL2_BIT          8u
#define ADV_TIMER_EVENT_CFG_SEL2_WIDTH        4u
#define ADV_TIMER_EVENT_CFG_SEL2_MASK         (0xfu << (ADV_TIMER_EVENT_CFG_SEL2_BIT))

#define ADV_TIMER_EVENT_CFG_SEL3_BIT          12u
#define ADV_TIMER_EVENT_CFG_SEL3_WIDTH        4u
#define ADV_TIMER_EVENT_CFG_SEL3_MASK         (0xfu << (ADV_TIMER_EVENT_CFG_SEL3_BIT))

#define ADV_TIMER_EVENT_CFG_ENA_BIT           16u
#define ADV_TIMER_EVENT_CFG_ENA_WIDTH         4u
#define ADV_TIMER_EVENT_CFG_ENA_MASK          (0xfu << (ADV_TIMER_EVENT_CFG_ENA_BIT))

/**
	* CLOCK GATING
	*/
#define ADV_TIMER_CG_ENA_BIT                  0u
#define ADV_TIMER_CG_ENA_WIDTH                4u
#define ADV_TIMER_CG_ENA_MASK                 (0xfu << (ADV_TIMER_CG_ENA_BIT))



/**
	* **************************************************
	* REGISTERS STRUCTS
	* **************************************************
	*/

#ifndef LANGUAGE_ASSEMBLY

/* register cmd (offset: 0x0000, 0x0040, 0x0080, 0x00c0) */
union adv_timer_tn_cmd_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    start        :  1;   /* [      0] */
		uint32_t    stop         :  1;   /* [      1] */
		uint32_t    update       :  1;   /* [      2] */
		uint32_t    reset        :  1;   /* [      3] */
		uint32_t    arm          :  1;   /* [      4] */
		uint32_t  __reserved_l_0 : 11;   /* [15 -  5] */

		/* 16-bit high */
		uint32_t  __reserved_h_0 : 16;   /* [31 - 16] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));

/* register config (offset: 0x0004, 0x0044, 0x0084, 0x00c4) */
union adv_timer_tn_config_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    insel        :  8;   /* [ 7 -  0] */
		uint32_t    mode         :  3;   /* [10 -  8] */
		uint32_t    clksel       :  1;   /* [     11] */
		uint32_t    updownsel    :  1;   /* [     12] */
		uint32_t  __reserved_l_0 :  3;   /* [15 - 13] */

		/* 16-bit high */
		uint32_t    presc        :  8;   /* [23 - 16] */
		uint32_t  __reserved_h_0 :  8;   /* [31 - 24] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));

/* register threshold (offset: 0x0008, 0x0048, 0x0088, 0x00c8) */
union adv_timer_tn_threshold_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    th_lo        : 16;   /* [15 -  0] */

		/* 16-bit high */
		uint32_t    th_hi        : 16;   /* [31 - 16] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));

/**
	* register th channel
	* T0: CHANNEL0: 0x000c, CHANNEL1: 0x0010, CHANNEL2: 0x0014, CHANNEL3: 0x0018
	* T1: CHANNEL0: 0x004c, CHANNEL1: 0x0050, CHANNEL2: 0x0054, CHANNEL3: 0x0058
	* T2: CHANNEL0: 0x008c, CHANNEL1: 0x0090, CHANNEL2: 0x0094, CHANNEL3: 0x0098
	* T3: CHANNEL0: 0x00cc, CHANNEL1: 0x00d0, CHANNEL2: 0x00d4, CHANNEL3: 0x00d8
	*/
union adv_timer_tn_th_channelm_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    th           : 16;   /* [15 -  0] */

		/* 16-bit high */
		uint32_t    mode         :  3;   /* [18 - 16] */
		uint32_t  __reserved_h_0 : 13;   /* [31 - 19] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));

/* register counter (offset: 0x002c, 0x006c, 0x00ac, 0x00ec) */
union adv_timer_tn_counter_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    counter      : 16;   /* [15 -  0] */

		/* 16-bit high */
		uint32_t  __reserved_h_0 : 16;   /* [31 - 16] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));


/* register event cfg (offset: 0x0100) */
union adv_timer_event_cfg_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    sel0         :  4;   /* [ 3 -  0] */
		uint32_t    sel1         :  4;   /* [ 7 -  4] */
		uint32_t    sel2         :  4;   /* [11 -  8] */
		uint32_t    sel3         :  4;   /* [15 - 12] */

		/* 16-bit high */
		uint32_t    ena          :  4;   /* [19 - 16] */
		uint32_t  __reserved_h_0 : 12;   /* [31 - 20] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));


/* register clk en (offset: 0x0104) */
union adv_timer_cg_u
{
	/* for fine grained bit access */
	struct /* unnamed struct */
	{
		/* in little endian system, we list struct member from lowest address */

		/* 16-bit low */
		uint32_t    ena          :  4;   /* [ 3 -  0] */
		uint32_t  __reserved_l_0 : 12;   /* [15 -  4] */

		/* 16-bit high */
		uint32_t  __reserved_h_0 : 16;   /* [31 - 16] */
	};

	/* for access whole register value */
	uint32_t        raw;
}
__attribute__ ((packed));

#endif /* ! defined(LANGUAGE_ASSEMBLY) */



/**
	* **************************************************
	* REGISTERS STRUCTS
	* **************************************************
	*/

/* gvsoc class is not yet available */
/*#ifdef __GVSOC__
#endif*/ /* defined(__GVSOC__) */



/**
	* **************************************************
	* REGISTERS GLOBAL STRUCT
	* **************************************************
	*/

#ifndef LANGUAGE_ASSEMBLY

/* register layout of apb_adv_timer */
typedef struct
{
	/**
		* timer t(0) to t(3)
		* space:
		*    0x00 - 0x3c
		*    0x40 - 0x7c
		*    0x80 - 0xbc
		*    0xc0 - 0xfc
		*/
	struct
	{
		union adv_timer_tn_cmd_u           cmd;            /*          0x0000 */
		union adv_timer_tn_config_u        config;         /*          0x0004 */
		union adv_timer_tn_threshold_u     threshold;      /*          0x0008 */
		union adv_timer_tn_th_channelm_u   th_channel[4];  /* 0x0018 - 0x000c */
		uint32_t                         __reserved_0[4];  /* 0x0028 - 0x001c */
		union adv_timer_tn_counter_u       counter;        /*          0x002c */
		uint32_t                         __reserved_1[4];  /* 0x003c - 0x0030 */
	}
	t[4];

	/* event cfg */
	union  adv_timer_event_cfg_u           event_cfg;      /*          0x0100 */

	/* cg */
	union  adv_timer_cg_u                  cg;             /*          0x0104 */
}
__attribute__ ((packed)) adv_timer_adv_timer_t;

#endif /* ! defined(LANGUAGE_ASSEMBLY) */



/**
	* **************************************************
	* REGISTERS ACCESS FUNCTIONS
	* **************************************************
	*/

#ifndef LANGUAGE_ASSEMBLY

/**
	* for addressing based on timer id and channel id
	*/

static inline uint32_t adv_timer_tn_cmd_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_CMD(timer_n));
}

static inline void adv_timer_tn_cmd_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_CMD(timer_n), (int) value);
}

static inline uint32_t adv_timer_tn_config_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_CONFIG(timer_n));
}

static inline void adv_timer_tn_config_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_CONFIG(timer_n), (int) value);
}

static inline uint32_t adv_timer_tn_threshold_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_THRESHOLD(timer_n));
}

static inline void adv_timer_tn_threshold_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_THRESHOLD(timer_n), (int) value);
}

static inline uint32_t adv_timer_tn_th_channelm_get(uintptr_t base, size_t timer_n, size_t channel_m)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_TH_CHANNELM(timer_n, channel_m));
}

static inline void adv_timer_tn_th_channelm_set(uintptr_t base, size_t timer_n, size_t channel_m, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_TH_CHANNELM(timer_n, channel_m), (int) value);
}

static inline uint32_t adv_timer_tn_counter_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_COUNTER(timer_n));
}

static inline void adv_timer_tn_counter_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_COUNTER(timer_n), (int) value);
}



/**
	* for addressing based on timer id, but fixed channel id
	*/

static inline uint32_t adv_timer_tn_th_channel0_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_TH_CHANNEL0(timer_n));
}

static inline void adv_timer_tn_th_channel0_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_TH_CHANNEL0(timer_n), (int) value);
}

static inline uint32_t adv_timer_tn_th_channel1_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_TH_CHANNEL1(timer_n));
}

static inline void adv_timer_tn_th_channel1_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_TH_CHANNEL1(timer_n), (int) value);
}

static inline uint32_t adv_timer_tn_th_channel2_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_TH_CHANNEL2(timer_n));
}

static inline void adv_timer_tn_th_channel2_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_TH_CHANNEL2(timer_n), (int) value);
}

static inline uint32_t adv_timer_tn_th_channel3_get(uintptr_t base, size_t timer_n)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_TN_TH_CHANNEL3(timer_n));
}

static inline void adv_timer_tn_th_channel3_set(uintptr_t base, size_t timer_n, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_TN_TH_CHANNEL3(timer_n), (int) value);
}



/**
	* for addressing on fixed timer id and fixed channel id
	*/

/**
	* TIMER T0
	*/
static inline uint32_t adv_timer_t0_cmd_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_CMD);
}

static inline void adv_timer_t0_cmd_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_CMD, (int) value);
}

static inline uint32_t adv_timer_t0_config_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_CONFIG);
}

static inline void adv_timer_t0_config_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_CONFIG, (int) value);
}

static inline uint32_t adv_timer_t0_threshold_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_THRESHOLD);
}

static inline void adv_timer_t0_threshold_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_THRESHOLD, (int) value);
}

static inline uint32_t adv_timer_t0_th_channel0_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_TH_CHANNEL0);
}

static inline void adv_timer_t0_th_channel0_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_TH_CHANNEL0, (int) value);
}

static inline uint32_t adv_timer_t0_th_channel1_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_TH_CHANNEL1);
}

static inline void adv_timer_t0_th_channel1_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_TH_CHANNEL1, (int) value);
}

static inline uint32_t adv_timer_t0_th_channel2_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_TH_CHANNEL2);
}

static inline void adv_timer_t0_th_channel2_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_TH_CHANNEL2, (int) value);
}

static inline uint32_t adv_timer_t0_th_channel3_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_TH_CHANNEL3);
}

static inline void adv_timer_t0_th_channel3_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_TH_CHANNEL3, (int) value);
}

static inline uint32_t adv_timer_t0_counter_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T0_COUNTER);
}

static inline void adv_timer_t0_counter_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T0_COUNTER, (int) value);
}

/**
	* TIMER T1
	*/
static inline uint32_t adv_timer_t1_cmd_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_CMD);
}

static inline void adv_timer_t1_cmd_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_CMD, (int) value);
}

static inline uint32_t adv_timer_t1_config_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_CONFIG);
}

static inline void adv_timer_t1_config_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_CONFIG, (int) value);
}

static inline uint32_t adv_timer_t1_threshold_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_THRESHOLD);
}

static inline void adv_timer_t1_threshold_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_THRESHOLD, (int) value);
}

static inline uint32_t adv_timer_t1_th_channel0_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_TH_CHANNEL0);
}

static inline void adv_timer_t1_th_channel0_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_TH_CHANNEL0, (int) value);
}

static inline uint32_t adv_timer_t1_th_channel1_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_TH_CHANNEL1);
}

static inline void adv_timer_t1_th_channel1_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_TH_CHANNEL1, (int) value);
}

static inline uint32_t adv_timer_t1_th_channel2_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_TH_CHANNEL2);
}

static inline void adv_timer_t1_th_channel2_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_TH_CHANNEL2, (int) value);
}

static inline uint32_t adv_timer_t1_th_channel3_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_TH_CHANNEL3);
}

static inline void adv_timer_t1_th_channel3_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_TH_CHANNEL3, (int) value);
}

static inline uint32_t adv_timer_t1_counter_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T1_COUNTER);
}

static inline void adv_timer_t1_counter_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T1_COUNTER, (int) value);
}

/**
	* TIMER T2
	*/
static inline uint32_t adv_timer_t2_cmd_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_CMD);
}

static inline void adv_timer_t2_cmd_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_CMD, (int) value);
}

static inline uint32_t adv_timer_t2_config_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_CONFIG);
}

static inline void adv_timer_t2_config_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_CONFIG, (int) value);
}

static inline uint32_t adv_timer_t2_threshold_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_THRESHOLD);
}

static inline void adv_timer_t2_threshold_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_THRESHOLD, (int) value);
}

static inline uint32_t adv_timer_t2_th_channel0_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_TH_CHANNEL0);
}

static inline void adv_timer_t2_th_channel0_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_TH_CHANNEL0, (int) value);
}

static inline uint32_t adv_timer_t2_th_channel1_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_TH_CHANNEL1);
}

static inline void adv_timer_t2_th_channel1_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_TH_CHANNEL1, (int) value);
}

static inline uint32_t adv_timer_t2_th_channel2_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_TH_CHANNEL2);
}

static inline void adv_timer_t2_th_channel2_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_TH_CHANNEL2, (int) value);
}

static inline uint32_t adv_timer_t2_th_channel3_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_TH_CHANNEL3);
}

static inline void adv_timer_t2_th_channel3_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_TH_CHANNEL3, (int) value);
}

static inline uint32_t adv_timer_t2_counter_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T2_COUNTER);
}

static inline void adv_timer_t2_counter_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T2_COUNTER, (int) value);
}

/**
	* TIMER T3
	*/
static inline uint32_t adv_timer_t3_cmd_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_CMD);
}

static inline void adv_timer_t3_cmd_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_CMD, (int) value);
}

static inline uint32_t adv_timer_t3_config_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_CONFIG);
}

static inline void adv_timer_t3_config_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_CONFIG, (int) value);
}

static inline uint32_t adv_timer_t3_threshold_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_THRESHOLD);
}

static inline void adv_timer_t3_threshold_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_THRESHOLD, (int) value);
}

static inline uint32_t adv_timer_t3_th_channel0_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_TH_CHANNEL0);
}

static inline void adv_timer_t3_th_channel0_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_TH_CHANNEL0, (int) value);
}

static inline uint32_t adv_timer_t3_th_channel1_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_TH_CHANNEL1);
}

static inline void adv_timer_t3_th_channel1_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_TH_CHANNEL1, (int) value);
}

static inline uint32_t adv_timer_t3_th_channel2_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_TH_CHANNEL2);
}

static inline void adv_timer_t3_th_channel2_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_TH_CHANNEL2, (int) value);
}

static inline uint32_t adv_timer_t3_th_channel3_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_TH_CHANNEL3);
}

static inline void adv_timer_t3_th_channel3_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_TH_CHANNEL3, (int) value);
}

static inline uint32_t adv_timer_t3_counter_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_T3_COUNTER);
}

static inline void adv_timer_t3_counter_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_T3_COUNTER, (int) value);
}



/**
	* EVENT CFG
	*/
static inline uint32_t adv_timer_event_cfg_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_EVENT_CFG);
}

static inline void adv_timer_event_cfg_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_EVENT_CFG, (int) value);
}



/**
	* CG
	*/
static inline uint32_t adv_timer_cg_get(uintptr_t base)
{
	return (uint32_t) ARCHI_READ(base, (int) ADV_TIMER_CG);
}

static inline void adv_timer_cg_set(uintptr_t base, uint32_t value)
{
	ARCHI_WRITE(base, (int) ADV_TIMER_CG, (int) value);
}

#endif /* ! defined(LANGUAGE_ASSEMBLY) */



/**
	* **************************************************
	* REGISTERS FIELDS MACROS
	* **************************************************
	*/

#ifndef LANGUAGE_ASSEMBLY

/**
	* TIMER(N) CMD
	*/
#define ADV_TIMER_TN_CMD_START_GET(value)               (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CMD_START_WIDTH, ADV_TIMER_TN_CMD_START_BIT))
#define ADV_TIMER_TN_CMD_START_GETS(value)              (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CMD_START_WIDTH, ADV_TIMER_TN_CMD_START_BIT))
#define ADV_TIMER_TN_CMD_START_SET(value, field)        (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CMD_START_WIDTH, ADV_TIMER_TN_CMD_START_BIT))
#define ADV_TIMER_TN_CMD_START(value)                   ((value) << (ADV_TIMER_TN_CMD_START_BIT))

#define ADV_TIMER_TN_CMD_STOP_GET(value)                (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CMD_STOP_WIDTH, ADV_TIMER_TN_CMD_STOP_BIT))
#define ADV_TIMER_TN_CMD_STOP_GETS(value)               (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CMD_STOP_WIDTH, ADV_TIMER_TN_CMD_STOP_BIT))
#define ADV_TIMER_TN_CMD_STOP_SET(value, field)         (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CMD_STOP_WIDTH, ADV_TIMER_TN_CMD_STOP_BIT))
#define ADV_TIMER_TN_CMD_STOP(value)                    ((value) << (ADV_TIMER_TN_CMD_STOP_BIT))

#define ADV_TIMER_TN_CMD_UPDATE_GET(value)              (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CMD_UPDATE_WIDTH, ADV_TIMER_TN_CMD_UPDATE_BIT))
#define ADV_TIMER_TN_CMD_UPDATE_GETS(value)             (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CMD_UPDATE_WIDTH, ADV_TIMER_TN_CMD_UPDATE_BIT))
#define ADV_TIMER_TN_CMD_UPDATE_SET(value, field)       (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CMD_UPDATE_WIDTH, ADV_TIMER_TN_CMD_UPDATE_BIT))
#define ADV_TIMER_TN_CMD_UPDATE(value)                  ((value) << (ADV_TIMER_TN_CMD_UPDATE_BIT))

#define ADV_TIMER_TN_CMD_RESET_GET(value)               (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CMD_RESET_WIDTH, ADV_TIMER_TN_CMD_RESET_BIT))
#define ADV_TIMER_TN_CMD_RESET_GETS(value)              (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CMD_RESET_WIDTH, ADV_TIMER_TN_CMD_RESET_BIT))
#define ADV_TIMER_TN_CMD_RESET_SET(value, field)        (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CMD_RESET_WIDTH, ADV_TIMER_TN_CMD_RESET_BIT))
#define ADV_TIMER_TN_CMD_RESET(value)                   ((value) << (ADV_TIMER_TN_CMD_RESET_BIT))

#define ADV_TIMER_TN_CMD_ARM_GET(value)                 (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CMD_ARM_WIDTH, ADV_TIMER_TN_CMD_ARM_BIT))
#define ADV_TIMER_TN_CMD_ARM_GETS(value)                (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CMD_ARM_WIDTH, ADV_TIMER_TN_CMD_ARM_BIT))
#define ADV_TIMER_TN_CMD_ARM_SET(value, field)          (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CMD_ARM_WIDTH, ADV_TIMER_TN_CMD_ARM_BIT))
#define ADV_TIMER_TN_CMD_ARM(value)                     ((value) << (ADV_TIMER_TN_CMD_ARM_BIT))


/**
	* TIMER(N) CONFIG
	*/
#define ADV_TIMER_TN_CONFIG_INSEL_GET(value)            (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CONFIG_INSEL_WIDTH, ADV_TIMER_TN_CONFIG_INSEL_BIT))
#define ADV_TIMER_TN_CONFIG_INSEL_GETS(value)           (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CONFIG_INSEL_WIDTH, ADV_TIMER_TN_CONFIG_INSEL_BIT))
#define ADV_TIMER_TN_CONFIG_INSEL_SET(value, field)     (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CONFIG_INSEL_WIDTH, ADV_TIMER_TN_CONFIG_INSEL_BIT))
#define ADV_TIMER_TN_CONFIG_INSEL(value)                ((value) << (ADV_TIMER_TN_CONFIG_INSEL_BIT))

#define ADV_TIMER_TN_CONFIG_MODE_GET(value)             (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CONFIG_MODE_WIDTH, ADV_TIMER_TN_CONFIG_MODE_BIT))
#define ADV_TIMER_TN_CONFIG_MODE_GETS(value)            (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CONFIG_MODE_WIDTH, ADV_TIMER_TN_CONFIG_MODE_BIT))
#define ADV_TIMER_TN_CONFIG_MODE_SET(value, field)      (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CONFIG_MODE_WIDTH, ADV_TIMER_TN_CONFIG_MODE_BIT))
#define ADV_TIMER_TN_CONFIG_MODE(value)                 ((value) << (ADV_TIMER_TN_CONFIG_MODE_BIT))

#define ADV_TIMER_TN_CONFIG_CLKSEL_GET(value)           (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CONFIG_CLKSEL_WIDTH, ADV_TIMER_TN_CONFIG_CLKSEL_BIT))
#define ADV_TIMER_TN_CONFIG_CLKSEL_GETS(value)          (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CONFIG_CLKSEL_WIDTH, ADV_TIMER_TN_CONFIG_CLKSEL_BIT))
#define ADV_TIMER_TN_CONFIG_CLKSEL_SET(value, field)    (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CONFIG_CLKSEL_WIDTH, ADV_TIMER_TN_CONFIG_CLKSEL_BIT))
#define ADV_TIMER_TN_CONFIG_CLKSEL(value)               ((value) << (ADV_TIMER_TN_CONFIG_CLKSEL_BIT))

#define ADV_TIMER_TN_CONFIG_UPDOWNSEL_GET(value)        (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CONFIG_UPDOWNSEL_WIDTH, ADV_TIMER_TN_CONFIG_UPDOWNSEL_BIT))
#define ADV_TIMER_TN_CONFIG_UPDOWNSEL_GETS(value)       (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CONFIG_UPDOWNSEL_WIDTH, ADV_TIMER_TN_CONFIG_UPDOWNSEL_BIT))
#define ADV_TIMER_TN_CONFIG_UPDOWNSEL_SET(value, field) (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CONFIG_UPDOWNSEL_WIDTH, ADV_TIMER_TN_CONFIG_UPDOWNSEL_BIT))
#define ADV_TIMER_TN_CONFIG_UPDOWNSEL(value)            ((value) << (ADV_TIMER_TN_CONFIG_UPDOWNSEL_BIT))

#define ADV_TIMER_TN_CONFIG_PRESC_GET(value)            (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_CONFIG_PRESC_WIDTH, ADV_TIMER_TN_CONFIG_PRESC_BIT))
#define ADV_TIMER_TN_CONFIG_PRESC_GETS(value)           (ARCHI_BEXTRACT(value, ADV_TIMER_TN_CONFIG_PRESC_WIDTH, ADV_TIMER_TN_CONFIG_PRESC_BIT))
#define ADV_TIMER_TN_CONFIG_PRESC_SET(value, field)     (ARCHI_BINSERT(value, field, ADV_TIMER_TN_CONFIG_PRESC_WIDTH, ADV_TIMER_TN_CONFIG_PRESC_BIT))
#define ADV_TIMER_TN_CONFIG_PRESC(value)                ((value) << (ADV_TIMER_TN_CONFIG_PRESC_BIT))

/**
	* TIMER(N) THRESHOLD
	*/
#define ADV_TIMER_TN_THRESHOLD_TH_LO_GET(value)         (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_THRESHOLD_TH_LO_WIDTH, ADV_TIMER_TN_THRESHOLD_TH_LO_BIT))
#define ADV_TIMER_TN_THRESHOLD_TH_LO_GETS(value)        (ARCHI_BEXTRACT(value, ADV_TIMER_TN_THRESHOLD_TH_LO_WIDTH, ADV_TIMER_TN_THRESHOLD_TH_LO_BIT))
#define ADV_TIMER_TN_THRESHOLD_TH_LO_SET(value, field)  (ARCHI_BINSERT(value, field, ADV_TIMER_TN_THRESHOLD_TH_LO_WIDTH, ADV_TIMER_TN_THRESHOLD_TH_LO_BIT))
#define ADV_TIMER_TN_THRESHOLD_TH_LO(value)             ((value) << (ADV_TIMER_TN_THRESHOLD_TH_LO_BIT))

#define ADV_TIMER_TN_THRESHOLD_TH_HI_GET(value)         (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_THRESHOLD_TH_HI_WIDTH, ADV_TIMER_TN_THRESHOLD_TH_HI_BIT))
#define ADV_TIMER_TN_THRESHOLD_TH_HI_GETS(value)        (ARCHI_BEXTRACT(value, ADV_TIMER_TN_THRESHOLD_TH_HI_WIDTH, ADV_TIMER_TN_THRESHOLD_TH_HI_BIT))
#define ADV_TIMER_TN_THRESHOLD_TH_HI_SET(value, field)  (ARCHI_BINSERT(value, field, ADV_TIMER_TN_THRESHOLD_TH_HI_WIDTH, ADV_TIMER_TN_THRESHOLD_TH_HI_BIT))
#define ADV_TIMER_TN_THRESHOLD_TH_HI(value)             ((value) << (ADV_TIMER_TN_THRESHOLD_TH_HI_BIT))

/**
	* TIMER(N) CHANNEL(M)
	*/
#define ADV_TIMER_TN_TH_CHANNELM_TH_GET(value)          (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_TH_CHANNELM_TH_WIDTH, ADV_TIMER_TN_TH_CHANNELM_TH_BIT))
#define ADV_TIMER_TN_TH_CHANNELM_TH_GETS(value)         (ARCHI_BEXTRACT(value, ADV_TIMER_TN_TH_CHANNELM_TH_WIDTH, ADV_TIMER_TN_TH_CHANNELM_TH_BIT))
#define ADV_TIMER_TN_TH_CHANNELM_TH_SET(value, field)   (ARCHI_BINSERT(value, field, ADV_TIMER_TN_TH_CHANNELM_TH_WIDTH, ADV_TIMER_TN_TH_CHANNELM_TH_BIT))
#define ADV_TIMER_TN_TH_CHANNELM_TH(value)              ((value) << (ADV_TIMER_TN_TH_CHANNELM_TH_BIT))

#define ADV_TIMER_TN_TH_CHANNELM_MODE_GET(value)        (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_TH_CHANNELM_MODE_WIDTH, ADV_TIMER_TN_TH_CHANNELM_MODE_BIT))
#define ADV_TIMER_TN_TH_CHANNELM_MODE_GETS(value)       (ARCHI_BEXTRACT(value, ADV_TIMER_TN_TH_CHANNELM_MODE_WIDTH, ADV_TIMER_TN_TH_CHANNELM_MODE_BIT))
#define ADV_TIMER_TN_TH_CHANNELM_MODE_SET(value, field) (ARCHI_BINSERT(value, field, ADV_TIMER_TN_TH_CHANNELM_MODE_WIDTH, ADV_TIMER_TN_TH_CHANNELM_MODE_BIT))
#define ADV_TIMER_TN_TH_CHANNELM_MODE(value)            ((value) << (ADV_TIMER_TN_TH_CHANNELM_MODE_BIT))

/**
	* TIMER(N) COUNTER
	*/
#define ADV_TIMER_TN_COUNTER_COUNTER_GET(value)         (ARCHI_BEXTRACTU(value, ADV_TIMER_TN_COUNTER_COUNTER_WIDTH, ADV_TIMER_TN_COUNTER_COUNTER_BIT))
#define ADV_TIMER_TN_COUNTER_COUNTER_GETS(value)        (ARCHI_BEXTRACT(value, ADV_TIMER_TN_COUNTER_COUNTER_WIDTH, ADV_TIMER_TN_COUNTER_COUNTER_BIT))
#define ADV_TIMER_TN_COUNTER_COUNTER_SET(value, field)  (ARCHI_BINSERT(value, field, ADV_TIMER_TN_COUNTER_COUNTER_WIDTH, ADV_TIMER_TN_COUNTER_COUNTER_BIT))
#define ADV_TIMER_TN_COUNTER_COUNTER(value)             ((value) << (ADV_TIMER_TN_COUNTER_COUNTER_BIT))

/**
	* EVENT CFG
	*/
#define ADV_TIMER_EVENT_CFG_SEL0_GET(value)             (ARCHI_BEXTRACTU(value, ADV_TIMER_EVENT_CFG_SEL0_WIDTH, ADV_TIMER_EVENT_CFG_SEL0_BIT))
#define ADV_TIMER_EVENT_CFG_SEL0_GETS(value)            (ARCHI_BEXTRACT(value, ADV_TIMER_EVENT_CFG_SEL0_WIDTH, ADV_TIMER_EVENT_CFG_SEL0_BIT))
#define ADV_TIMER_EVENT_CFG_SEL0_SET(value, field)      (ARCHI_BINSERT(value, field, ADV_TIMER_EVENT_CFG_SEL0_WIDTH, ADV_TIMER_EVENT_CFG_SEL0_BIT))
#define ADV_TIMER_EVENT_CFG_SEL0(value)                 ((value) << (ADV_TIMER_EVENT_CFG_SEL0_BIT))

#define ADV_TIMER_EVENT_CFG_SEL1_GET(value)             (ARCHI_BEXTRACTU(value, ADV_TIMER_EVENT_CFG_SEL1_WIDTH, ADV_TIMER_EVENT_CFG_SEL1_BIT))
#define ADV_TIMER_EVENT_CFG_SEL1_GETS(value)            (ARCHI_BEXTRACT(value, ADV_TIMER_EVENT_CFG_SEL1_WIDTH, ADV_TIMER_EVENT_CFG_SEL1_BIT))
#define ADV_TIMER_EVENT_CFG_SEL1_SET(value, field)      (ARCHI_BINSERT(value, field, ADV_TIMER_EVENT_CFG_SEL1_WIDTH, ADV_TIMER_EVENT_CFG_SEL1_BIT))
#define ADV_TIMER_EVENT_CFG_SEL1(value)                 ((value) << (ADV_TIMER_EVENT_CFG_SEL1_BIT))

#define ADV_TIMER_EVENT_CFG_SEL2_GET(value)             (ARCHI_BEXTRACTU(value, ADV_TIMER_EVENT_CFG_SEL2_WIDTH, ADV_TIMER_EVENT_CFG_SEL2_BIT))
#define ADV_TIMER_EVENT_CFG_SEL2_GETS(value)            (ARCHI_BEXTRACT(value, ADV_TIMER_EVENT_CFG_SEL2_WIDTH, ADV_TIMER_EVENT_CFG_SEL2_BIT))
#define ADV_TIMER_EVENT_CFG_SEL2_SET(value, field)      (ARCHI_BINSERT(value, field, ADV_TIMER_EVENT_CFG_SEL2_WIDTH, ADV_TIMER_EVENT_CFG_SEL2_BIT))
#define ADV_TIMER_EVENT_CFG_SEL2(value)                 ((value) << (ADV_TIMER_EVENT_CFG_SEL2_BIT))

#define ADV_TIMER_EVENT_CFG_SEL3_GET(value)             (ARCHI_BEXTRACTU(value, ADV_TIMER_EVENT_CFG_SEL3_WIDTH, ADV_TIMER_EVENT_CFG_SEL3_BIT))
#define ADV_TIMER_EVENT_CFG_SEL3_GETS(value)            (ARCHI_BEXTRACT(value, ADV_TIMER_EVENT_CFG_SEL3_WIDTH, ADV_TIMER_EVENT_CFG_SEL3_BIT))
#define ADV_TIMER_EVENT_CFG_SEL3_SET(value, field)      (ARCHI_BINSERT(value, field, ADV_TIMER_EVENT_CFG_SEL3_WIDTH, ADV_TIMER_EVENT_CFG_SEL3_BIT))
#define ADV_TIMER_EVENT_CFG_SEL3(value)                 ((value) << (ADV_TIMER_EVENT_CFG_SEL3_BIT))

#define ADV_TIMER_EVENT_CFG_ENA_GET(value)              (ARCHI_BEXTRACTU(value, ADV_TIMER_EVENT_CFG_ENA_WIDTH, ADV_TIMER_EVENT_CFG_ENA_BIT))
#define ADV_TIMER_EVENT_CFG_ENA_GETS(value)             (ARCHI_BEXTRACT(value, ADV_TIMER_EVENT_CFG_ENA_WIDTH, ADV_TIMER_EVENT_CFG_ENA_BIT))
#define ADV_TIMER_EVENT_CFG_ENA_SET(value, field)       (ARCHI_BINSERT(value, field, ADV_TIMER_EVENT_CFG_ENA_WIDTH, ADV_TIMER_EVENT_CFG_ENA_BIT))
#define ADV_TIMER_EVENT_CFG_ENA(value)                  ((value) << (ADV_TIMER_EVENT_CFG_ENA_BIT))

/**
	* CLOCK GATING
	*/
#define ADV_TIMER_CG_ENA_GET(value)                     (ARCHI_BEXTRACTU(value, ADV_TIMER_CG_ENA_WIDTH, ADV_TIMER_CG_ENA_BIT))
#define ADV_TIMER_CG_ENA_GETS(value)                    (ARCHI_BEXTRACT(value, ADV_TIMER_CG_ENA_WIDTH, ADV_TIMER_CG_ENA_BIT))
#define ADV_TIMER_CG_ENA_SET(value, field)              (ARCHI_BINSERT(value, field, ADV_TIMER_CG_ENA_WIDTH, ADV_TIMER_CG_ENA_BIT))
#define ADV_TIMER_CG_ENA(value)                         ((value) << (ADV_TIMER_CG_ENA_BIT))

#endif /* ! defined(LANGUAGE_ASSEMBLY) */

#endif /* ! defined(__ARCHI_ADV_TIMER_ADV_TIMER_V1_H__) */
