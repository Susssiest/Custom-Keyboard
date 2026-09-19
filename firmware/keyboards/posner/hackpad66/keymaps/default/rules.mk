# The PCF8574A expander is driven directly from keymap.c, so the standalone
# I2C master driver has to be pulled in explicitly.
I2C_DRIVER_REQUIRED = yes
