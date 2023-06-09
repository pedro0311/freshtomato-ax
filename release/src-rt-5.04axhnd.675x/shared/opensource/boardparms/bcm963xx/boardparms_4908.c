#include "bp_defs.h"
#include "boardparms.h"
#include "bcmSpiRes.h"

#ifdef _CFE_
#define MOVED_DT(x...) x
#else
#define MOVED_DT(x...)
#endif

#define BCM94908_PHY_BASE             0x8

static bp_elem_t g_bcm94908sv[] = {
  {bp_cpBoardId,               .u.cp = "94908SV"},
  {bp_ulCompatChipId,          .u.ul = 0x4908},
  {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_EMMC}, 
  {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_I2S}, 
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_2_AH},)
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x0f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},
  {bp_last}
};

static bp_elem_t g_bcm94908dvt[] = {
  {bp_cpBoardId,               .u.cp = "94908DVT"},
  {bp_ulCompatChipId,          .u.ul = 0x4908},
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usButtonIdx,             .u.us = 0},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_0 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_23_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESTORE_DEFAULTS | BP_BTN_TRIG_PRESS  },)
  MOVED_DT({bp_usButtonIdx,             .u.us = 1},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_1 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_26_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_SES | BP_BTN_TRIG_PRESS  },)
  /* GPIO 14 to 17 shared with SPI slave inteface. Disable 
   * it to make SPI slave work. Uncomment these lines if 
   * SPI slave interface is not used*/
  MOVED_DT({bp_usGpioLedPwmReserved,    .u.us = BP_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedPwmReserved,    .u.us = BP_GPIO_15_AL},)
  {bp_usGpioSfpDetect,         .u.us = BP_GPIO_22_AL},
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AH},)  // placeholder for GPHY4 Link/Act
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_26_AH},)  // placeholder spare serial led
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_27_AH},)  // placeholder spare serial led
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_28_AH},)  // placeholder spare serial led
  MOVED_DT({bp_usGpioLedSesWireless,    .u.us = BP_GPIO_24_AL},) 
  MOVED_DT({bp_usGpioLedWanData,        .u.us = BP_SERIAL_GPIO_22_AH},)  /* sw control led */
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_18_AH }, 
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_19_AH },
  {bp_usSpiSlaveSelectNum,     .u.us = 1}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 56},
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulCrossbar,              .u.ul = 11},
  {bp_ulCrossbarPhyId,         .u.ul = 0x18 | PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_GPIO_10_AL},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_GPIO_11_AL},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_21_AL},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  //{bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPortMap,               .u.ul = 0x0f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)
  /* Uncomment this line if SPI slave interface is not used */  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_16_AL},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  /* Uncomment this line if SPI slave interface is not used */  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_17_AL},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_GPIO_4_AL},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_GPIO_5_AL},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_18_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_GPIO_6_AL},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_GPIO_7_AL},)  
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  //{bp_ulCrossbar,              .u.ul = 11},
  //{bp_ulCrossbarPhyId,         .u.ul = 0x18 | PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},
  //{bp_ulCrossbar,              .u.ul = 9},    /* Removed due to Serdes GPIO enforcement */
  //{bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_19_AH},)
  MOVED_DT({bp_usGpioVoip1Led,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usGpioVoip2Led,          .u.us = BP_SERIAL_GPIO_25_AH},) /* board schematic assign wrong channel26 which cause serial channel overflow */
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},
  {bp_last}
};

static bp_elem_t g_bcm94908dvt_sfpwan[] = {
  {bp_cpBoardId,               .u.cp = "94908DVT_SFPWAN"},
  {bp_ulCompatChipId,          .u.ul = 0x4908},
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usButtonIdx,             .u.us = 0},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_0 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_23_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESTORE_DEFAULTS | BP_BTN_TRIG_PRESS  },)
  MOVED_DT({bp_usButtonIdx,             .u.us = 1},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_1 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_26_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_SES | BP_BTN_TRIG_PRESS  },)
  /* GPIO 14 to 17 shared with SPI slave inteface. Disable 
   * it to make SPI slave work. Uncomment these lines if 
   * SPI slave interface is not used*/
  MOVED_DT({bp_usGpioLedPwmReserved,    .u.us = BP_GPIO_14_AL},)
  MOVED_DT({bp_usGpioLedPwmReserved,    .u.us = BP_GPIO_15_AL},)
  {bp_usGpioSfpDetect,         .u.us = BP_GPIO_22_AL},
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_20_AH},)  // placeholder for GPHY4 Link/Act
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_26_AH},)  // placeholder spare serial led
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_27_AH},)  // placeholder spare serial led
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_28_AH},)  // placeholder spare serial led
  MOVED_DT({bp_usGpioLedSesWireless,    .u.us = BP_GPIO_24_AL},) 
  MOVED_DT({bp_usGpioLedWanData,        .u.us = BP_SERIAL_GPIO_22_AH},)  /* sw control led */
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_18_AH }, 
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_19_AH },
  {bp_usSpiSlaveSelectNum,     .u.us = 1}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 56},
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  //{bp_ulCrossbar,              .u.ul = 9},    /* Removed due to Serdes GPIO enforcement */
  //{bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_ulCrossbar,              .u.ul = 11},
  {bp_ulCrossbarPhyId,         .u.ul = 0x18 | PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_GPIO_10_AL},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_GPIO_11_AL},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_21_AL},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x0f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)
  /* Uncomment this line if SPI slave interface is not used */  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_16_AL},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  /* Uncomment this line if SPI slave interface is not used */  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_17_AL},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_GPIO_4_AL},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_GPIO_5_AL},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_18_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_GPIO_6_AL},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_GPIO_7_AL},)  
  //{bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  //{bp_ulCrossbar,              .u.ul = 11},
  //{bp_ulCrossbarPhyId,         .u.ul = 0x18 | PHY_INTEGRATED_VALID | MAC_IF_RGMII_1P8V | PHY_EXTERNAL},
  //{bp_ulCrossbar,              .u.ul = 9},
  //{bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_19_AH},)
  MOVED_DT({bp_usGpioVoip1Led,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usGpioVoip2Led,          .u.us = BP_SERIAL_GPIO_25_AH},) /* board schematic assign wrong channel26 which cause serial channel overflow */
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},
  {bp_last}
};

static bp_elem_t g_bcm94908ref[] = {
  {bp_cpBoardId,               .u.cp = "94908REF"},
  {bp_ulCompatChipId,          .u.ul = 0x4908},
  {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_EMMC}, 
#if 0
  {bp_usGpioUart2Cts,          .u.us = BP_GPIO_10_AH},
  {bp_usGpioUart2Rts,          .u.us = BP_GPIO_11_AH},
  {bp_usGpioUart2Sdin,         .u.us = BP_GPIO_12_AH}, // uart2 is /dev/ttyH0
  {bp_usGpioUart2Sdout,        .u.us = BP_GPIO_13_AH}, // uart2 is /dev/ttyH0
#endif
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_1_AH},)
  {bp_usGpioBtWake,            .u.us = BP_GPIO_9_AH},
  {bp_usGpioBtReset,           .u.us = BP_GPIO_8_AH},
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_2_AH},)
  MOVED_DT({bp_usButtonIdx,             .u.us = 0},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_0 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_30_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_PRINT | BP_BTN_TRIG_PRESS },)
  MOVED_DT({      bp_ulButtonActionParm,  .u.ptr = (void*)"Button Press -- Hold for 5s to do restore to default" },)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESET | BP_BTN_TRIG_RELEASE | BP_BTN_TRIG_0S  },)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESTORE_DEFAULTS | BP_BTN_TRIG_HOLD | BP_BTN_TRIG_5S},)
  MOVED_DT({bp_usButtonIdx,             .u.us = 1},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_1 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_29_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_SES | BP_BTN_TRIG_PRESS  },)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_30_AH},)  // placeholder for GPHY4 Link/Act
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_9_AH},)  // placeholder for GPHY4 Speed
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_8_AH},)  // placeholder for GPHY4 Speed
  MOVED_DT({bp_usGpioLedSesWireless,    .u.us = BP_SERIAL_GPIO_12_AH},) 
  MOVED_DT({bp_usGpioLedWanData,        .u.us = BP_SERIAL_GPIO_16_AH},)  /* sw control led */
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_18_AH }, 
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_19_AH },
  {bp_usSpiSlaveSelectNum,     .u.us = 1}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 56},
  {bp_usSpiSlaveSelectNum,     .u.us = 2}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 23},
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  //{bp_ulCrossbar,              .u.ul = 9},
  //{bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_22_AH},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_21_AH},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x0f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)
  MOVED_DT({bp_usGpioLedAggregateLnk,   .u.us = BP_SERIAL_GPIO_24_AH},)
  MOVED_DT({bp_usGpioLedAggregateAct,   .u.us = BP_SERIAL_GPIO_25_AH},)
  MOVED_DT({bp_usGpioVoip1Led,          .u.us = BP_SERIAL_GPIO_18_AH},)
  MOVED_DT({bp_usGpioVoip2Led,          .u.us = BP_SERIAL_GPIO_19_AH},)
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},
  {bp_usSpiSlaveSelectNum,     .u.us = 1}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 56},
  {bp_usSpiSlaveSelectNum,     .u.us = 2},
  {bp_usSpiSlaveSelectGpioNum, .u.us = 23},
  {bp_usSpiSlaveSelectNum,     .u.us = 3},
  {bp_usSpiSlaveSelectGpioNum, .u.us = 22},
  {bp_last}
};


static bp_elem_t g_bcm962118ref[] = {
  {bp_cpBoardId,               .u.cp = "962118REF"},
  {bp_ulCompatChipId,          .u.ul = 0x62118},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};

static bp_elem_t g_bcm94908ref_extphy[] = {
  {bp_cpBoardId,               .u.cp = "94908REF_XPHY"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
#if defined(CONFIG_BCM_ETHWAN)
  {bp_ulPortMap,               .u.ul = 0x9},
#else
  {bp_ulPortMap,               .u.ul = 0x1},
#endif
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
#if defined(CONFIG_BCM_ETHWAN)
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_22_AH},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_21_AH},)
#endif
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_usGpioPhyReset,          .u.us = BP_GPIO_31_AH},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};

/* Board with 2.5GPHY connected to WAN port */
static bp_elem_t g_bcm94908ref_wan2p5g[] = {
  {bp_cpBoardId,               .u.cp = "94908REF_W2P5"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_22_AH},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_21_AH},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};

/* Board with 2.5GPHY and single GPHY connected to WAN port */
static bp_elem_t g_bcm94908ref_wan2p5g_n_g[] = {
  {bp_cpBoardId,               .u.cp = "94908REF_W2P5NG"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES | PHY_EXTERNAL},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_22_AH},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_21_AH},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x0f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};

static bp_elem_t g_bcm94906ref[] = {
  {bp_cpBoardId,               .u.cp = "94906REF"},
  {bp_ulCompatChipId,          .u.ul = 0x4906},
  {bp_ulInterfaceEnable,       .u.ul = BP_PINMUX_FNTYPE_EMMC}, 
  MOVED_DT({bp_usSerialLedData,         .u.us = BP_GPIO_0_AH},)
  MOVED_DT({bp_usSerialLedClk,          .u.us = BP_GPIO_1_AH},)
  MOVED_DT({bp_usSerialLedMask,         .u.us = BP_GPIO_2_AH},)
  {bp_usGpioBtReset,           .u.us = BP_GPIO_8_AH},
  {bp_usGpioBtWake,            .u.us = BP_GPIO_9_AH},
  {bp_usGpioUart2Cts,          .u.us = BP_GPIO_10_AH},
  {bp_usGpioUart2Rts,          .u.us = BP_GPIO_11_AH},
  {bp_usGpioUart2Sdin,         .u.us = BP_GPIO_12_AH}, // uart2 is /dev/ttyH0
  {bp_usGpioUart2Sdout,        .u.us = BP_GPIO_13_AH}, // uart2 is /dev/ttyH0
  {bp_usGpioI2cSda,            .u.us = BP_GPIO_18_AH }, 
  {bp_usGpioI2cScl,            .u.us = BP_GPIO_19_AH },
  MOVED_DT({bp_usButtonIdx,             .u.us = 0},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_0 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_23_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_PRINT | BP_BTN_TRIG_PRESS },)
  MOVED_DT({      bp_ulButtonActionParm,  .u.ptr = (void*)"Button Press -- Hold for 5s to do restore to default" },)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESET | BP_BTN_TRIG_RELEASE | BP_BTN_TRIG_0S  },)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESTORE_DEFAULTS | BP_BTN_TRIG_HOLD | BP_BTN_TRIG_5S},)
  MOVED_DT({bp_usButtonIdx,             .u.us = 1},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_1 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_22_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_SES | BP_BTN_TRIG_PRESS  },)
  {bp_usSpiSlaveSelectNum,     .u.us = 1}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 56},
  {bp_usSpiSlaveSelectNum,     .u.us = 5}, /* define the SPI select for voice */
  {bp_usSpiSlaveSelectGpioNum, .u.us = 20},
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_30_AH},)  // placeholder for GPHY4 Link/Act
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_9_AH},)  // placeholder for GPHY4 Speed
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_SERIAL_GPIO_8_AH},)  // placeholder for GPHY4 Speed
  MOVED_DT({bp_usGpioLedSesWireless,    .u.us = BP_SERIAL_GPIO_12_AH},) 
  MOVED_DT({bp_usGpioLedWanData,        .u.us = BP_SERIAL_GPIO_16_AH},)  /* sw control led */
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_22_AH},) 
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_23_AH},)
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_21_AH},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x0f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)

  MOVED_DT({bp_usGpioLedAggregateLnk,   .u.us = BP_SERIAL_GPIO_24_AH},)
  MOVED_DT({bp_usGpioLedAggregateAct,   .u.us = BP_SERIAL_GPIO_25_AH},)

  MOVED_DT({bp_usGpioVoip1Led,          .u.us = BP_SERIAL_GPIO_18_AH},)
  MOVED_DT({bp_usGpioVoip2Led,          .u.us = BP_SERIAL_GPIO_19_AH},)
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_SSC_CONFIG_1},
  {bp_last}
};


static bp_elem_t g_bcm962116ref[] = {
  {bp_cpBoardId,               .u.cp = "962116REF"},
  {bp_ulCompatChipId,          .u.ul = 0x62116},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94906ref},
  {bp_last}
};

/* fake 4906REF board id. Used on 4908 board as 4906 board */
static bp_elem_t g_bcm94906ref_fake[] = {
  {bp_cpBoardId,               .u.cp = "94906REF_FAKE"},
  {bp_ulMaxNumCpu,             .u.ul = 2},
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_512MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_16BIT | BP_DDR_SSC_CONFIG_1},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};

static bp_elem_t g_bcm949408eap_extphy[] = {
  {bp_cpBoardId,               .u.cp = "949408EAP_XPHY"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x1},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},
  {bp_usGpioPhyReset,          .u.us = BP_GPIO_31_AL},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};


static bp_elem_t g_bcm949408eap_extphy_54991[] = {
  {bp_cpBoardId,               .u.cp = "949408EAP_54991"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1f},
  {bp_ulPortMap,               .u.ul = 0x1},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_0_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_1_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_26_AH},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_2_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_3_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_27_AH},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_4_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_5_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_28_AH},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usSpeedLed100,           .u.us = BP_SERIAL_GPIO_6_AH},)
  MOVED_DT({bp_usSpeedLed1000,          .u.us = BP_SERIAL_GPIO_7_AH},)  
  MOVED_DT({bp_usLinkLed,               .u.us = BP_SERIAL_GPIO_29_AH},)
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1f | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},
  {bp_usGpioPhyReset,          .u.us = BP_GPIO_31_AL},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908ref},
  {bp_last}
};

static bp_elem_t g_bcm94908eap_4dax[] = {
  {bp_cpBoardId,               .u.cp = "949408EAP_4DAX"},
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_2048MB| BP_DDR_DEVICE_WIDTH_8 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm949408eap_extphy},
  {bp_last}
};

static bp_elem_t g_bcm94908tbrhx[] = {
  {bp_cpBoardId,               .u.cp = "94908TBRHX"},
  {bp_ulCompatChipId,          .u.ul = 0x4908},
  {bp_usGpioUart2Cts,          .u.us = BP_GPIO_10_AH},
  {bp_usGpioUart2Rts,          .u.us = BP_GPIO_11_AH},
  {bp_usGpioUart2Sdin,         .u.us = BP_GPIO_12_AH}, // uart2 is /dev/ttyH0
  {bp_usGpioUart2Sdout,        .u.us = BP_GPIO_13_AH}, // uart2 is /dev/ttyH0
  MOVED_DT({bp_usButtonIdx,             .u.us = 0},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_0 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_4_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_PRINT | BP_BTN_TRIG_PRESS },)
  MOVED_DT({      bp_ulButtonActionParm,  .u.ptr = (void*)"Button Press -- Hold for 5s to do restore to default" },)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESET | BP_BTN_TRIG_RELEASE | BP_BTN_TRIG_0S  },)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_RESTORE_DEFAULTS | BP_BTN_TRIG_HOLD | BP_BTN_TRIG_5S},)
  MOVED_DT({bp_usButtonIdx,             .u.us = 1},)
  MOVED_DT({  bp_usButtonExtIntr,       .u.us = BP_EXT_INTR_1 },)
  MOVED_DT({  bp_usGpio_Intr,           .u.us = BP_GPIO_2_AL},)
  MOVED_DT({    bp_usButtonAction,      .u.us = BP_BTN_ACTION_SES | BP_BTN_TRIG_PRESS  },)
  MOVED_DT({bp_usGpioLedSesWireless,    .u.us = BP_GPIO_0_AL},)
  MOVED_DT({bp_usGpioLedReserved,       .u.us = BP_GPIO_3_AL},) // reserved for misc led
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_31_AL},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0xf},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_26_AL},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_27_AL},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_28_AL},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_29_AL},)
  {bp_ulMemoryConfig,          .u.ul = BP_DDR_SPEED_800_11_11_11 | BP_DDR_TOTAL_SIZE_1024MB| BP_DDR_DEVICE_WIDTH_16 | BP_DDR_TOTAL_WIDTH_32BIT | BP_DDR_SSC_CONFIG_1},
  {bp_usUsbPwrOn0,             .u.us = BP_GPIO_64_AH},
  {bp_usUsbPwrFlt0,            .u.us = BP_GPIO_63_AL},
  {bp_usUsbPwrOn1,             .u.us = BP_GPIO_67_AH},
  {bp_usUsbPwrFlt1,            .u.us = BP_GPIO_66_AL},
  {bp_last}
};

/* Board with 2.5GPHY connected to LAN port */
static bp_elem_t g_bcm94908tbrhx_extphy[] = {
  {bp_cpBoardId,               .u.cp = "94908TBRHX_XPHY"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  /* use the WAN LED from runner */
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_31_AL},)
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_26_AL},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_27_AL},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_28_AL},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_29_AL},)
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},
  {bp_usGpioPhyReset,          .u.us = BP_GPIO_22_AL},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908tbrhx},
  {bp_last}
};

/* Board with 2.5GPHY connected to WAN port */
static bp_elem_t g_bcm94908tbrhx_wan2p5g[] = {
  {bp_cpBoardId,               .u.cp = "94908TBRHX_W2P5"},
  {bp_usMiiMdc,                .u.us = BP_GPIO_48_AH},
  {bp_usMiiMdio,               .u.us = BP_GPIO_49_AH},
  {bp_usGphyBaseAddress,       .u.us = BCM94908_PHY_BASE},  // use phy addressses on SF2 with base address 0x8
  {bp_ucPhyType0,              .u.uc = BP_ENET_NO_PHY}, // Runner
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP},
  {bp_ucPhyAddress,            .u.uc = 0x1e},
  {bp_ulPortMap,               .u.ul = 0x9},
  {bp_ulPhyId0,                .u.ul = GMII_DIRECT | EXTSW_CONNECTED},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_MGMT }, // Managment port is on switch
  {bp_ulPhyId3,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 9},
  {bp_ulCrossbarPhyId,         .u.ul = 6 |  PHY_INTEGRATED_VALID | MAC_IF_SERDES},
  {bp_ulCrossbarPhyId,         .u.ul = 0x1e | PHY_EXTERNAL | PHY_TYPE_CL45GPHY},
  {bp_ulPortFlags,             .u.ul = PORT_FLAG_SWAP_PAIR},
  {bp_usGpioPhyReset,          .u.us = BP_GPIO_22_AL},
  {bp_ucPhyType1,              .u.uc = BP_ENET_EXTERNAL_SWITCH},
  {bp_usConfigType,            .u.us = BP_ENET_CONFIG_MMAP}, // Accessing SF2 as MMapped external switch
  {bp_ulPortMap,               .u.ul = 0x8f},
  {bp_ulPhyId0,                .u.ul = (BCM94908_PHY_BASE + 0x00) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_26_AL},)
  {bp_ulPhyId1,                .u.ul = (BCM94908_PHY_BASE + 0x01) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_27_AL},)
  {bp_ulPhyId2,                .u.ul = (BCM94908_PHY_BASE + 0x02) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_28_AL},)
  {bp_ulPhyId3,                .u.ul = (BCM94908_PHY_BASE + 0x03) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  MOVED_DT({bp_usLinkLed,               .u.us = BP_GPIO_29_AL},)
  {bp_ulPhyId7,                .u.ul = BP_PHY_ID_NOT_SPECIFIED},
  {bp_ulCrossbar,              .u.ul = 10},
  {bp_ulCrossbarPhyId,         .u.ul = (BCM94908_PHY_BASE + 0x04) | (ADVERTISE_ALL_GMII | PHY_ADV_CFG_VALID)},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94908tbrhx},
  {bp_last}
};

static bp_elem_t g_bcm94906ref2[] = {
  {bp_cpBoardId,               .u.cp = "94906REF2"},
  {bp_usUsbPwrOn0,             .u.us = BP_GPIO_64_AH},
  {bp_usUsbPwrFlt0,            .u.us = BP_GPIO_63_AL},
  {bp_usUsbPwrOn1,             .u.us = BP_GPIO_67_AH},
  {bp_usUsbPwrFlt1,            .u.us = BP_GPIO_66_AL},
  {bp_elemTemplate,            .u.bp_elemp = g_bcm94906ref},
  {bp_last}
};

static bp_elem_t g_ebg19[] = {
  {bp_cpBoardId,               .u.cp = "EBG19"},

  {bp_last}
};


static bp_elem_t g_ebg15[] = {
  {bp_cpBoardId,               .u.cp = "EBG15"},

  {bp_last}
};

bp_elem_t * g_BoardParms[] = {
  g_bcm94908sv, g_bcm94908dvt, g_bcm94908dvt_sfpwan, g_bcm94908ref, g_bcm94908ref_extphy,
  g_bcm94906ref, g_bcm94906ref_fake, g_bcm94908ref_wan2p5g, g_bcm94908ref_wan2p5g_n_g, g_bcm962118ref, g_bcm962116ref,
  g_bcm949408eap_extphy, g_bcm94908tbrhx, g_bcm94908tbrhx_extphy, g_bcm94908tbrhx_wan2p5g, 
  g_bcm94906ref2, g_bcm949408eap_extphy_54991, g_bcm94908eap_4dax, g_ebg19, g_ebg15, 0
};

