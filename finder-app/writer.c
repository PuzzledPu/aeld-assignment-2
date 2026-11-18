#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Usage: %s <writefile> <writestr>", argv[0]);
        closelog();
        return EXIT_FAILURE;
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];
    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Cannot open %s: %s", writefile, strerror(errno));
        closelog();
        return EXIT_FAILURE;
    }

    int result = EXIT_SUCCESS;
    if (fputs(writestr, file) == EOF) {
        syslog(LOG_ERR, "Cannot write to %s: %s", writefile, strerror(errno));
        result = EXIT_FAILURE;
    }

    if (fclose(file) == EOF) {
        syslog(LOG_ERR, "Cannot close %s: %s", writefile, strerror(errno));
        result = EXIT_FAILURE;
    }

    if (result == EXIT_SUCCESS) {
        syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
    }

    closelog();
    return result;
}
