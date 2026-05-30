Making Soul Unicode Database
============================

The Soul Unicode database is used by the **util** library that is used by all Soul tools and libraries.
It is located in the **%SOUL_ROOT%/unicode** directory.
Here are instructions how to generate the Unicode database:

*   Extract the ucd.all.flat.zip (extracts to ucd.all.flat.xml)

*   Run make\_soul\_ucd.exe

*   The make\_soul\_ucd program generates a binary file 'soul\_ucd.bin' that contains an uncompressed Unicode character database.

*   Now the Unicode database is ready for use by the Soul tools and libraries.
