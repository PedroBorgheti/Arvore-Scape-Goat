FROM gcc:13
WORKDIR /app
COPY . .
RUN gcc -O2 -std=c11 -Wall -Wextra -pedantic \
	src/main.c src/ScapegoatTree.c -lm -o benchmark
CMD ["./benchmark"]