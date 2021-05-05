EXE = detector
# remember output of `pkg-config --cflags --libs opencv` to see whole linker path
CVINCLUDES = `pkg-config --cflags opencv`
CVLINKERS = `pkg-config --libs opencv`
CAMINCLUDES = -I/opt/MVS/include -Wl,-rpath=$(MVCAM_COMMON_RUNENV)/aarch64
CAMLINKERS = -L$(MVCAM_COMMON_RUNENV)/aarch64 -lMvCameraControl -lpthread -lstdc++ -lm

.PHONY: clean

$(EXE): *.cpp
	#g++ -ggdb `pkg-config --cflags opencv` -I/opt/MVS/include -Wl,-rpath=$(MVCAM_COMMON_RUNENV)/aarch64 *.cpp `pkg-config --libs opencv` -L$(MVCAM_COMMON_RUNENV)/aarch64 -lMvCameraControl -lpthread -lstdc++ -lm -o $(EXE)
	g++ -ggdb $(CVINCLUDES) $(CAMINCLUDES) *.cpp $(CVLINKERS) $(CAMLINKERS) -o $(EXE)

clean:
	rm $(EXE)
