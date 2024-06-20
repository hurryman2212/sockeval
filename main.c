#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

extern unsigned int crc_pcl(unsigned char *buffer, int len,
                            unsigned int crc_init);

bool is_client = false, use_crc = false, use_fork = false;
const char *host = "localhost", *testname = "TCP_STREAM", *iomux = "epoll";
int numsocket = 1, numthread = 1, server_portnum = 50000;

void do_getopt(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "cCfF:hH:p:s:t:T:")) != -1) {
    switch (opt) {
    case 'c':
      is_client = true;
      break;
    case 'C':
      use_crc = true;
      break;
    case 'f':
      use_fork = true;
      break;
    case 'F':
      numsocket = atoi(optarg);
      if (numsocket <= 0) {
        fprintf(stderr, "Invalid number of sockets: %s\n", optarg);
        exit(EXIT_FAILURE);
      }
      break;
    case 'H':
      host = optarg;
      break;
    case 'p':
      server_portnum = atoi(optarg);
      if (server_portnum < 0 || server_portnum > 65535) {
        fprintf(stderr, "Invalid server port number: %s\n", optarg);
        exit(EXIT_FAILURE);
      }
      break;
    case 's':
      if (!strcmp(optarg, "epoll") || !strcmp(optarg, "poll") ||
          !strcmp(optarg, "select"))
        iomux = optarg;
      else {
        fprintf(stderr, "Invalid I/O multiplexing system call name: %s\n",
                optarg);
        exit(EXIT_FAILURE);
      }
    case 't':
      if (!strcmp(optarg, "TCP_STREAM") || !strcmp(optarg, "TCP_RR"))
        testname = optarg;
      else {
        fprintf(stderr, "Invalid test name: %s\n", optarg);
        exit(EXIT_FAILURE);
      }
      break;
    case 'T':
      numthread = atoi(optarg);
      if (numthread <= 0) {
        fprintf(stderr, "Invalid number of threads: %s\n", optarg);
        exit(EXIT_FAILURE);
      }
      break;
    case 'h':
    default:
      fprintf(stderr,
              "Usage: %s [-c] [-C] [-f] [-F numsocket] [-H host] [-p portnum] "
              "[-s iomux] [-t testname] [-T numthread]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  if (numsocket % numthread) {
    fprintf(stderr, "Invalid number of sockets: %d\n", numsocket);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  do_getopt(argc, argv);

  return EXIT_SUCCESS;
}
