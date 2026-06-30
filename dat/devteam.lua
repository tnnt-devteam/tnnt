-- "The NetHack Devteam Office" by the TNNT devteam (loosely based off of 'Orc Temple' by Khor)

des.level_init({ style = "solidfill", fg = " " });

des.level_flags("mazelevel");

des.level_flags("nommap", "noteleport", "hardfloor")
des.message("You arrive to a place bustling with activity and excitement.")
des.message("You hear incoherent mumbling, something to do about 'the source code'.")

--         1         2         3         4         5         6         7
--123456789012345678901234567890123456789012345678901234567890123456789012345
des.map([[
                               -----                      
                               |...|       -----          
          ----------------------LLL-----   |...|          
          |.+.....+.....+...|..........S###+...|          
          |.|.K.K.|------...|..........|   |...|          
          |--------  ---....|-F--++--F-|--------          
        ---.....---  |......|...|..|...|....|    ---------
 --------.........----.....--...|..|...|....|    |LLLLLLL|
--........................--T..........|-+-------|L.....L|
|.........................+............+........+.L.....L|
--........................--T..........|-+-------|L.....L|
 --------.........----.....--...|..|...|......|  |LLLLLLL|
        ---.....---  |......|...|..|...|....\.|  ---------
          -------    ---....|-F--++--F-|--------          
                       --...|..........|   |...|          
                        |...|..........S###+...|          
                        -----S----------   |...|          
                            |..|           -----          
                            ----                          
]]);

des.levregion({ region = {01,09,01,09}, exclude = {0,0,0,0}, type="branch" })
des.region(selection.area(00,00,57,16), "lit")
des.non_diggable(selection.area(0,0,57,16))

des.engraving({02,09}, "burn",
   "This is my program. There are many programs like it, but this one is mine...")

--The Devteam
des.monster({ id = "DevTeam member", x=29, y=06, peaceful = 1, name = "bhaak" })
des.monster({ id = "DevTeam member", x=38, y=06, peaceful = 1, name = "paxed",
              inventory = function()
   des.object("axe")
end, keep_default_invent = true })
des.monster({ id = "DevTeam member", x=29, y=12, peaceful = 1, name = "lorimer" })
des.monster({ id = "DevTeam member", x=38, y=12, peaceful = 1, name = "ais523" })
des.monster({ id = "DevTeam member", x=43, y=07, peaceful = 1, name = "PatR" })
des.monster({ id = "DevTeam member", x=44, y=12, peaceful = 1, name = "Mike Stephenson" })

--THE OFFICE
-- des.fountain(13,09)
des.door("closed",48,09)
des.door("closed",26,09)
des.door("open",39,09)
des.door("closed",33,05)
des.door("closed",34,05)
des.door("closed",33,13)
des.door("closed",34,13)
des.door("locked",39,03)
des.door("closed",43,03)
des.door("locked",39,15)
des.door("closed",43,15)
des.door("closed",41,08)
des.door("open",41,10)
des.door("closed",12,03)
des.door("open",18,03)
des.door("closed",24,03)

-- Centerpiece.
des.object({ id = "statue", x = 33, y = 09, montype = "balrog", name = "The RNG",
                contents = function()
   des.object({ id = "egg", name = "Easter Egg" })
end })

-- Chest with identify scrolls.
des.object({ id = "chest", x = 53, y = 09, contents = function()
   des.object({ class = "?", id = "identify", buc = 'blessed' })
   des.object({ class = "?", buc = 'blessed' })
   if percent(50) then
      des.object("?")
   end
end })
des.engraving({47,09}, "burn", "Beware - PEBCAK disposal area")

-- Some devs have books lying around.
devbooks = { {45,11},{45,12},{43,06} }
booknames = { "grep", "ls", "htop" }
shuffle(devbooks)
shuffle(booknames)
if percent(30) then
  des.object({ id = "detect monsters", coord=devbooks[0], buc='uncursed', name=booknames[0] })
end
if percent(30) then
  des.object({ id = "light", coord=devbooks[1], buc='uncursed', name=booknames[1] })
end
if percent(30) then
  des.object({ id = "detect unseen", coord=devbooks[2], buc='uncursed', name=booknames[2] })
end

-- janitor closet
des.door("locked",29,16)
if percent(30) then
  des.object("grappling hook", 30, 17)
end
if percent(30) then
  des.object("rubber hose", 30, 17)
end
if percent(30) then
  des.object("tin opener", 30, 17)
end

-- snack area
snackrack = { {32,01},{33,01},{34,01} }
shuffle(snackrack)
des.object({ id = "candy bar", coord = snackrack[0], quan = d(3) })
des.object({ id = "apple", coord = snackrack[1], quan = d(5) })
des.object({ id = "banana", coord = snackrack[2], quan = d(4) })
des.engraving({33,03}, "burn", "Due to snack theft, lava has been installed.")

-- novel
if percent(20) then
  des.object({ id = "novel", x = 11, y = 03, buc = 'uncursed' })
end

-- Funny bits that makes K2 chuckle
if percent(20) then
  des.monster({ id = "woodchuck", x=46, y=03, peaceful = 1, name = "Carl" })
end
if percent(20) then
  des.monster({ id = "kitten", x=46, y=15, peaceful = 1, name = "Poes" })
end
if percent(20) then
  des.monster({ id = "little dog", x=46, y=02, peaceful = 1, name = "Yermak" })
end
if percent(20) then
  des.monster({ id = "kitten", x=46, y=04, peaceful = 1, name = "Micka" })
end
if percent(20) then
  des.monster({ id = "little dog", x=46, y=16, peaceful = 1, name = "Fudd" })
end
if percent(20) then
  des.monster({ id = "little dog", x=46, y=14, peaceful = 1, name = "Rocky" })
end
if percent(20) then
  des.monster({ id = "lizard", peaceful = 1, name = "the Lizard of Yendor" })
end
if percent(20) then
  des.monster({ id = "troll", peaceful = 1, name = "MR SHOUTY" })
end

-- end devteam.des
