# SiennaVM
This is a virtual machine with my own architecture based on one of my previous projects called AVM, though this is written in C, C is better than C++ thats why im rewriting it.

Reminder about the assembly language. You have to have a hex value that is larger than one digit.

So for example, if you want to use jne and jump to address 0x3, you cant do ``jne &3`` you have to use ``jne &03``