bzToolkit
=========

An external BZFlag plug-in API that consists of functions that are not in the official BZFS API due to them being too hackish or too simple but still should exist.

## Contributors
allejo  
alezakos  
mdskpr  
Murielle  
sigonasr2  

## How to Use the API
Add both bzToolkit header files to same directory as your plug-in and add `#include "bzToolkitAPI.h` to the top of your file after the `bzfsAPI.h` line and you are ready to use the toolkit's API in your plugin.

## How to Contribute
If you have written a useful function in the past and would like to add it to the toolkit, send a [pull request](https://github.com/allejo/bztoolkit/pulls) and it will most likely be added to the toolkit. _Please note, your function may be renamed to fit the toolkit's naming convention and the code style may be also be changed to fit with the rest of the code._

## Documentation
The documentation for the API functions can be found in the [Wiki](https://github.com/allejo/bztoolkit/wiki), which is regularly maintained and updated.

## Versions
There are no "stable releases" as this project is barely getting started. Future releases will have their own tags.

## License
GPLv3
