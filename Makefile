CC := gcc
CFLAGS := -fPIC -Wall -O0 -g
LDFLAGS := -shared

MDBUS_SO := libzcom_mdbus.so
MDBUS_SRCS := $(wildcard src/libzcom_mdbus*.c) src/libzcom_common.c

LTBUS_SO := libzcom_ltbus.so
LTBUS_SRCS := $(wildcard src/libzcom_ltbus*.c) src/libzcom_common.c

LIBZCOM_SO := libzcom.so
LIBZCOM_SRCS := $(wildcard src/libzcom_mdbus*.c) $(wildcard src/libzcom_ltbus*.c) src/libzcom_common.c

$(MDBUS_SO): $(MDBUS_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MDBUS_SRCS) -Iinc -o $(MDBUS_SO)

$(LTBUS_SO): $(LTBUS_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LTBUS_SRCS) -Iinc -o $(LTBUS_SO)

$(LIBZCOM_SO): $(LIBZCOM_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBZCOM_SRCS) -Iinc -o $(LIBZCOM_SO)

clean:
	rm $(MDBUS_SO) $(LTBUS_SO) $(LIBZCOM_SO)
