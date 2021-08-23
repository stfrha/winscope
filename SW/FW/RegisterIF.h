#ifndef __REGISTERS_IF__
#define  __REGISTERS_IF__

//------------------------------------
// Register addresses
//------------------------------------

#define    ADR_TRIGG_SETUP          0x10
#define    ADR_PRBA_TRG_LEVEL       0x11
#define    ADR_PRBB_TRG_LEVEL       0x12
#define    ADR_PODP_TRG_PTRN        0x13
#define    ADR_PODQ_TRG_PTRN        0x14
#define    ADR_PODP_PTRN_VALID      0x15
#define    ADR_PODQ_PTRN_VALID      0x16
#define    ADR_PODP_EDGE_SLOPE      0x17
#define    ADR_PODQ_EDGE_SLOPE      0x18
#define    ADR_PODP_EDGE_VALID      0x19
#define    ADR_PODQ_EDGE_VALID        0x1A
#define    ADR_TRG_DELY_L           0x20
#define    ADR_TRG_DELY_M           0x21
#define    ADR_TRG_DELY_H           0x22
#define    ADR_BUFFER_SIZE_L        0x24
#define    ADR_BUFFER_SIZE_M        0x25
#define    ADR_BUFFER_SIZE_H        0x26
#define    ADR_CONTROL                0x30
#define    ADR_STATUS                0x31
#define    ADR_PRBA_IN                0x40
#define    ADR_PRBB_IN                0x41
#define    ADR_PODP_IN                0x42
#define    ADR_PODQ_IN                0x43
#define    ADR_CLK_TOGGLE_L            0x50
#define    ADR_CLK_TOGGLE_M            0x51
#define    ADR_CLK_TOGGLE_H            0x52
#define    ADR_CLK_RESET_L          0x54
#define    ADR_CLK_RESET_M          0x55
#define    ADR_CLK_RESET_H          0x56
#define    ADR_PRB_GAIN                0x60
#define    ADR_PRB_POD_CTRL            0x61
#define    ADR_LEDS                    0x70
#define    ADR_ID                    0x80
#define    ADR_REV                    0x82
 
 
//------------------------------------
// Register bits
//------------------------------------

// TRIGG_SETUP

typedef union _trigg_setup {
    struct _ts_flags {
        unsigned char TRIGG_ALWAYS   : 1;
        unsigned char TRIGG_PRB_EDGE : 1;
        unsigned char TRIGG_POD_PTRN : 1;
        unsigned char TRIGG_POD_EDGE : 1;
        unsigned char TRIGG_EXT_EDGE : 1;
        unsigned char PRB_TRG_SLOPE  : 1;
        unsigned char PRB_TRG_SOURCE : 1;
        unsigned char EXT_EDGE_SLOPE : 1;
    } bits;
    unsigned char field;
} TRIGG_SETUP_TYPE;


// PRB_POD_CTRL

typedef union _prb_pod_ctrl {
    struct _ppc_flags {
        unsigned char PRBA           : 2;
        unsigned char PRBB           : 2;
        unsigned char PRBA_ACTIVE    : 1;
        unsigned char PRBB_ACTIVE    : 1;
        unsigned char PODP_ACTIVE    : 1;
        unsigned char PODQ_ACTIVE    : 1;
    } bits;
    unsigned char field;
} PRB_POD_CTRL_TYPE;

// CONTROL

typedef union _cntrl {
    struct _c_flags {
        unsigned char SWEEP_MODE     : 2;
        unsigned char CLOCK_SELECT   : 1;
        unsigned char IRQ_EN         : 1;
        unsigned char                 : 2;
        unsigned char LOCK_PRB_POD   : 1;
        unsigned char NEXT_STATE     : 1;
    } bits;
    unsigned char field;
} CONTROL_TYPE;

// STATUS

typedef union _status {
    struct _s_flags {
        unsigned char SWEEP_DONE     : 1;
        unsigned char DMA_TX_DONE    : 1;
        unsigned char                : 3;
        unsigned char SWEEP_STATE    : 3;
    } bits;
    unsigned char field;
} STATUS_TYPE;

// PRB_GAIN

typedef union _prb_gain {
    struct _pg_flags {
        unsigned char PRBA_ATT0      : 1;
        unsigned char PRBA_ATT1      : 1;
        unsigned char PRBA_AMP0      : 1;
        unsigned char PRBA_AMP1      : 1;
        unsigned char PRBB_ATT0      : 1;
        unsigned char PRBB_ATT1      : 1;
        unsigned char PRBB_AMP0      : 1;
        unsigned char PRBB_AMP1      : 1;
    } bits;
    struct _pg_minifields {
        unsigned char PRBA             : 4;
        unsigned char PRBB             : 4;
    } minifields;
    unsigned char field;
} PRB_GAIN_TYPE;



//------------------------------------
// FSM states
//------------------------------------

// TRIGG_CNTRL

#define    TC_SM_NORMAL            0x00
#define    TC_SM_AUTO              0x01
#define    TC_SM_SINGLE            0x02

// STATUS

#define    S_SS_IDLE               0x0
#define    S_SS_PRE_FILL           0x1
#define    S_SS_ARMED              0x2
#define    S_SS_TRIGGED            0x3
#define    S_SS_FULL               0x4

#ifdef __IAR_SYSTEMS_ICC

#define    TRIGG_SETUP_REG          (* (TRIGG_SETUP_TYPE xdata *)   ADR_TRIGG_SETUP)
#define    PRBA_TRG_LEVEL_REG       (* (unsigned char xdata *)      ADR_PRBA_TRG_LEVEL)
#define    PRBB_TRG_LEVEL_REG       (* (unsigned char xdata *)      ADR_PRBB_TRG_LEVEL)
#define    PODP_TRG_PTRN_REG        (* (unsigned char xdata *)      ADR_PODP_TRG_PTRN)
#define    PODQ_TRG_PTRN_REG        (* (unsigned char xdata *)      ADR_PODQ_TRG_PTRN)
#define    PODP_PTRN_VALID_REG      (* (unsigned char xdata *)      ADR_PODP_PTRN_VALID)
#define    PODQ_PTRN_VALID_REG      (* (unsigned char xdata *)      ADR_PODQ_PTRN_VALID)
#define    PODP_EDGE_SLOPE_REG      (* (unsigned char xdata *)      ADR_PODP_EDGE_SLOPE)
#define    PODQ_EDGE_SLOPE_REG      (* (unsigned char xdata *)      ADR_PODQ_EDGE_SLOPE)
#define    PODP_EDGE_VALID_REG      (* (unsigned char xdata *)      ADR_PODP_EDGE_VALID)
#define    PODQ_EDGE_VALID_REG      (* (unsigned char xdata *)      ADR_PODQ_EDGE_VALID)
#define    TRG_DELY_L_REG           (* (unsigned char xdata *)      ADR_TRG_DELY_L)
#define    TRG_DELY_M_REG           (* (unsigned char xdata *)      ADR_TRG_DELY_M)
#define    TRG_DELY_H_REG           (* (unsigned char xdata *)      ADR_TRG_DELY_H)
#define    BUFFER_SIZE_L_REG        (* (unsigned char xdata *)      ADR_BUFFER_SIZE_L)
#define    BUFFER_SIZE_M_REG        (* (unsigned char xdata *)      ADR_BUFFER_SIZE_M)
#define    BUFFER_SIZE_H_REG        (* (unsigned char xdata *)      ADR_BUFFER_SIZE_H)
#define    CONTROL_REG                (* (CONTROL_TYPE xdata *)   ADR_CONTROL)
#define    STATUS_REG               (* (STATUS_TYPE xdata *)        ADR_STATUS)
#define    PRBA_IN_REG                (* (unsigned char xdata *)      ADR_PRBA_IN)
#define    PRBB_IN_REG                (* (unsigned char xdata *)      ADR_PRBB_IN)
#define    PODP_IN_REG                (* (unsigned char xdata *)      ADR_PODP_IN)
#define    PODQ_IN_REG                (* (unsigned char xdata *)      ADR_PODQ_IN)
#define    CLK_TOGGLE_L_REG         (* (unsigned char xdata *)      ADR_CLK_TOGGLE_L)
#define    CLK_TOGGLE_M_REG         (* (unsigned char xdata *)      ADR_CLK_TOGGLE_M)
#define    CLK_TOGGLE_H_REG         (* (unsigned char xdata *)      ADR_CLK_TOGGLE_H)
#define    CLK_RESET_L_REG          (* (unsigned char xdata *)      ADR_CLK_RESET_L)
#define    CLK_RESET_M_REG          (* (unsigned char xdata *)      ADR_CLK_RESET_M)
#define    CLK_RESET_H_REG          (* (unsigned char xdata *)      ADR_CLK_RESET_H)
#define    PRB_GAIN_REG             (* (PRB_GAIN_TYPE xdata *)      ADR_PRB_GAIN)
#define    PRB_POD_CTRL_REG            (* (PRB_POD_CTRL_TYPE xdata *)    ADR_PRB_POD_CTRL)
#define    LEDS_REG                 (* (unsigned char xdata *)      ADR_LEDS)
#define    ID_REG                    (* (unsigned char xdata *)      ADR_ID)
#define    REV_REG                    (* (unsigned char xdata *)      ADR_REV)

#endif


#endif
