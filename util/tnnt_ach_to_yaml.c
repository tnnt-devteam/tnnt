#include <stdio.h>

#define DESC_SIZE 256

#define SIZE(x) (int) (sizeof (x) / sizeof (x[0]))

const char *escape_string(const char *s);

#define ACH(nam, desc, id) { nam, desc, #id }
struct ach_info {
    const char *name, *desc, *id;
} ach_list[] = {
#include "tnnt_achievements.h"
};

int
main(void)
{
    int i;
    for (i = 0; i < SIZE(ach_list); i++) {
        struct ach_info ach = ach_list[i];
        int lfnum = i / 64, lfbit = i % 64;
        printf("- model: scoreboard.achievement\n  fields:\n");
        printf("    name: \"%s\"\n", escape_string(ach.name));
        printf("    description: \"%s\"\n", escape_string(ach.desc));
        printf("    ingameid: \"%03d\"\n", i + 1);
        printf("    define: \"A_%s\"\n", ach.id);
        printf("    xlogfield: \"tnntachieve%d\"\n    bit: %d\n", lfnum,
               lfbit);
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
