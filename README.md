# bzToolkit

A third-party BZFlag plug-in API that consists of functions that do not exist in the official API. There are a lot of functions access BZFS headers directly, a practice that is discouraged, and there are some convenience functions as well.

## Contributing

If you have written a useful function in the past and would like to add it to the toolkit, send a [pull request](https://github.com/allejo/bztoolkit/pulls) for it to be reviewed and added. _Please note, your function may be renamed to fit the toolkit's naming convention and the code style may be also be changed in order for the style remain the same._

### Contributors

- alezakos
- allejo
- Dracos85
- mdskpr
- Murielle
- sigonasr2  


Using the API
-----

### How to Use the API

If you are using Git to developer your plug-in, add bzToolkit as a submodule to your plug-in repository.

```
git submodule add https://github.com/allejo/bztoolkit.git
```

If you aren't, simply clone bzToolkit into the same folder as your plugin.


```
git clone https://github.com/allejo/bztoolkit.git
```

After you have added bztoolkit to the folder, simply add the following header to your code.

```c++
#include "bztoolkit/bzToolkitAPI.h"
```

### Known Issues

If this toolkit is included with "plugin_utils.h," you will recieve conflict errors. If you are including "plugin_utils.h" to support reading a cofiguration file using the `PluginConfig` class, simply include "plugin_config.h" instead and it will still work.

## License
[MIT](https://github.com/allejo/bztoolkit/blob/master/LICENSE.md)
