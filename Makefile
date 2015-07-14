OS = $(shell uname -s)
ifeq ($(OS), Linux)
  CC = g++ -std=c++0x
  LINKER += -lrtaudio
  LINKER += -lasound
  LINKER += -lwiringPi
else ifeq ($(OS), Darwin)
  CC = g++ -std=c++11 -Wno-deprecated-register -Wno-format-extra-args
  DEFINE += -D__MACOSX_CORE__
  LINKER += -framework CoreAudio
  LINKER += -framework CoreFoundation
  LINKER += -lpthread
else
endif

#click: click.cpp RtAudio.o
click: click.cpp
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

playsaw: playsaw.cpp
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

#probe: probe.cpp RtAudio.o
#	$(CC) $(DEFINE) -o $@ $^ $(LINKER)
#
#search: search.cpp RtAudio.o
#	$(CC) $(DEFINE) -o $@ $^ $(LINKER)
#
#cli: cli.cpp
#	$(CC) $(DEFINE) -o $@ $^ $(LINKER)
#
#RtAudio.o: RtAudio.cpp
#	$(CC) $(DEFINE) -o $@ -c $< $(LINKER)

clean:
	rm -rf probe search click cli *.o
