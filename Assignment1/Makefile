CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic
LIBS=-lpthread

all: daemon

daemon: daemon.c backup_dashboard.c check_file_uploads.c collect_reports.c lock_directories.c sig_handler.c unlock_directories.c update_timer.c
	$(CC) $(CFLAGS) -o daemon daemon.c backup_dashboard.c check_file_uploads.c collect_reports.c lock_directories.c sig_handler.c unlock_directories.c update_timer.c $(LIBS)

bd: backup_dashboard_2.c
	$(CC) -o backup_dashboard_2 backup_dashboard_2.c

clean:
	rm -f daemon