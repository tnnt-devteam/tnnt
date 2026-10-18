#include <stdio.h>

#define DESC_SIZE 256

#define SIZE(x) (int) (sizeof (x) / sizeof (x[0]))

const char *escape_string(const char *s);

#define TNNT_ACH_DUMPYAML_INFO
struct ach_info {
    const char *name, *desc, *id;
} ach_list[] = {
#include "tnnt_achievements.h"
};
#undef TNNT_ACH_DUMPYAML_INFO

/* The category of each achievement, built the same way as
 * tnnt_ach_categories[] in the game (see include/tnnt.h and src/decl.c). */
#define TNNT_ACH_ENUM
enum tnnt_achievements {
#include "tnnt_achievements.h"
};
#undef TNNT_ACH_ENUM

#define TNNT_ACH_CATEGORY_INDICES_ENUM
enum tnnt_ach_category_indices {
#include "tnnt_achievements.h"
};
#undef TNNT_ACH_CATEGORY_INDICES_ENUM

#define TNNT_ACH_CATEGORY_NAMES
struct ach_category {
    int first_ach_id;
    const char *name;
} cat_list[] = {
#include "tnnt_achievements.h"
};
#undef TNNT_ACH_CATEGORY_NAMES

int
main(void)
{
    int i, cat = -1;
    for (i = 0; i < SIZE(ach_list); i++) {
        struct ach_info ach = ach_list[i];
        int lfnum = i / 64, lfbit = i % 64;

        /* an achievement belongs to the last category starting at or before
         * it; a category with no achievements is skipped, as in the game */
        while (cat + 1 < SIZE(cat_list) && cat_list[cat + 1].first_ach_id <= i)
            cat++;
        printf("- model: scoreboard.achievement\n  fields:\n");
        printf("    name: \"%s\"\n", escape_string(ach.name));
        printf("    description: \"%s\"\n", escape_string(ach.desc));
        printf("    ingameid: \"%03d\"\n", i + 1);
        printf("    define: \"A_%s\"\n", ach.id);
        printf("    xlogfield: \"tnntachieve%d\"\n    bit: %d\n", lfnum,
               lfbit);
        if (cat >= 0)
            printf("    category: \"%s\"\n",
                   escape_string(cat_list[cat].name));
    }

    return 0;
}

/* escape the string so it can be quoted in the YAML file.
 * ex: input:  "Tame seven \"f\"" [prints as: Tame seven "f"]
 *     output: "Tame seven \\\"f\\\"" [prints as: Tame seven \"f\"]
 */
const char *
escape_string(const char *s)
{
    static char result[DESC_SIZE];
    int i, j;

    /* copy 's' to 'result' a char at a time */
    for (i = j = 0; s[i] != '\0' && j < DESC_SIZE - 1; i++) {
        /* prepend a backslash '\' before any characters that should be
           escaped in the resulting string (just quotes for now) */
        if (s[i] == '"') {
            result[j++] = '\\';
        }
        result[j++] = s[i];
    }
    result[j] = '\0';
    return result;
}
