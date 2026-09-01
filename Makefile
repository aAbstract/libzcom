CC := gcc
CFLAGS := -fPIC -Wall -O0 -g
LDFLAGS := -shared

MDBUS_SO := libzcom_mdbus.so
MDBUS_SRCS := $(wildcard src/libzcom_mdbus*.c)

LTBUS_SO := libzcom_ltbus.so
LTBUS_SRCS := $(wildcard src/libzcom_ltbus*.c)

all: $(MDBUS_SO) $(LTBUS_SO)

$(MDBUS_SO): $(MDBUS_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MDBUS_SRCS) -Iinc -o $(MDBUS_SO)

$(LTBUS_SO): $(LTBUS_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MDBUS_SRCS) -Iinc -o $(LTBUS_SO)

clean:
	rm $(MDBUS_SO) $(LTBUS_SO)
