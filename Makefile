OS = $(shell uname -s)
ifeq ($(OS), Linux)
  CC = g++ -std=c++0x
  LINKER += -lrtaudio
  LINKER += -lasound
  LINKER += -lwiringPi
  LINKER += -lcuttlebone
  LINKER += -L./cuttlebone/build/
  INCLUDE += -I./cuttlebone/
else ifeq ($(OS), Darwin)
  CC = g++ -std=c++11 -Wno-deprecated-register -Wno-format-extra-args
  DEFINE += -D__MACOSX_CORE__
  LINKER += -framework CoreAudio
  LINKER += -framework CoreFoundation
  LINKER += -lpthread
  LINKER += -L./cuttlebone/build/
  INCLUDE += -I./cuttlebone/
else
endif

test_network: test_network.cpp
	$(CC) $(DEFINE) $(INCLUDE) -o $@ $^ $(LINKER)

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
	rm -rf probe search click playsaw cli *.o
