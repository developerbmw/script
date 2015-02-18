# script
Scripting language interpreter written in C++

Syntax similar to JavaScript

Parses script into 'operations' similar to x86 assembly, then these operations are processed by the 'CPU'. This design decision should make it easier to make this a compiler if wanted.

Designed and built with simplicity in mind rather than speed/optimization (at this point)