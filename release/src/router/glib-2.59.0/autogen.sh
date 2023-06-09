#!/bin/sh
# Run this to generate all the initial makefiles, etc.

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
cd "$srcdir"

if ! command -v gtkdocize >/dev/null 2>&1; then
        echo "You don't have gtk-doc installed, and thus won't be able to generate the documentation."
        rm -f gtk-doc.make
        cat > gtk-doc.make <<EOF
EXTRA_DIST =
CLEANFILES =
EOF
else
        gtkdocize || exit $?
fi

if ! command -v autoreconf >/dev/null 2>&1; then
        echo "*** No autoreconf found, please install it ***"
        exit 1
fi

# INSTALL is required by automake, but may be deleted by clean
# up rules. to get automake to work, simply touch it here. It will be
# regenerated from its corresponding *.in file by ./configure anyway.
touch INSTALL

autoreconf --force --install --verbose || exit $?

#cd "$olddir"
#test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"
