//
// Created by Andrew Bailey on 3/5/20.
//

#include <banded_alignment/BandedAlignment.hpp>

extern "C" {
#include "gaba.h"										/* just include gaba.h */
}

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unordered_map>

using namespace std;

int printer(void *fp, uint64_t len, char c)
{
  return(fprintf((FILE *)fp, "%ld%c", len, c));
}

//uint8_t bits[4] = {0x01, 0x02, 0x04, 0x08};
unordered_map<char, char const> base_map = {{'A', 0x01},
                                              {'C', 0x02},
                                              {'G', 0x04},
                                              {'T', 0x08}};
char char_to_encoding(char base){
  return base_map[base];
}

char const* string_to_encoding(string& sequence){
  char *outStr;
  outStr = (char *)std::malloc(sizeof(char) * sequence.length());
  for (std::string::size_type i = 0; i < sequence.size(); i++) {
    outStr[i] = char_to_encoding(sequence[i]);
  }
  return outStr;
}



string banded_alignment(string ref, string query){
  /* create config */
  signed char _m = 2;
  signed char _x = 3;
  struct gaba_params_s data =
      {.score_matrix = {static_cast<signed char>(_m),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(_m),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(_m),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(-_x),
                        static_cast<signed char>(_m)},
          .gi = 5,
          .ge = 1,
          .gfa = 0,
          .gfb = 0,
          .xdrop = 100};
  gaba_t *ctx = gaba_init(&data);

//  char const *a = "\x01\x08\x01\x08\x01\x04\x02\x01\x08\x08\x08\x08";			/* 4-bit encoded "ATATAGCATTTT" */
//  char const *b = "\x01\x08\x04\x02\x01\x08\x08\x08\x08";			/* 4-bit encoded "ATGCATTTT" */
//  char const *a = "CATGTCCTGCATGGCATTAGTACGTGGCTTGCCTGGCG";
//  char const *b = "CATGTCAGATTGAACGTGTTGAACGTTGCCTGGCG";

  char const *a = string_to_encoding(ref);
  char const *b = string_to_encoding(query);
//  char const *a = "\x02\x01\x08\x04\x08\x02\x02\x08\x04\x02\x01\x08\x04\x04\x02\x01\x08\x08\x01\x04\x08\x01\x02\x04\x08\x04\x04\x02\x08\x08\x04\x02\x02\x08\x04\x04\x02\x04";
//  char const *b = "\x01\x04\x01\x08\x08\x04\x01\x01\x02\x04\x08\x04\x08\x08\x04\x01\x01\x02\x04\x08\x08\x04\x02\x02\x08\x04\x04\x02\x04";
  char const t[64] = { 0 };							/* tail array */

  uint32_t a_len = strlen(a);
  uint32_t b_len = strlen(b);

  struct gaba_section_s asec = gaba_build_section(0, a, a_len);
  struct gaba_section_s bsec = gaba_build_section(2, b, b_len);
  struct gaba_section_s tail = gaba_build_section(4, t, 64);

  /* create thread-local object */
  gaba_dp_t *dp = gaba_dp_init(ctx);					/* dp[0] holds a 64-cell-wide context */
  // gaba_dp_t *dp_32 = &dp[_dp_ctx_index(32)];			/* dp[1] and dp[2] are narrower ones */
  // gaba_dp_t *dp_16 = &dp[_dp_ctx_index(16)];

  /* init section pointers */
  struct gaba_section_s const *ap = &asec, *bp = &bsec;
  struct gaba_fill_s const *f = gaba_dp_fill_root(dp,	/* dp -> &dp[_dp_ctx_index(band_width)] makes the band width selectable */
                                                  ap, 0,											/* a-side (reference side) sequence and start position */
                                                  bp, 0,											/* b-side (query) */
                                                  UINT32_MAX										/* max extension length */
  );

  /* until X-drop condition is detected */
  struct gaba_fill_s const *m = f;					/* track max */
  while((f->status & GABA_TERM) == 0) {
    if(f->status & GABA_UPDATE_A) { ap = &tail; }	/* substitute the pointer by the tail section's if it reached the end */
    if(f->status & GABA_UPDATE_B) { bp = &tail; }

    f = gaba_dp_fill(dp, f, ap, bp, UINT32_MAX);	/* extend the banded matrix */
    m = f->max > m->max ? f : m;					/* swap if maximum score was updated */
  }

  struct gaba_alignment_s *r = gaba_dp_trace(dp,
                                             m,												/* section with the max */
                                             nullptr											/* custom allocator: see struct gaba_alloc_s in gaba.h */
  );


  uint64_t const len = 16384;
  char *c = (char *)malloc(len);
  gaba_dump_cigar_forward(c, len, r->path, 0, r->plen);
  /* clean up */
  gaba_dp_res_free(dp, r);
  gaba_dp_clean(dp);
  gaba_clean(ctx);
  std::free((void *) a);
  std::free((void *) b);
  string cigar = string(c);
  free(c);
  return cigar;
}

