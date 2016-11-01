#ifndef DiB3000M_FrontEnd_HwCfg_h
#define DiB3000M_FrontEnd_HwCfg_h

#define DEF_NUM_DEMODS              2

#define DEF_DIB3000MC_I2C_ADDRESS   S16 DIB3000MC_I2C_ADDRESS[] = {0x0F,0x10,0x0c,0x0d}


#define DEF_TUNER_TYPE     TUNER_PANASONIC_ENV57H13D5

#define DEF_TUNER_I2C_ADDRESS       0xC0

#define DEF_PHASE_NOISE_MODE                0

#define DEF_MOBILE_MODE      Auto_Reception


#define DEF_IMPULSE_NOISE_MODE      0

#define DEF_MPEG2_OUTPUT_188        0

#define DEF_OUTPUT_MODE       MPEG2_SERIAL

#define DEF_REG_34_0  write_i2c_frame(i2cHandle,34 , 0x1294 , device_address)
#define DEF_REG_35_0  write_i2c_frame(i2cHandle,35 , 0x1ff8 , device_address)

#define DEF_REG_34_1  write_i2c_frame(i2cHandle,34 , 0x1294 , device_address)
#define DEF_REG_35_1  write_i2c_frame(i2cHandle,35 , 0x1ff8 , device_address)

#define DEF_REG_34_2  write_i2c_frame(i2cHandle,34 , 0x1294 , device_address)
#define DEF_REG_35_2  write_i2c_frame(i2cHandle,35 , 0x1ff8 , device_address)

#define DEF_REG_34_3  write_i2c_frame(i2cHandle,34 , 0x1294 , device_address)
#define DEF_REG_35_3  write_i2c_frame(i2cHandle,35 , 0x1ff8 , device_address)

#define DEF_REG_34_4  write_i2c_frame(i2cHandle,34 , 0x1294 , device_address)
#define DEF_REG_35_4  write_i2c_frame(i2cHandle,35 , 0x1ff8 , device_address)



#define DEF_REG_129_0  write_i2c_frame(i2cHandle,129 , 0x99a , device_address)
#define DEF_REG_130_0  write_i2c_frame(i2cHandle,130 , 0x7fae , device_address)
#define DEF_REG_131_0  write_i2c_frame(i2cHandle,131 , 0x333 , device_address)
#define DEF_REG_132_0  write_i2c_frame(i2cHandle,132 , 0x7ff0 , device_address)

#define DEF_REG_129_1  write_i2c_frame(i2cHandle,129 , 0x23d , device_address)
#define DEF_REG_130_1  write_i2c_frame(i2cHandle,130 , 0x7fdf , device_address)
#define DEF_REG_131_1  write_i2c_frame(i2cHandle,131 , 0xa4 , device_address)
#define DEF_REG_132_1  write_i2c_frame(i2cHandle,132 , 0x7ff0 , device_address)

#define DEF_REG_129_2  write_i2c_frame(i2cHandle,129 , 0x148 , device_address)
#define DEF_REG_130_2  write_i2c_frame(i2cHandle,130 , 0x7ff0 , device_address)
#define DEF_REG_131_2  write_i2c_frame(i2cHandle,131 , 0xa4 , device_address)
#define DEF_REG_132_2  write_i2c_frame(i2cHandle,132 , 0x7ff8 , device_address)

#define DEF_SampFreq_KHz     27700


#define Clock_Ref                   0x151fd2



#define DEF_REG_21_0  write_i2c_frame(i2cHandle,21 , 0x164 , device_address)
#define DEF_REG_21_1  write_i2c_frame(i2cHandle,21 , 0x364 , device_address)
#define DEF_REG_22  write_i2c_frame(i2cHandle,22 , 0x463d , device_address)


#define DEF_REG_23_5  write_i2c_frame(i2cHandle,23 , 0x69 , device_address)
#define DEF_REG_24_5  write_i2c_frame(i2cHandle,24 , 0x9f18 , device_address)

#define DEF_REG_23_6  write_i2c_frame(i2cHandle,23 , 0x7e , device_address)
#define DEF_REG_24_6  write_i2c_frame(i2cHandle,24 , 0xbee9 , device_address)

#define DEF_REG_23_7  write_i2c_frame(i2cHandle,23 , 0x93 , device_address)
#define DEF_REG_24_7  write_i2c_frame(i2cHandle,24 , 0xdebb , device_address)

#define DEF_REG_23_8  write_i2c_frame(i2cHandle,23 , 0xa8 , device_address)
#define DEF_REG_24_8  write_i2c_frame(i2cHandle,24 , 0xfe8c , device_address)


#define DEF_REG_40_1  write_i2c_frame(i2cHandle,40 , 0x119 , device_address)
#define DEF_REG_41_1  write_i2c_frame(i2cHandle,41 , 0x330 , device_address)

#define DEF_REG_40_0  write_i2c_frame(i2cHandle,40 , 0x119 , device_address)
#define DEF_REG_41_0  write_i2c_frame(i2cHandle,41 , 0x330 , device_address)


#define DEF_REG_6_5  write_i2c_frame(i2cHandle,6 , 0x28 , device_address)
#define DEF_REG_7_5  write_i2c_frame(i2cHandle,7 , 0x9380 , device_address)
#define DEF_REG_8_5  write_i2c_frame(i2cHandle,8 , 0x87 , device_address)
#define DEF_REG_9_5  write_i2c_frame(i2cHandle,9 , 0x4100 , device_address)
#define DEF_REG_10_5  write_i2c_frame(i2cHandle,10 , 0x2a4 , device_address)
#define DEF_REG_11_5  write_i2c_frame(i2cHandle,11 , 0x4500 , device_address)
#define DEF_REG_12_5  write_i2c_frame(i2cHandle,12 , 0x0 , device_address)
#define DEF_REG_13_5  write_i2c_frame(i2cHandle,13 , 0x3e8 , device_address)
#define DEF_REG_14_5  write_i2c_frame(i2cHandle,14 , 0x0 , device_address)
#define DEF_REG_15_5  write_i2c_frame(i2cHandle,15 , 0x3f2 , device_address)
#define DEF_REG_16_5  write_i2c_frame(i2cHandle,16 , 0x1 , device_address)
#define DEF_REG_17_5  write_i2c_frame(i2cHandle,17 , 0xb0d0 , device_address)

#define DEF_REG_6_6  write_i2c_frame(i2cHandle,6 , 0x21 , device_address)
#define DEF_REG_7_6  write_i2c_frame(i2cHandle,7 , 0xd040 , device_address)
#define DEF_REG_8_6  write_i2c_frame(i2cHandle,8 , 0x70 , device_address)
#define DEF_REG_9_6  write_i2c_frame(i2cHandle,9 , 0xb62b , device_address)
#define DEF_REG_10_6  write_i2c_frame(i2cHandle,10 , 0x233 , device_address)
#define DEF_REG_11_6  write_i2c_frame(i2cHandle,11 , 0x8ed5 , device_address)
#define DEF_REG_12_6  write_i2c_frame(i2cHandle,12 , 0x0 , device_address)
#define DEF_REG_13_6  write_i2c_frame(i2cHandle,13 , 0x3e8 , device_address)
#define DEF_REG_14_6  write_i2c_frame(i2cHandle,14 , 0x0 , device_address)
#define DEF_REG_15_6  write_i2c_frame(i2cHandle,15 , 0x3f2 , device_address)
#define DEF_REG_16_6  write_i2c_frame(i2cHandle,16 , 0x1 , device_address)
#define DEF_REG_17_6  write_i2c_frame(i2cHandle,17 , 0xb0d0 , device_address)

#define DEF_REG_6_7  write_i2c_frame(i2cHandle,6 , 0x1c , device_address)
#define DEF_REG_7_7  write_i2c_frame(i2cHandle,7 , 0xfba5 , device_address)
#define DEF_REG_8_7  write_i2c_frame(i2cHandle,8 , 0x60 , device_address)
#define DEF_REG_9_7  write_i2c_frame(i2cHandle,9 , 0x9c25 , device_address)
#define DEF_REG_10_7  write_i2c_frame(i2cHandle,10 , 0x1e3 , device_address)
#define DEF_REG_11_7  write_i2c_frame(i2cHandle,11 , 0xcb7 , device_address)
#define DEF_REG_12_7  write_i2c_frame(i2cHandle,12 , 0x0 , device_address)
#define DEF_REG_13_7  write_i2c_frame(i2cHandle,13 , 0x3e8 , device_address)
#define DEF_REG_14_7  write_i2c_frame(i2cHandle,14 , 0x0 , device_address)
#define DEF_REG_15_7  write_i2c_frame(i2cHandle,15 , 0x3f2 , device_address)
#define DEF_REG_16_7  write_i2c_frame(i2cHandle,16 , 0x1 , device_address)
#define DEF_REG_17_7  write_i2c_frame(i2cHandle,17 , 0xb0d0 , device_address)

#define DEF_REG_6_8  write_i2c_frame(i2cHandle,6 , 0x19 , device_address)
#define DEF_REG_7_8  write_i2c_frame(i2cHandle,7 , 0x5c30 , device_address)
#define DEF_REG_8_8  write_i2c_frame(i2cHandle,8 , 0x54 , device_address)
#define DEF_REG_9_8  write_i2c_frame(i2cHandle,9 , 0x88a0 , device_address)
#define DEF_REG_10_8  write_i2c_frame(i2cHandle,10 , 0x1a6 , device_address)
#define DEF_REG_11_8  write_i2c_frame(i2cHandle,11 , 0xab20 , device_address)
#define DEF_REG_12_8  write_i2c_frame(i2cHandle,12 , 0x0 , device_address)
#define DEF_REG_13_8  write_i2c_frame(i2cHandle,13 , 0x3e8 , device_address)
#define DEF_REG_14_8  write_i2c_frame(i2cHandle,14 , 0x0 , device_address)
#define DEF_REG_15_8  write_i2c_frame(i2cHandle,15 , 0x3f2 , device_address)
#define DEF_REG_16_8  write_i2c_frame(i2cHandle,16 , 0x1 , device_address)
#define DEF_REG_17_8  write_i2c_frame(i2cHandle,17 , 0xb0d0 , device_address)


 /********   Bw = 5 ********/
#define DEF_REG_55_5  write_i2c_frame(i2cHandle,55 , 0x489 , device_address)
#define DEF_REG_56_5  write_i2c_frame(i2cHandle,56 , 0x89 , device_address)
#define DEF_REG_57_5  write_i2c_frame(i2cHandle,57 , 0x72 , device_address)

 /********   Bw = 6 ********/
#define DEF_REG_55_6  write_i2c_frame(i2cHandle,55 , 0x4a5 , device_address)
#define DEF_REG_56_6  write_i2c_frame(i2cHandle,56 , 0xa5 , device_address)
#define DEF_REG_57_6  write_i2c_frame(i2cHandle,57 , 0x89 , device_address)

 /********   Bw = 7 ********/
#define DEF_REG_55_7  write_i2c_frame(i2cHandle,55 , 0x4c0 , device_address)
#define DEF_REG_56_7  write_i2c_frame(i2cHandle,56 , 0xc0 , device_address)
#define DEF_REG_57_7  write_i2c_frame(i2cHandle,57 , 0xa0 , device_address)

 /********   Bw = 8 ********/
#define DEF_REG_55_8  write_i2c_frame(i2cHandle,55 , 0x4db , device_address)
#define DEF_REG_56_8  write_i2c_frame(i2cHandle,56 , 0xdb , device_address)
#define DEF_REG_57_8  write_i2c_frame(i2cHandle,57 , 0xb7 , device_address)

 /******** FFT mode 0     ********/
#define DEF_REG_58_0  write_i2c_frame(i2cHandle,58 , 0x38 , device_address)
#define DEF_REG_59_0  write_i2c_frame(i2cHandle,59 , 0x6d9 , device_address)
#define DEF_REG_60_0  write_i2c_frame(i2cHandle,60 , 0x3f28 , device_address)
#define DEF_REG_61_0  write_i2c_frame(i2cHandle,61 , 0x7a7 , device_address)
#define DEF_REG_62_0  write_i2c_frame(i2cHandle,62 , 0x3a74 , device_address)
#define DEF_REG_63_0  write_i2c_frame(i2cHandle,63 , 0x196 , device_address)
#define DEF_REG_64_0  write_i2c_frame(i2cHandle,64 , 0x32a , device_address)
#define DEF_REG_65_0  write_i2c_frame(i2cHandle,65 , 0x48c , device_address)
#define DEF_REG_66_0  write_i2c_frame(i2cHandle,66 , 0x3ffe , device_address)
#define DEF_REG_67_0  write_i2c_frame(i2cHandle,67 , 0x7f3 , device_address)
#define DEF_REG_68_0  write_i2c_frame(i2cHandle,68 , 0x2d94 , device_address)
#define DEF_REG_69_0  write_i2c_frame(i2cHandle,69 , 0x76 , device_address)
#define DEF_REG_70_0  write_i2c_frame(i2cHandle,70 , 0x53d , device_address)
#define DEF_REG_71_0  write_i2c_frame(i2cHandle,71 , 0x3ff8 , device_address)
#define DEF_REG_72_0  write_i2c_frame(i2cHandle,72 , 0x7e3 , device_address)
#define DEF_REG_73_0  write_i2c_frame(i2cHandle,73 , 0x3320 , device_address)
#define DEF_REG_74_0  write_i2c_frame(i2cHandle,74 , 0x76 , device_address)
#define DEF_REG_75_0  write_i2c_frame(i2cHandle,75 , 0x5b3 , device_address)
#define DEF_REG_76_0  write_i2c_frame(i2cHandle,76 , 0x3feb , device_address)
#define DEF_REG_77_0  write_i2c_frame(i2cHandle,77 , 0x7d2 , device_address)
#define DEF_REG_78_0  write_i2c_frame(i2cHandle,78 , 0x365e , device_address)
#define DEF_REG_79_0  write_i2c_frame(i2cHandle,79 , 0x76 , device_address)
#define DEF_REG_80_0  write_i2c_frame(i2cHandle,80 , 0x48c , device_address)
#define DEF_REG_81_0  write_i2c_frame(i2cHandle,81 , 0x3ffe , device_address)
#define DEF_REG_82_0  write_i2c_frame(i2cHandle,82 , 0x5b3 , device_address)
#define DEF_REG_83_0  write_i2c_frame(i2cHandle,83 , 0x3feb , device_address)
#define DEF_REG_84_0  write_i2c_frame(i2cHandle,84 , 0x76 , device_address)
#define DEF_REG_85_0  write_i2c_frame(i2cHandle,85 , 0x0 , device_address)
#define DEF_REG_86_0  write_i2c_frame(i2cHandle,86 , 0xd , device_address)

 /******** FFT mode 1     ********/
#define DEF_REG_58_1  write_i2c_frame(i2cHandle,58 , 0x3b , device_address)
#define DEF_REG_59_1  write_i2c_frame(i2cHandle,59 , 0x6d9 , device_address)
#define DEF_REG_60_1  write_i2c_frame(i2cHandle,60 , 0x3f28 , device_address)
#define DEF_REG_61_1  write_i2c_frame(i2cHandle,61 , 0x7a7 , device_address)
#define DEF_REG_62_1  write_i2c_frame(i2cHandle,62 , 0x3a74 , device_address)
#define DEF_REG_63_1  write_i2c_frame(i2cHandle,63 , 0x196 , device_address)
#define DEF_REG_64_1  write_i2c_frame(i2cHandle,64 , 0x32a , device_address)
#define DEF_REG_65_1  write_i2c_frame(i2cHandle,65 , 0x48c , device_address)
#define DEF_REG_66_1  write_i2c_frame(i2cHandle,66 , 0x3ffe , device_address)
#define DEF_REG_67_1  write_i2c_frame(i2cHandle,67 , 0x7f3 , device_address)
#define DEF_REG_68_1  write_i2c_frame(i2cHandle,68 , 0x2d94 , device_address)
#define DEF_REG_69_1  write_i2c_frame(i2cHandle,69 , 0x76 , device_address)
#define DEF_REG_70_1  write_i2c_frame(i2cHandle,70 , 0x53d , device_address)
#define DEF_REG_71_1  write_i2c_frame(i2cHandle,71 , 0x3ff8 , device_address)
#define DEF_REG_72_1  write_i2c_frame(i2cHandle,72 , 0x7e3 , device_address)
#define DEF_REG_73_1  write_i2c_frame(i2cHandle,73 , 0x3320 , device_address)
#define DEF_REG_74_1  write_i2c_frame(i2cHandle,74 , 0x76 , device_address)
#define DEF_REG_75_1  write_i2c_frame(i2cHandle,75 , 0x5b3 , device_address)
#define DEF_REG_76_1  write_i2c_frame(i2cHandle,76 , 0x3feb , device_address)
#define DEF_REG_77_1  write_i2c_frame(i2cHandle,77 , 0x7d2 , device_address)
#define DEF_REG_78_1  write_i2c_frame(i2cHandle,78 , 0x365e , device_address)
#define DEF_REG_79_1  write_i2c_frame(i2cHandle,79 , 0x76 , device_address)
#define DEF_REG_80_1  write_i2c_frame(i2cHandle,80 , 0x48c , device_address)
#define DEF_REG_81_1  write_i2c_frame(i2cHandle,81 , 0x3ffe , device_address)
#define DEF_REG_82_1  write_i2c_frame(i2cHandle,82 , 0x5b3 , device_address)
#define DEF_REG_83_1  write_i2c_frame(i2cHandle,83 , 0x3feb , device_address)
#define DEF_REG_84_1  write_i2c_frame(i2cHandle,84 , 0x0 , device_address)
#define DEF_REG_85_1  write_i2c_frame(i2cHandle,85 , 0x8200 , device_address)
#define DEF_REG_86_1  write_i2c_frame(i2cHandle,86 , 0xd , device_address)



#endif
