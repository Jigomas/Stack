C_FLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
EXECUTION_NAME = a.exe

all: main.o ver-tor.o constructor-destructor.o push-pop.o reader.o
	g++ .\main.o .\ver-tor.o .\constructor-destructor.o .\push-pop.o .\reader.o -o $(EXECUTION_NAME)

main.o: main.c
	echo "Hello world"
	g++ .\main.c -c $(C_FLAGS)

ver-tor.o: ver-tor.c
	g++ .\ver-tor.c -c $(C_FLAGS)

constructor-destructor.o: constructor-destructor.c
	g++ .\constructor-destructor.c -c $(C_FLAGS)

push-pop.o: push-pop.c
	g++ .\push-pop.c -c $(C_FLAGS)

reader.o: reader.c
	g++ .\reader.c -c $(C_FLAGS)

run:
	.\a.exe

clean:
	rm *.o
	rm *.exe
 g++ .\main.c .\ver-tor.c .\constructor-destructor.c .\push-pop.c .\reader.c
