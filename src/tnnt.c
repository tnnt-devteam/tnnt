/* This file holds functions that were introduced to provide TNNT functionality.
 * These functions used to be scattered around in the files that best fit them.
 * However, they are now placed here in anticipation of the eventual NetHack 3.7
 * merge introducing a lot of function splits, moves, and additions of macros
 * between functions. It will let us avoid a ton of merge conflicts.
 *
 * TNNT functions that do NOT live in this file:
 * 1) topten related functions that rely on struct toptenentry and other things
 *    defined only in topten.c
 * 2) robotfindskitten related functions that rely on static helpers defined
 *    only in rumors.c
 */

/* TNNT TODO FOR 3.7: Ensure function signature style is consistent with the new
 * style used in 3.7, rather than old K&R style. */

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "hack.h"
#include "dlb.h"
#include "integer.h"
#include "qtext.h"

/* this must be kept the same as in topten.c */
#define NAMSZ PL_NSIZ

/* static forward declarations */
static boolean tnnt_common_monst(int mndx);
#ifdef TNNT_SWAPCHEST_DIR
static char *make_swapobj_filename(struct obj *o);
static struct obj * mkswapobj(struct obj *swapchest,
                              char *filename, short *rcode);
#endif
#ifdef TNNT_NPC_DIR
static char * pick_npc_file(void);
static void collect_transient_within(struct obj *container,
                                     struct obj **olist);
#endif

/* ######################################################################
 * TNNT COMMANDS
 * ###################################################################### */

static winid en_win = WIN_ERR;

/* #tnntdebug command */
int
dotnntdebug(VOID_ARGS)
{
    char buf[BUFSZ];
    char buf2[BUFSZ];
    menu_item *choice = NULL;
    char response;
    anything any;
#ifdef SYSCF
    if (!wizard
        && (!sysopt.tnnt_devs || !sysopt.tnnt_devs[0]
            || !check_user_string(sysopt.tnnt_devs))) {
        Norep("'#tnntdebug' command not available.");
        return 0;
    }
#endif
    any.a_char = 's';
    en_win = create_nhwindow(NHW_MENU);
    start_menu(en_win);
    add_menu(en_win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
             "show debug stats", MENU_UNSELECTED);
    any.a_char = 'w';
    add_menu(en_win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
             "write out a test npcdeath file", MENU_UNSELECTED);
    any.a_char = 'c';
    add_menu(en_win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
             "create an NPC deathmatch monster", MENU_UNSELECTED);
    end_menu(en_win, "What would you like to do?");
    if (select_menu(en_win, PICK_ONE, &choice) > 0) {
        response = choice->item.a_char;
        free((genericptr_t) choice);
    }
    else {
        destroy_nhwindow(en_win);
        return 0;
    }
    destroy_nhwindow(en_win);
    en_win = WIN_ERR;

    if (response == 's') {
        int i;
        en_win = create_nhwindow(NHW_MENU);
        /* tnnt achievements */
        putstr(en_win, ATR_BOLD, "TNNT achievements (in hexadecimal):");
        for (i = 0; i < SIZE(tnnt_globals.tnnt_achievements); ++i) {
            Sprintf(buf, "tnntachieve%d: 0x%" PRIx64, i,
                    tnnt_globals.tnnt_achievements[i]);
            putstr(en_win, 0, buf);
        }
        putstr(en_win, 0, "");

        /* devteam quest info */
        putstr(en_win, ATR_BOLD, "Devteam Quest:");
        Strcpy(buf, "Scroll levels:");
        for (i = 0; i < NUM_MISSING_CODE_SCROLLS; ++i) {
            if (tnnt_globals.missing_scroll_levels[i] == 0) {
                Strcat(buf, " [DONE]");
            }
            else {
                Sprintf(buf2, " %d", tnnt_globals.missing_scroll_levels[i]);
                Strcat(buf, buf2);
            }
        }
        putstr(en_win, 0, buf);
        switch(tnnt_globals.devteam_quest_status) {
            case DTQUEST_NOTSTARTED:
                putstr(en_win, 0, "Quest not started yet.");
                break;
            case DTQUEST_INPROGRESS:
                putstr(en_win, 0, "Quest in progress.");
                break;
            case DTQUEST_COMPLETED:
                putstr(en_win, 0, "Quest completed.");
                break;
        }

        display_nhwindow(en_win, TRUE);
        destroy_nhwindow(en_win);
        en_win = WIN_ERR;
    }
#ifdef TNNT_NPC_DIR
    else if (response == 'w') {
        write_npc_data();
        pline("Finished writing your data as an NPC.");
    }
    else if (response == 'c') {
        pline("Summoning the NPC deathmatch monster.");
        create_tnnt_npc(u.ux, u.uy);
    }
#else
    else {
        pline("NPC arena monsters not supported in this build.");
    }
#endif
    return 0;
}

/* #tnntstats command.
 * Non-spoilery non-info-leaky stuff that a regular tournament player can check
 * during play.
 * Non-spoilery in the sense that the user is presumed to know what all the
 * achievements are and what they require. It doesn't alert an unaware user that
 * there's a devteam quest. */
int
dotnntstats(VOID_ARGS)
{
    return show_tnnt_stats(FALSE);
}

int
show_tnnt_stats(final)
boolean final;
{
#define maybe_have(final) (!final ? "have " : "")
    char buf[BUFSZ];
    int i, count, total;
    char eaten[16] = DUMMY;
    char uneaten[16] = DUMMY;
    en_win = create_nhwindow(NHW_MENU);

    /* mention overall #achievements display */
    if (final) {
        putstr(en_win, ATR_HEADING, "TNNT statistics:");
    }
    else {
        putstr(en_win, 0, "To view complete or incomplete achievements, use #achievements.");
    }

    /* devteam quest progress */
    if (tnnt_globals.devteam_quest_status != DTQUEST_NOTSTARTED) {
        int found_scrolls = 0;
        for (i = 0; i < NUM_MISSING_CODE_SCROLLS; ++i) {
            if (tnnt_globals.missing_scroll_levels[i] == 0) {
                found_scrolls++;
            }
        }
        if (found_scrolls == NUM_MISSING_CODE_SCROLLS) {
            putstr(en_win, 0, "Devteam Quest: Completed!");
        }
        else {
            Sprintf(buf, "Devteam Quest: %d/%d scrolls returned",
                    found_scrolls, NUM_MISSING_CODE_SCROLLS);
            putstr(en_win, 0, buf);
        }
    }

    /* npc deathmatch status */
    if (tnnt_globals.deathmatch_started) {
        if (tnnt_globals.deathmatch_completed)
            putstr(en_win, 0, "Deathmatch: Completed!");
        else if (Is_deathmatch_level(&u.uz))
            putstr(en_win, 0, "Deathmatch: Happening Now");
        else /* fled the deathmatch */
            putstr(en_win, 0, "Deathmatch: Bravely Ran Away");
    }

    /* other counters for various things */
    /* these _could_ be made to respect final (e.g. leave out the "have"), but I
     * rather doubt anyone cares about the grammar that much */
    Sprintf(buf, "You %sfound %d graffit%c.", maybe_have(final),
            tnnt_globals.graffiti_found,
            tnnt_globals.graffiti_found == 1 ? 'o' : 'i');
    putstr(en_win, 0, buf);

    Sprintf(buf, "You %stamed %d%s feline%s.",
            maybe_have(final), tnnt_globals.felines_tamed,
            (tnnt_globals.felines_tamed >= MAX_TAMED_FELINES ? "+" : ""),
            plur(tnnt_globals.felines_tamed));
    putstr(en_win, 0, buf);

    Sprintf(buf, "You %schanged gender %d time%s.", maybe_have(final),
            tnnt_globals.genderflips, plur(tnnt_globals.genderflips));
    putstr(en_win, 0, buf);

    Sprintf(buf, "You %s %d artifact%s.", !final ? "have eaten" : "ate",
            tnnt_globals.artis_eaten, plur(tnnt_globals.artis_eaten));
    putstr(en_win, 0, buf);

    if (final || u.uachieve.amulet) {
        Sprintf(buf, "You %s sent back %d level%s by the mysterious force.",
                !final ? "have been" : "were",
                tnnt_globals.mysterious_forced_back,
                plur(tnnt_globals.mysterious_forced_back));
        putstr(en_win, 0, buf);
    }

    Sprintf(buf, "You %shad a door resist %d consecutive time%s.",
            maybe_have(final), tnnt_globals.door_resist_max,
            plur(tnnt_globals.door_resist_max));
    if (tnnt_globals.consecutive_door_resists && !final)
        Sprintf(eos(buf), " (current streak: %d)",
                tnnt_globals.consecutive_door_resists);
    putstr(en_win, 0, buf);

    for (i = 0; i < TNNT_LITROOM_GOAL; ++i) {
        if (tnnt_globals.dark_rooms_lit[i].ledgerno == 0)
            break;
    }
    Sprintf(buf, "You %slit up %d dark room%s in Dungeons levels.",
            maybe_have(final), i, plur(i));
    putstr(en_win, 0, buf);

    Sprintf(buf, "You %sgained %d max Pw from eating corpses or tins.",
            maybe_have(final), tnnt_globals.maxpw_from_corpses);
    putstr(en_win, 0, buf);

#define print_visited(altars)    \
    if ((altars) == 0)           \
        Strcat(buf, " none");    \
    if ((altars) & AM_LAWFUL)    \
        Strcat(buf, " lawful");  \
    if ((altars) & AM_NEUTRAL)   \
        Strcat(buf, " neutral"); \
    if ((altars) & AM_CHAOTIC)   \
        Strcat(buf, " chaotic");
    /* Initially I had this check for tnnt_globals.high_altars != 0, but it's
     * probably better to show this a little earlier to avoid someone forgetting
     * to step on Moloch's high altar and not realizing it until it's too late.
     * Starting to show it around the start of the ascension run is fine. */
    if (final || u.uevent.udemigod) {
        Strcpy(buf, "High altar alignments visited:");
        print_visited(tnnt_globals.high_altars);
        putstr(en_win, 0, buf);
    }

    Strcpy(buf, "Regular altar alignments visited:");
    print_visited(tnnt_globals.regular_altars);
    putstr(en_win, 0, buf);
#undef print_visited

    Sprintf(buf, "You %sreceived wishes from:", maybe_have(final));
    if (tnnt_globals.wish_sources == 0) {
        Strcat(buf, " nothing");
    }
    else {
        if ((tnnt_globals.wish_sources & WISHSRC_WAND) != 0)
            Strcat(buf, " wand");
        if ((tnnt_globals.wish_sources & WISHSRC_LAMP) != 0)
            Strcat(buf, " lamp");
        if ((tnnt_globals.wish_sources & WISHSRC_SMOKY_POTION) != 0)
            Strcat(buf, " bottle");
        if ((tnnt_globals.wish_sources & WISHSRC_THRONE) != 0)
            Strcat(buf, " throne");
        if ((tnnt_globals.wish_sources & WISHSRC_WATER_DEMON) != 0)
            Strcat(buf, " fountain");
    }
    putstr(en_win, 0, buf);

    Sprintf(buf, "Things you %sread:", maybe_have(final));
    if (tnnt_globals.readables == 0)
        Strcat(buf, " nothing");
    if ((tnnt_globals.readables & RDBL_SCROLL) != 0)
        Strcat(buf, " scroll");
    if ((tnnt_globals.readables & RDBL_BOOK) != 0)
        Strcat(buf, " book");
    if ((tnnt_globals.readables & RDBL_COIN) != 0)
        Strcat(buf, " coin");
    if ((tnnt_globals.readables & RDBL_SHIRT) != 0)
        Strcat(buf, " shirt");
    if ((tnnt_globals.readables & RDBL_APRON) != 0)
        Strcat(buf, " apron");
    if ((tnnt_globals.readables & RDBL_MARKER) != 0)
        Strcat(buf, " marker");
    if ((tnnt_globals.readables & RDBL_CANDY) != 0)
        Strcat(buf, " candy");
    if ((tnnt_globals.readables & RDBL_CARD) != 0)
        Strcat(buf, " card");
    if ((tnnt_globals.readables & RDBL_COOKIE) != 0)
        Strcat(buf, " cookie");
    putstr(en_win, 0, buf);

    Strcpy(buf, "Traps #untrapped:");
    if (tnnt_globals.untrapped_types == 0)
        Strcat(buf, " none");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_ARROW)
        Strcat(buf, " arrow");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_DART)
        Strcat(buf, " dart");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_BOARD)
        Strcat(buf, " board");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_BEARTRAP)
        Strcat(buf, " bear");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_LANDMINE)
        Strcat(buf, " landmine");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_WEB)
        Strcat(buf, " web");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_DOOR)
        Strcat(buf, " door");
    if (tnnt_globals.untrapped_types & TNNT_UNTRAP_CONT)
        Strcat(buf, " box");
    putstr(en_win, 0, buf);

    Sprintf(buf, "%sarmful potions drunk:", !final ? "Known h" : "H");
    {
        /* we can't check '!(pots_drunk & HARMFUL_DRUNK)' to print "none"
         * right away (some bits set may be from unIDed pots that shouldn't be
         * revealed) so need to count them individually with 'known' */
        int known = 0;
        for (i = bases[POTION_CLASS]; objects[i].oc_class == POTION_CLASS;
             i++) {
            if (!tnnt_pot_is_harmful(i))
                continue;
            if ((tnnt_globals.pots_drunk & (1UL << (i - bases[POTION_CLASS])))
                && (final || objects[i].oc_name_known)) {
                Strcat(buf, " ");
                /* chop the potion name at 5 characters to avoid overly long
                 * line */
                (void) strncat(buf, OBJ_NAME(objects[i]), 5);
                known++;
            }
        }
        if (known == 0)
            Strcat(buf, " none");
    }
    putstr(en_win, 0, buf);

    Strcpy(buf, "Polearms collected:");
    if (!tnnt_globals.polearms_found) {
        Strcat(buf, " none");
    } else {
        for (i = FIRST_POLEARM; i <= LAST_POLEARM; i++) {
            if (objects[i].oc_skill != P_POLEARMS)
                continue;
            if (tnnt_globals.polearms_found & (1UL << (i - FIRST_POLEARM))) {
                Strcat(buf, " ");
                /* 3 characters should be sufficient for polearms */
                (void) strncat(buf, OBJ_NAME(objects[i]), 3);
            }
        }
    }
    putstr(en_win, 0, buf);

    if (final || tnnt_is_achieved(A_ENTERED_LUDIOS)) {
        Sprintf(buf, "You %s%s%s up a land mine in Fort Ludios.",
                maybe_have(final),
                tnnt_globals.blew_up_ludios ? "" : (!final ? "not " : "never "),
                !final ? "blown" : "blew");
        putstr(en_win, 0, buf);
    }

    /* kill a certain number of foo */
    for (i = 0; i < 9; ++i) {
        if (tnnt_globals.nazgul_ring_o_ids[i] == 0)
            break;
    }
    Sprintf(buf, "You %scollected %d/9 Nazgul rings.", maybe_have(final),
            i);
    putstr(en_win, 0, buf);
    Sprintf(buf, "You %skilled %d/3 erinyes.", maybe_have(final),
            mvitals[PM_ERINYS].ukilled);
    putstr(en_win, 0, buf);
    if (tnnt_globals.non_wish_djinni > 0) {
        Sprintf(buf, "You have summoned %d djinni who did not give you a wish.",
                tnnt_globals.non_wish_djinni);
        putstr(en_win, 0, buf);
    }
    if (final || u.uevent.udemigod) {
        Sprintf(buf, "You %skilled the Wizard of Yendor %d time%s.",
                maybe_have(final), tnnt_globals.wizards_killed,
                plur(tnnt_globals.wizards_killed));
        putstr(en_win, 0, buf);
        /* don't let this mislead someone into thinking they still have the
         * "never kill the Wizard" conduct if they allowed him to die by some
         * other means which doesn't tick wizards_killed */
        if (tnnt_globals.wizards_killed == 0
            && tnnt_globals.unique_info[tnnt_uniqndx(PM_WIZARD_OF_YENDOR)].died)
            putstr(en_win, 0, "  ...but did not prevent him from dying.");
    }
    if (final || In_endgame(&u.uz)) {
        Sprintf(buf, "Home plane elementals killed: %d/4 %d/4 %d/4 %d/4",
                tnnt_globals.elementals_killed_on_planes[0],
                tnnt_globals.elementals_killed_on_planes[1],
                tnnt_globals.elementals_killed_on_planes[2],
                tnnt_globals.elementals_killed_on_planes[3]);
        putstr(en_win, 0, buf);
    }
    if (final || Is_astralevel(&u.uz)) {
        Sprintf(buf, "Monsters on Astral eaten by purple worms: %d",
                tnnt_globals.astral_worm_gulps);
        putstr(en_win, 0, buf);
    }

    for (i = 0; i < MAXOCLASSES; ++i) {
        char ochar[2];
        ochar[0] = def_oc_syms[i].sym;
        ochar[1] = '\0';
        if (!(ALL_EDIBLE_OCLASSES & (1 << i)))
            continue; /* invalid class such as ILLOBJ_CLASS or VENOM_CLASS */
        if ((tnnt_globals.objclasses_eaten >> i) & 0x1)
            Strcat(eaten, ochar);
        else
            Strcat(uneaten, ochar);
    }
    Sprintf(buf, "Object classes eaten: %s Uneaten: %s", eaten, uneaten);
    putstr(en_win, 0, buf);
    /* brief summary of (and pointer to) #snacks */
    count = 0;
    for (i = TRIPE_RATION; i <= TIN; ++i) {
        int foodidx = i - TRIPE_RATION;
        if (i >= GLOB_OF_GRAY_OOZE && i <= GLOB_OF_BLACK_PUDDING) {
            i = GLOB_OF_BLACK_PUDDING; /* count only one glob */
        }
        if (tnnt_globals.foods_eaten & (1L << foodidx)) {
            count++;
        }
    }
    total = (1 + TIN - TRIPE_RATION
             - (GLOB_OF_BLACK_PUDDING - GLOB_OF_GRAY_OOZE));
    Sprintf(buf, "You %s %d/%d foods.", !final ? "have eaten" : "ate",
            count, total);
    if (!final)
        Strcat(buf, "  For details, use #snacks.");
    putstr(en_win, 0, buf);

    /* ditto for #species */
    count = total = 0;
    for (i = LOW_PM; i < SPECIAL_PM; ++i) {
        if (tnnt_common_monst(i)) {
            count += (mvitals[i].ukilled > 0);
            total++;
        }
    }
    Sprintf(buf, "You %skilled %d/%d eligible species.", maybe_have(final),
            count, total);
    if (!final)
        Strcat(buf, "  For details, use #species.");
    putstr(en_win, 0, buf);
    display_nhwindow(en_win, TRUE);
    destroy_nhwindow(en_win);
    en_win = WIN_ERR;
    return 0;
#undef maybe_have
}

/* #achievements command. Ask whether they want to see achievements earned
 * or unearned, then display whichever list is appropriate. */
int
dotnntachievements(VOID_ARGS)
{
    return show_tnnt_achievements(FALSE);
}

int
show_tnnt_achievements(final)
boolean final;
{
    menu_item *choice = NULL;
    winid win = create_nhwindow(NHW_MENU);
    char response, buf[BUFSZ], searchbuf[BUFSZ];
    int i, num_earned = 0;

    start_menu(win);
    if (!final) {
        anything any;
        any.a_char = 'e';
        add_menu(win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
                "show only achievements earned this game", MENU_UNSELECTED);
        any.a_char = 'u';
        add_menu(win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
                "show only achievements not yet earned this game", MENU_UNSELECTED);
        any.a_char = 'a';
        add_menu(win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
                "show all achievements, marked as earned or not", MENU_UNSELECTED);
        any.a_char = 's';
        add_menu(win, NO_GLYPH, &any, flags.lootabc ? 0 : any.a_char, '\0', ATR_NONE,
                "show all achievements matching a search string", MENU_UNSELECTED);
        end_menu(win, "Which achievements do you want a list of?");
        if (select_menu(win, PICK_ONE, &choice) > 0) {
            response = choice->item.a_char;
            free((genericptr_t) choice);
        }
        else {
            destroy_nhwindow(win);
            return 0;
        }
        destroy_nhwindow(win);
    }
    else {
        response = 'e'; /* dumplog: show only achievements earned */
    }

    searchbuf[0] = '\0';
    if (response == 's') {
        getlin("Enter your search string:", searchbuf);
        if (strlen(searchbuf) < 1)
            return 0;
    }

    /* TNNT TODO: will need NHW_MENU if we can get paging to work in tty */
    win = create_nhwindow(NHW_TEXT);
    if (response == 'a') {
        putstr(win, ATR_HEADING, "All achievements:");
    }
    else if (response == 's') {
        Sprintf(buf, "Achievements matching \"%s\":", searchbuf);
        putstr(win, ATR_HEADING, buf);
    }
    else {
        Sprintf(buf, "Achievements %searned%s:",
                response == 'e' ? "" : "not ", final ? "" : " so far");
        putstr(win, ATR_HEADING, buf);
    }
    if (!final) {
        putstr(win, ATR_BOLD,
               "(Use #tnntstats to check progress of certain ones.)");
    }

    for (i = 0; i < NUM_TNNT_ACHIEVEMENTS; ++i) {
        struct tnnt_achvmt_data* dat = &tnnt_achievements[i];
        char *p;
        /* a response of "both" unconditionally prints any achievement;
         * otherwise, only print the achievement if earned and response was
         * "earned", or if not earned and response was "not earned" */
        boolean earned = tnnt_is_achieved(i);
        boolean searchmatch = ((p = strstri(dat->name, searchbuf)) != 0
                               || (p = strstri(dat->descr, searchbuf)) != 0);
        if (earned)
            num_earned++;
        if (response == 'a'
            || (response == 'e' && earned)
            || (response == 'u' && !earned)
            || (response == 's' && searchmatch)) {
            Sprintf(buf, "[%c] #%03d \"%s\" - %s", (earned ? 'X' : ' '),
                    i + 1, dat->name, dat->descr);
            putstr(win, 0, buf);
        }
    }
    Sprintf(buf, "%d/%d achievements earned in this game.", num_earned,
            NUM_TNNT_ACHIEVEMENTS);
    putstr(win, 0, buf);
    display_nhwindow(win, TRUE);
    /* select_menu(win, PICK_ONE, &choice); */
    destroy_nhwindow(win);
    return 0;
}

/* #snacks command */
int
doshowfoodseaten(VOID_ARGS)
{
    /* More highly unstable food-based code. Assumes that TRIPE_RATION is the
     * first food and TIN is the last food defined in objects.c.
     * Also assumes that globs appear contiguously, and that gray ooze is first
     * and black pudding is last.
     */
    int i;
    boolean glob_eaten = FALSE;
    en_win = create_nhwindow(NHW_MENU);

    putstr(en_win, ATR_BOLD, "Foods you have eaten at least once:");
    for (i = TRIPE_RATION; i <= TIN; ++i) {
        int foodidx = i - TRIPE_RATION;
        if (tnnt_globals.foods_eaten & (1L << foodidx)) {
            if (i >= GLOB_OF_GRAY_OOZE && i <= GLOB_OF_BLACK_PUDDING) {
                putstr(en_win, 0, "glob");
                glob_eaten = TRUE;
                /* skip the rest of the globs to avoid repeating */
                i = GLOB_OF_BLACK_PUDDING;
            }
            else if (i == SLIME_MOLD) {
                putstr(en_win, 0, "custom fruit");
            }
            else {
                putstr(en_win, 0, OBJ_NAME(objects[i]));
            }
        }
    }

    putstr(en_win, 0, "");
    putstr(en_win, ATR_BOLD, "Foods you have never eaten:");
    for (i = TRIPE_RATION; i <= TIN; ++i) {
        int foodidx = i - TRIPE_RATION;
        if (!(tnnt_globals.foods_eaten & (1L << foodidx))) {
            if (i >= GLOB_OF_GRAY_OOZE && i <= GLOB_OF_BLACK_PUDDING) {
                if (!glob_eaten)
                    putstr(en_win, 0, "glob");
                /* skip the rest of the globs to avoid repeating */
                i = GLOB_OF_BLACK_PUDDING;
            }
            else if (i == SLIME_MOLD) {
                putstr(en_win, 0, "custom fruit");
            }
            else {
                putstr(en_win, 0, OBJ_NAME(objects[i]));
            }
        }
    }

    display_nhwindow(en_win, TRUE);
    destroy_nhwindow(en_win);
    en_win = WIN_ERR;
    return 0;
}

/* #species command */
int
dotnntspecies(VOID_ARGS)
{
    int i, total = 0, numeligible = 0;
    char buf[BUFSZ];
    char lowercaselets[27] = "                          "; /* 26 spaces */
    char uppercaselets[27] = "        *                 "; /* same, no 'I' */
    en_win = create_nhwindow(NHW_MENU);
    putstr(en_win, ATR_BOLD, "Eligible species you have personally killed at least 1 member of:");

    for (i = LOW_PM; i < SPECIAL_PM; ++i) {
        if (tnnt_common_monst(i)) {
            boolean killed = (mvitals[i].ukilled > 0);
            Sprintf(buf, "[%c] %s", (killed ? 'X' : ' '), mons[i].mname);
            putstr(en_win, 0, buf);
            numeligible++;
            if (killed) {
                char mlet;
                total++;
                /* note that mons[].mlet isn't the actual character it displays
                 * as. S_ANT is 1, for instance. */
                mlet = mons[i].mlet - S_ANT + 'a';
                if (mlet >= 'a' && mlet <= 'z') {
                    lowercaselets[mlet - 'a'] = mlet;
                }
                mlet = mons[i].mlet - S_ANGEL + 'A';
                if (mlet >= 'A' && mlet <= 'Z') {
                    uppercaselets[mlet - 'A'] = mlet;
                }
            }
        }
    }
    Sprintf(buf, "%d/%d eligible species killed.", total, numeligible);
    putstr(en_win, 0, buf);
    Sprintf(buf, "Lowercase-letter species killed: [%s]", lowercaselets);
    putstr(en_win, 0, buf);
    Sprintf(buf, "Uppercase-letter species killed: [%s]", uppercaselets);
    putstr(en_win, 0, buf);

    display_nhwindow(en_win, TRUE);
    destroy_nhwindow(en_win);
    en_win = WIN_ERR;
    return 0;
}

/* ######################################################################
 * SWAP CHEST FUNCTIONS
 * ###################################################################### */

#ifdef TNNT_SWAPCHEST_DIR
static char *
make_swapobj_filename(o)
struct obj *o;
{
    /* needs to be big enough for full path */
    static char buf[BUFSZ];
    const char *pfx = (const char *) 0;
#ifndef LOCAL_SWAPCHESTS
    static const char *const prefixes[] = { "eu.", "us.", "au." };
    const schar num_servers = SIZE(prefixes);
    /* swap chest files go to one of the three servers, weighted so the most
     * likely destination is the current/source server (currently weighted at
     * 60% for the source server, 20% for the other two -- this should be
     * reevaluated after it's been tested a bit).
     * we mark the swap item files with a prefix if they are going to a server
     * other than this one. a cronjob will transfer them periodically. */
#define LOCAL_RETENTION_WEIGHT 60
    if (rn2(100) >= LOCAL_RETENTION_WEIGHT) {
        schar destination = rn2(num_servers);
        /* if we select the current server, pick some other one.  this should
         * never select the same server a second time. */
        if (!strncmpi(SERVER_LOCATION, prefixes[destination], 3)) {
            destination += rn1(num_servers - 2, 1);
            destination %= num_servers;
        }
        pfx = prefixes[destination];
    }
#endif
    Sprintf(buf, "%s/%sSW-%ld-%s-%x-%x", TNNT_SWAPCHEST_DIR,
            pfx ? pfx : "", time(NULL), plname, (unsigned) ubirthday,
            o->o_id);
    return strdup(buf);
}

boolean
write_swapobj_file(o, swapnum)
struct obj *o;
xchar swapnum;
{
    char *filename = make_swapobj_filename(o);
    FILE *f;
    char playmode = wizard ? 'D' : discover ? 'X' : '-';
    if (!filename)
        return FALSE;
    f = fopen(filename,"w");
    free(filename);
    if (!f)
        return FALSE;
    fprintf(f, "o_id=%x\totyp=%d\towt=%d\tquan=%ld\tspe=%d\toclass=%d\t"
               "cursed=%d\tblessed=%d\toeroded=%d\toeroded2=%d\toerodeproof=%d\t"
               "recharged=%d\tgreased=%d\topoisoned=%d\tusecount=%d\t"
               "corpsenm=%d\tswapnum=%d\tplaymode=%c\tname=%s\n",
               o->o_id, o->otyp, o->owt, o->quan, o->spe, o->oclass,
               o->cursed, o->blessed, o->oeroded, o->oeroded2, o->oerodeproof,
               o->recharged, o->greased, o->opoisoned, o->usecount,
               o->corpsenm, swapnum, playmode, plname);
    /* the second line is just for humans to read what the object is, for debugging */
    iflags.override_ID = 1;
    fprintf(f, "%s\n", doname(o));
    iflags.override_ID = 0;
    fclose(f);
    return TRUE;
}

#define MKSWAPOBJ_SUCCESS   0 /* nothing went wrong, put the object in chest */
#define MKSWAPOBJ_ERROR     1 /* something went wrong */
#define MKSWAPOBJ_IGNOREOBJ 2 /* nothing went wrong but we should suppress this
                                 object from appearing in the swap chest */

/* Create object from file and add to swapchest */
static struct obj *
mkswapobj(swapchest, filename, rcode)
struct obj *swapchest;
char *filename;
short *rcode;
{
#define _N2STR(n) #n
#define N2STR(n) _N2STR(n)
    char buf[BUFSZ]; /* multi-use */
    char donorname[PL_NSIZ + 1];
    int swapnum = -1;
    char o_playmode = '-', playmode;
    FILE *f;
    struct obj *o;
    int tmp_bitfield;
    *rcode = MKSWAPOBJ_ERROR;
    Sprintf(buf, "%s/%s", TNNT_SWAPCHEST_DIR, filename);
    f = fopen(buf,"r");
    if (!f)
        return (struct obj *) 0;

    /* there is no nice way to do this... */
    o = newobj();
    *o = zeroobj;
    o->age = monstermoves;
    o->o_id = context.ident++;
    if (!o->o_id)
        o->o_id = context.ident++; /* overflow */
    o->corpsenm = NON_PM;
    o->known = 1;
    o->dknown = 1;
    o->bknown = 1;
    o->rknown = 1;

    /* this only works because we separate fields with whitespace */
    while (fscanf(f,"%s",buf) == 1) {
        if (sscanf(buf, "otyp=%hd", &(o->otyp)) == 1)
            continue;
        if (sscanf(buf, "owt=%u", &(o->owt)) == 1)
            continue;
        if (sscanf(buf, "quan=%ld", &(o->quan)) == 1)
            continue;
        if (sscanf(buf, "spe=%hhd", &(o->spe)) == 1)
            continue;
        if (sscanf(buf, "oclass=%d", &tmp_bitfield) == 1) {
            /* oclass isn't a bitfield but read it as an int to silence a
             * compiler warning for reading an int directly into a char */
            o->oclass = (char) tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "cursed=%d", &tmp_bitfield) == 1) {
            o->cursed = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "blessed=%d", &tmp_bitfield) == 1) {
            o->blessed = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "oeroded=%d", &tmp_bitfield) == 1) {
            o->oeroded = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "oeroded2=%d", &tmp_bitfield) == 1) {
            o->oeroded2 = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "oerodeproof=%d", &tmp_bitfield) == 1) {
            o->oerodeproof = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "recharged=%d", &tmp_bitfield) == 1) {
            o->recharged = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "greased=%d", &tmp_bitfield) == 1) {
            o->greased = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "opoisoned=%d", &tmp_bitfield) == 1) {
            o->opoisoned = tmp_bitfield;
            continue;
        }
        if (sscanf(buf, "usecount=%d", &(o->usecount)) == 1)
            continue;
        if (sscanf(buf, "corpsenm=%d", &(o->corpsenm)) == 1)
            continue;
        if (sscanf(buf, "swapnum=%d", &swapnum) == 1)
            continue;
        if (sscanf(buf, "playmode=%c", &o_playmode) == 1)
            continue;
        if (sscanf(buf, "name=%" N2STR(PL_NSIZ) "s", donorname) == 1) {
            char new_name[BUFSZ];
            static const char *swprefixes[SWAP_ITEMS_MAX] = {
                /* this needs to be adjusted if SWAP_ITEMS_MAX changes */
                /* playername will be appended to the end, and there is code
                 * elsewhere that assumes playername is ALWAYS at the end of
                 * the name */
                "a token from",
                "kindly donated by",
                "generously bestowed by"
            };
            const char *pfx = (swapnum >= 0 && swapnum < SWAP_ITEMS_MAX)
                                ? swprefixes[swapnum] : "a gift from";

            if (!strcmp(donorname, plname) && !wizard && !discover) {
                /* The player doesn't get to see their own items, unless in
                 * wizard or explore mode. */
                *rcode = MKSWAPOBJ_IGNOREOBJ;
                goto free_swapobj;
            }
            Sprintf(new_name, "%s %s", pfx, donorname);
            o = oname(o, new_name);
        }
    }
    fclose(f);
    /* Each mode (other than wizard mode) can only see/remove objects donated
     * in that playmode.  If the current playmode doesn't match the one in the
     * file, we'll throw away the object. */
    playmode = wizard ? 'D' : discover ? 'X' : '-';
    if (!wizard && o_playmode != playmode) {
        *rcode = MKSWAPOBJ_IGNOREOBJ;
        goto free_swapobj;
    }
    if (!o->otyp) {
 free_swapobj:
        if (o->oextra)
            dealloc_oextra(o);
        free(o);
        return (struct obj *) 0;
    }
    add_to_container(swapchest, o);
    o->where = OBJ_INSWAP;
    o->swapobj_filename = strdup(filename);
    *rcode = MKSWAPOBJ_SUCCESS;
    return o;
#undef N2STR
#undef _N2STR
}

void
refresh_swap_chest_contents(swapchest)
struct obj *swapchest;
{
    DIR *d;
    struct dirent *de;
    struct obj *otmp;
    delete_swap_chest_contents(swapchest);
    d = opendir(TNNT_SWAPCHEST_DIR);
    if (!d) {
        const char *errormsg;
        if (!(errormsg = strerror(errno)))
            errormsg = "unknown error";
        impossible("can't open swap chest directory at %s (%s)",
                   TNNT_SWAPCHEST_DIR, errormsg);
        return;
    }
    while ((de = readdir(d)) != NULL) {
        if (!strncmp(de->d_name, "SW-", 3)) {
            short rcode;
            otmp = mkswapobj(swapchest, de->d_name, &rcode);
            if (!otmp && rcode == MKSWAPOBJ_ERROR) {
                impossible("Swapchest obj not read from %s", de->d_name);
            }
        }
    }
    closedir(d);
}

boolean
delete_swapobj_file(o)
struct obj *o;
{
    char path[BUFSZ];
    Sprintf(path, "%s/%s", TNNT_SWAPCHEST_DIR, o->swapobj_filename);
    if (unlink(path) == -1) {
        if (errno != ENOENT)
            impossible("delete_swapobj_file %d", errno);
        return FALSE; /* someone else deleted it first */
    }
    return TRUE;
}

const char *
swapobj_donor_name(o)
struct obj *o;
{
    const char *donor, *oname;
    if (!has_oname(o)) {
        impossible("swapchest obj with no name?");
        return (const char *) 0;
    }
    /* Assumption: otmp has just come from a swap chest and the player hasn't
     * had a chance to rename it yet, so the donor name saved as part of the
     * object name is still valid.
     * Assumption: object name is "words words ... plname", key part is a
     * space before the donor name */
    oname = ONAME(o);
    donor = eos((char *) oname);
    while (donor >= oname && *donor != ' ') {
        donor--;
    }
    donor++; /* don't include the leading space */
    return donor;
}

#define TNNT_DONOR_FILE HACKDIR "/donors"
void
credit_swapobj_donor(otmp)
struct obj *otmp;
{
    const char *donor = swapobj_donor_name(otmp);
    FILE *donorfile;

    if (!donor || !strcmp(donor, plname)) {
        return; /* you don't get credit for removing your own items */
    }
    donorfile = fopen(TNNT_DONOR_FILE, "a");
    if (!donorfile) {
        impossible("Error writing donor data to file %s", TNNT_DONOR_FILE);
        return;
    }
    fprintf(donorfile, "%s\n", donor);
    fclose(donorfile);
}

/* Determine if obj may be placed in the swap chest. */
boolean
swap_chest_eligible(obj)
struct obj *obj;
{
    if (obj->oartifact)
        return FALSE;
    switch (obj->oclass) {
        case RING_CLASS:
            /* reject cursed rings and chargeable rings at +0 or worse */
            if ((objects[obj->otyp].oc_charged && obj->spe <= 0)
                || obj->cursed)
                return FALSE;
            return TRUE;
        case WAND_CLASS:
            switch (obj->otyp) {
            case WAN_NOTHING: /* useless */
            case WAN_WISHING: /* too powerful */
                return FALSE;
            default:
                return obj->spe > 0;
            }
        case AMULET_CLASS:
            switch (obj->otyp) {
            case AMULET_OF_STRANGULATION:
            case AMULET_OF_RESTFUL_SLEEP:
            case FAKE_AMULET_OF_YENDOR: /* real one already checked */
                return FALSE;
            default:
                return !obj->cursed;
            }
        case POTION_CLASS:
            if (objects[obj->otyp].oc_magic)
                return TRUE;
            return (obj->otyp == POT_WATER) && (obj->blessed || obj->cursed);
        case TOOL_CLASS:
            if (Has_contents(obj))
                return FALSE; /* bags with stuff in them not allowed */
            switch (obj->otyp) {
            /* no gifting of (likely) wishes */
            case MAGIC_LAMP:
                return FALSE;
            /* charged allowed tools: conveniently, these all work the same */
            case EXPENSIVE_CAMERA:
            case TINNING_KIT:
            case MAGIC_MARKER:
                return (obj->spe > 10);
            /* magical instruments act like their mundane counterparts when
             * empty, so if we accept the mundane version accept the magical
             * one regardless of charges.  that they may have charges (and
             * could be recharged) is just lagniappe. */
            case FROST_HORN:
            case FIRE_HORN:
            case DRUM_OF_EARTHQUAKE:
                return TRUE;
            /* useful non-magical tools */
            case STETHOSCOPE:
            case OILSKIN_SACK:
            case BUGLE:
            case TOOLED_HORN:
            case LEATHER_DRUM:
                return TRUE;
            }
            /* other tools only require at least 1 charge */
            if (!is_weptool(obj) && objects[obj->otyp].oc_charged)
                return (obj->spe > 0);
            if (objects[obj->otyp].oc_magic)
                return TRUE; /* magic things including empty BoH ok */
            if (!is_weptool(obj))
                return FALSE;
            /* FALLTHRU */
        case WEAPON_CLASS:
            if (obj->cursed)
                return FALSE;
            /* permit silver weapons (saber, spear, dagger), athame suitable
               for fast semi-permanent engraving, and any other >=+3 weapon */
            switch (obj->otyp) {
            case SILVER_SABER:
            case SILVER_DAGGER:
            case SILVER_SPEAR:
                return TRUE;
            /* allow athame as long as it's not so dull it can't engrave */
            case ATHAME:
                return (obj->spe > -3);
            default:
                return (obj->spe > 2);
            }
        case ARMOR_CLASS:
            if (obj->cursed)
                return FALSE;
            switch (obj->otyp) {
            case DUNCE_CAP:
            case FUMBLE_BOOTS:
            case GAUNTLETS_OF_FUMBLING:
                return FALSE;
            case GRAY_DRAGON_SCALES:
            case SILVER_DRAGON_SCALES:
            case RED_DRAGON_SCALES:
            case ORANGE_DRAGON_SCALES:
            case WHITE_DRAGON_SCALES:
            case BLACK_DRAGON_SCALES:
            case BLUE_DRAGON_SCALES:
            case GREEN_DRAGON_SCALES:
            case YELLOW_DRAGON_SCALES:
                return TRUE;
            default:
                return (objects[obj->otyp].oc_magic || obj->spe > 2);
            }
        case SPBOOK_CLASS:
            if (!objects[obj->otyp].oc_magic)
                return FALSE;
            if (obj->cursed)
                return FALSE;
            if (obj->spestudied > MAX_SPELL_STUDY - 1)
                return FALSE; /* this probably allows 2 more readings */
            return TRUE;
        case SCROLL_CLASS:
            if (!objects[obj->otyp].oc_magic)
                return FALSE;
            switch (obj->otyp) {
            case SCR_AMNESIA:
            case SCR_PUNISHMENT:
                return FALSE;
            default:
                return TRUE;
            }
        case FOOD_CLASS:
            switch(obj->otyp) {
            case LUMP_OF_ROYAL_JELLY:
            case SPRIG_OF_WOLFSBANE:
                return TRUE;
            case TIN:
                /* spinach only; currently not allowing other potentially-
                   useful tins (like dragon) */
                if (obj->spe == 1)
                    return TRUE;
                /* FALLTHRU */
            default:
                return FALSE;
            }
        case GEM_CLASS:
            /* opinions have been divided on whether to accept objects in this
             * class; the current reasoning on why not is:
             * 1. valuable gems: anyone who gets to a swap chest will have found
             *    some of these already and one more is not useful.
             * 2. luckstones: there's always one on the Mines End swapchest
             *    level, and most players would have one by the Valley.
             * 3. touchstones: not uncommon to find one in the Mines; even if
             *    you didn't, allowing this would let you try other gray stones
             *    that get rejected and thereby help you identify them. It'd
             *    also probably be a common discard for Archeologists who have
             *    by now used it to identify most glass, while not being useful
             *    enough for other players to take. */
            return FALSE;
        default: /* boulders, statues, iron chains, etc */
            return FALSE;
    }
}

#endif /* TNNT_SWAPCHEST_DIR */

/* ######################################################################
 * DEATHMATCH FUNCTIONS
 * ###################################################################### */

#ifdef TNNT_NPC_DIR

/* Take the (presumed to be ascending) player, and encode all of their
 * applicable stats and inventory into a data file which can be reloaded to
 * provide the character for someone else's deathmatch.
 *
 * Additional gear stocking-up to cover deficiencies in the ascender is not done
 * here; it is done when the NPC gets loaded into another game. However, any
 * logic that might want to remove items for whatever reason goes here.
 *
 * This should only be called from really_done after urealtime.finish_time gets
 * set.
 */
void
write_npc_data(VOID_ARGS)
{
    FILE *npcfile;
    char buf[BUFSZ];
    unsigned short mintrinsics;
    struct obj *obj;
    Sprintf(buf, "%s/NPC-%s", TNNT_NPC_DIR, plname);
    npcfile = fopen(buf, "w");
    if (!npcfile) {
        impossible("Error writing player data to '%s' file", buf);
        return;
    }
    /* line 1: timestamp */
    fprintf(npcfile, "%ld\n", program_state.gameover ? urealtime.finish_time
                                                     : time(NULL));
    /* line 2: player name */
    fprintf(npcfile, "%s\n", plname);
    /* line 3: player role index and gender */
    fprintf(npcfile, "%d %d\n", (flags.female && urole.femalenum != NON_PM) ?
                                    urole.femalenum : urole.malenum,
                                flags.female);
    /* line 4: player experience level */
    fprintf(npcfile, "%d\n", u.ulevel);
    /* line 5: player hit point maximum */
    fprintf(npcfile, "%d\n", u.uhpmax);
    /* line 6: player intrinsics, converted to mextrinsics format...
     * fire cold shock sleep poison disintegration are the only ones that will
     * actually do anything and that are obtainable by the player intrinsically
     */
    mintrinsics = 0x0;
    if (HFire_resistance & INTRINSIC)
        mintrinsics |= MR_FIRE;
    if (HCold_resistance & INTRINSIC)
        mintrinsics |= MR_COLD;
    if (HShock_resistance & INTRINSIC)
        mintrinsics |= MR_ELEC;
    if (HSleep_resistance & INTRINSIC)
        mintrinsics |= MR_SLEEP;
    if (HPoison_resistance & INTRINSIC)
        mintrinsics |= MR_POISON;
    if (HDisint_resistance & INTRINSIC)
        mintrinsics |= MR_DISINT;
    fprintf(npcfile, "0x%x\n", mintrinsics);
    /* lines 7-end: carried objects, we preserve only certain fields because
     * others would be pointless... */
    for (obj = invent; obj; obj = obj->nobj) {
        boolean clear_usecount = FALSE;
        /* Items that should be excluded go here.
         * We don't go into cobj, so a container with stuff in it turns into an
         * empty container later. */
        if (obj->otyp == WAN_WISHING || obj->otyp == MAGIC_LAMP /* no wishes */
            || obj->otyp == SCR_MISSING_CODE        /* no devteam quest help */
            || objects[obj->otyp].oc_unique)         /* no sequence breaking */
            continue;
        /* Clear fields for items marked as TNNT-special for one achievement or
         * another, to prevent people e.g. getting the "athame generated in a
         * lich's inventory" achivement just by taking one out of the swap
         * chest. */
        if ((obj->otyp == ATHAME && obj->lichathame)
            || (obj->otyp == RIN_INVISIBILITY && obj->nazgul_ring)
            || (obj->otyp == CANDY_BAR && obj->soko_candy))
            clear_usecount = TRUE;
        /* TNNT TODO: handling for items that shouldn't be excluded entirely,
         * but should be modified somehow (e.g. artifacts). */

        fprintf(npcfile, "%d %ld %d %d %d %d %d %d %d %d %d",
                obj->otyp,
                obj->quan,
                obj->spe,
                obj->cursed,
                obj->blessed,
                obj->oerodeproof,
                obj->recharged,
                obj->greased,
                obj->corpsenm,
                (clear_usecount ? 0 : obj->usecount),
                obj->oeaten);
        if (has_oname(obj)) {
            char nambuf[BUFSZ], *namep = nambuf;
            Strcpy(nambuf, ONAME(obj));
            if (obj->oartifact) {
                /* scuff up the artifact name so it's recognizable but not
                 * actually an artiname, like "slippage" in do_name.c */
                if (!strncmpi(namep, "the ", 4))
                    namep += 4;
                do {
                    wipeout_text(namep, 1 + rn2_on_display_rng(2), 0U);
                } while (!strcmp(nambuf, ONAME(obj)));
            }
            fprintf(npcfile, " %s", namep);
        }
        fprintf(npcfile, "\n");
    }
    fclose(npcfile);
}

static char *
pick_npc_file(void)
{
    static char npcpath[BUFSZ];
    DIR *d = opendir(TNNT_NPC_DIR);
    int chance = 0;
    struct dirent *de;
    npcpath[0] = '\0';
    if (!d) {
        const char *errormsg;
        if (!(errormsg = strerror(errno)))
            errormsg = "unknown error";
        impossible("can't open NPC directory at %s (%s)",
                   TNNT_NPC_DIR, errormsg);
        return npcpath; /* with leading \0 */
    }
    while ((de = readdir(d)) != NULL) {
        if (!strncmp(de->d_name, "NPC-", 4)
            && !rn2(++chance)) {
            Strcpy(npcpath, de->d_name);
        }
    }
    closedir(d);
    return npcpath;
}

/* Create the NPC and insert them into the game, reading from the data file for
 * its attributes and inventory.
 *
 * By design, this only reads the contents out of the file. It does not delete
 * the file after using it. The only way the current NPC data is intended to be
 * destroyed is by being replaced with another NPC.
 */
struct monst*
create_tnnt_npc(x, y)
xchar x, y;
{
    FILE* npcfile;
    int pm_num, gender, npc_level, hpmax;
    unsigned short mintrinsics;
    char npcname[BUFSZ], path[BUFSZ], line[BUFSZ];
    char *npcfilename = pick_npc_file();
    const long int mmflags = (NO_MINVENT | MM_NOCOUNTBIRTH | MM_ADJACENTOK
                              | MM_ANGRY | MM_ASLEEP);
    struct obj *obj;
    int strategy;
    struct monst *npc;
    Sprintf(path, "%s/%s", TNNT_NPC_DIR, npcfilename);
    if (!*npcfilename || !(npcfile = fopen(path, "r"))) {
        int mndx;
        /* NB: in the actual tournament the directory should be stocked with
         * NPC file(s) in advance, so that users will not see this error even
         * if they enter the arena before anyone has ascended. */
        if (!*npcfilename)
            impossible("No NPC files available - creating mplayer instead");
        else
            impossible(
                     "Error opening NPC file '%s' - creating mplayer instead",
                       path);
        mndx = rn1(PM_WIZARD - PM_ARCHEOLOGIST + 1, PM_ARCHEOLOGIST);
        /* special = true better approximates an ascending character... */
        npc = mk_mplayer(&mons[mndx], x, y, TRUE);
        npc->mcloned = 1;
        npc->msleeping = 1;
        return npc;
    }
    /* Get base data from the file. */
    fgets(npcname, BUFSZ, npcfile); /* eat up timestamp; game doesn't use it */
    fgets(npcname, BUFSZ, npcfile); /* actually read in name this time */
    npcname[strlen(npcname)-1] = '\0'; /* strip \n that was read in */
    fscanf(npcfile, "%d %d\n", &pm_num, &gender);  /* get PM_* of correct player monster */
    fscanf(npcfile, "%d\n", &npc_level);   /* get ascender's XL */
    fscanf(npcfile, "%d\n", &hpmax);   /* get ascender's hp max */
    fscanf(npcfile, "0x%hx\n", &mintrinsics); /* get ascender's monster-format intrinsics */

    /* Make the monster normally and take care of all the boilerplate. */
    npc = makemon(&mons[pm_num], x, y, mmflags);

    /* Setup! */
    npc->m_lev = max(npc_level, 14);
    hpmax = min(hpmax, 500); /* cap ascender's hpmax at this */
    hpmax = max(hpmax, d((int) npc->m_lev, 10) + 100 + rnd(30)); /* beefed up mplayer formula */
    hpmax = max(hpmax, 200); /* prevent low HP rolls */
    npc->mhp = npc->mhpmax = hpmax;
    Strcat(npcname, " the ");
    Strcat(npcname, rank_of((int) npc->m_lev, pm_num, gender));
    npc = christen_monst(npc, npcname);
    /* Minor issue: monsters in vanilla don't store true resistances;
     * mextrinsics is meant to store extrinsic resistances from gear. If the NPC
     * gets, say, poison resistance from this, they'll act as if they have
     * innate poison resistance up until they put on and then take off an apron,
     * which will lose the poison resistance.
     * TNNT TODO FOR 3.7: revisit this, because monster intrinsic interactions
     * will have changed.
     */
    npc->mextrinsics = mintrinsics;
    npc->mpeaceful = 0;
    set_malign(npc);
    /* Regular player monsters never use the mcloned flag, so this is a good way
     * to put a special marker on the NPC. Plus they ARE sort of a clone. */
    npc->mcloned = 1;

    /* Inventory! */
    strategy = NEED_HTH_WEAPON;
    while (fgets(line, BUFSZ, npcfile) != NULL) {
        char objnam[BUFSZ];
        int n, otyp, quan, spe, cursed, blessed, oerodeproof, recharged,
            greased, corpsenm, usecount, oeaten;
        n = sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %[^\n]",
                   &otyp, &quan, &spe, &cursed, &blessed, &oerodeproof,
                   &recharged, &greased, &corpsenm, &usecount, &oeaten,
                   objnam);
        if (n >= 11) {
            obj = mksobj(otyp, FALSE, FALSE);
            obj->quan = quan;
            obj->spe = spe;
            obj->cursed = cursed;
            obj->blessed = blessed;
            obj->oerodeproof = oerodeproof;
            obj->recharged = recharged;
            obj->greased = greased;
            obj->corpsenm = corpsenm;
            obj->usecount = usecount;
            obj->oeaten = oeaten;
            if (n == 12)
                obj = oname(obj, objnam);
            mpickobj(npc, obj);
        }
    }
    m_dowear(npc, TRUE);
    npc->weapon_check = strategy;
    mon_wield_item(npc);

    /* Check to see if our NPC is missing gear, and if so,
     * provide them with ascension kit worthy items. */
    /* Armor selection */
    if (!which_armor(npc, W_ARM) && npc->data != &mons[PM_MONK]) {
        obj = rn2(2) ? mksobj(GRAY_DRAGON_SCALE_MAIL, FALSE, FALSE)
                     : mksobj(SILVER_DRAGON_SCALE_MAIL, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        mpickobj(npc, obj);
    }
    if (!which_armor(npc, W_ARMC)) {
        if (npc->data == &mons[PM_MONK])
            obj = mksobj(ROBE, FALSE, FALSE);
        else if (npc->data == &mons[PM_WIZARD])
            obj = mksobj(CLOAK_OF_MAGIC_RESISTANCE, FALSE, FALSE);
        else
            obj = rn2(2) ? mksobj(CLOAK_OF_PROTECTION, FALSE, FALSE)
                         : mksobj(CLOAK_OF_DISPLACEMENT, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    if (!which_armor(npc, W_ARMU)) {
        if (npc->data == &mons[PM_TOURIST])
            obj = mksobj(HAWAIIAN_SHIRT, FALSE, FALSE);
        else
            obj = mksobj(T_SHIRT, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    if (!which_armor(npc, W_ARMH)) {
        if (npc->data == &mons[PM_WIZARD])
            obj = mksobj(CORNUTHAUM, FALSE, FALSE);
        else
            obj = rn2(2) ? mksobj(HELM_OF_TELEPATHY, FALSE, FALSE)
                         : mksobj(HELM_OF_BRILLIANCE, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    if (!which_armor(npc, W_ARMG)) {
        if (npc->data != &mons[PM_WIZARD] && npc->data != &mons[PM_PRIEST]
            && npc->data != &mons[PM_PRIESTESS] && npc->data != &mons[PM_HEALER])
            obj = rn2(4) ? mksobj(GAUNTLETS_OF_POWER, FALSE, FALSE)
                         : mksobj(LEATHER_GLOVES, FALSE, FALSE);
        else
            obj = rn2(4) ? mksobj(GAUNTLETS_OF_DEXTERITY, FALSE, FALSE)
                         : mksobj(LEATHER_GLOVES, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    if (!which_armor(npc, W_ARMS) && npc->data != &mons[PM_MONK]
        && npc->data != &mons[PM_WIZARD] && npc->data != &mons[PM_HEALER]
        && npc->data != &mons[PM_PRIEST] && npc->data != &mons[PM_PRIESTESS]
        && npc->data != &mons[PM_BARBARIAN]) {
        obj = rn2(2) ? mksobj(SMALL_SHIELD, FALSE, FALSE)
                     : mksobj(SHIELD_OF_REFLECTION, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        mpickobj(npc, obj);
    }
    if (!which_armor(npc, W_ARMF)) {
        obj = rn2(2) ? mksobj(SPEED_BOOTS, FALSE, FALSE)
                     : mksobj(WATER_WALKING_BOOTS, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(2) + 3;
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    /* Melee weapons selection */
    if (!MON_WEP(npc) && npc->data != &mons[PM_MONK]) {
        if (npc->data == &mons[PM_ARCHEOLOGIST]
            || npc->data == &mons[PM_TOURIST])
            obj = mksobj(SILVER_SABER, FALSE, FALSE);
        else if (npc->data == &mons[PM_BARBARIAN])
            obj = mksobj(BATTLE_AXE, FALSE, FALSE);
        else if (npc->data == &mons[PM_CAVEMAN]
                 || npc->data == &mons[PM_CAVEWOMAN]
                 || npc->data == &mons[PM_PRIEST]
                 || npc->data == &mons[PM_PRIESTESS])
            obj = mksobj(MACE, FALSE, FALSE);
        else if (npc->data == &mons[PM_HEALER]
                 || npc->data == &mons[PM_WIZARD])
            obj = mksobj(QUARTERSTAFF, FALSE, FALSE);
        else if (npc->data == &mons[PM_KNIGHT]
                 || npc->data == &mons[PM_VALKYRIE])
            obj = mksobj(LONG_SWORD, FALSE, FALSE);
        else if (npc->data == &mons[PM_RANGER]
                 || npc->data == &mons[PM_ROGUE])
            obj = mksobj(SHORT_SWORD, FALSE, FALSE);
        else if (npc->data == &mons[PM_SAMURAI])
            obj = mksobj(KATANA, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(3) + 4;
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    /* Ranged weapons - NPC will spawn with these
     * regardless of whether the last ascending player
     * had ranged weapons or not */
    if (npc->data == &mons[PM_ARCHEOLOGIST]
        || npc->data == &mons[PM_CAVEMAN]
        || npc->data == &mons[PM_CAVEWOMAN]) {
        (void) mongets(npc, SLING);
        obj = mksobj(FLINT, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(3) + 2;
        obj->oerodeproof = 1;
        obj->quan = (long) rn1(20, 10);
        obj->owt = weight(obj);
        mpickobj(npc, obj);
        strategy = NEED_RANGED_WEAPON;
    }
    if (npc->data == &mons[PM_HEALER]
        || npc->data == &mons[PM_PRIEST]
        || npc->data == &mons[PM_PRIESTESS]
        || npc->data == &mons[PM_TOURIST]
        || npc->data == &mons[PM_WIZARD]) {
        obj = mksobj(DART, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(3) + 2;
        obj->oerodeproof = 1;
        obj->quan = (long) rn1(20, 10);
        obj->owt = weight(obj);
        mpickobj(npc, obj);
        strategy = NEED_RANGED_WEAPON;
    }
    if (npc->data == &mons[PM_SAMURAI]
        || npc->data == &mons[PM_MONK]) {
        obj = mksobj(SHURIKEN, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(3) + 2;
        obj->oerodeproof = 1;
        obj->quan = (long) rn1(20, 10);
        obj->owt = weight(obj);
        mpickobj(npc, obj);
        strategy = NEED_RANGED_WEAPON;
    }
    if (npc->data == &mons[PM_BARBARIAN]
        || npc->data == &mons[PM_KNIGHT]
        || npc->data == &mons[PM_RANGER]) {
        (void) mongets(npc, BOW);
        obj = mksobj(ARROW, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(3) + 2;
        obj->oerodeproof = 1;
        obj->quan = (long) rn1(20, 10);
        obj->owt = weight(obj);
        mpickobj(npc, obj);
        strategy = NEED_RANGED_WEAPON;
    }
    if (npc->data == &mons[PM_ROGUE]
        || npc->data == &mons[PM_VALKYRIE]) {
        obj = mksobj(DAGGER, FALSE, FALSE);
        bless(obj);
        obj->spe = rnd(3) + 2;
        obj->oerodeproof = 1;
        obj->quan = (long) rn1(10, 6);
        obj->owt = weight(obj);
        mpickobj(npc, obj);
        strategy = NEED_RANGED_WEAPON;
    }
    /* Amulets */
    if (!which_armor(npc, W_AMUL)) {
        obj = rn2(5) ? mksobj(AMULET_OF_REFLECTION, FALSE, FALSE)
                     : mksobj(AMULET_OF_LIFE_SAVING, FALSE, FALSE);
        bless(obj);
        obj->oerodeproof = 1;
        mpickobj(npc, obj);
    }
    /* Stoning defense */
    if (is_mplayer(npc->data)) {
        obj = mkcorpstat(CORPSE, NULL, &mons[PM_LIZARD], x, y, CORPSTAT_NONE);
        obj->quan = (long) rn1(3, 8);
        obj->owt = weight(obj);
        obj_extract_self(obj);
        mpickobj(npc, obj);
    }
    m_dowear(npc, TRUE);
    npc->weapon_check = strategy;
    mon_wield_item(npc);

    for (obj = npc->minvent; obj; obj = obj->nobj) {
        obj->transient = TRUE;
    }

    return npc;
}

#endif /* TNNT_NPC_FILE */

/* Things that need to happen when the NPC wakes up and the Deathmatch begins.
 * Assumes the caller is making the NPC wake up. */
void
npc_awakens()
{
    if (tnnt_globals.deathmatch_started) {
        return;
    }

    if (Deaf) {
        pline("The ground shakes a bit.");
    } else {
        pline("A voice echoes in the arena:");
        verbalize("Thou art brave indeed, mortal!  Now prove thy prowess!");
    }
    tnnt_globals.deathmatch_started = TRUE;
    livelog_printf(LL_UMONST, "has entered the Arena");
}

/* hero is traveling to newlev - is it the deathmatch, and illegal at this
 * moment because it might let them render the game unwinnable by leaving items
 * needed to win the game in the sealed-off deathmatch? */
boolean
is_illegal_deathmatch(newlev)
d_level *newlev;
{
    if (!on_level(newlev, &deathmatch_level))
        return FALSE; /* these rules apply only to entering deathmatch */
    if (tnnt_globals.deathmatch_completed)
        return FALSE; /* the branch will never get sealed off */

    if (u.uhave.amulet)
        return TRUE;
    if (u.uevent.invoked)
        return FALSE; /* invocation items are fine to bring in if you have
                         already used them for the invocation */
    if (u.uhave.bell || u.uhave.menorah || u.uhave.book)
        return TRUE;
    return FALSE;
}

/* Recursively collect transient objects in containers and stick them into the
 * olist of extracted objects.
 * Doesn't assume that this has been checked for actually containing objects.
 * Doesn't try to collect container itself if that is transient. */
static void
collect_transient_within(container, olist)
struct obj* container;
struct obj** olist;
{
    struct obj *otmp, *next;
    for (otmp = container->cobj; otmp; otmp = next) {
        next = otmp->nobj;
        if (otmp->cobj)
            collect_transient_within(otmp, olist);

        if (otmp->transient) {
            obj_extract_self(otmp);
            otmp->nobj = *olist;
            *olist = otmp;
        }
    }
    container->owt = weight(container);
}

/* Collect all transient objects on the level - floor, in monster inventories,
 * everything in *your* inventory except the one object "exception".
 * Return a list of objects that have been extracted and now exist only here. */
struct obj*
collect_all_transient(exception)
struct obj* exception;
{
    int x, y;
    struct obj *otmp, *next, *collected;
    struct monst* mtmp;
    collected = NULL;

#define t_collect(obj)                                 \
    do {                                               \
        if (Has_contents(obj))                         \
            collect_transient_within(obj, &collected); \
        if (obj->transient) {                          \
            obj->owornmask = 0;                        \
            obj_extract_self(obj);                     \
            obj->nobj = collected;                     \
            collected = obj;                           \
        }                                              \
    } while (0)

    /* floor objects */
    for (y = 0; y < ROWNO; ++y) {
        for (x = 0; x < COLNO; ++x) {
            for (otmp = level.objects[x][y]; otmp; otmp = next) {
                xchar oldox = otmp->ox, oldoy = otmp->oy;
                next = otmp->nobj;
                t_collect(otmp);
                if (otmp->transient)
                    newsym(oldox, oldoy);
            }
        }
    }
    /* monster inventories */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        for (otmp = mtmp->minvent; otmp; otmp = next) {
            next = otmp->nobj;
            if (otmp->transient && otmp->oclass == ARMOR_CLASS) {
                mtmp->misc_worn_check &= ~otmp->owornmask;
                if (otmp->owornmask)
                    update_mon_intrinsics(mtmp, otmp, FALSE, FALSE);
            } else if ((otmp->owornmask & W_WEP) != 0L) {
                mtmp->mw = (struct obj *) 0;
            }
            t_collect(otmp);
        }
    }
    /* buried objects */
    for (otmp = level.buriedobjlist; otmp; otmp = next) {
        next = otmp->nobj;
        t_collect(otmp);
    }
    /* your inventory */
    for (otmp = invent; otmp; otmp = next) {
        next = otmp->nobj;
        if (otmp != exception) {
            if (otmp->transient && otmp->owornmask)
                setnotworn(otmp);
            t_collect(otmp);
        }
    }
#undef t_collect
    return collected;
}

/* ######################################################################
 * DEVTEAM QUEST FUNCTIONS
 * ###################################################################### */

/* The DevTeam sends the player on a quest to find the missing scrolls
 * containing their source code.
 * thrownscroll is a scroll of missing code if and only if the player actually
 * threw it at the devteam member. If thrownscroll is null, the player must be
 * chatting to the special "leader" devteam member. If non-null, it might be any
 * devteam member. */
void
devteam_quest(devteam, thrownscroll)
struct monst *devteam;
struct obj *thrownscroll;
{
    xchar qstatus = tnnt_globals.devteam_quest_status;
    boolean is_leader = !strcmp(MNAME(devteam), "Mike Stephenson");

    if (u.uhave.amulet && !thrownscroll && qstatus == DTQUEST_COMPLETED) {
        com_pager(QT_DEVTEAM_AOY_REACTION);
        return;
    }

    if (thrownscroll && !(is_leader && (Deaf || qstatus == DTQUEST_INPROGRESS))) {
        /* Throwing to leader with the quest in progress is OK and will make him
         * accept the scroll. Throwing to leader while Deaf will make him accept
         * the scroll even to begin the quest (because the player could be
         * permadeaf and unable to #chat).
         * Any other scenario doesn't work, but won't anger the devteam. */
        const char* annoyance[] = { "peeved", "indignant", "irritated" };
        pline_The("scroll bounces off %s's head and falls to the floor.", mon_nam(devteam));
        pline("%s looks %s.", Monnam(devteam), annoyance[rn2(SIZE(annoyance))]);
        place_object(thrownscroll, devteam->mx, devteam->my);
        if (is_leader && !Deaf) {
            verbalize("Why don't you just #chat to me like a normal adventurer?");
        }
        return;
    }
    if (qstatus == DTQUEST_NOTSTARTED) {
        /* Even if you manage to find all the scrolls before finding the actual
         * devteam, this will still only assign the quest and it requires a
         * second #chat to complete it. */
        if (Deaf)
            pline("%s signs:", Monnam(devteam));
        com_pager(QT_DEVTEAM_GIVENQUEST);
        qstatus = tnnt_globals.devteam_quest_status = DTQUEST_INPROGRESS;
    }
    /* not else if; starting the quest by throwing the scroll when Deaf could
     * hit the if immediately above and then should go into this one */
    if (qstatus == DTQUEST_INPROGRESS) {
        /* Fork over any scrolls of missing code.
         * Mark them as completed in missing_scroll_levs, by removing the
         * number.  See allmain.c - the scrolls SHOULD be all on different
         * levels so there should be no duplicates in the array, but I realized
         * that this doesn't actually require them to be on different levels.
         * Hopefully, if these scrolls become stackable in the future, this code
         * is tolerant of it - the useup() will remove one, then the next
         * carrying() will find the remaining one(s).
         */
        int i;
        int nscrolls_given = 0;
        xchar scrolls_remaining = 0,
              nextlevel; /* they tell you where to look next */
        struct obj* scroll = (thrownscroll ? thrownscroll : carrying(SCR_MISSING_CODE));
        while (scroll) {
            int dt_level = scroll->corpsenm;
            boolean matched;

            if (dt_level <= 0) {
                /* wizmode wished scroll? */
                impossible("scroll marked with bad level?");
                return;
            }

            matched = FALSE;
            for (i = 0; i < NUM_MISSING_CODE_SCROLLS; ++i) {
                if (tnnt_globals.missing_scroll_levels[i] == dt_level) {
                    tnnt_globals.missing_scroll_levels[i] = 0;
                    nscrolls_given++;
                    matched = TRUE;
                    break;
                }
            }
            if (!matched) {
                impossible("don't remember sending a scroll to level %d?", dt_level);
                nscrolls_given++; /* treat it as accepted anyway */
            }

            if (thrownscroll)
                obfree(scroll, NULL);
            else
                useup(scroll);
            scroll = (thrownscroll ? NULL : carrying(SCR_MISSING_CODE));
        }
        for (i = 0; i < NUM_MISSING_CODE_SCROLLS; ++i) {
            if (tnnt_globals.missing_scroll_levels[i] != 0) {
                /* still scrolls out there that haven't been handed over */
                scrolls_remaining++;
                nextlevel = tnnt_globals.missing_scroll_levels[i];
            }
        }
        if (nscrolls_given == 0) {
            verbalize("How is your search going?");
            /* make them actually find the first one... no message when none
             * have been found yet. */
            if (scrolls_remaining < NUM_MISSING_CODE_SCROLLS) {
                verbalize(
                   "Paxed thinks a scroll might have ended up on level %d...",
                          nextlevel);
            }
        }
        else {
            if (thrownscroll) {
                pline("%s catches the scroll.", Monnam(devteam));
            }
            else {
                pline("%s gratefully takes the scroll%s from you.", Monnam(devteam),
                    (nscrolls_given > 1 ? "s" : ""));
            }
            if (scrolls_remaining > 0) {
                /* TNNT TODO: make better. */
                if (Deaf)
                    pline("%s signs:", Monnam(devteam));
                verbalize("Thank you. There should only be %d more scroll%s.",
                          scrolls_remaining, (scrolls_remaining > 1 ? "s" : ""));
            }
            else {
                /* finished!!! */
                struct obj *reward;
                tnnt_globals.devteam_quest_status = DTQUEST_COMPLETED;
                tnnt_achieve(A_FINISHED_DEVTEAM_QUEST);
                if (tnnt_globals.killed_izchak) {
                    /* credit for finishing the quest, but no reward */
                    com_pager(QT_DEVTEAM_BADFINISH);
                }
                else {
                    if (Deaf)
                        pline("%s signs:", Monnam(devteam));
                    com_pager(QT_DEVTEAM_FINISHQUEST);
                    reward = mksobj(T_SHIRT, FALSE, FALSE);
                    reward = oname(reward, artiname(ART_REALLY_COOL_SHIRT));
                    /* player should have just given up at least one scroll, so
                       should have room for this in inventory, but might get
                       encumbered and want to decline :d */
                    dropy(reward);
                    pickup_object(reward, 1, FALSE);
                }
                livelog_printf(LL_ACHIEVE, "completed the DevTeam Quest");
            }
        }
    }
    else if (qstatus == DTQUEST_COMPLETED) {
        if (!tnnt_globals.killed_izchak) {
            verbalize("Thank you again for finding our lost code.");
        }
        return;
    }
    else {
        impossible("weird devteam quest status?");
        return;
    }
}

/* ######################################################################
 * TNNT ACHIEVEMENTS SYSTEM
 * ###################################################################### */

#ifdef TNNT_ACHIEVEMENTS_DIR
static char*
get_temp_achfile_path(void)
{
    static char buf[BUFSZ];
    Sprintf(buf, "%s/%s.tach.", TNNT_ACHIEVEMENTS_DIR, plname);
#ifdef SERVER_LOCATION
    /* hardfought specific assumption: SERVER_LOCATION is "us.hardfought.org"
     * or "eu" or "au"
     * general assumption: SERVER_LOCATION contains no characters invalid for
     * file paths */
    strncat(buf, SERVER_LOCATION, 2);
    Strcat(buf, ".");
#endif
    Strcat(buf, "txt");
    return buf;
}

void
erase_temp_achievements_file(void)
{
    char *fname = get_temp_achfile_path();
    if (unlink(fname) == -1 && errno != ENOENT)
        impossible("can't unlink temp achievements file (%d)", errno);
}
#endif

/* This used to be a macro that just set the bit in tnnt_achievements, but now
 * we also write out to a file. */
void
tnnt_achieve(achvmt)
short achvmt;
{
    char* fname;
    FILE *achfile;
    int i;
    const char *achnam = (const char *) 0;

    if (achvmt == NO_TNNT_ACHIEVEMENT) {
        /* formerly this was used to prompt reading from encodeachieve() for
         * achievements tracked by vanilla. Now we just track them as TNNT
         * achievements, so this shouldn't happen anymore. */
        impossible("tnnt_achieve called with null achievement");
        return;
    }

    if (tnnt_is_achieved(achvmt))
        return; /* nothing to update */

    tnnt_globals.tnnt_achievements[(achvmt) / 64] |= 1L << ((achvmt) % 64);
    achnam = tnnt_achievements[achvmt].name;

    if (flags.notify_achievements && achnam
	/* since Mines' End tends to have other gray stones to mislead you from
	 * the luckstone, don't identify it by printing this (there
	 * are ways around it, but don't be blatantly obvious) */
	&& achvmt != A_GOT_LUCKSTONE) {
        const char *endpunct = "";
        int ln = (int) strlen(achnam);
        /* some achievements have their own punctuation, so only append
            * additional punctuation if that isn't the case */
        if (ln > 0 && !index(".!?", achnam[ln - 1]))
            endpunct = ".";
        pline("Achievement unlocked: \"%s\"%s", achnam, endpunct);
    }

    /* don't write temp achievements file for explore-mode games */
    if (discover)
        return;

#ifdef TNNT_ACHIEVEMENTS_DIR
    fname = get_temp_achfile_path();
    achfile = fopen(fname, "w");
    if (!achfile) {
        impossible("Error writing player achievements data to '%s' file", fname);
        return;
    }
    fprintf(achfile, "0x%lx\n", encodeachieve());
    for (i = 0; i < SIZE(tnnt_globals.tnnt_achievements); ++i) {
        fprintf(achfile, "0x%" PRIx64 "\n", tnnt_globals.tnnt_achievements[i]);
    }
    fclose(achfile);
#endif /* TNNT_ACHIEVEMENTS_DIR */
}

/* ######################################################################
 * MISCELLANEOUS UTILITY FUNCTIONS FOR ACHIEVEMENTS
 * ###################################################################### */

/* Return TRUE if there are any Minetown Watch monsters on the current level
 * (which we presume to be Minetown but doesn't have to be), FALSE if there
 * aren't */
boolean
tnnt_is_watch_present()
{
    struct monst *mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (is_watch(mtmp->data)) {
            return TRUE;
        }
    }
    return FALSE;
}

/* are you in the main non-Gehennom dungeon? */
boolean
In_dungeons_of_doom(lev)
d_level *lev;
{
    return (boolean) (lev->dnum == dungeons_dnum);
}

/* are you in Minetown? */
boolean
Is_minetown(lev)
d_level *lev;
{
    s_level *slev = Is_special(lev);
    return (slev && !strcmp(slev->proto, "minetn"));
}

/* player has just disarmed one of the types of traps. Add it and check to see
 * if they earned the achievement. */
void
tnnt_add_untrap(mask)
unsigned int mask; /* one of the TNNT_UNTRAP_* */
{
    tnnt_globals.untrapped_types |= mask;
    if ((tnnt_globals.untrapped_types & ALL_UNTRAPPABLE_TRAPTYPES)
        == ALL_UNTRAPPABLE_TRAPTYPES)
        tnnt_achieve(A_UNTRAPPED_ALL_TRAPS);
}

/* A door at this point is resisting (either from opening or closing).
 * Update accordingly and maybe award achievement. */
void
tnnt_door_resists(x, y)
xchar x, y; // TNNT TODO FOR 3.7: use coordxy
{
    /* The resists don't necessarily have to happen all at once (e.g. player can
     * be having the door resist, walk away to fight a monster, and come back to
     * resume trying the door), but getting a resist from another door will
     * break the streak on the previous door */
    if (tnnt_globals.door_attempt_ledger == ledger_no(&u.uz)
        && tnnt_globals.door_attempt_x == x
        && tnnt_globals.door_attempt_y == y) {
        tnnt_globals.consecutive_door_resists++;
        if (tnnt_globals.consecutive_door_resists >= TNNT_DOOR_RESIST_GOAL)
            tnnt_achieve(A_DOOR_RESIST_8X);
    }
    else { /* new door */
        tnnt_globals.door_attempt_ledger = ledger_no(&u.uz);
        tnnt_globals.door_attempt_x = x;
        tnnt_globals.door_attempt_y = y;
        tnnt_globals.consecutive_door_resists = 1;
    }
    if (tnnt_globals.consecutive_door_resists > tnnt_globals.door_resist_max)
        tnnt_globals.door_resist_max = tnnt_globals.consecutive_door_resists;
}

/* Return true if a given monster counts towards certain achievements (the ones
 * for killing a certain amount of species). */
static boolean
tnnt_common_monst(mndx)
int mndx;
{
    /* Rule 0: We don't want anything after SPECIAL_PM (long worm tail, player
     * monsters, quest guardians/nemeses/leaders). This also excludes the
     * off-by-ones of PM_NINJA and PM_CAVEWOMAN. */
    if (mndx >= SPECIAL_PM)
        return FALSE;

    /* Rule 1: Uniques tend to have their own achievements already, and aren't
     * really "species". High priests are the exception, because they aren't
     * really unique. */
    if ((mons[mndx].geno & G_UNIQ) && mndx != PM_HIGH_PRIEST)
        return FALSE;

    switch (mndx) {
        /* Rule 2: Duplicates of other monsters that should be reduced down
         * already. For weres, only the PM_HUMAN_WERE* counts. */
        case PM_WERERAT:
        case PM_WEREJACKAL:
        case PM_WEREWOLF:
        /* Rule 3: You shouldn't have to kill nonthreatening humans. */
        case PM_SHOPKEEPER:
        case PM_GUARD:
        case PM_WATCHMAN:
        case PM_WATCH_CAPTAIN:
        case PM_PRISONER:
        case PM_DEVTEAM_MEMBER:
        /* Rule 4: No monsters that might never be generated in a game.
         * If none generate, the player shouldn't have to go out of their
         * way to engineer their generation. */
        case PM_WATER_TROLL:
        case PM_WATER_DEMON:
        case PM_QUEEN_BEE:
        case PM_GIANT:
        case PM_ELF:
        case PM_ORC:
        case PM_HUMAN:
#ifdef MAIL
        case PM_MAIL_DAEMON:
#endif
            return FALSE;
        /* There used to be a Rule 5 about not counting baby versions of
         * monsters at separate species, but we currently have it as
         * counting the baby versions separately.
         */
    }
    return TRUE;
}

/* The player personally just killed one of monster mndx. Update ukilled, doing
 * any filtration or translation as necessary, then process achievements that
 * trigger based on the player personally killing stuff. */
void
tnnt_update_ukilled(mndx)
int mndx;
{
    int i, ct = 0;
    int32_t lowercase_killed = 0x0, uppercase_killed = 0x0;
    uint16_t dragons_killed = 0x0, baby_dragons_killed = 0x0;
    boolean missedany = FALSE;
    /* First: translate monsters that exist in multiple forms. */
    if (mndx == PM_WERERAT)
        mndx = PM_HUMAN_WERERAT;
    else if (mndx == PM_WEREJACKAL)
        mndx = PM_HUMAN_WEREJACKAL;
    else if (mndx == PM_WEREWOLF)
        mndx = PM_HUMAN_WEREWOLF;

    if (mvitals[mndx].ukilled < 255)
        mvitals[mndx].ukilled++;

    /* Count the number of monster species that have had at least 1 monster
     * killed. */
    for (i = LOW_PM; i < SPECIAL_PM; ++i) {
        /* Certain species don't count */
        if (!tnnt_common_monst(i))
            continue;

        if (mvitals[i].ukilled) {
            ct++;
            if (ct >= 25)
                tnnt_achieve(A_KILLED_25_SPECIES);
            if (ct >= 50)
                tnnt_achieve(A_KILLED_50_SPECIES);
            if (ct >= 100)
                tnnt_achieve(A_KILLED_100_SPECIES);
            if (ct >= 200)
                tnnt_achieve(A_KILLED_200_SPECIES);

            /* a to z achievement */
            if (mons[i].mlet >= S_ANT && mons[i].mlet <= S_ZRUTY) {
                xchar offset = (mons[i].mlet - S_ANT);
                lowercase_killed |= (1 << offset);
                if (lowercase_killed == 0x03FFFFFF) /* low 26 bits */
                    tnnt_achieve(A_KILLED_A_Z_LOWERCASE);
            }
            /* A to Z achievement */
            if (mons[i].mlet >= S_ANGEL && mons[i].mlet <= S_ZOMBIE) {
                xchar offset = (mons[i].mlet - S_ANGEL);
                uppercase_killed |= (1 << offset);
                /* low 26 bits, EXCEPT the one for 'I' since that is not a
                 * monster class. (Bit for 'I' is 1 << ('I'-'A') = 0x100) */
                if (uppercase_killed == 0x03FFFEFF)
                    tnnt_achieve(A_KILLED_A_Z_UPPERCASE);
            }
            /* Kill all baby dragons achievement */
            if (i >= PM_BABY_GRAY_DRAGON && i <= PM_BABY_YELLOW_DRAGON) {
                xchar offset = i - PM_BABY_GRAY_DRAGON;
                baby_dragons_killed |= (1 << offset);
                /* TNNT TODO FOR 3.7: with the introduction of gold dragons,
                 * 0x1ff will change to 0x3ff, both for this one and for
                 * A_KILLED_ALL_DRAGONS a couple lines down */
                if (baby_dragons_killed == 0x1ff)
                    tnnt_achieve(A_KILLED_ALL_BABY_DRAGONS);
            }
            /* Kill all dragons achievement */
            if (i >= PM_GRAY_DRAGON && i <= PM_YELLOW_DRAGON) {
                xchar offset = i - PM_GRAY_DRAGON;
                dragons_killed |= (1 << offset);
                if (dragons_killed == 0x1ff)
                    tnnt_achieve(A_KILLED_ALL_DRAGONS);
            }
        }
        else {
            missedany = TRUE;
        }
    }
    /* Checks that should only happen after the whole mons array has been gone
     * through. */
    if (!missedany)
        tnnt_achieve(A_KILLED_ALL_SPECIES);
}

/* Would identifying otyp count towards an "identify multiple items of a
 * set" achievement? If so, return that achivement; if not, return
 * NO_TNNT_ACHIEVEMENT.
 * The only collisions currently are from the "both detect food sources" with
 * "all scrolls" and "all spellbooks". This only returns the latter. */
#define FIRST_GEM DILITHIUM_CRYSTAL /* same as in end.c */
int
tnnt_id_achvmt(otyp)
short otyp;
{
    /* for performance, catch all the specific items that do NOT count towards
     * anything first */
    switch (otyp) {
    case AMULET_OF_YENDOR:
    case FAKE_AMULET_OF_YENDOR:
    case POT_WATER:
    case SCR_BLANK_PAPER:
    case SCR_MISSING_CODE:
#ifdef MAIL
    case SCR_MAIL:
#endif
    case SPE_BLANK_PAPER:
    case SPE_NOVEL:
    case ROCK:
        return NO_TNNT_ACHIEVEMENT;
    }

    switch (objects[otyp].oc_class) {
    case ARMOR_CLASS:
        /* Armor achievements have a lot of ranges, unfortunately. */
        /* TNNT TODO FOR 3.7: At least one of these ranges has changed (helm of
         * brilliance is no longer shuffled, and comes before the shuffled ones;
         * identifying brilliance should still be required for the achievement).
         * Review and fix these ranges. */
        if ((otyp >= CORNUTHAUM && otyp <= DUNCE_CAP)
            || (otyp >= HELMET && otyp <= HELM_OF_TELEPATHY))
            return A_IDENTIFIED_ALL_HELMS;
        if (otyp >= CLOAK_OF_PROTECTION && otyp <= CLOAK_OF_DISPLACEMENT)
            return A_IDENTIFIED_ALL_CLOAKS;
        if (otyp >= LEATHER_GLOVES && otyp <= GAUNTLETS_OF_DEXTERITY)
            return A_IDENTIFIED_ALL_GLOVES;
        if (otyp >= SPEED_BOOTS && otyp <= LEVITATION_BOOTS)
            return A_IDENTIFIED_ALL_BOOTS;
        break;
    case RING_CLASS:
        return A_IDENTIFIED_ALL_RINGS; /* no exceptions */
    case WAND_CLASS:
        if (OBJ_NAME(objects[otyp]) == 0) /* exclude random shuffled names */
            return NO_TNNT_ACHIEVEMENT;
        else
            return A_IDENTIFIED_ALL_WANDS;
    case AMULET_CLASS:
        return A_IDENTIFIED_ALL_AMULETS;
    case POTION_CLASS:
        return A_IDENTIFIED_ALL_POTIONS;
    case SCROLL_CLASS:
        if (OBJ_NAME(objects[otyp]) == 0) /* exclude random shuffled names */
            return NO_TNNT_ACHIEVEMENT;
        else
            return A_IDENTIFIED_ALL_SCROLLS;
    case SPBOOK_CLASS:
        return A_IDENTIFIED_ALL_BOOKS;
    case GEM_CLASS:
        if (otyp >= LUCKSTONE && otyp <= FLINT)
            return A_IDENTIFIED_ALL_STONES;
        else if (otyp > LAST_GEM && otyp < LUCKSTONE)
            /* Ugh, this is a bad condition, it relies on glass coming after gems
             * and before gray stones in objects[], but they don't get constants
             * defined for them so there's no better way... */
            return A_IDENTIFIED_ALL_GLASS;
        else if (otyp >= FIRST_GEM && otyp <= LAST_GEM)
            return A_IDENTIFIED_ALL_GEMS;
        break;
    case TOOL_CLASS:
        if (otyp >= SACK && otyp <= BAG_OF_TRICKS)
            return A_IDENTIFIED_ALL_BAGS;
        else if (otyp >= TOOLED_HORN && otyp <= HORN_OF_PLENTY)
            return A_IDENTIFIED_ALL_HORNS;
        else if (otyp == TALLOW_CANDLE || otyp == WAX_CANDLE
                 || otyp == OIL_LAMP || otyp == MAGIC_LAMP)
            /* can't simply use tallow...magic lamp range because brass lantern
             * falls in the middle of it and does not need to be identified;
             * this causes the discoveries list to show a phantom 1/5 identified
             * instead of not showing anything */
            return A_IDENTIFIED_ALL_LIGHTS;
        /* magic instruments are a mess because there aren't any consecutive
         * entries. Horns are their own achievement so this doesn't include fire
         * or frost horns. */
        else if (otyp == MAGIC_WHISTLE || otyp == MAGIC_FLUTE
                 || otyp == MAGIC_HARP || otyp == DRUM_OF_EARTHQUAKE)
            return A_IDENTIFIED_ALL_MAGIC_INSTRUMENTS;
        break;
    }
    return NO_TNNT_ACHIEVEMENT;
}

/* Various checks for if the player has identified all of some set of
 * objects to earn an achievement, prompted by otyp having just been
 * identified.
 * This used to be checked at end-of-game, but there are some reasons for doing
 * it here: the player might have identified the full set earlier, and then got
 * hit with amnesia and doesn't remember them anymore at the end, and the
 * achievement isn't otherwise trackable until the xlogfile is written.
 */
void
tnnt_check_identifications(otyp)
int otyp;
{
    const char oclass = objects[otyp].oc_class;
    int tmp_otyp;
    int this_achvmt;
    if (otyp == SPE_DETECT_FOOD || otyp == SCR_FOOD_DETECTION) {
        /* special case: identify both of these for an achievement.
         * Note that it is possible to start the game (as a Wizard) with
         * both of these; in this case, since tnnt_check_identifications is
         * called only during the game and on items that aren't yet known, it
         * will require amnesia to "forget" one or both of them and then
         * re-identify it. But this combination should be pretty rare.
         * TNNT TODO FOR 3.7: amnesia won't exist anymore, so update this
         * comment to reflect that a wizard starting with both of these will
         * just be out of luck. (That's preferred versus allowing an achievement
         * to be earned just by starting the game.)
         */
        if (objects[SCR_FOOD_DETECTION].oc_name_known
            && objects[SPE_DETECT_FOOD].oc_name_known) {
            tnnt_achieve(A_ID_DETECT_FOOD);
        }
        /* don't return; check scroll/spellbook class for completion */
    }

    this_achvmt = tnnt_id_achvmt(otyp);
    if (this_achvmt == NO_TNNT_ACHIEVEMENT)
        return;

    for (tmp_otyp = bases[(int) oclass]; tmp_otyp < bases[oclass + 1];
         ++tmp_otyp) {
        if (tnnt_id_achvmt(tmp_otyp) == this_achvmt
            && !objects[tmp_otyp].oc_name_known)
            return;
    }
    /* if we got here, we must have oc_name_known on all objects which map to
     * this_achvmt */
    tnnt_achieve(this_achvmt);
}

/* Give credit for permanently lighting a position in a previously-unlit
 * room. */
void
tnnt_roomlit_credit(x, y)
int x, y; /* TNNT TODO FOR 3.7: these should be coordxy */
{
    if (levl[x][y].lit == 0 && In_dungeons_of_doom(&u.uz)
        && (levl[x][y].roomno >= ROOMOFFSET || Is_bigroom(&u.uz))
        && !tnnt_is_achieved(A_LIT_20_ROOMS)) {
        int i;
        for (i = 0; i < TNNT_LITROOM_GOAL; ++i) {
            if (tnnt_globals.dark_rooms_lit[i].ledgerno > 0) {
                /* hope this doesn't interact weirdly with SHARED */
                unsigned short lno = tnnt_globals.dark_rooms_lit[i].ledgerno,
                               rno = tnnt_globals.dark_rooms_lit[i].roomno;
                if (lno == ledger_no(&u.uz) && rno == levl[x][y].roomno)
                    /* already credited for lighting this room */
                    break;
                /* else if index i already stores a room, but it's not the
                 * current room, just move on and check the next one */
                continue;
            }
            /* this is a new room we're lighting! */
            tnnt_globals.dark_rooms_lit[i].ledgerno = ledger_no(&u.uz);
            tnnt_globals.dark_rooms_lit[i].roomno = levl[x][y].roomno;
            if (i == TNNT_LITROOM_GOAL - 1)
                tnnt_achieve(A_LIT_20_ROOMS);
            break;
        }
    }
}

/* Return the index of one of the tracked uniques in
 * tnnt_globals.unique_info[] */
int
tnnt_uniqndx(int mndx)
{
    int i;
    for (i = 0; i < NUM_UNIQUES_TRACKED; ++i) {
        if (tnnt_globals.unique_info[i].mndx == mndx) {
            return i;
        }
    }
    impossible("tnnt_uniqndx: no unique found for mndx %d", mndx);
    return 0;
}

/* Grant the "Captain Ahab" achievement (A_SURVIVED_DROWNING) if the hero is
 * being grabbed by a sea monster with a drowning attack.  This function is
 * called just before zeroing out u.ustuck. */
void
tnnt_maybe_grant_ahab()
{
    if (u.ustuck && !u.uswallow && !sticks(youmonst.data)
        && u.ustuck->data->mlet == S_EEL
#if 0
        /* unnecessary I think because no other scenario but attempted
         * drowning would lead to a sea monster becoming u.ustuck(?) */
        && dmgtype(u.ustuck->data, AD_WRAP)
#endif
        ) {
        tnnt_achieve(A_SURVIVED_DROWNING);
    }
}

/* Grant "The Great Heist" achievement if player has entered every vault in the
 * game, and there are no more levels that might generate another vault. */
void
tnnt_maybe_award_heist()
{
    d_level lvl = u.uz;
    int i;
    int not_entered = 0;
    int entered = 0;
    if (!In_dungeons_of_doom(&lvl)) {
        impossible("checking vaults not in Dungeons?");
        return;
    }
    for (i = 1; i <= dunlevs_in_dungeon(&u.uz); ++i) {
        lvl.dlevel = i;
        if (tnnt_globals.vault_status[i] == LEVEL_NOT_GENERATED) {
            if (Is_special(&lvl)) {
                /* Medusa, Castle etc; we already know this level will not have
                 * a vault, so we can just fix it up */
                tnnt_globals.vault_status[i] = LEVEL_HAS_NO_VAULT;
            }
            else {
                /* not enough information to tell if we have plundered all
                 * vaults */
                return;
            }
        }
        else if (tnnt_globals.vault_status[i] == VAULT_NOT_ENTERED)
            not_entered++;
        else if (tnnt_globals.vault_status[i] == VAULT_ENTERED)
            entered++;
    }
    /* we do need to check that at least one vault has been entered; in the
     * extremely rare case that a dungeon contains no vaults, the achievement
     * cannot be earned */
    if (not_entered == 0 && entered > 0) {
        tnnt_achieve(A_ENTERED_ALL_VAULTS);
    }
}
