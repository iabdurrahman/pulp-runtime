 #include <stdint.h>

 #define PADMUX_0 (ARCHI_APB_SOC_CTRL_ADDR + 0x10)
 #define PADMUX_1 (ARCHI_APB_SOC_CTRL_ADDR + 0x14)
 
 int digitalRead(int pin);
 void digitalWrite(int pin, int value);
 void pinMode(int pin, uint8_t mode);

 static const uint32_t digital_pin_to_pad[] = {
    0, //GPIO_0 -> spim_sdio0 -> pad_mux [0]
    1, //GPIO_1 -> spim_sdio1 -> pad_mux [1]
    2, //GPIO_2 -> spim_sdio2 -> pad_mux [2]
    3, //GPIO_3 -> spim_sdio3 -> pad_mux [3]
    4, //GPIO_4 -> spim_csn0 -> pad_mux [4]
    5, //GPIO_5 -> spim_csn1 -> pad_mux [5] -> led0_o
    6, //GPIO_6 -> spim_sck -> pad_mux [6]
    7, //GPIO_7 -> uart_rx -> pad_mux [7] -> uart_rx
    8, //GPIO_8 -> uart-tx -> pad_mux [8] -> uart_tx
    9, //GPIO_9 -> cam_pclk -> pad_mux [9] -> led1_o
    10, //GPIO_10 -> cam_hsync -> pad_mux [10] -> led2_o
    11, //GPIO_11 -> cam_data0 -> pad_mux [11] -> led3_o
    12, //GPIO_12 -> cam_data1 -> pad_mux [12]
    13, //GPIO_13 -> cam_data2 -> pad_mux [13]
    14, //GPIO_14 -> cam_data3 -> pad_mux [14]
    15, //GPIO_15 -> cam_data4 -> pad_mux [15]
    16, //GPIO_16 -> cam_data5 -> pad_mux [16]
    17, //GPIO_17 -> cam_data6 -> pad_mux [17]
    18, //GPIO_18 -> cam_data7 -> pad_mux [18]
    19, //GPIO_19 -> cam_vsync -> pad_mux [19]
    20, //GPIO_20 -> sdio_clk -> pad_mux [20]
    21, //GPIO_21 -> sdio_cmd -> pad_mux [21]
    22, //GPIO_22 -> sdio_data0 -> pad_mux [22]
    23, //GPIO_23 -> sdio_data1 -> pad_mux [23]
    24, //GPIO_24 -> sdio_data2 -> pad_mux [24]
    25, //GPIO_25 -> sdio_data3 -> pad_mux [25]
    33, //GPIO_26 -> i2c0_sda -> pad_mux [33]
    34, //GPIO_27 -> i2c0_scl -> pad_mux [34]
    35, //GPIO_28 -> i2s0_ -> pad_mux [35]
    36, //GPIO_29 -> i2s0_ -> pad_mux [36]
    37, //GPIO_30 -> i2s0_ -> pad_mux [37]
    38 //GPIO_31 -> i2s1_ -> pad_mux [38]
};
