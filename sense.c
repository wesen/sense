#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

pid_t childpid = -1;

void usage(char *progname) {
   fprintf(stderr, "Usage: %s [-h] [-n time] <command>\n", progname);
}

static void stirb(int bla) {
   exit(0);
}

static void kindermord(int bla) {
   int status;

   if (childpid == -1) {
      /* no way this could happen */
      fprintf(stderr, "Child not launched!\n");
      exit(1);
   }

   if (wait(&status) < 0) {
      perror("wait");
      exit(1);
   }

   printf("Child finished\n");
   exit(0);
}

int main(int argc, char *argv[], char *envp[]) {
   unsigned char *cmd = NULL, *ptr;
   char          *args[4];
   int           retval = EXIT_SUCCESS, c, interval = 2,
                 len, i, j, *lens = NULL;

   while ((c = getopt(argc, argv, "+hn:")) != -1) {
      switch (c) {
         case 'h':
            retval = EXIT_SUCCESS;
            goto main_exit;
            
         case 'n':
            interval = atoi(optarg);
            if (!interval) {
               retval = EXIT_SUCCESS;
               goto main_exit;
            }
            break;

         default:
            retval = EXIT_FAILURE;
            goto main_exit;
      }
   }
  
   if (argc <= optind) {
      usage(argv[0]);
      retval = EXIT_FAILURE;
      goto main_exit;
   }

   if (!(lens = malloc(sizeof(int) * (argc - optind)))) {
      fprintf(stderr, "Not enough memory\n");
      retval = EXIT_FAILURE;
      goto main_exit;
   }

   for (len = j = 0, i = optind; i < argc; i++, j++) {
      len += lens[j] = strlen(argv[i]);
      len += 2;
   }

   if (!(cmd = malloc(len + 1))) {
      fprintf(stderr, "Not enough memory\n");
      retval = EXIT_FAILURE;
      goto main_exit;
   }

   for (ptr = cmd, i = optind, j = 0; i < argc; i++) {
      memcpy(ptr, argv[i], lens[j]);
      ptr += lens[j];
      *ptr = ' ';
      *ptr++;
   }
   *(ptr - 1) = '\0';

   signal(SIGINT,  stirb);
   signal(SIGTERM, stirb);
   signal(SIGHUP,  stirb);
   signal(SIGCHLD, kindermord);

   args[0] = argv[optind];
   args[1] = "-c";
   args[2] = cmd;
   args[3] = NULL;

   if ((childpid = fork()) == 0) {
      /* child */
      if (execve("/bin/sh", args, envp) < 0) {
         perror(argv[optind]);
         exit(1);
      }
   } else {
      sleep(interval);
      printf("Bringe jetzt Kind um!\n");
      kill(childpid, SIGTERM);
   }

main_exit:
   if (cmd)
      free(cmd);
   if (lens)
      free(lens);

   return retval;
}
