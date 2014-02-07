bzToolkit
=========

An external BZFlag plug-in API that consists of functions that are not in the official BZFS API due to them being too hackish or too simple but still should exist.

Contributing
------------

### Contributors

- alezakos

- allejo

- mdskpr

- Murielle

- sigonasr2  

### How to Contribute

If you have written a useful function in the past and would like to add it to the toolkit, send a [pull request](https://github.com/allejo/bztoolkit/pulls) for it to be reviewed and added. _Please note, your function may be renamed to fit the toolkit's naming convention and the code style may be also be changed in order for the style remain the same._

Using the API
-----

### How to Use the API

Add both bzToolkit header files to the same directory as your plug-in and add `#include "bzToolkitAPI.h"` to the top of your plugin after the include for `bzfsAPI.h` and you are ready to use the toolkit's API in your plugin. You may also put the toolkit in a central location to be used with other plugins but be sure to update the respective `#include` statements with the appropriate paths.

### Documentation

The documentation for the API functions can be found in the [Wiki](https://github.com/allejo/bztoolkit/wiki), which is regularly maintained and updated.

### Known Issues

If this toolkit is included with "plugin_utils.h," you will recieve conflict errors. If you are including "plugin_utils.h" to support reading a cofiguration file using the `PluginConfig` class, simply include "plugin_config.h" instead and it will still work.

## Versions
[Version 1.0](https://github.com/allejo/bztoolkit/releases/tag/v1.0)

## License
[GNU General Public License v3](https://github.com/allejo/bztoolkit/blob/master/LICENSE.markdown)