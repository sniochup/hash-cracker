#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "crypt.h"
#include <mpi.h>

#define MSG_DONE 100

int MPI_done(const char *text, int len, int size)
{
    MPI_Request request;
    for (int r = 0; r < size; r++)
        MPI_Isend(text, len, MPI_INT, r, MSG_DONE, MPI_COMM_WORLD, &request);
    return 0;
}

int compare_sring(char *text, int len, int counter, char *target, char *salt, int rank, int size)
{
    if (len - counter == 3)
    {
        for (int a = 0; a < 26; a++)
        {
            text[len - 3] = a + 97;

            int mess = 0;
            MPI_Status status;
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mess, &status);

            if (mess)
            {
                printf("Work in node [ %d ] ended\n", rank);
                MPI_Finalize();
                exit(0);
            }

            for (int b = 0; b < 26; b++)
            {
                text[len - 2] = b + 97;

                for (int c = 0; c < 26; c++)
                {
                    text[len - 1] = c + 97;

                    char text_cpy[len];
                    strncpy(text_cpy, text, len);

                    if (strcmp(crypt(text_cpy, salt), target) == 0)
                    {
                        printf("Encrypted text: %s - in node [ %d ]\n", text_cpy, rank);
                        MPI_done(text_cpy, len, size);
                        MPI_Finalize();
                        exit(0);
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            text[counter] = i + 97;
            compare_sring(text, len, counter + 1, target, salt, rank, size);
        }
    }

    return 0;
}

int task_distribution(int rank, int size)
{
    if (rank == size - 1)
    {
        return 26 / size + 26 % size;
    }

    return 26 / size;
}

int main(int argc, char **argv)
{
    int rank, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int namelen;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &namelen);

    int text_len = atoi(argv[1]);
    char *target = argv[2];
    char *salt = "aa";

    char *text = (char *)malloc(text_len * sizeof(char));

    int task_num = task_distribution(rank, size);

    int temp = 26 / size;
    text[0] = rank * temp + 97;

    for (int t = 0; t < task_num; t++)
    {
        printf("Letter [ %c ] processing on node: %d\n", text[0], rank);
        compare_sring(text, text_len, 1, target, salt, rank, size);
        text[0] += 1;
    }

    MPI_Finalize();

    return 0;
}