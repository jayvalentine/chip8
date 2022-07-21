
#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit/munit.h"

#include "include/state.h"
#include "include/execute.h"

#include "string.h"

/* Tests that executing the clear_screen instruction results in the display memory being cleared,
 * and the "display changed" flag being set.
 */
MunitResult test_clear_screen(const MunitParameter params[], void* user_data_or_fixture)
{
    State state;

    memset(state.display, 0, sizeof(state.display));
    state.display_changed = false;

    exec_clear_screen(&state);

    /* Check that all bits in the display are now off. */
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < (DISPLAY_WIDTH / 8); y++)
        {
            assert_uint8(0, ==, state.display[y][x]);
        }
    }

    /* Check that the display_changed flag has been set
     * now that the display has changed.
     */
    assert_true(state.display_changed);
}