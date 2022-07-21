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
