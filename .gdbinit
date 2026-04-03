# Prevent GDB from downloading debug symbols for system libraries
set debug-file-directory /nonexistent
set auto-load python off
set auto-load local-gdbinit off
set auto-load libthread-db off
set skip-all-files on
skip file /lib/*
skip file /usr/lib/*
skip library
handle SIGTRAP pass noprint nostop
handle SIGPIPE pass noprint nostop
handle SIGTERM pass noprint nostop
