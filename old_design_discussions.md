# Old TNNT design discussions

This document exists to preserve design discussions about TNNT in-game features
which haven't actually been discussed in a while. The idea is that if a
suggestion comes back up for discussion, this will be a reference point for its
history, while clearing the clutter out of the forum used for active TNNT design
discussions.

The intended style is to summarize the relevant points rather than copying the
raw back-and-forth.

This doesn't cover design discussions about the scoreboard, only the TNNT game
binary itself.

While this file covers a lot of reasons why various features were rejected,
don't assume that every idea in here has been considered and rejected. Sometimes
an idea just never got enough interest from anyone to get implemented.

The discussions in this file date from prior to the 2024 tournament, and do not
cover anything during or after that tournament.

## Swap Chest

The current system (as of the 2024 tournament) for swap items is that items put
into either swap chest go into the same directory. Every so often, some items
from each server get shuffled off to other servers, with a chance based on
whether they originated on the current server or not. Also, the oldest objects
are periodically purged to prevent the list from getting bloated with junk. This
is all managed with out-of-game scripts, not from within it.

One very common criticism of the swap chest has been that it's susceptible to
abuse, i.e. a needy clan member waits at a swap chest while coordinating
out-of-game with another clan member who will drop a dragon scale mail or other
"top-tier" acceptable item in the swap chest, and the first player will
immediately take it without anyone else getting a chance at it. Many
solutions have been proposed:

- Several revolve around the idea of blocking clans swapping items amongst
  themselves. This is technically difficult because the TNNT game binary has no
  concept of clans. Adding in mysql queries to check in real time sounds insane,
  but a polling system would lag behind changes in clans. It also doesn't do
  anything to prevent players using sockpuppet accounts who aren't in the clan
  to feed items to accounts in the clan (which might sound like a waste of time,
  but the incentive for giving three dragon scale mails to clan friends at
  Mines' End is certainly high.)
- The swap chest won't offer dragon scale mail unless the would-be recipient has
  killed a dragon in their own game. This hasn't been implemented for two
  reasons: one, it only targets one type of item and other powerful items
  wouldn't be affected, and two, there's the sentiment that if you can only
  trade for items as useful as you're likely to get at your current level, swap
  chests aren't a very fun mechanic.

Another common criticism of the swap chest is that it significantly changes the
difficulty curve of the game, i.e. you can trade away a junk item and get, in
the best case, a free early-game wish (that doesn't break wishless conduct) and
which makes the rest of the early game significantly easier. It doesn't
necessarily represent a big sacrifice on the donor's side either. The main
reason nothing has been done about it is because of fears that it would sap most
of the fun out of the swap chest (both of giving someone an overpowered item,
and of getting one).
- One proposal that mitigates but doesn't completely solve this is to make the
  Valley and Mines' End chests separate item pools. Players at Mines' End are
  unlikely to give a top tier item unless they've already got a duplicate
  somehow.
- Alternatively, track the level (or maximum level) of a donor, and only show
  their items to players of equal or higher level.
- Move the Mines' End chest later in the dungeon, to somewhere around the
  devteam office (if not IN the devteam office).

One problem that arose was people camping on a swap chest and routinely looking
into it to see if there was anything good and declining to take anything if
there wasn't. This problem was bad enough that one year there was a bot camping
on a chest and reporting its contents every minute to be posted on a website to
spare players the hassle. The bot was shut down, but one proposal to stop it
from happening again was to limit the number of times you could look in the
chest.

Miscellaneous suggestions:
- Only show items of object classes that correspond to the object classes that
  were put in.
- Don't have the swap chests in fixed locations. Put the first one on a random
  square in dungeon levels 10-14 and the second one on a random square in the
  first 5 levels of Gehennom after the Valley.
- If someone puts in a duplicate item that is mergeable with something already
  in the chest, the stacks merge and the new donor's name overwrites the old
  donor's name, so the new donor will get full credit. The idea here is that
  unwanted items will stack up until they become desirable.
- If someone puts in a duplicate item that could *never* merge due to being
  non-stackable, the old item is deleted and replaced by the new one.
- The chest will simply reject having 2 of the same object type. (Though due to
  syncing among servers, it could later wind up with 2 of the same object type,
  oh well.)
- Send a mail message, or use the mail system but special case it to deliver a
  custom "you feel warm and fuzzy somehow" message, to a player when someone
  playing another game takes one of their donations.
- Swap chest deposit boxes: a different variety of sapient pearwood container
  with which you can make donations but not take anything out. The idea for this
  was it would let you have more of them around the dungeon without breaking the
  existing balance of having 2 where you can get items.
- A sapient pearwood bookshelf or some other alternate variety of container that
  only accepts spellbooks. (The original intent here was to prevent non-caster
  roles from dumping spellbooks in the chest and thus accumulating it full of
  spellbooks. Current swap chest sync and trimming rules may have mitigated that
  problem.)
- The more items you donate, the newer the pool of items you get to choose from.
  I.e. donating 1 item only lets you pick from the oldest third of items,
  donating 2 lets you pick from the oldest two thirds, and donating 3 lets you
  pick from everything.
- If you don't donate 3 items before taking an item, the chest will let you
  donate more, up to the normal total of 3.

## Deathmatch

- Add a variety of deathmatch maps so it's not always the same one.
- Pets you ascend with get recorded in the NPC data file and get spawned in as
  part of the deathmatch.

## Achievements

Some achievements that we do want to add, but only once TNNT is based on a
stable 3.7 release, are covered in tnnt\_achievements.h.

There are several guiding principles that have emerged when considering an
achievement:
1. Achievements should happen during the game, not at the end. Thus, don't
   include any achievement amounting to "end the game in a certain way" - even
   if the achievement allows you to survive with lifesaving, they just end up
   being not that interesting and there's a limit on how many you'd be able to
   get in one game. Examples:
   - Any of various Stupid Ascension Tricks: ascend, while doing Cool Thing!
   - Any of various Stupid Death Tricks: die, while doing Cool Thing!
   - Go from full HP to 0 in one turn.
   - Escape in celestial disgrace.
   - Go to heaven prematurely.
   - After getting the Mines luckstone and Sokoban prize and leaving both
     branches, never return to either.
2. Achievements should be something a player can accomplish in one NetHack game,
   which is how the achievement framework works. Things outside that framework
   are difficult to compute and can't be shown to the player in-game.
   - Have an item you donated to the swap chest taken by someone else.
   - Play 60 turns in 60 games in less than 60 minutes.
3. Achievements shouldn't be boring or tedious. This is obviously subjective,
   but try to consider the median reasonable player. If it's likely that most
   players will see it as a chore, and it isn't really that impressive to talk
   about, it may not make the cut.
   - Destroy some large amount of doors.
   - Dig out an entire level.
   - Kill 200 monsters with polearms.
   - Extinct a bunch of monster species.
4. Achievements shouldn't amount to "make a quirky wish". While players might
   wish for nonstandard things in order to accomplish achievements, such as the
   last missing piece of magic armor for Fashionista, having the esoteric wish
   be the only thing you need to get the achievement isn't interesting.
    - Wish for a boulder.
5. Achievements can be mutually exclusive with other ones, and it's fine that
   you can't get them all in a single game. There are enough interesting
   achievements (such as killing each demon lord versus bribing one away) that
   shouldn't be sacrificed so that a hypothetical player could get all of them
   at once. Similarly, it's fine for achievements to require favorable dungeon
   generation or other RNG such that they might be unachievable in a given game;
   otherwise we couldn't have Big Room or Ludios based achievements, for
   instance.

Other unsorted rejected ideas:
- Minesweeper should not be entirely locked behind Ludios; it should just
  involve disarming some number of land mines over the game, which could be the
  ones in Ludios but don't have to be.
  - That number would have to be pretty low in order to not still soft-lock
    the achievement behind Ludios existing and getting into it to disarm mines.
- The +13 and -13 Luck achievements should require sustaining that level for
  some period of time.
  - Players would already do this for +13 Luck, and would just boringly lock
    themselves in a closet for the duration of the -13 Luck.
- An achievement for getting expelled from the Quest permanently.
  - "Make your game unwinnable" achievement falls under the same principle as
    not having "die in a specific way" achievements. Plus, getting expelled from
    the Quest permanently won't be possible in 3.7.
- An achievement for identifying a loadstone with a touchstone.
  - Loadstones identify themselves pretty easily; the main challenge would be
    remembering to identify it with the touchstone before trying to drop it,
    which isn't very compelling as an achievement. Plus, you have to identify a
    loadstone anyway for Geologist.
- An achievement for finishing a game with INT\_MAX points.
  - This was more achievable in 3.4.3 with automatable pudding farms; it's
    unclear whether it's achievable now, and in any case, getting it would be a
    huge slog.
- Achievement for formally identifying an item you already had type-named with
  the correct name.
  - Given how easy it is to informally identify an item, or even just guess
    correctly when the pool is small (i.e. magic armor) this is not interesting
    enough to make the cut.
- Achievement for restoring an attribute point with restore ability.
  - Too similar to Drained and Regained.
- Achievement for attempting to levelport deeper than Gehennom goes.
  - This is commonly done to get to the Vibrating Square level, and even if you
    weren't planning to do that, the cost (1 scroll of teleportation) is
    minimal.
- Achievement for performing genocide from all available sources
  - There are only 2 sources and there's already an achievement for performing
    genocide 5 times, in which you're likely to have used both sources.
- Achievement for restoring a level or attribute point with restore ability.
  - The achievement for restoring several levels with restore ability is
    basically this and is more challenging to boot.
- "You Break It, You Bought It" achievement for breaking an object belonging to
  a shopkeeper.
  - Not distinct enough to be an achievement, this is easy to do and comparable
    to simply buying an item from a shop.

### Achievement categorization

Another popular suggestion is to put the achievements into categories so the
\#achievements command isn't a huge list that takes up several screens.
Achievements are currently sorted into loose unofficial categories in the source
code, but that's it.

### Achievement visibility from past games

Some way of distinguishing in the #achievements command which achievements the
player has earned in previous games this tournament, but haven't yet earned in
the current game, the inverse of the scoreboard's display of achievements earned
in a current game which hasn't yet finished.

This is difficult without having the game make requests into the database
somehow, or read through its own xlogfile, but that would be an expensive
operation and wouldn't account for multiple servers. Probably the simplest
implementation is to write a persistent file into the data directory (probably
in a new subdirectory like donors, because you'd have one for each player) that
stores the bitwise OR of all tnntachieveX xlogfile fields earned in previous
games. Upon starting a new game, this file would be read and loaded into some
data structure which is capable of telling #achievements the ones it had
previously. Of course this STILL does not solve the multi-server problem, so a
complete implementation would need to sync these files across servers, bitwise
ORing the fields together if they differ.

## DevTeam Quest

A common criticism of the devteam quest is that it's a pile of resources
(throne, sinks for rings and foocubi, identify scrolls, tameable pets,
spellbooks, etc) and the presence of these resources makes the game easier than
vanilla.

Miscellaneous suggestions:
- The devteam members could give you helpful in-game information, such as item
  identities or the passtune. An argument against implementing is that it would
  be throwing even more free resources (information in this case) at the player.
- The devteam level could suppress normal monster spawning. The problem with
  this is that it enables you to do things like nurse dancing and polypiling
  and have no chance of interruptions.

## Potential future custom challenges

- Suggestion that whatever is added next, it be an item sink rather than a
  source. So far, all the TNNT custom stuff has been either a source of free
  items that base NetHack doesn't have, or neutral.
- Routinely cycle out custom stuff and replace it with new stuff to keep the
  tournament fresh. The main problem here may be finding the inspiration and
  developer will to make new custom stuff every year.
- Horatius challenge; monsters are trying to cross a (wide?) bridge to get to
  some objective behind you and you must prevent them from doing so. Note: would
  have to rename "Horatius at the Bridge" achievement.
- Monty Python branch that involves defeating the Black Knight and the Rabbit of
  Caerbannog. The unique twist here would be that ghosts of players that died to
  the Rabbit in other games would pile up in your game (and possibly be buffed
  to do something relevant to the challenge).
- The Council of Nine: inspired by the devnull Waldo challenge, it involves
  exploring a secret society in the Mazes of Menace, which is possibly working
  against the devteam.
  - Chattable monsters may generate as "Councilor X", where X is either their
    species or a famous NetHack player name - Deathonastick, Dudley, etc.
  - Complete the challenge by finding or sidequesting for all nine councilors.
  - Should involve secret messages passed by fortune cookie - either the
    challenge should start and make Councilors spawn by finding (intercepting) a
    message in a fortune cookie, or one councilor asks you to pass a message to
    another via a fortune cookie.
  - A council chamber either embedded in a Dungeons of Doom level and
    inaccessible, or on a separate branch, reached very late in the challenge
    after you have gained the council's trust.submitted 4 days ago by soulreaverdan
- Challenge where there are 7 (or however many) magic gems / dilithium crystals
  / ZAPM objects scattered around the dungeon that you must collect all of to
  finish the challenge.
- Land of the Lost challenge based on the TV series of that name, because of the
  nature of this land its branch is inescapable until you complete it and get
  whatever MacGuffin enables you to escape.
- Challenge themed around Alice in Wonderland with a bunch of custom,
  appropriately themed enemies.
- Devnull-type restriction (lifted upon completing a challenge) that shopkeepers
  won't let you in unless you're wearing a shirt and shoes. Unspecified whether
  body armor or a cloak obviates the need for a shirt.
- A popular suggestion from post tournament surveys: bring back some of the
  devnull challenges such as Pac-Man, or at least lock certain NetHack things
  behind devnull-style restrictions, where you must complete a TNNT challenge
  in order to unlock it.
- Don't just keep adding new content and challenges to TNNT. Rotate them out to
  keep things fresh, even the swap chest. People have noted that new TNNT
  challenges should stop helping water down the difficulty of the game by
  providing players with extra resources, in comparison to devnull challenges
  where the extra items you could obtain from them were useless outside them.
- Restore the legacy Elf quest (minus its artifact reward). The drawback of this
  is that it's kind of a boring quest, and it would provide yet more levels full
  of loot and monsters with death drops for even more loot, when TNNT already
  throws extra loot at the player.

## Other game enhancements

- Add telemetry (really additional xlogfile fields) in the game to collect data
  on interesting things such as wands of wishing generated, total monsters
  killed, minimum AC, number of times quaffing, reading, or identifying
  something, etc. These probably wouldn't be used on the scoreboard for
  anything, they would just be for interest.
- Allow you to see the vanquished list (not the same as #species, vanquished
  shows monsters that died from any source). The reason against allowing this is
  because careful checking of it would allow players to negate a disadvantage of
  zen or permahallu conducts, that they don't know what they just killed.
- For the benefit of the NetHack Scoreboard, don't keep the version number the
  same as the release of NetHack that TNNT is based on and bump it every year so
  different tournaments can be tracked.
  - One proposal was to use the current year somewhere in the version number,
    but that won't work because each part of the version number is limited to an
    8-bit integer type.
  - The scoreboard can track different tournaments based on the year in the
    starttime of games, rather than the version, and would have to do this
    anyway since there are multiple past years in which TNNT used the same
    version.
- Address the criticism that TNNT games are ineligible for NetHack world records
  because they contain extra resources that a regular NetHack game doesn't have,
  in one of a few ways:
  - Birth option that disables TNNT branch stairs and portals from spawning or
    being traversable, and prevents devteam scrolls and swap chests from
    spawning. This would be communicated in the xlogfile.
  - Conduct that is broken if you interact with a devteam scroll, swap chest,
    etc, or enter a TNNT branch and do anything besides exit straight away.
- On the week of (USA) Thanksgiving, make randomly generated fruits appear using
  names of traditional Thanksgiving foods, such as roast turkeys and cranberry
  sauce. This might confuse players outside the USA though.
