+-----------------------------+
|     DL ISO-8583 Library     |
|-----------------------------|
|   v0.0.4 - 16th July 2008   |
+-----------------------------+

CONTENTS
--------

This package contains the following source code files:

  - demo.c

      Demo application source, with message packing/unpacking example

  - benchmark.c
  
      Benchmark source, to benchmark pack/unpack operations

  - dl_c_iso8583_v0_0_3 (16th July 2008)

      DL ISO-8583 library source files

  - dl_c_common_v0_0_2 (14th October 2007)

      DL Common source files (used by DL ISO-8583)

  - benchmark / demo
  
      Sample MSVC 6.0 project files

COMPILATION
-----------

Simply compile all the provided source code to run the demo application, from
there you can easily integrate the DL ISO-8583 library into other projects.

If using MSVC 6.0 then simply open the applicable DSW file.

CHANGE HISTORY
--------------

This version includes the following changes:

  [Bug-fix] Fixed issue in 'VarLen_Get' (dl_iso8583_fields.c) which resulted in
            the length being incorrectly determined (during unpack) for any
            variable length field with a length equal to or exeeding 100

BUGS / SUGGESTIONS
------------------

Please email any bugs or suggestions to 'o.sanderson@gmail.com'.
