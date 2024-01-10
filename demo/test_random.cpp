#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "ctype.h"
#include "/home/byd/Desktop/test_security/security/securityservice.h"

void helpmenu(void)
{
    printf("\nHelp menu: test_Random <option> \n");
    printf("option:- \n");
    printf("-l<random length>  : \n");
    printf("-h                 : Print this help \n");
    printf("                   : -l \n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("\n");
        printf("--------------------------\n");
        helpmenu();
        exit(0);
    }

    int ch;
    int nKeyLen = 16;
    int nLenFalg = 0;

    while ((ch = getopt(argc, argv, "l:")) != -1)
    {
        switch (ch)
        {
        case 'l':
            if (strncmp(optarg, "0x", 2) == 0)
                sscanf(optarg, "%x", &nKeyLen);
            else
                sscanf(optarg, "%d", &nKeyLen);
            // if ((16 != nKeyLen) && (24 != nKeyLen) && (32 != nKeyLen))
            // {
            //     printf("wrong: random length(%d) within 16, 24, 32\n", nKeyLen);
            //     exit(0);
            // }
            nLenFalg = 1;
            break;
        case 'h':
        default:
            helpmenu();
            exit(0);
            break;
        }
    }

    if (0 == nLenFalg)
    {
        printf("falg: l: %d\n", nLenFalg);
        helpmenu();
        exit(0);
    }

    printf("\n-----------------test start--------------------\n");
    unsigned int nRet = CSS_open_service();
    if (ERR_CODE_OK != nRet)
    {
        printf("CSS_open_service failed, err: %d\n", nRet);
        exit(0);
    }

    printf("-----------------generate random----------------\n");

    unsigned char *szKey = new unsigned char[nKeyLen];
    memset(szKey, 0, nKeyLen);
    nRet = CSS_generate_random(nKeyLen, szKey);
    if (ERR_CODE_OK == nRet)
    {
        printf("CSS_generate_random success\n");
        for (int i = 0; i < nKeyLen; i++)
        {
            printf("%02X", szKey[i]);
        }
        printf("\n");
    }
    else
    {
        printf("CSS_generate_random failed, err: %d\n", nRet);
    }
    delete[] szKey;

    CSS_close_service();
    printf("\n-----------------test End--------------------\n");

    return 0;
}
