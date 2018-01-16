# ~/.bashrc: executed by bash(1) for non-login shells.

PS1='\u@\h:\W\$ '

# `ls' enhancements:
export LS_OPTIONS='--color=auto'
eval `dircolors`
alias ls='ls $LS_OPTIONS'
alias ll='ls $LS_OPTIONS -l'

umask 022
