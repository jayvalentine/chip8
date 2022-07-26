
#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit/munit.h"
#include "test/test.h"

#include "include/state.h"
#include "include/execute.h"

#include "string.h"
#include "stdio.h"

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

    Instruction instr;
    instr.opcode = CLEAR;

    execute(&state, &instr);

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

    Instruction instr;
    instr.opcode = JUMP;
    instr.NNN = 123;

    execute(&state, &instr);

    assert_uint16(123, ==, state.pc);

    return MUNIT_OK;
}

TEST(jump_loop)
{
    State state;

    state.pc = 99;

    Instruction instr;
    instr.opcode = JUMP;
    instr.NNN = 99;

    execute(&state, &instr);

    assert_uint16(99, ==, state.pc);

    return MUNIT_OK;
}

TEST(set_reg_imm)
{
    State state;

    state.registers[0xf] = 12;
    state.registers[0x2] = 99;
    state.registers[0x0] = 123;

    Instruction instr1;
    instr1.opcode = SET_REG_IMM;
    instr1.X = 0xf;
    instr1.NN = 255;

    Instruction instr2;
    instr2.opcode = SET_REG_IMM;
    instr2.X = 0x2;
    instr2.NN = 42;

    Instruction instr3;
    instr3.opcode = SET_REG_IMM;
    instr3.X = 0x0;
    instr3.NN = 0;

    execute(&state, &instr1);
    execute(&state, &instr2);
    execute(&state, &instr3);

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

    Instruction instr1;
    instr1.opcode = ADD_REG_IMM;
    instr1.X = 0x8;
    instr1.NN = 12;

    Instruction instr2;
    instr2.opcode = ADD_REG_IMM;
    instr2.X = 0xd;
    instr2.NN = 1;

    Instruction instr3;
    instr3.opcode = ADD_REG_IMM;
    instr3.X = 0x1;
    instr3.NN = 4;

    execute(&state, &instr1);
    execute(&state, &instr2);
    execute(&state, &instr3);

    assert_uint8(24, ==, state.registers[0x8]);
    assert_uint8(100, ==, state.registers[0xd]);
    assert_uint8(127, ==, state.registers[0x1]);

    return MUNIT_OK;
}

TEST(set_index_imm)
{
    State state;

    state.i = 123;

    Instruction instr;
    instr.opcode = SET_INDEX_IMM;
    instr.NNN = 42;

    execute(&state, &instr);

    assert_uint16(42, ==, state.i);

    return MUNIT_OK;
}

TEST(draw_all_aligned)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0xa] = 8;
    state.registers[0xc] = 15;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 15; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 15; y < 18; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0b11111111, ==, state.display[y][1]);
    }

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 18; y < DISPLAY_HEIGHT; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            fprintf(stderr, "y: %d, x: %d", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_all_aligned_wrap_x)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0xa] = 72;
    state.registers[0xc] = 15;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 15; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 15; y < 18; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0b11111111, ==, state.display[y][1]);
    }

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 18; y < DISPLAY_HEIGHT; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            fprintf(stderr, "y: %d, x: %d", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_all_aligned_wrap_y)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0xa] = 8;
    state.registers[0xc] = 47;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 15; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 15; y < 18; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0b11111111, ==, state.display[y][1]);
    }

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 18; y < DISPLAY_HEIGHT; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            fprintf(stderr, "y: %d, x: %d", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_all_aligned_oob_y)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0xa] = 8;
    state.registers[0xc] = 30;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 30; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 30; y < DISPLAY_HEIGHT; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0b11111111, ==, state.display[y][1]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            fprintf(stderr, "y: %d, x: %d", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_all_unaligned)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0x2] = 10;
    state.registers[0x5] = 15;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0x2;
    instr.Y = 0x5;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 15; y++)
    {
        assert_uint8(0, ==, state.display[y][1]);
        assert_uint8(0, ==, state.display[y][2]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 15; y < 18; y++)
    {
        assert_uint8(0b00111111, ==, state.display[y][1]);
        assert_uint8(0b11000000, ==, state.display[y][2]);
    }

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 18; y < DISPLAY_HEIGHT; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
        assert_uint8(0, ==, state.display[y][2]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            if (x == 2) continue;

            fprintf(stderr, "y: %d, x: %d", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_all_unaligned_oob_x)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b11111111;
    state.memory[43] = 0b11111111;
    state.memory[44] = 0b11111111;

    state.i = 42;

    state.registers[0x2] = 60;
    state.registers[0x5] = 19;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0x2;
    instr.Y = 0x5;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 19; y++)
    {
        assert_uint8(0, ==, state.display[y][7]);
    }

    /* Check that the sprite bits have been set. */
    for (int y = 19; y < 22; y++)
    {
        assert_uint8(0b00001111, ==, state.display[y][7]);
    }

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 22; y < DISPLAY_HEIGHT; y++)
    {
        assert_uint8(0, ==, state.display[y][7]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 7) continue;
            
            fprintf(stderr, "y: %d, x: %d\n", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_checkerboard)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b10101010;
    state.memory[43] = 0b01010101;
    state.memory[44] = 0b10101010;

    state.i = 42;

    state.registers[0xa] = 8;
    state.registers[0xc] = 15;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that the bits above the sprite are unaffected. */
    for (int y = 0; y < 15; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that the sprite bits have been set. */
    assert_uint8(0b10101010, ==, state.display[15][1]);
    assert_uint8(0b01010101, ==, state.display[16][1]);
    assert_uint8(0b10101010, ==, state.display[17][1]);

    /* Check that the bits below the sprite are unaffected. */
    for (int y = 18; y < DISPLAY_HEIGHT; y++)
    {
        fprintf(stderr, "y: %d, x: %d\n", y, 1);
        assert_uint8(0, ==, state.display[y][1]);
    }

    /* Check that other columns are unaffected. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            if (x == 1) continue;
            fprintf(stderr, "y: %d, x: %d", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has not been set. */
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_checkerboard_switch_off)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b10101010;
    state.memory[43] = 0b01010101;
    state.memory[44] = 0b10101010;

    state.display[15][1] = 0b10101010;
    state.display[16][1] = 0b01010101;
    state.display[17][1] = 0b10101010;

    state.i = 42;

    state.registers[0xa] = 8;
    state.registers[0xc] = 15;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that display is clear. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            fprintf(stderr, "y: %d, x: %d\n", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has been set. */
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(draw_checkerboard_switch_off_unaligned)
{
    State state;
    state.registers[0xf] = 0;

    memset(state.display, 0, sizeof(state.display));

    state.memory[42] = 0b10101010;
    state.memory[43] = 0b01010101;
    state.memory[44] = 0b10101010;

    state.display[15][1] = 0b00101010;
    state.display[15][2] = 0b10000000;
    state.display[16][1] = 0b00010101;
    state.display[16][2] = 0b01000000;
    state.display[17][1] = 0b00101010;
    state.display[17][2] = 0b10000000;

    state.i = 42;

    state.registers[0xa] = 10;
    state.registers[0xc] = 15;

    Instruction instr;
    instr.opcode = DRAW;
    instr.X = 0xa;
    instr.Y = 0xc;
    instr.N = 3;

    execute(&state, &instr);

    /* Check that display is clear. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); x++)
        {
            fprintf(stderr, "y: %d, x: %d\n", y, x);
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that VF flag has been set. */
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(skip_eq_reg_true)
{
    State state;
    
    state.registers[0xa] = 42;
    state.registers[0x2] = 42;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_EQ_REG;
    instr.X = 0xa;
    instr.Y = 0x2;

    execute(&state, &instr);

    assert_true(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_eq_reg_false)
{
    State state;
    
    state.registers[0xd] = 42;
    state.registers[0x3] = 12;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_EQ_REG;
    instr.X = 0xd;
    instr.Y = 0x3;

    execute(&state, &instr);

    assert_false(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_eq_imm_true)
{
    State state;
    
    state.registers[0xa] = 42;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_EQ_IMM;
    instr.X = 0xa;
    instr.NN = 42;

    execute(&state, &instr);

    assert_true(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_eq_imm_false)
{
    State state;
    
    state.registers[0xd] = 42;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_EQ_IMM;
    instr.X = 0xa;
    instr.NN = 99;

    execute(&state, &instr);

    assert_false(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_neq_reg_true)
{
    State state;
    
    state.registers[0xa] = 42;
    state.registers[0x2] = 99;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_NEQ_REG;
    instr.X = 0xa;
    instr.Y = 0x2;

    execute(&state, &instr);

    assert_true(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_neq_reg_false)
{
    State state;
    
    state.registers[0xd] = 42;
    state.registers[0x3] = 42;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_NEQ_REG;
    instr.X = 0xd;
    instr.Y = 0x3;

    execute(&state, &instr);

    assert_false(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_neq_imm_true)
{
    State state;
    
    state.registers[0xa] = 42;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_NEQ_IMM;
    instr.X = 0xa;
    instr.NN = 99;

    execute(&state, &instr);

    assert_true(state.skip_next);

    return MUNIT_OK;
}

TEST(skip_neq_imm_false)
{
    State state;
    
    state.registers[0xd] = 42;
    state.skip_next = false;

    Instruction instr;
    instr.opcode = SKIP_NEQ_IMM;
    instr.X = 0xd;
    instr.NN = 42;

    execute(&state, &instr);

    assert_false(state.skip_next);

    return MUNIT_OK;
}

TEST(subroutine_call)
{
    State state;
    
    state.pc = 42;
    state.stack.size = 0;

    Instruction instr;
    instr.opcode = CALL;
    instr.NNN = 123;

    execute(&state, &instr);
    
    assert_uint8(1, ==, state.stack.size);
    assert_uint16(42, ==, state.stack.values[0]);
    assert_uint16(123, ==, state.pc);

    return MUNIT_OK;
}

TEST(subroutine_return)
{
    State state;
    
    state.pc = 99;
    state.stack.size = 1;
    state.stack.values[0] = 244;

    Instruction instr;
    instr.opcode = RETURN;

    execute(&state, &instr);
    
    assert_uint8(0, ==, state.stack.size);
    assert_uint16(244, ==, state.pc);

    return MUNIT_OK;
}

TEST(set_reg_reg)
{
    State state;

    state.registers[0xf] = 12;
    state.registers[0x9] = 23;

    state.registers[0x2] = 99;
    state.registers[0x3] = 44;

    state.registers[0x0] = 123;

    Instruction instr1;
    instr1.opcode = SET_REG_REG;
    instr1.X = 0xf;
    instr1.Y = 0x9;

    Instruction instr2;
    instr2.opcode = SET_REG_REG;
    instr2.X = 0x2;
    instr2.Y = 0x3;

    Instruction instr3;
    instr3.opcode = SET_REG_REG;
    instr3.X = 0x0;
    instr3.Y = 0x0;

    execute(&state, &instr1);
    execute(&state, &instr2);
    execute(&state, &instr3);

    assert_uint8(23, ==, state.registers[0xf]);
    assert_uint8(23, ==, state.registers[0x9]);

    assert_uint8(44, ==, state.registers[0x2]);
    assert_uint8(44, ==, state.registers[0x3]);

    assert_uint8(123, ==, state.registers[0x0]);

    return MUNIT_OK;
}

TEST(or_reg_reg)
{
    State state;

    state.registers[0xf] = 0b10101010;
    state.registers[0x9] = 0b01010101;

    state.registers[0x2] = 0b11110000;
    state.registers[0x3] = 0b11111000;

    state.registers[0x0] = 0b11101001;

    Instruction instr1;
    instr1.opcode = OR_REG_REG;
    instr1.X = 0xf;
    instr1.Y = 0x9;

    Instruction instr2;
    instr2.opcode = OR_REG_REG;
    instr2.X = 0x2;
    instr2.Y = 0x3;

    Instruction instr3;
    instr3.opcode = OR_REG_REG;
    instr3.X = 0x0;
    instr3.Y = 0x0;

    execute(&state, &instr1);
    execute(&state, &instr2);
    execute(&state, &instr3);

    assert_uint8(0b11111111, ==, state.registers[0xf]);
    assert_uint8(0b01010101, ==, state.registers[0x9]);

    assert_uint8(0b11111000, ==, state.registers[0x2]);
    assert_uint8(0b11111000, ==, state.registers[0x3]);

    assert_uint8(0b11101001, ==, state.registers[0x0]);

    return MUNIT_OK;
}

TEST(and_reg_reg)
{
    State state;

    state.registers[0xf] = 0b10101010;
    state.registers[0x9] = 0b01010101;

    state.registers[0x2] = 0b11110000;
    state.registers[0x3] = 0b11111000;

    state.registers[0x0] = 0b11101001;

    Instruction instr1;
    instr1.opcode = AND_REG_REG;
    instr1.X = 0xf;
    instr1.Y = 0x9;

    Instruction instr2;
    instr2.opcode = AND_REG_REG;
    instr2.X = 0x2;
    instr2.Y = 0x3;

    Instruction instr3;
    instr3.opcode = AND_REG_REG;
    instr3.X = 0x0;
    instr3.Y = 0x0;

    execute(&state, &instr1);
    execute(&state, &instr2);
    execute(&state, &instr3);

    assert_uint8(0b00000000, ==, state.registers[0xf]);
    assert_uint8(0b01010101, ==, state.registers[0x9]);

    assert_uint8(0b11110000, ==, state.registers[0x2]);
    assert_uint8(0b11111000, ==, state.registers[0x3]);

    assert_uint8(0b11101001, ==, state.registers[0x0]);

    return MUNIT_OK;
}

TEST(xor_reg_reg)
{
    State state;

    state.registers[0xf] = 0b10101010;
    state.registers[0x9] = 0b01010101;

    state.registers[0x2] = 0b11110000;
    state.registers[0x3] = 0b11111000;

    state.registers[0x0] = 0b11101001;

    Instruction instr1;
    instr1.opcode = XOR_REG_REG;
    instr1.X = 0xf;
    instr1.Y = 0x9;

    Instruction instr2;
    instr2.opcode = XOR_REG_REG;
    instr2.X = 0x2;
    instr2.Y = 0x3;

    Instruction instr3;
    instr3.opcode = XOR_REG_REG;
    instr3.X = 0x0;
    instr3.Y = 0x0;

    execute(&state, &instr1);
    execute(&state, &instr2);
    execute(&state, &instr3);

    assert_uint8(0b11111111, ==, state.registers[0xf]);
    assert_uint8(0b01010101, ==, state.registers[0x9]);

    assert_uint8(0b00001000, ==, state.registers[0x2]);
    assert_uint8(0b11111000, ==, state.registers[0x3]);

    assert_uint8(0b00000000, ==, state.registers[0x0]);

    return MUNIT_OK;
}

TEST(add_reg_reg_no_carry)
{
    State state;

    state.registers[0x8] = 10;
    state.registers[0x3] = 32;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = ADD_REG_REG;
    instr.X = 0x8;
    instr.Y = 0x3;

    execute(&state, &instr);

    assert_uint8(42, ==, state.registers[0x8]);
    assert_uint8(32, ==, state.registers[0x3]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(add_reg_reg_carry)
{
    State state;

    state.registers[0x2] = 200;
    state.registers[0x9] = 100;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = ADD_REG_REG;
    instr.X = 0x2;
    instr.Y = 0x9;

    execute(&state, &instr);

    assert_uint8(44, ==, state.registers[0x2]);
    assert_uint8(100, ==, state.registers[0x9]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(add_reg_reg_carry_add_1)
{
    State state;

    state.registers[0x1] = 255;
    state.registers[0x3] = 1;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = ADD_REG_REG;
    instr.X = 0x1;
    instr.Y = 0x3;

    execute(&state, &instr);

    assert_uint8(0, ==, state.registers[0x1]);
    assert_uint8(1, ==, state.registers[0x3]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(add_reg_reg_carry_max)
{
    State state;

    state.registers[0x8] = 255;
    state.registers[0x6] = 255;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = ADD_REG_REG;
    instr.X = 0x8;
    instr.Y = 0x6;

    execute(&state, &instr);

    assert_uint8(254, ==, state.registers[0x8]);
    assert_uint8(255, ==, state.registers[0x6]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_xy)
{
    State state;

    state.registers[0x8] = 42;
    state.registers[0x3] = 32;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_XY;
    instr.X = 0x8;
    instr.Y = 0x3;

    execute(&state, &instr);

    assert_uint8(10, ==, state.registers[0x8]);
    assert_uint8(32, ==, state.registers[0x3]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_xy_lower_limit)
{
    State state;

    state.registers[0x6] = 42;
    state.registers[0x4] = 42;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_XY;
    instr.X = 0x6;
    instr.Y = 0x4;

    execute(&state, &instr);

    assert_uint8(0, ==, state.registers[0x6]);
    assert_uint8(42, ==, state.registers[0x4]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_xy_borrow_one)
{
    State state;

    state.registers[0x9] = 42;
    state.registers[0xd] = 43;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_XY;
    instr.X = 0x9;
    instr.Y = 0xd;

    execute(&state, &instr);

    assert_uint8(255, ==, state.registers[0x9]);
    assert_uint8(43, ==, state.registers[0xd]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_xy_borrow_from_zero)
{
    State state;

    state.registers[0xa] = 0;
    state.registers[0xc] = 255;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_XY;
    instr.X = 0xa;
    instr.Y = 0xc;

    execute(&state, &instr);

    assert_uint8(1, ==, state.registers[0xa]);
    assert_uint8(255, ==, state.registers[0xc]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_yx)
{
    State state;

    state.registers[0x8] = 42;
    state.registers[0x3] = 32;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_YX;
    instr.X = 0x3;
    instr.Y = 0x8;

    execute(&state, &instr);

    assert_uint8(10, ==, state.registers[0x8]);
    assert_uint8(32, ==, state.registers[0x3]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_yx_lower_limit)
{
    State state;

    state.registers[0x6] = 42;
    state.registers[0x4] = 42;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_YX;
    instr.X = 0x4;
    instr.Y = 0x6;

    execute(&state, &instr);

    assert_uint8(0, ==, state.registers[0x6]);
    assert_uint8(42, ==, state.registers[0x4]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_yx_borrow_one)
{
    State state;

    state.registers[0x9] = 42;
    state.registers[0xd] = 43;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_YX;
    instr.X = 0xd;
    instr.Y = 0x9;

    execute(&state, &instr);

    assert_uint8(255, ==, state.registers[0x9]);
    assert_uint8(43, ==, state.registers[0xd]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(sub_yx_borrow_from_zero)
{
    State state;

    state.registers[0xa] = 0;
    state.registers[0xc] = 255;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SUB_YX;
    instr.X = 0xc;
    instr.Y = 0xa;

    execute(&state, &instr);

    assert_uint8(1, ==, state.registers[0xa]);
    assert_uint8(255, ==, state.registers[0xc]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(shift_r)
{
    State state;

    state.registers[0xc] = 0b10101100;
    state.registers[0xa] = 0;
    state.registers[0xf] = 1;

    Instruction instr;
    instr.opcode = SHIFT_R;
    instr.X = 0xc;
    instr.Y = 0xa;

    execute(&state, &instr);

    assert_uint8(0b01010110, ==, state.registers[0xc]);
    assert_uint8(0, ==, state.registers[0xa]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(shift_r_carry)
{
    State state;

    state.registers[0x3] = 0b10100101;
    state.registers[0xa] = 0;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SHIFT_R;
    instr.X = 0x3;
    instr.Y = 0xa;

    execute(&state, &instr);

    assert_uint8(0b01010010, ==, state.registers[0x3]);
    assert_uint8(0, ==, state.registers[0xa]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(shift_l)
{
    State state;

    state.registers[0xc] = 0b01010011;
    state.registers[0xa] = 0;
    state.registers[0xf] = 1;

    Instruction instr;
    instr.opcode = SHIFT_L;
    instr.X = 0xc;
    instr.Y = 0xa;

    execute(&state, &instr);

    assert_uint8(0b10100110, ==, state.registers[0xc]);
    assert_uint8(0, ==, state.registers[0xa]);
    assert_uint8(0, ==, state.registers[0xf]);

    return MUNIT_OK;
}

TEST(shift_l_carry)
{
    State state;

    state.registers[0x3] = 0b10100101;
    state.registers[0xa] = 0;
    state.registers[0xf] = 0;

    Instruction instr;
    instr.opcode = SHIFT_L;
    instr.X = 0x3;
    instr.Y = 0xa;

    execute(&state, &instr);

    assert_uint8(0b01001010, ==, state.registers[0x3]);
    assert_uint8(0, ==, state.registers[0xa]);
    assert_uint8(1, ==, state.registers[0xf]);

    return MUNIT_OK;
}
