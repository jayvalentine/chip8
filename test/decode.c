#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit/munit.h"
#include "test/test.h"

#include "include/state.h"
#include "include/decode.h"

TEST(decode_jump)
{
    uint16_t opcode = 0x102a;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == JUMP);
    assert_uint16(42, ==, instr.NNN);

    return MUNIT_OK;
}

TEST(decode_clear)
{
    uint16_t opcode = 0x00e0;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == CLEAR);

    return MUNIT_OK;
}

TEST(decode_set_reg_imm)
{
    uint16_t opcode = 0x6499;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SET_REG_IMM);
    assert_uint8(0x4, ==, instr.X);
    assert_uint16(0x99, ==, instr.NN);

    return MUNIT_OK;
}

TEST(decode_add_reg_imm)
{
    uint16_t opcode = 0x7c22;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == ADD_REG_IMM);
    assert_uint8(0xc, ==, instr.X);
    assert_uint16(0x22, ==, instr.NN);

    return MUNIT_OK;
}

TEST(decode_set_index_imm)
{
    uint16_t opcode = 0xA123;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SET_INDEX_IMM);
    assert_uint16(0x123, ==, instr.NNN);

    return MUNIT_OK;
}

TEST(decode_skip_eq_imm)
{
    uint16_t opcode = 0x3D42;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SKIP_EQ_IMM);
    assert_uint8(0xd, ==, instr.X);
    assert_uint8(0x42, ==, instr.NN);

    return MUNIT_OK;
}

TEST(decode_skip_neq_imm)
{
    uint16_t opcode = 0x4C99;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SKIP_NEQ_IMM);
    assert_uint8(0xc, ==, instr.X);
    assert_uint8(0x99, ==, instr.NN);

    return MUNIT_OK;
}

TEST(decode_skip_eq_reg)
{
    uint16_t opcode = 0x5720;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SKIP_EQ_REG);
    assert_uint8(0x7, ==, instr.X);
    assert_uint8(0x2, ==, instr.Y);

    return MUNIT_OK;
}

TEST(decode_skip_neq_reg)
{
    uint16_t opcode = 0x9930;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SKIP_NEQ_REG);
    assert_uint8(0x9, ==, instr.X);
    assert_uint8(0x3, ==, instr.Y);

    return MUNIT_OK;
}

TEST(decode_draw)
{
    uint16_t opcode = 0xD5c9;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == DRAW);
    assert_uint8(0x5, ==, instr.X);
    assert_uint8(0xc, ==, instr.Y);
    assert_uint8(0x9, ==, instr.N);

    return MUNIT_OK;
}

TEST(decode_call)
{
    uint16_t opcode = 0x2998;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == CALL);
    assert_uint16(0x998, ==, instr.NNN);

    return MUNIT_OK;
}

TEST(decode_return)
{
    uint16_t opcode = 0x00EE;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == RETURN);

    return MUNIT_OK;
}

TEST(decode_set_reg)
{
    uint16_t opcode = 0x8750;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SET_REG_REG);
    assert_uint8(instr.X, ==, 0x7);
    assert_uint8(instr.Y, ==, 0x5);

    return MUNIT_OK;
}

TEST(decode_or)
{
    uint16_t opcode = 0x8921;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == OR_REG_REG);
    assert_uint8(instr.X, ==, 0x9);
    assert_uint8(instr.Y, ==, 0x2);

    return MUNIT_OK;
}

TEST(decode_and)
{
    uint16_t opcode = 0x8452;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == AND_REG_REG);
    assert_uint8(instr.X, ==, 0x4);
    assert_uint8(instr.Y, ==, 0x5);

    return MUNIT_OK;
}

TEST(decode_xor)
{
    uint16_t opcode = 0x8ad3;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == XOR_REG_REG);
    assert_uint8(instr.X, ==, 0xa);
    assert_uint8(instr.Y, ==, 0xd);

    return MUNIT_OK;
}

TEST(decode_add_reg_reg)
{
    uint16_t opcode = 0x8244;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == ADD_REG_REG);
    assert_uint8(instr.X, ==, 0x2);
    assert_uint8(instr.Y, ==, 0x4);

    return MUNIT_OK;
}

TEST(decode_sub_XY)
{
    uint16_t opcode = 0x8455;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SUB_XY);
    assert_uint8(instr.X, ==, 0x4);
    assert_uint8(instr.Y, ==, 0x5);

    return MUNIT_OK;
}

TEST(decode_sub_YX)
{
    uint16_t opcode = 0x8947;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SUB_YX);
    assert_uint8(instr.X, ==, 0x9);
    assert_uint8(instr.Y, ==, 0x4);

    return MUNIT_OK;
}

TEST(decode_shift_r)
{
    uint16_t opcode = 0x8126;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SHIFT_R);
    assert_uint8(instr.X, ==, 0x1);
    assert_uint8(instr.Y, ==, 0x2);

    return MUNIT_OK;
}

TEST(decode_shift_l)
{
    uint16_t opcode = 0x843e;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == SHIFT_L);
    assert_uint8(instr.X, ==, 0x4);
    assert_uint8(instr.Y, ==, 0x3);

    return MUNIT_OK;
}

TEST(decode_random)
{
    uint16_t opcode = 0xc466;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == RANDOM);
    assert_uint8(instr.X, ==, 0x4);
    assert_uint8(instr.NN, ==, 0x66);

    return MUNIT_OK;
}

TEST(decode_store)
{
    uint16_t opcode = 0xf755;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == STORE);
    assert_uint8(instr.X, ==, 0x7);

    return MUNIT_OK;
}

TEST(decode_load)
{
    uint16_t opcode = 0xfd65;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == LOAD);
    assert_uint8(instr.X, ==, 0xd);

    return MUNIT_OK;
}

TEST(decode_bcd_convert)
{
    uint16_t opcode = 0xf433;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == BCD_CONVERT);
    assert_uint8(instr.X, ==, 0x4);

    return MUNIT_OK;
}

TEST(add_index)
{
    uint16_t opcode = 0xf91e;
    Instruction instr;

    decode(&instr, opcode);

    assert(instr.opcode == ADD_INDEX);
    assert_uint8(instr.X, ==, 0x9);

    return MUNIT_OK;
}
