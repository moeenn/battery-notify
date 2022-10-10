#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum { LOW, HIGH } Level;

#define DEBUG 1 
#define LEVEL_LOW 20
#define LEVEL_HIGH 80
#define SLEEP 600

void read_int(char *path, int *target);
int get_battery();
void send_notification(Level l);

/**
 *  program entry-point
 *
 */
int main(int argc, char **argv) {
  int perc;

  for (perc = get_battery();; sleep(SLEEP)) {
    if (perc <= LEVEL_LOW) {
      send_notification(LOW);
      continue;
    }

    if (perc >= LEVEL_HIGH) {
      send_notification(HIGH);
    }

#if DEBUG == 1
    printf("polling battery: %d\n", perc);
#endif
  }

  return 0;
}

/**
 *  read an integer value from top of a file
 *
 */
void read_int(char *path, int *target) {
  FILE *fd;

  fd = fopen(path, "r");
  fscanf(fd, "%d", target);

  fclose(fd);
}

/**
 *  get current battery percentage from the system
 *
 */
int get_battery() {
  int now, full;

  read_int("/sys/class/power_supply/BAT0/charge_now", &now);
  read_int("/sys/class/power_supply/BAT0/charge_full", &full);

  return 100 * now / full;
}

/**
 *  send a notification to the user using the native system services
 *  (i.e notify-send)
 *
 */
void send_notification(Level l) {
  char *program = "notify-send";

  char *low_args[] = {program, "--icon=battery-low", "Low battery", NULL};
  char *high_args[] = {program, "--icon=battery", "Battery charged", NULL};

  if (fork() == 0) {
    execvp(program, l == LOW ? low_args : high_args);
    exit(EXIT_SUCCESS);
  }
}
