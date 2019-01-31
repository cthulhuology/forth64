Forth64 
=======

The concept behind this project is to explore creating a new 64bit forth
that runs on modern hardware.  The goal will be native compilation on a
couple different architectures, and basic support for common operating
systems.  Currently, development is happening in a Slackware Docker 
container, with a basic Linux target being the goal.

About the Design
----------------

To start with, forth64 will have a MISC defined for it, and the C program
will compile forth64 to that MISC instruction set and then interpret it.
The MISC interpreter will then compile a forth64 source listing that 
compiles a forth64 compiler.  At this point we can throw away the C program
and set aside the MISC instruction set.

To generate the appropriate x86_64 instructions from our forth64 compiler,
we need to establish the most appropriate encodings for our use case. Towards
this end, the MISC will also be implemented in x86_64 assembler, and we will
use that output to inform our compiler encodings.  Once we have 1-to-1 parity
running the C test suite with the assembler encodings, we can be confident
that our final compiler will behave correctly.

About the Source
----------------

There are two current tracks of development:

1.) A assembler source which is being used to experiment with instruction
encoding and data organization concepts.

2.) A C source which is being use to experiment with operand behavior and
linux system interface and system bootstrapping.

Neither source file is currently targeted for the final product, rather 
they are a means to explore the design space.  The goal of each is to 
ease the process of bootstrapping.  The finial interpreter will be written
in forth64 running on forth64.  The C binary will allow us to interpret
our forth64 implementation generating the native binary.  We can then use
the resulting binary to recompile the source, and iff all goes well, the
resulting binary will be identitical.  At that point, future development
of forth64 can be done entirely in forth64.

About the Language
------------------

Forth64 will not be ANSI Forth.  In fact, a lot of the instruction design
and behavior is incorporating design elements from Chuck Moore's later work
including Colorforth and Okad.  That said, it will not be a colorforth or
even a spiritual successor.  The design of the language itself will be 
aimed at practical applications, interfacing with existing software systems.

Once the language and implementation stablize, it will be ported to 
architectures other than x86_64.  The MISC will hopefully be reimplemented
in Verilog and run on a FPGA.  The main compiler will be ported to both
ARMv8 and RISC-V instructions sets.

Inititally the design space for the language will be what is necessary 
to implement both embedded applications and network connections between 
devices.  A user interface code base may also be attempted once the
system and ffi implementations are operational.  

LICENSE
-------

Copyright 2019 David J. Goehrig <dave@dloh.org>

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:  The above copyright 
notice and this permission notice shall be included in all copies or substantial 
portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.	

