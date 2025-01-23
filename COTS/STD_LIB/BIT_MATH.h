#ifndef BIT_MATH_H_
#define BIT_MATH_H_

// Set bit BIT in REG
#define SET_BIT(Reg, Bit) (Reg |= (1 << (Bit)))

// Clear bit BIT in REG
#define CLR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))

// Toggle bit BIT in REG
#define TOG_BIT(Reg, Bit) (Reg ^= (1 << (Bit)))

// Get bit BIT in REG
#define GET_BIT(Reg, Bit) ((Reg >> (Bit)) & (0x01))

// Set high nibble in REG
#define SET_HIGH_NIB(Reg) (Reg |= 0xF0)

// Clear high nibble in REG
#define CLR_HIGH_NIB(Reg) (Reg &= 0x0F)

// Get high nibble in REG
#define GET_HIGH_NIB(Reg) ((Reg >> 4) & 0x0F)

// Toggle high nibble in REG
#define TOG_HIGH_NIB(Reg) (Reg ^= 0xF0)

// Set low nibble in REG
#define SET_LOW_NIB(Reg) (Reg |= 0x0F)

// Clear low nibble in REG
#define CLR_LOW_NIB(Reg) (Reg &= 0xF0)

// Get low nibble in REG
#define GET_LOW_NIB(Reg) (Reg & 0x0F)

// Toggle low nibble in REG
#define TOG_LOW_NIB(Reg) (Reg ^= 0x0F)

// Assign VALUE to REG
#define ASSIGN_REG(Reg, Value) (Reg = (Value))

// Assign VALUE to high nibble of REG
#define ASSIGN_HIGH_NIB(Reg, Value) (Reg = (Reg & 0x0F) | ((Value & 0x0F) << 4))

// Assign VALUE to low nibble of REG
#define ASSIGN_LOW_NIB(Reg, Value) (Reg = (Reg & 0xF0) | (Value & 0x0F))

// Right shift by NO in REG
#define RSHFT_REG(Reg, NO) (Reg >>= (NO))

// Left shift by NO in REG
#define LSHFT_REG(Reg, NO) (Reg <<= (NO))

// Circular right shift by NO in REG
#define CRSHFT_REG(Reg, NO) (Reg = (Reg >> (NO)) | (Reg << (8 - (NO))))

// Circular left shift by NO in REG
#define CLSHFT_REG(Reg, NO) (Reg = (Reg << (NO)) | (Reg >> (8 - (NO))))

#endif // BIT_MATH_H_
