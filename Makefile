EchoChamber: Random.o SocialMedia.o Agent.o main.o
	g++ -g Random.o SocialMedia.o Agent.o main.o -o EchoChamber

Random.o: Random.cpp include/prottype.hpp include/Class.hpp include/Random.hpp include/Struct.hpp include/Const.hpp
	g++ -g -Og Random.cpp -c -o Random.o

SocialMedia.o: SocialMedia.cpp include/prottype.hpp include/Class.hpp include/Random.hpp include/Struct.hpp include/Const.hpp
	g++ -g -Og SocialMedia.cpp -c -o SocialMedia.o

Agent.o: Agent.cpp include/prottype.hpp include/Class.hpp include/Random.hpp include/Struct.hpp include/Const.hpp
	g++ -g -Og Agent.cpp -c -o Agent.o

main.o: main.cpp include/prottype.hpp include/Class.hpp include/Random.hpp include/Struct.hpp include/Const.hpp
	g++ -g -Og main.cpp -c -o main.o
	
.PHONY: clean distclean

clean:
	rm *.o