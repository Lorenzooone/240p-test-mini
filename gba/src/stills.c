/*
Static test patterns for 160p Test Suite
Copyright 2018 Damian Yerrick

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/
#include <gba_input.h>
#include <gba_video.h>
#include <gba_sound.h>
#include <gba_dma.h>
#include <gba_compression.h>
#include <gba_systemcalls.h>
#include <stdint.h>
#include "global.h"
#include "4bcanvas.h"
#include "posprintf.h"

#include "monoscope_chr.h"
#include "sharpness_chr.h"
#include "convergence_chr.h"
#include "pluge_shark_6color_chr.h"

#define PFMAP 23
#define PFOVERLAY 22

static const unsigned short pluge_shark_dark[] = {
  RGB5( 0, 0, 0),RGB5( 0, 0, 0),RGB5( 0, 0, 0),RGB5( 2, 2, 2),
  RGB5( 2, 2, 2),RGB5( 4, 4, 4)
};
static const unsigned short pluge_shark_darker[] = {
  RGB5( 0, 0, 0),RGB5( 0, 0, 0),RGB5( 0, 0, 0),RGB5( 1, 1, 1),
  RGB5( 1, 1, 1),RGB5( 2, 2, 2)
};
static const unsigned short pluge_shark_light[] = {
  RGB5(27,27,27),RGB5(29,29,29),RGB5(29,29,29),RGB5(31,31,31),
  RGB5(31,31,31),RGB5(31,31,31)
};
static const unsigned short pluge_shark_lighter[] = {
  RGB5(29,29,29),RGB5(30,30,30),RGB5(30,30,30),RGB5(31,31,31),
  RGB5(31,31,31),RGB5(31,31,31)
};
static const unsigned short *const pluge_shark_palettes[] = {
  pluge_shark_6color_chrPal,
  pluge_shark_dark, pluge_shark_darker,
  pluge_shark_light, pluge_shark_lighter
};

const unsigned short gray4pal[4] = {
  RGB5( 0, 0, 0),RGB5(15,15,15),RGB5(23,23,23),RGB5(31,31,31)
};
const unsigned short invgray4pal[4] = {
  RGB5(31,31,31),RGB5(23,23,23),RGB5(15,15,15),RGB5( 0, 0, 0)
};
static const unsigned short smptePalettes[][13] = {
  {
    // 75% with setup
    RGB5( 2, 2, 2),RGB5( 2, 2,24),RGB5(24, 2, 2),RGB5(24, 2,24),
    RGB5( 2,24, 2),RGB5( 2,24,24),RGB5(24,24, 2),RGB5(24,24,24),
    RGB5( 6, 0,13),RGB5(31,31,31),RGB5( 0, 4, 6),RGB5( 1, 1, 1),
    RGB5( 3, 3, 3)
  },
  {
    // 75% without setup
    RGB5( 0, 0, 0),RGB5( 0, 0,23),RGB5(23, 0, 0),RGB5(23, 0,23),
    RGB5( 0,23, 0),RGB5( 0,23,23),RGB5(23,23, 0),RGB5(23,23,23),
    RGB5( 6, 0,13),RGB5(31,31,31),RGB5( 0, 4, 6),RGB5( 0, 0, 0),
    RGB5( 1, 1, 1)

  },
  {
    // 100% with setup
    RGB5( 2, 2, 2),RGB5( 2, 2,31),RGB5(31, 2, 2),RGB5(31, 2,31),
    RGB5( 2,31, 2),RGB5( 2,31,31),RGB5(31,31, 2),RGB5(31,31,31),
    RGB5( 6, 0,13),RGB5(31,31,31),RGB5( 0, 4, 6),RGB5( 1, 1, 1),
    RGB5( 3, 3, 3)
  },
  {
    // 100% without setup
    RGB5( 0, 0, 0),RGB5( 0, 0,31),RGB5(31, 0, 0),RGB5(31, 0,31),
    RGB5( 0,31, 0),RGB5( 0,31,31),RGB5(31,31, 0),RGB5(31,31,31),
    RGB5( 6, 0,13),RGB5(31,31,31),RGB5( 0, 4, 6),RGB5( 0, 0, 0),
    RGB5( 1, 1, 1)
  },
};
static const unsigned short plugePaletteNTSC[] = {
  RGB5( 2, 2, 2),RGB5( 3, 3, 3),RGB5( 0, 0, 0),RGB5( 0, 0, 0),
  RGB5(13,13,13),RGB5(19,19,19),RGB5(25,25,25),RGB5(31,31,31)
};
static const unsigned short plugePaletteNTSCJ[] = {
  RGB5( 0, 0, 0),RGB5( 1, 1, 1),RGB5( 1, 0, 1),RGB5( 0, 1, 0),
  RGB5(13,13,13),RGB5(19,19,19),RGB5(25,25,25),RGB5(31,31,31)
};
static const unsigned short brickspal[] = {
  RGB5( 5, 0, 0),RGB5(10, 5, 5),RGB5(15,15,10)
};
static const uint32_t brickstile[8] = {
  0x22111111,
  0x11221111,
  0x11112211,
  0x01111022,
  0x22011102,
  0x11221002,
  0x11112202,
  0x11111122,
};

static const uint16_t monoscope_whites[] = {
  RGB5( 0, 0, 0),RGB5(13,13,13),RGB5(22,22,22),RGB5(31,31,31),RGB5(31,31,31)
};

void activity_monoscope(void) {
  unsigned int brightness = 1;

  bitunpack2(PATRAM4(0, 0), monoscope_chrTiles, sizeof(monoscope_chrTiles));
  RLUnCompVram(monoscope_chrMap, MAP[PFMAP]);
  BG_COLORS[1] = RGB5(31, 0, 0);

  while (1) {
    read_pad_help_check(helpsect_monoscope);
    if (new_keys & KEY_UP) {
      if (++brightness >= 5) brightness = 0;
    }
    if (new_keys & KEY_DOWN) {
      if (!brightness--) brightness = 4;
    }
    if (new_keys & KEY_B) {
      return;
    }

    VBlankIntrWait();
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFMAP);
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    BG_COLORS[0] = (brightness >= 4) ? RGB5(13,13,13) : 0;
    BG_COLORS[2] = monoscope_whites[brightness];
    REG_DISPCNT = MODE_0 | BG0_ON;
  }
}

void activity_sharpness(void) {
  unsigned int inverted = 0;
  unsigned int is_bricks = 0;

  bitunpack2(PATRAM4(0, 0), sharpness_chrTiles, sizeof(sharpness_chrTiles));
  RLUnCompVram(sharpness_chrMap, MAP[PFMAP]);
  dmaCopy(brickstile, PATRAM4(0, 63), sizeof(brickstile));
  dma_memset16(MAP[PFOVERLAY], 0x043F, 32*20*2);

  while (1) {
    read_pad_help_check(helpsect_sharpness);
    if (new_keys & KEY_SELECT) {
      inverted = !inverted;
    }
    if (new_keys & KEY_B) {
      return;
    }
    if (new_keys & KEY_A) {
      is_bricks = !is_bricks;
    }

    VBlankIntrWait();
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)
                |SCREEN_BASE(is_bricks ? PFOVERLAY : PFMAP);
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    const unsigned short *palsrc = inverted ? invgray4pal : gray4pal;
    if (is_bricks) palsrc = brickspal;
    dmaCopy(palsrc, BG_COLORS+0x00, sizeof(gray4pal));
    REG_DISPCNT = MODE_0 | BG0_ON;
  }
}

static const BarsListEntry smpterects[] = {
  {  0,  0, 34,104, 7},
  { 34,  0, 69,104, 6},
  { 69,  0,103,104, 5},
  {103,  0,137,104, 4},
  {137,  0,171,104, 3},
  {171,  0,206,104, 2},
  {206,  0,240,104, 1},

  {  0,104, 34,120, 1},
  { 69,104,103,120, 3},
  {137,104,171,120, 5},
  {206,104,240,120, 7},

  {  0,120, 44,160,10},
  { 44,120, 88,160, 9},
  { 88,120,132,160, 8},
  {171,120,183,160,11},
  {194,120,206,160,12},

  {-1}
};

static const BarsListEntry cbogrects[] = {
  {  0,  0,240,160, 7},

  { 34, 32, 69, 64, 6},
  { 69, 32,103, 64, 5},
  {103, 32,137, 64, 4},
  {137, 32,171, 64, 3},
  {171, 32,206, 64, 2},
  {206, 32,240, 64, 1},

  {  0, 96, 34,128, 1},
  { 34, 96, 69,128, 2},
  { 69, 96,103,128, 3},
  {103, 96,137,128, 4},
  {137, 96,171,128, 5},
  {171, 96,206,128, 6},

  {-1}
};

static const BarsListEntry plugerects[] = {
  { 16,  8, 32,152, 1},  // Left outer bar

  {208,  8,224,152, 1},  // Right outer bar

  // Inner bar is drawn separately

  { 80,  8,160, 44, 7},  // light grays
  { 80, 44,160, 80, 6},
  { 80, 80,160,116, 5},
  { 80,116,160,152, 4},

  {-1}
};

void draw_barslist(const BarsListEntry *rects) {
  canvasInit(&screen, 0);
  for(; rects->l < 240; ++rects) {
    canvasRectfill(&screen,
                   rects->l, rects->t, rects->r, rects->b,
                   rects->color);
  }
}

static void do_bars(const BarsListEntry *rects, helpdoc_kind helpsect) {
  unsigned int bright = 0, beep = 0;

  draw_barslist(rects);

  // Init sound to play 1 kHz tone
  REG_SOUNDCNT_X = 0x0080;  // 00: reset; 80: run
  REG_SOUNDBIAS = 0xC200;  // 4200: 65.5 kHz PWM (for PCM); C200: 262 kHz PWM (for PSG)
  REG_SOUNDCNT_H = 0x0002;  // PSG/PCM mixing
  REG_SOUNDCNT_L = 0xFF77;  // PSG vol/pan
  REG_SOUND3CNT_L = 0;  // unlock waveram
  dmaCopy(waveram_sin2x, (void *)WAVE_RAM, 16);
  REG_SOUND3CNT_L = 0xC0;    // lock waveram
  REG_SOUND3CNT_H = 0;       // volume control
  REG_SOUND3CNT_X = (2048 - 131) + 0x8000;  // full volume
  while (1) {
    read_pad_help_check(helpsect);
    if (new_keys & KEY_UP) {
      bright ^= 2;
    }
    if (new_keys & KEY_A) {
      bright ^= 1;
    }
    if (new_keys & KEY_SELECT) {
      beep = beep ^ 0x2000;
    }
    if (new_keys & KEY_B) {
      REG_SOUNDCNT_X = 0;  // turn sound off
      return;
    }

    VBlankIntrWait();
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFMAP);
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    dmaCopy(smptePalettes[bright], BG_COLORS+0x00, sizeof(smptePalettes[0]));
    REG_DISPCNT = MODE_0 | BG0_ON;
    REG_SOUND3CNT_H = beep;
  }
}

void activity_smpte(void) {
  do_bars(smpterects, helpsect_smpte_color_bars);
}

void activity_601bars(void) {
  do_bars(cbogrects, helpsect_color_bars_on_gray);
}

void activity_pluge(void) {
  unsigned int bright = 0;
  unsigned int shark = 0;

  draw_barslist(plugerects);

  // PLUGE rects is missing checkerboard stipples of colors 2 and 3
  // at (48, 8)-(64, 152) and (176, 8)-(192, 152)
  for (unsigned int x = 6; x < 24; x += 1) {
    if (x == 8) x = 22;
    unsigned int stride = screen.height * 8;
    uint32_t *tile = screen.chrBase + stride * x + 8;
    for (unsigned int yleft = 72; yleft > 0; --yleft) {
      *tile++ = 0x23232323;
      *tile++ = 0x32323232;
    }
  }
  
  LZ77UnCompVram(pluge_shark_6color_chrTiles, PATRAM4(0, 640));
  for (unsigned int y = 0; y < 20; ++y) {
    for (unsigned int x = 0; x < 30; ++x) {
      MAP[PFOVERLAY][y][x] = 640 + ((y & 0x03) << 2) + ((x + 1) & 0x03);
    }
  }

  while (1) {
    read_pad_help_check(helpsect_pluge);
    if (new_keys & KEY_A) {
      bright += 1;
      if (bright >= (shark ? 5 : 2)) bright = 0;
    }
    if (new_keys & KEY_SELECT) {
      shark = !shark;
      bright = 0;
    }
    if (new_keys & KEY_B) {
      return;
    }

    VBlankIntrWait();
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(shark ? PFOVERLAY : PFMAP);
    BG_OFFSET[0].y = BG_OFFSET[0].x = 0;
    if (shark) {
      dmaCopy(pluge_shark_palettes[bright], BG_COLORS+0x00, sizeof(pluge_shark_6color_chrPal));
    } else {
      dmaCopy(bright ? plugePaletteNTSCJ : plugePaletteNTSC, BG_COLORS+0x00, sizeof(plugePaletteNTSC));
    }
    REG_DISPCNT = MODE_0 | BG0_ON;
  }
}

static const unsigned short gcbars_palramps[4] = {
  RGB5(1, 0, 0),
  RGB5(0, 1, 0),
  RGB5(0, 0, 1),
  RGB5(1, 1, 1)
};

static const char gcbars_labels[] =
  "\x40""\x10""0\n"
  "\x54""\x10""2\n"
  "\x68""\x10""4\n"
  "\x7c""\x10""6\n"
  "\x90""\x10""8\n"
  "\xa4""\x10""A\n"
  "\xb8""\x10""C\n"
  "\xcc""\x10""E\n"
  "\x10""\x18""Red\n"
  "\x10""\x38""Green\n"
  "\x10""\x58""Blue\n"
  "\x10""\x78""White";

void activity_gcbars(void) {
  unsigned int bgctrl0 = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFMAP);

  load_common_bg_tiles();
  // Draw 5-pixel-wide vertical bars
  static const TileCanvas graiety = {
    8, 1, 5, 1, PATRAM4(0, 0x3B), PFMAP, 0, 0x3B
  };
  canvasInit(&graiety, 1);
  for (unsigned int i = 0; i < 8; ++i) {
    canvasRectfill(&graiety,
                   5 * i, 0, 5 + 5 * i, 8,
                   i + 8);
  }

  // Draw map
  dma_memset16(MAP[PFMAP], 0x0004, 32*20*2);
  unsigned int starttn = 0x003B;
  for (unsigned int y = 3; y < 18; ++y) {
    if ((y & 0x03) == 2) {
      ++y;
      starttn += 0x4000;
    }

    unsigned int tn = starttn;
    for (unsigned int x = 8; x < 28; ++x) {
      MAP[PFMAP][y][x] = tn;
      if ((++tn & 0xFF) == 0x40) {
        tn += 0x103B - 0x40;
      }
    }
  }
  
  // Draw secondary map (with grid)
  for (unsigned int y = 0; y < 20; ++y) {
    for (unsigned int x = 0; x < 32; ++x) {
      unsigned int tilenum = MAP[PFMAP][y][x];
      if (tilenum <= 4) {
        tilenum = 0x20;
        if (x & 1) tilenum |= 0x400;
        if (y & 1) tilenum |= 0x800;
      }
      MAP[PFOVERLAY][y][x] = tilenum;
    }
  }
  
  // Draw labels on primary map
  vwfDrawLabels(gcbars_labels, PFMAP, 0x40);

  while (1) {
    read_pad_help_check(helpsect_gradient_color_bars);
    if (new_keys & KEY_B) {
      return;
    }
    if (new_keys & KEY_A) {
      bgctrl0 ^= SCREEN_BASE(PFMAP) ^ SCREEN_BASE(PFOVERLAY);
    }

    VBlankIntrWait();
    BGCTRL[0] = bgctrl0;
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    BG_COLORS[0] = RGB5(0, 0, 0);
    BG_COLORS[1] = RGB5(31, 31, 31);

    // Calculate the color gradient
    unsigned int c = 0;
    const unsigned short *palramp = gcbars_palramps;
    for (unsigned int p = 8; p < 256; ++p) {
      if ((p & 0x0F) == 0) {
        p += 8;
        if ((p & 0x30) == 0) {
          ++palramp;
          c = 0;
        }
      }
      BG_COLORS[p] = c;
      c += *palramp;
    }
    REG_DISPCNT = MODE_0 | BG0_ON;
  }
}

void activity_gray_ramp(void) {

  // Draw 7-pixel-wide vertical bars
  static const TileCanvas graiety = {
    0, 1, 8, 1, PATRAM4(0, 0), PFMAP, 0, 0
  };
  canvasInit(&graiety, 1);
  for (unsigned int i = 0; i < 8; ++i) {
    canvasRectfill(&graiety,
                   8 + 7 * i, 0, 15 + 7 * i, 8,
                  i + 1);
  }

  // Draw map
  dma_memset16(MAP[PFMAP], 0x0000, 32*20*2);
  for (unsigned int y = 1; y < 10; ++y) {
    unsigned int tn = 0x0001;
    for (unsigned int x = 1; x < 29; ++x) {
      MAP[PFMAP][y][x] = tn;
      MAP[PFMAP][y + 9][29 - x] = tn | 0x0400;
      if ((++tn & 0x0F) == 0x08) {
        tn += 0x1001 - 0x08;
      }
    }
  }
  
  
  // TODO: Make the tilemap with palette for the custom canvas
  while (1) {
    read_pad_help_check(helpsect_gray_ramp);
    if (new_keys & KEY_B) {
      REG_BLDCNT = 0;
      return;
    }

    VBlankIntrWait();
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFMAP);
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    
    unsigned int c = 0;
    for (unsigned int p = 1; p < 64; ++p) {
      BG_COLORS[p] = c;
      c += RGB5(1, 1, 1);
      if ((p & 0x0F) == 8) p += 8;
    }
    REG_DISPCNT = MODE_0 | BG0_ON;
  }
}

static const uint32_t full_stripes_patterns[3][2] = {
  {0x11111111,0x22222222},
  {0x12121212,0x12121212},
  {0x12121212,0x21212121}
};

static const unsigned short full_stripes_colors[10][2] = {
  { RGB5(31, 0, 0), 0 },
  { RGB5(31,31, 0), 0 },
  { RGB5( 0,31, 0), 0 },
  { RGB5( 0,31,31), 0 },
  { RGB5( 0, 0,31), 0 },
  { RGB5(31, 0,31), 0 },
  { RGB5(31,31,31), 0 },
  { RGB5(31,31,31), 0 },
  { RGB5(16,16,16), 0 },
  { RGB5(31, 0, 0), RGB5( 0, 0,31) }
};


static void do_full_stripes(helpdoc_kind helpsect) {
  unsigned int pattern = 0, inverted = 0, frame = 0;
  unsigned int lcdcvalue = MODE_1 | BG1_ON;

  // tile 0: blank
  dma_memset16(PATRAM4(0, 0), 0x0000, 32);

  // row 19: frame counter
  dma_memset16(MAP[PFOVERLAY], 0x0000, 32*20*2);
  loadMapRowMajor(&(MAP[PFOVERLAY][19][24]), 0x6002, 6, 1);

  while (1) {
    read_pad_help_check(helpsect);
    autorepeat(KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT);
    if (das_timer <= 3) das_timer = 1;
    if (new_keys & KEY_A) {
      if (++pattern >= 3) pattern = 0;
    }
    if (new_keys & KEY_SELECT) {
      lcdcvalue ^= BG0_ON;
    }
    if (new_keys & (KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT)) {
      inverted ^= 0x33333333;
    }
    if (new_keys & KEY_B) {
      return;
    }

    if (++frame >= 60) frame = 0;
    posprintf(help_line_buffer, "Frame %02d", frame);

    VBlankIntrWait();
    BGCTRL[1] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFMAP);
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFOVERLAY);
    BG_OFFSET[0].x = BG_OFFSET[0].y = BG_OFFSET[1].x = 0;
    BG_OFFSET[1].y = 4;
    BG_COLORS[0] = RGB5(0, 0, 0);
    for (int c = 0; c < 10; ++c) {
      BG_COLORS[c * 16 + 1] = full_stripes_colors[c][0];
      BG_COLORS[c * 16 + 2] = full_stripes_colors[c][1];
    }
    REG_DISPCNT = lcdcvalue;

    // Draw the pattern
    uint32_t *dst = PATRAM4(0, 1);
    for (unsigned int i = 4; i > 0; --i) {
      *dst++ = full_stripes_patterns[pattern][0] ^ inverted;
      *dst++ = full_stripes_patterns[pattern][1] ^ inverted;
    }

    // Draw the frame counter
    dma_memset16(PATRAM4(0, 2), 0x2222, 32*6);
    vwf8Puts(PATRAM4(0, 2), help_line_buffer, 4, 1);
  }
}

void activity_full_stripes(void) {
  // Clear the screen
  dma_memset16(MAP[PFMAP], 0x6001, 32*21*2);
  do_full_stripes(helpsect_full_screen_stripes);
}

void activity_color_bleed(void) {
  // Clear the screen
  dma_memset16(MAP[PFMAP], 0x0000, 32*21*2);
  
  // Draw stripe regions
  for (unsigned int sy = 0; sy < 5; ++sy) {
    for (unsigned int sx = 0; sx < 2; ++sx) {
      unsigned short *src = &(MAP[PFMAP][sy * 4 + 1][sx * 15 + 1]);
      unsigned int tilenum = ((sy * 2 + sx) << 12) + 1;
      for (unsigned int j = 0; j < 96; j += 32) {
        dma_memset16(src + j, tilenum, 13*2);
      }
    }
  }
  do_full_stripes(helpsect_color_bleed);
}

static const unsigned short solid_colors[4] = {
  RGB5( 0, 0, 0),
  RGB5(31, 0, 0),
  RGB5( 0,31, 0),
  RGB5( 0, 0,31)
};

static const unsigned char rgbnames[3] = {'R', 'G', 'B'};

static void solid_color_draw_edit_box(const unsigned char rgb[3], unsigned int y) {
  dma_memset16(PATRAM4(0, 1), 0x2222, 32*9);
  for (unsigned int i = 0; i < 3; ++i) {
    uint32_t *dst = PATRAM4(0, 3 * i + 1);

    // Draw label
    if (i == y) vwf8PutTile(dst, '>', 1, 1);
    vwf8PutTile(dst, rgbnames[i], 6, 1);

    // Draw component value
    unsigned int value = rgb[i];
    uint32_t tens = value / 10;
    if (tens > 0) vwf8PutTile(dst, tens + '0', 14, 1);
    vwf8PutTile(dst, (value - tens * 10) + '0', 19, 1);
  }
}

void activity_solid_color(void) {
  unsigned int showeditbox = 0, x = 4, y = 0;
  unsigned char rgb[3] = {31, 31, 31};

  // Clear the screen
  dma_memset16(MAP[PFMAP], 0x0000, 32*20*2);
  dma_memset16(PATRAM4(0, 0), 0x0000, 32);

  // Allocate canvas for the RGB editing box
  loadMapRowMajor(&(MAP[PFMAP][1][26]), 0x0001, 3, 3);

  while (1) {
    read_pad_help_check(helpsect_solid_color_screen);
    if ((new_keys & KEY_A) && (x == 4)) {
      showeditbox = !showeditbox;
    }
    if (new_keys & KEY_B) {
      return;
    }
    if (showeditbox) {
      autorepeat(KEY_LEFT|KEY_RIGHT);
      if ((new_keys & KEY_UP) && y > 0) {
        --y;
      }
      if ((new_keys & KEY_DOWN) && y < 2) {
        ++y;
      }
      if ((new_keys & KEY_LEFT) && rgb[y] > 0) {
        --rgb[y];
      }
      if ((new_keys & KEY_RIGHT) && rgb[y] < 31) {
        ++rgb[y];
      }
      solid_color_draw_edit_box(rgb, y);
    } else {
      if (new_keys & KEY_RIGHT) {
        x += 1;
        if (x >= 5) x = 0;
      }
      if (new_keys & KEY_LEFT) {
        x = (x ? x : 5) - 1;
      }
    }

    VBlankIntrWait();
    BGCTRL[0] = BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)|SCREEN_BASE(PFMAP);
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    BG_COLORS[0] = x < 4 ? solid_colors[x] : RGB5(rgb[0], rgb[1], rgb[2]);
    BG_COLORS[1] = RGB5(31, 31, 31);
    BG_COLORS[2] = RGB5(0, 0, 0);
    REG_DISPCNT = showeditbox ? (MODE_0 | BG0_ON) : 0;
  }
}

void activity_convergence(void) {
  unsigned char cur_side = 0;
  unsigned char gridinvert = 0;
  unsigned char griddepth = 0;
  unsigned char colors_border = 0;

  bitunpack2(PATRAM4(0, 0), convergence_chrTiles, sizeof(convergence_chrTiles));
  dma_memset16(MAP[PFMAP], 0x04, 32*20*2); 
  for (unsigned int y = 0; y < 20; ++y) {
    unsigned int rowtilebase = (((y & 3) == 3) << 1) | ((y & 4) ? 0x2000 : 0);
    for (unsigned int x = 0; x < 30; ++x) {
      unsigned int tile = ((x & 3) == 3) | ((x & 4) ? 0x1000 : 0) | rowtilebase;
      MAP[PFOVERLAY][y][x] = tile;
    }
  }

  while (1) {
    read_pad_help_check(helpsect_convergence);

    if (new_keys & KEY_A) cur_side = !cur_side;
    if (new_keys & KEY_SELECT) gridinvert = !gridinvert;
    if (new_keys & KEY_B) return;
    if (cur_side != 0) {
      // Color transition boundary: Toggle border
      if (new_keys & (KEY_UP | KEY_DOWN)) {
        colors_border = !colors_border;
      }
    } else {
      if (new_keys & KEY_UP) griddepth += 2;
      if (new_keys & KEY_DOWN) griddepth += 1;
      if (griddepth >= 3) griddepth -= 3;
    }

    VBlankIntrWait();
    BGCTRL[0] = (BG_16_COLOR|BG_WID_32|BG_HT_32|CHAR_BASE(0)
                 |SCREEN_BASE(PFMAP-cur_side));
    BG_OFFSET[0].x = BG_OFFSET[0].y = 0;
    REG_DISPCNT = MODE_0 | BG0_ON;
    if (cur_side != 0) {
      BG_COLORS[0x03] = RGB5(31,31,31);
      BG_COLORS[0x13] = RGB5( 0, 0,31);
      BG_COLORS[0x23] = RGB5( 0,31, 0);
      BG_COLORS[0x33] = RGB5(31, 0, 0);
      BG_COLORS[0x02] = colors_border ? 0 : RGB5(31,31,31);
      BG_COLORS[0x12] = colors_border ? 0 : RGB5( 0, 0,31);
      BG_COLORS[0x22] = colors_border ? 0 : RGB5( 0,31, 0);
      BG_COLORS[0x32] = colors_border ? 0 : RGB5(31, 0, 0);
    } else {
      // Set palette for grid
      unsigned int color = gridinvert ? 0x7FFF : 0;
      for (unsigned int i = 0; i < 4; ++i) {
        BG_COLORS[i] = color;
        if (i == griddepth) color = color ^ 0x7FFF;
      }
    }
  }

}
