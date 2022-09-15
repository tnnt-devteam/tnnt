#!/bin/bash
#
# Ideas and some parts from the original dgl-create-chroot (by joshk@triplehelix.org, modifications by jilles@stack.nl)
# More by <paxed@alt.org>
# More by Michael Andrew Streib <dtype@dtype.org>
# Licensed under the MIT License
# https://opensource.org/licenses/MIT

# autonamed chroot directory. Can rename.
DATESTAMP=`date +%Y%m%d-%H%M%S`
TNNT_CHROOT="/opt/nethack/chroot"
NETHACK_GIT="/home/build/tnnt"
# the user & group from dgamelaunch config file.
USRGRP="games:games"
# COMPRESS from include/config.h; the compression binary to copy. leave blank to skip.
COMPRESSBIN="/bin/gzip"
# fixed data to copy (leave blank to skip)
NH_GIT="/home/build/tnnt"
NH_BRANCH="master"
# HACKDIR from include/config.h; aka nethack subdir inside chroot
NHSUBDIR="tnnt"
# VAR_PLAYGROUND from include/unixconf.h
NH_VAR_PLAYGROUND="/tnnt/var/"
# END OF CONFIG
##############################################################################

errorexit()
{
    echo "Error: $@" >&2
    exit 1
}

findlibs()
{
  for i in "$@"; do
      if [ -z "`ldd "$i" | grep 'not a dynamic executable'`" ]; then
         echo $(ldd "$i" | awk '{ print $3 }' | egrep -v ^'\(' | grep lib)
         echo $(ldd "$i" | grep 'ld-linux' | awk '{ print $1 }')
      fi
  done
}

set -e

umask 022

echo "Creating inprogress and extrainfo directories"
mkdir -p "$TNNT_CHROOT/dgldir/inprogress-tnnt"
chown "$USRGRP" "$TNNT_CHROOT/dgldir/inprogress-tnnt"
mkdir -p "$TNNT_CHROOT/dgldir/extrainfo-tnnt"
chown "$USRGRP" "$TNNT_CHROOT/dgldir/extrainfo-tnnt"

echo "Making $TNNT_CHROOT/$NHSUBDIR"
mkdir -p "$TNNT_CHROOT/$NHSUBDIR"

NETHACKBIN="$NETHACK_GIT/src/nethack"
if [ -n "$NETHACKBIN" -a ! -e "$NETHACKBIN" ]; then
  errorexit "Cannot find NetHack binary $NETHACKBIN"
fi

if [ -n "$NETHACKBIN" -a -e "$NETHACKBIN" ]; then
  echo "Copying $NETHACKBIN"
  cd "$TNNT_CHROOT/$NHSUBDIR"
  NHBINFILE="`basename $NETHACKBIN`-$DATESTAMP"
  cp "$NETHACKBIN" "$NHBINFILE"
  ln -fs "$NHBINFILE" nethack
  LIBS="$LIBS `findlibs $NETHACKBIN`"
  cd "$TNNT_CHROOT"
fi

echo "Copying NetHack playground stuff"
cp "$NETHACK_GIT/dat/nhdat" "$TNNT_CHROOT/$NHSUBDIR"
chmod 644 "$TNNT_CHROOT/$NHSUBDIR/nhdat"
cp "$NETHACK_GIT/dat/symbols" "$TNNT_CHROOT/$NHSUBDIR"
chmod 644 "$TNNT_CHROOT/$NHSUBDIR/symbols"
cp "$NETHACK_GIT/dat/NHdump.css" "$TNNT_CHROOT/$NHSUBDIR"
chmod 644 "$TNNT_CHROOT/$NHSUBDIR/NHdump.css"

echo "Copying sysconf file"
SYSCF="$TNNT_CHROOT/$NHSUBDIR/sysconf"
cp "$NETHACK_GIT/sys/unix/sysconf" "$SYSCF"
chmod 644 $SYSCF

echo "Creating NetHack variable dir stuff."
mkdir -p "$TNNT_CHROOT/$NHSUBDIR/var"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var"
mkdir -p "$TNNT_CHROOT/$NHSUBDIR/var/save"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/save"
mkdir -p "$TNNT_CHROOT/$NHSUBDIR/var/save/backup"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/save/backup"
mkdir -p "$TNNT_CHROOT/$NHSUBDIR/var/whereis"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/whereis"
mkdir -p "$TNNT_CHROOT/$NHSUBDIR/swapchest"
# chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/swapchest"
mkdir -p "$TNNT_CHROOT/$NHSUBDIR/npcfiles"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/npcfiles"

touch "$TNNT_CHROOT/$NHSUBDIR/var/logfile"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/logfile"
touch "$TNNT_CHROOT/$NHSUBDIR/var/perm"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/perm"
touch "$TNNT_CHROOT/$NHSUBDIR/var/record"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/record"
touch "$TNNT_CHROOT/$NHSUBDIR/var/xlogfile"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/xlogfile"
touch "$TNNT_CHROOT/$NHSUBDIR/var/livelog"
chown -R "$USRGRP" "$TNNT_CHROOT/$NHSUBDIR/var/livelog"

RECOVER="$NETHACK_GIT/util/recover"

if [ -n "$RECOVER" -a -e "$RECOVER" ]; then
  echo "Copying $RECOVER"
  cp "$RECOVER" "$TNNT_CHROOT/$NHSUBDIR/var"
  LIBS="$LIBS `findlibs $RECOVER`"
  cd "$TNNT_CHROOT"
fi

LIBS=`for lib in $LIBS; do echo $lib; done | sort | uniq`
echo "Copying libraries:" $LIBS
for lib in $LIBS; do
        mkdir -p "$TNNT_CHROOT`dirname $lib`"
        if [ -f "$TNNT_CHROOT$lib" ]
	then
		echo "$TNNT_CHROOT$lib already exists - skipping."
	else
		cp $lib "$TNNT_CHROOT$lib"
	fi
done

echo "Finished."

