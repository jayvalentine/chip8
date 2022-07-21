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
