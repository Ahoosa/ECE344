#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
//#include "common.h"
#include "wc.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct wc {
    /* you can define this struct to have whatever fields you want. */
    char word[70];
    int code;
    int count;

    bool taken;
};

int findkey(char *word, int size) {
    int hash = 5381;
    int c = 0;
    while ((c = *word++) != 0) {
        hash = ((hash << 5) + hash) + c;
    }
    if (hash < 0) hash *= -1;
    hash %= (size);
    return hash;

}

struct wc *
wc_init(char *word_array, long size) {


    struct wc *wc;

    wc = (struct wc *) malloc(sizeof (struct wc));
    assert(wc);
    char word[70];
    memset(word, 0, sizeof (word));
    int n = 0;
    bool counted = false;




    char **words = malloc(size * sizeof (char*));
    for (int i = 0; i < size; ++i) {
        memset(&words[i], 0, sizeof (words[i]));
    }

    int num = 0;
    for (int i = 0; i < size + 1; ++i) {


        if (!isspace(word_array[i]) && i < size) {

            //       word[n] = word_array[i];
            n++;

        } else {
            //     words[num] = malloc(strlen(word)+1 * sizeof (char*));
            //     strcpy(words[num], word);

            n = 0;
            //memset(word, 0, sizeof (word));

            num++;
        }
    }


    struct wc *container = calloc(num, sizeof (struct wc));
    for (int i = 0; i < num; ++i) {
        memset(&container[i], 0, sizeof (struct wc));
    }
    strcpy(container[ (num) - 1].word, word);
    container[ (num) - 1].count = 0;
    container[(num) - 1].code = -1;
    container[(num) - 1].taken = true;

    n=0;
    for (int i = 0; i < size; ++i) {
        if (!isspace(word_array[i]) && i < size) {

            word[n] = word_array[i];
            n++;

        } else {


            n = 0;
          //  if (words[i] != NULL) {
                //               printf("im here2 %s \n",words[i]);
                if (strcmp(word, "\0") == 0) continue;

                int hashCode = findkey(word, num);
                if (container[hashCode].taken == true) {
                    while (container[hashCode].taken == true) {

                        if (strcmp(container[hashCode].word, word) == 0) {

                            container[hashCode].count += 1;

                            counted = true;
                            break;
                        } else {
                            hashCode++;
                            if (hashCode >= num - 1) hashCode = hashCode % (num);
                        }
                    }
                    if (counted == false) {

                        container[hashCode].taken = true;
                        container[hashCode].code = hashCode;
                        strcpy(container[hashCode].word, word);

                        container[hashCode].count = 1;

                    }
                } else {


                    container[hashCode].taken = true;
                    container[hashCode].code = hashCode;
                    strcpy(container[hashCode].word, word);

                    container[hashCode].count = 1;


                }

                counted = false;
                memset(word, 0, sizeof (word));
            }

     //   }
    }

    free(wc);
    wc = NULL;

    //    for (int i = 0; i < num; ++i) {
    //        free(words[i]);
    //        words[i] = NULL;
    //    }
    //    free(words);





    return container;
}

void
wc_output(struct wc * wc) {
    //    printf("im here");
    while (wc->code != -1) {
        if (wc->taken == true) {
            if (wc->code == -1) break;
            else printf("%s:%d\n", wc->word, wc->count);
        }
        wc++;
    }


}

void
wc_destroy(struct wc * wc) {

    free(wc);
    wc = NULL;

}


