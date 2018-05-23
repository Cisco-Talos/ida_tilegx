Tilera Tile-GX support for IDA Pro
==================================

This is an IDA processor module for the Tile-GX processor architecture.
The code leverages libbfd and libopcodes for translating to assembler.
It has a lot of rough edges, i.e., don't expect it to behave completely correct
if an instruction inside a packed instruction diverts the control flow. IDA Pro
doesn't reflect packed instructions at all (They are currently represented by
a leading '+' expressing that the instruction is packed with previous
instructions).

Building and installing
=======================
The build process has been tested with IDA Pro 7.3 on Linux (Ubuntu 18.04).
Create a file `idacfg.mk` in this directory and set the variables
```
idabin=...
idasdk=...
```
to the IDA installation directory and the SDK directory.

Then run 
```
make -f Makefile.linux install
```
to build and install the plugin. Currently only the Linux makefile is working,
building on Windows or MacOS is not supported.

Using
=========
Start ida64. Select "Tilera Tile-GX" as processor in the dialog when opening a
file. Accept to change the processor type. Disassembly will be slow.

License
=========
This project is licensed under Apache-2.0.

Author
=======
Jonas Zaddach <jzaddach@cisco.com>. Based on a processor module for hexagon by
Willem Hengeveld (itsme@gsmk.de) and IDA Pro SDK sample code.
