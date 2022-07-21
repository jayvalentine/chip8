
#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit/munit.h"
#include "test/test.h"

#include "include/state.h"
#include "include/execute.h"

#include "string.h"

/* Tests that executing the clear_screen instruction results in the display memory being cleared,
 * and the "display changed" flag being set.
 */
TEST(clear_screen)
{
    State state;

    memset(state.display, 0, sizeof(state.display));
    state.display_changed = false;

    state.display[4][2] = 160;
    state.display[3][1] = 42;
    state.display[31][7] = 99;

    exec_clear_screen(&state);

    /* Check that all bits in the display are now off. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that the display_changed flag has been set
     * now that the display has changed.
     */
    assert_true(state.display_changed);

    return MUNIT_OK;
}

TEST(jump)
{
    State state;

    state.pc = 42;

    exec_jump(&state, 123);

    assert_uint16(123, ==, state.pc);

    return MUNIT_OK;
}

TEST(jump_loop)
{
    State state;

    state.pc = 99;

    exec_jump(&state, 99);

    assert_uint16(99, ==, state.pc);

    return MUNIT_OK;
}

TEST(set_reg_imm)
{
    State state;

    state.registers[0xf] = 12;
    state.registers[0x2] = 99;
    state.registers[0x0] = 123;

    exec_set_reg_imm(&state, 0xf, 255);
    exec_set_reg_imm(&state, 0x2, 42);
    exec_set_reg_imm(&state, 0x0, 0);

    assert_uint8(255, ==, state.registers[0xf]);
    assert_uint8(42, ==, state.registers[0x2]);
    assert_uint8(0, ==, state.registers[0x0]);

    return MUNIT_OK;
}

TEST(add_reg_imm)
{
    State state;

    state.registers[0x8] = 12;
    state.registers[0xd] = 99;
    state.registers[0x1] = 123;

    exec_add_reg_imm(&state, 0x8, 12);
    exec_add_reg_imm(&state, 0xd, 1);
    exec_add_reg_imm(&state, 0x1, 4);

    assert_uint8(24, ==, state.registers[0x8]);
    assert_uint8(100, ==, state.registers[0xd]);
    assert_uint8(127, ==, state.registers[0x1]);

    return MUNIT_OK;
}

TEST(set_index_imm)
{
    State state;

    state.i = 123;

    exec_set_index_imm(&state, 42);

    assert_uint16(42, ==, state.i);

    return MUNIT_OK;
}

TEST(draw_all_aligned)
{
    State state;

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0xa] = 8;
    state.registers[0xc] = 15;

    exec_draw(&state, 0xa, 0xc, 3);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 15; y++)
    {
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 15; y < 18; y++)
    {
        assert_uint8(0b11111111, ==, state.display[y][1]);
    }

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 18; y < DISPLAY_HEIGHT; y++)
    {
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    return MUNIT_OK;
}
