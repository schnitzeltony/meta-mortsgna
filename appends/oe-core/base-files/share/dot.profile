# ~/.profile: executed by Bourne-compatible login shells.

if [ -f ~/.bashrc ]; then
  . ~/.bashrc
fi

# Might fail after "su - myuser" when /dev/tty* is not writable by "myuser".
mesg n 2>/dev/null

# locale fallback for UTF8
if [ -z "$LANG" ]; then
    LANG="en_US.utf8"
    export LANG
fi
