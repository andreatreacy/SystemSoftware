CC=gcc
objects = main.o areaOfRectangle.o areaOfSquare.o areaOfCircle.o area_of_triangle.o 
headers = areaOfRectangle.h areaOfSquare.h areaOfCircle.h area_of_triangle.h 

build: createReport.c
	gcc -o createReport createReport.c

run: createReport.o
	./createReport


build-b: createReport.c
	gcc -o createReport createReport.c

run-b: createReport.o
	./createReport

clean:
	rm go


CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lm

SRCS = daemon.c collect_reports.c backup_dashboard.c lock_directories.c unlock_directories.c generate_reports.c check_file_uploads.c
OBJS = daemon.o collect_reports.o backup_dashboard.o lock_directories.o unlock_directories.o generate_reports.o check_file_uploads.o
DEPS = daemon_task.h

TARGET = daemon

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

daemon.o: daemon.c $(DEPS)
	$(CC) $(CFLAGS) -c daemon.c -o daemon.o

collect_reports.o: collect_reports.c $(DEPS)
	$(CC) $(CFLAGS) -c collect_reports.c -o collect_reports.o

backup_dashboard.o: backup_dashboard.c $(DEPS)
	$(CC) $(CFLAGS) -c backup_dashboard.c -o backup_dashboard.o

lock_directories.o: lock_directories.c $(DEPS)
	$(CC) $(CFLAGS) -c lock_directories.c -o lock_directories.o

unlock_directories.o: unlock_directories.c $(DEPS)
	$(CC) $(CFLAGS) -c unlock_directories.c -o unlock_directories.o

generate_reports.o: generate_reports.c $(DEPS)
	$(CC) $(CFLAGS) -c generate_reports.c -o generate_reports.o

check_file_uploads.o: check_file_uploads.c $(DEPS)
	$(CC) $(CFLAGS) -c check_file_uploads.c -o check_file_uploads.o

clean:
	rm -f $(OBJS) $(TARGET)