CC = g++ -std=c++11
OS = $(shell uname -s)
ifeq ($(OS), Linux)
  DEFINE += -D__LINUX_ALSA__
  LINKER += -lasound
  LINKER += -lpthread
  LINKER += -lwiringPi
else ifeq ($(OS), Darwin)
  DEFINE += -D__MACOSX_CORE__
  LINKER += -framework CoreAudio
  LINKER += -framework CoreFoundation
  LINKER += -lpthread
  DEFINE += -Wno-deprecated-register
  DEFINE += -Wno-format-extra-args
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
