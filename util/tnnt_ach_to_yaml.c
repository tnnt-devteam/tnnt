#include <stdio.h>

#define SIZE(x) (int) (sizeof (x) / sizeof (x[0]))

#define ACH(nam, desc, id) { #nam, #desc }
struct ach_info {
const char *name, *desc;
} ach_list[] = {
#include "tnnt_achievements.h"
};
#undef ACH

int
main(void)
{
    int i;
    for (i = 0; i < SIZE(ach_list); i++) {
        struct ach_info ach = ach_list[i];
        int lfnum = i / 64, lfbit = i % 64;
        printf("- model: scoreboard.achievement\n  fields:\n");
        printf("    name: %s\n    description: %s\n", ach.name, ach.desc);
        printf("    xlogfield: \"tnntachieve%d\"\n    bit: %d\n", lfnum,
               lfbit);
    }

    return 0;
}
