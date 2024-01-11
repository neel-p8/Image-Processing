build:
	g++ -std=c++11 -o project2.out src/main.cpp

tasks: part11 part12 part13

part11:
	./project2.out output/part11.tga input/circles.tga multiply input/layer1.tga

part12:
	./project2.out output/part12.tga input/layer1.tga flip

part13:
	./project2.out output/part13.tga input/layer1.tga subtract input/layer2.tga