#ifndef HLCD_H_
#define HLCD_H_

#include "../../../STD_LIB/STD_TYPES.h"
#include "../../../COTS/MCAL/MGPIO/inc/MGPIO.h"

// #define CLEAR_DISPLAY              0x01    // Clear display and return home
// #define RETURN_HOME                0x02    // Return cursor to home position
// #define ENTRY_MODE_SET             0x04    // Configure cursor move direction
// #define DISPLAY_CONTROL            0x08    // Control display/cursor/blink
// #define CURSOR_DISPLAY_SHIFT       0x10    // Shift display/cursor position
// #define FUNCTION_SET               0x20    // Interface/line/font configuration
// #define SET_CGRAM_ADDR             0x40    // Set CGRAM address
// #define SET_DDRAM_ADDR             0x80    // Set DDRAM address

// /* Entry Mode Set Options (use with ENTRY_MODE_SET) */
// #define ENTRY_INCREMENT            0x02    // Cursor moves right (I/D=1)
// #define ENTRY_DECREMENT            0x00    // Cursor moves left (I/D=0)
// #define ENTRY_SHIFT_ENABLE         0x01    // Display shifts on write (S=1)
// #define ENTRY_SHIFT_DISABLE        0x00    // Display doesn't shift (S=0)

// /* Display Control Options (use with DISPLAY_CONTROL) */
// #define DISPLAY_ON                 0x04    // Turn display on (D=1)
// #define DISPLAY_OFF                0x00    // Turn display off (D=0)
// #define CURSOR_ON                  0x02    // Turn cursor on (C=1)
// #define CURSOR_OFF                 0x00    // Turn cursor off (C=0)
// #define BLINK_ON                   0x01    // Enable cursor blink (B=1)
// #define BLINK_OFF                  0x00    // Disable cursor blink (B=0)
// /* Cursor/Display Shift Options (use with CURSOR_DISPLAY_SHIFT) */
// #define SHIFT_DISPLAY              0x08    // Shift entire display (S/C=1)
// #define SHIFT_CURSOR               0x00    // Shift cursor only (S/C=0)
// #define SHIFT_RIGHT                0x04    // Shift right (R/L=1)
// #define SHIFT_LEFT                 0x00    // Shift left (R/L=0)

// /* Function Set Options (use with FUNCTION_SET) */
// #define DATA_LENGTH_8BIT           0x10    // 8-bit interface (DL=1)
// #define DATA_LENGTH_4BIT           0x00    // 4-bit interface (DL=0)
// #define TWO_LINES                  0x08    // 2-line display (N=1)
// #define ONE_LINE                   0x00    // 1-line display (N=0)
// #define FONT_5X10                  0x04    // 5x10 dot font (F=1)
// #define FONT_5X8                   0x00    // 5x8 dot font (F=0)

// /* Status Masks */
// #define BUSY_FLAG_MASK             0x80    // Mask for busy flag (BF)
// #define ADDRESS_COUNTER_MASK       0x7F    // Mask for address counter value

// /* Common Command Combinations */
// // Entry modes
// #define ENABLE_AUTO_SHIFT_RIGHT            (ENTRY_MODE_SET | ENTRY_DECREMENT | ENTRY_SHIFT_ENABLE)  // 0x05
// #define ENABLE_AUTO_SHIFT_LEFT             (ENTRY_MODE_SET | ENTRY_INCREMENT | ENTRY_SHIFT_ENABLE)  // 0x07
// #define DISABLE_AUTO_SHIFT                 (ENTRY_MODE_SET | ENTRY_INCREMENT | ENTRY_SHIFT_DISABLE) // 0x06
// // Display control        
// #define DISPLAY_ON_CURSOR_OFF              (DISPLAY_CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF)  // 0x0C
// #define DISPLAY_ON_CURSOR_ON               (DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON | BLINK_OFF)   // 0x0E
// #define DISPLAY_ON_CURSOR_BLINK            (DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON | BLINK_ON)    // 0x0F
        
// // Shift commands        
// #define SHIFT_DISPLAY_RIGHT                (CURSOR_DISPLAY_SHIFT | SHIFT_DISPLAY | SHIFT_RIGHT)     // 0x1C
// #define SHIFT_DISPLAY_LEFT                 (CURSOR_DISPLAY_SHIFT | SHIFT_DISPLAY | SHIFT_LEFT)      // 0x18
// #define MOVE_CURSOR_RIGHT                  (CURSOR_DISPLAY_SHIFT | SHIFT_CURSOR | SHIFT_RIGHT)      // 0x14
// #define MOVE_CURSOR_LEFT                   (CURSOR_DISPLAY_SHIFT | SHIFT_CURSOR | SHIFT_LEFT)       // 0x10
// #define FUNCTION_SET_8BIT_2_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_8BIT | TWO_LINES | FONT_5X8)
// #define FUNCTION_SET_8BIT_2_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_8BIT | TWO_LINES | FONT_5X10)
// #define FUNCTION_SET_8BIT_1_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_8BIT | ONE_LINE | FONT_5X8)
// #define FUNCTION_SET_8BIT_1_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_8BIT | ONE_LINE | FONT_5X10)
// #define FUNCTION_SET_4BIT_2_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_4BIT | TWO_LINES | FONT_5X8)
// #define FUNCTION_SET_4BIT_2_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_4BIT | TWO_LINES | FONT_5X10)
// #define FUNCTION_SET_4BIT_1_LINES_5x8      (FUNCTION_SET | DATA_LENGTH_4BIT | ONE_LINE | FONT_5X8)
// #define FUNCTION_SET_4BIT_1_LINES_5x10     (FUNCTION_SET | DATA_LENGTH_4BIT | ONE_LINE | FONT_5X10)

typedef enum 
{
    HLCD_CMD01_CLR_DISPLAY                   = 0b00000001U,
    HLCD_CMD02_RET_HOME                      = 0b00000010U,
    HLCD_CMD03_ENTRY_MODE_CURSOR_DIR_INC     = 0b00000110U,
    HLCD_CMD03_ENTRY_MODE_CURSOR_DIR_DEC     = 0b00000100U,
    HLCD_CMD03_ENTRY_MODE_DISPLAY_SHIFT_ON   = 0b00000101U,
    HLCD_CMD03_ENTRY_MODE_DISPLAY_SHIFT_OFF  = 0b00000100U,
    HLCD_CMD04_DISPLAY_ON                    = 0b00001100U,
    HLCD_CMD04_DISPLAY_OFF                   = 0b00001000U,
    HLCD_CMD04_DISPLAY_CURSOR_ON             = 0b00001010U,
    HLCD_CMD04_DISPLAY_CURSOR_OFF            = 0b00001000U,
    HLCD_CMD04_DISPLAY_CURSOR_BLINK_ON       = 0b00001001U,
    HLCD_CMD04_DISPLAY_CURSOR_BLINK_OFF      = 0b00001000U,
    HLCD_CMD05_DISPLAY_SHIFT_SET             = 0b00011000U,
    HLCD_CMD05_CURSOR_SHIFT_SET              = 0b00010000U,
    HLCD_CMD05_CURSOR_DISPLAY_SHIFT_RIGHT    = 0b00010100U,
    HLCD_CMD05_CURSOR_DISPLAY_SHIFT_LEFT     = 0b00010000U,
    HLCD_CMD06_FUNC_SET_8_BIT_MODE           = 0b00110000U,
    HLCD_CMD06_FUNC_SET_4_BIT_MODE           = 0b00100000U,
    HLCD_CMD06_FUNC_SET_2_DISPLAY_LINE       = 0b00101000U,
    HLCD_CMD06_FUNC_SET_1_DISPLAY_LINE       = 0b00100000U,
    HLCD_CMD06_FUNC_SET_5x10_DOTS            = 0b00100100U,
    HLCD_CMD06_FUNC_SET_5x7_DOTS             = 0b00100000U,

} HLCD_enuLCDCommands_t;

typedef enum
{
	HLCD_PIN_0 = 0,
	HLCD_PIN_1,
	HLCD_PIN_2,
	HLCD_PIN_3,
	HLCD_PIN_4,
	HLCD_PIN_5,
	HLCD_PIN_6,
	HLCD_PIN_7,
	HLCD_PIN_RS = 0,
	HLCD_PIN_RW,
	HLCD_PIN_EN

} HLCD_enuPIN_t;

typedef enum
{
	HLCD_8_BIT_MODE = 8,
	HLCD_4_BIT_MODE = 4

} HLCD_enuHLCD_Mode_t; 

/**** Enum for Error Status ****/
typedef enum
{
	HLCD_OK = 0,
	HLCD_NOK,
	HLCD_NULL_POINTER,
	HLCD_INVALID_MODE,
	HLCD_INVALID_LINE,
	HLCD_INVALID_COLUMN,
	HLCD_INVALID_BATTERN_NUMBER,
    HLCD_BUSY,
	
} HLCD_enuErrorStatus_t ;

typedef enum
{
	HLCD_FIRST_LINE = 0,
	HLCD_SECOND_LINE

} HLCD_Row_t;

typedef struct
{
    MGPIO_Port_t HLCD_portNum;
    MGPIO_Pin_t HLCD_pinNum;

} HCLCD_strPortPin_t;

typedef struct
{
	HLCD_enuHLCD_Mode_t HLCD_Mode;
    HCLCD_strPortPin_t HLCD_DataPins[8];
	HCLCD_strPortPin_t HLCD_ControlPins[3];
    
} HLCD_strCfg_t;


extern HLCD_enuErrorStatus_t HLCD_enuAsyncInit(void);
extern HLCD_enuErrorStatus_t HLCD_enuAsyncWriteCommand(u8 Copy_u8Command, u8 Copy_u8LCDNum);
extern HLCD_enuErrorStatus_t HLCD_enuAsyncWriteChar(u8 Copy_u8Data,u8 Copy_u8LCDNum);
extern HLCD_enuErrorStatus_t HLCD_enuAsyncWriteString(u8 * Add_String,u8 Copy_u8LCDNum);
extern HLCD_enuErrorStatus_t HLCD_enuAsyncWriteNumber(s64 Copy_Number,u8 Copy_u8LCDNum);
extern HLCD_enuErrorStatus_t HLCD_enuGoToRowColumn(HLCD_Row_t Copy_enuRow, u8 Copy_enuColumn,u8 Copy_u8LCDNum);
extern HLCD_enuErrorStatus_t HLCD_enuStoreSpecialChar(u8 * Copy_u8BatternArr,u8 Copy_BatternNum,u8 Copy_u8LCDNum);
extern HLCD_enuErrorStatus_t HLCD_enuDisplaySpecialChar(u8 Copy_BatternNum,HLCD_Row_t Copy_enuRow, u8 Copy_enuColumn,u8 Copy_u8LCDNum);

#endif // HLCD_H_