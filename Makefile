CC = g++
DEFINE += -D__LINUX_ALSA__

LINKER += -lasound
LINKER += -lpthread
LINKER += -lwiringPi

click: click.cpp RtAudio.o
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

probe: probe.cpp RtAudio.o
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

search: search.cpp RtAudio.o
	$(CC) $(DEFINE) -o $@ $^ $(LINKER)

RtAudio.o: RtAudio.cpp
	$(CC) $(DEFINE) -o $@ -c $< $(LINKER)

clean:
	rm -rf probe search click *.o
