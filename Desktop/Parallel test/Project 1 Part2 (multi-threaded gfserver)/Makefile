CC     = gcc
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
ASAN_LIBS = -static-libasan
CFLAGS := -Wall -Werror --std=gnu99 -g3

OS := $(shell uname)
ifneq ($(OS),Darwin)
  LDFLAGS += -lpthread
endif

# default is to build with address sanitizer enabled
all: gfserver_main gfclient_download

# the noasan version can be used with valgrind
all_noasan: gfserver_main_noasan gfclient_download_noasan

gfserver_main: gfserver.o handler.o gfserver_main.o content.o gf-student.o steque.o
	$(CC) -o $@ $(CFLAGS) $(ASAN_FLAGS) $(CURL_CFLAGS) $^ $(LDFLAGS) $(CURL_LIBS) $(ASAN_LIBS)

gfclient_download: gfclient.o workload.o gfclient_download.o gf-student.o steque.o
	$(CC) -o $@ $(CFLAGS) $(ASAN_FLAGS) $^ $(LDFLAGS) $(ASAN_LIBS)

gfserver_main_noasan: gfserver_noasan.o handler_noasan.o gfserver_main_noasan.o content_noasan.o gf-student_noasan.o steque.o
	$(CC) -o $@ $(CFLAGS) $(CURL_CFLAGS) $^ $(LDFLAGS) $(CURL_LIBS)

gfclient_download_noasan: gfclient_noasan.o workload_noasan.o gfclient_download_noasan.o gf-student_noasan.o steque.o
	$(CC) -o $@ $(CFLAGS)  $^ $(LDFLAGS)

%_noasan.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $(ASAN_FLAGS) $<

.PHONY: clean

clean:
	mv handler.o handler.o-sav
	mv handler_noasan.o handler_noasan.o-sav
	rm -fr *.o gfserver_main gfclient_download gfserver_main_noasan gfclient_download_noasan
	mv handler_noasan.o-sav handler_noasan.o
	mv handler.o-sav handler.o
