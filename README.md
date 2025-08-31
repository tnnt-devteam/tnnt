## TNNT - The November NetHack Tournament

TNNT is a fork of the dungeon exploration game NetHack designed for tournament
play. Its website is https://tnnt.org, and the tournament runs each November on
the Hardfought public NetHack servers at https://hardfought.org.

For more information on NetHack, visit https://nethack.org, and for the NetHack
public Git repository, see https://github.com/NetHack/NetHack.

TNNT is intended to be a "vanilla" tournament focused on the accomplishments
possible in the base NetHack game, a counterpart to the Junethack tournament
that focuses on variants. To that end, most of the changes in TNNT's code
provide additional telemetry for the tournament with no impact on the gameplay.
However, the gameplay is not entirely unchanged from NetHack, because there are
also several features added that provide additional tournament challenges and
additional ways for influencing other players' games besides bones files.

TNNT's public Git repository can be accessed at
https://github.com/tnnt-devteam/tnnt and it also has a page on NetHackWiki at
https://nethackwiki.com/wiki/The_November_NetHack_Tournament.

## Supported Builds

Because we mainly care about running TNNT as a server build, we only support
building the game on Linux with the text-based windowports. You're welcome to
try building it for other architectures or windowports supported by NetHack via
the instructions provided in the `README` file also found in this directory,
which is the unchanged README provided by the NetHack devteam. But the build
might fail, or the tournament features might not work correctly, and it won't
be a priority for us to fix those issues.

We do keep the TNNT tileset up to date, though, because Hardfought supports
playing TNNT in a browser using web tiles (which uses a text based windowport
and vt_tiledata under the hood).
