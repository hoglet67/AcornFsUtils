#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adfs_utils.h"

#define TRACK_SIZE (16 * 256)

#define TRACKS_PER_SIDE (80)

#define SIZE (2 * TRACKS_PER_SIDE * TRACK_SIZE)

#define TITLE_OFFSET (0x6D9)

char src[SIZE];
char dst[SIZE];

// non-interleaved: A0 A1 A2 A3 B0 B1 B2 B3
//      interlaved: A0 B0 A1 B1 A2 B2 A3 B3
//
// To interleave
// - copy track N to (N MOD TRACKS_PER_SIDE)*2 + (N DIV TRACKS_PER_SIDE)
//
// // To deinterleave
// - copy track N to (N/2 + (N & 1) * TRACKS_PER_SIZE)

int adfs_utils(int argc, char *argv[], int operation) {
   int s;
   int d;

   if (argc != 3 && argc != 4) {
      fprintf(stderr, "usage: %s <src disk> <dsk disk> [ title ]\n", argv[0]);
      exit(1);
   }

   char *title = (argc == 4) ? argv[3] : NULL;
   
   FILE* sf = fopen(argv[1], "r");
   if (sf == NULL) {
      fprintf(stderr, "file not found: %s\n", argv[1]);
      exit(2);
   }
   if (fread(src, SIZE, 1, sf) != 1) {
      fprintf(stderr, "failed to read: %s\n", argv[1]);
      exit(3);
   }
   fclose(sf);

   // Work a track at a time
   for (s = 0; s < TRACKS_PER_SIDE * 2; s++) {
      if (operation == OP_INTERLEAVE) {
         d = (s % TRACKS_PER_SIDE) * 2 + (s / TRACKS_PER_SIDE);
      } else if (operation == OP_DEINTERLEAVE) {
         d = (s >> 1) + (s & 1) * TRACKS_PER_SIDE;
      } else {
         d = s;
      }
      memcpy(dst + d * TRACK_SIZE, src + s * TRACK_SIZE, TRACK_SIZE);
   }

   if (title) {
      for (int i = 0; i < 19; i++) {
         dst[TITLE_OFFSET + i] = 13;
      }
      memcpy(dst + TITLE_OFFSET, title, strlen(title));
   }
   
   FILE* df = fopen(argv[2], "w");
   if (df == NULL) {
      fprintf(stderr, "failed to create: %s\n", argv[2]);
      exit(2);
   }
   fwrite(dst, SIZE, 1, df);
   fclose(df);

   return 0;
}
