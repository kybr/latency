OS = $(shell uname -s)
ifeq ($(OS), Linux)
  CC = g++ -std=c++0x
  DEFINE += -D__LINUX_ALSA__
  LINKER += -lasound
  LINKER += -lpthread
  LINKER += -lwiringPi
else ifeq ($(OS), Darwin)
  CC = g++ -std=c++11 -Wno-deprecated-register -Wno-format-extra-args
  DEFINE += -D__MACOSX_CORE__
  LINKER += -framework CoreAudio
  LINKER += -framework CoreFoundation
  LINKER += -lpthread
else
endif

cli: cli.cpp
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

click: click.cpp RtAudio.o
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

probe: probe.cpp RtAudio.o
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

search: search.cpp RtAudio.o
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

RtAudio.o: RtAudio.cpp
	$(CC) $(DEFINE) -o $@ -c $< $(LINKER)

clean:
	rm -rf probe search click cli *.o
