/*  Quick and dirty WAV output tone generator.
 *  Copyright (C) 2017 - Jérôme Kirman
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

typedef short sample;

#define SAMPLE_MAX SHRT_MAX

// Black GCC magic AFAIC.
struct __attribute__((packed)) header {
	// RIFF header
	char riff[4];
	unsigned int rlen;

	// > "WAVE"
	char wave[4];

	// > fmt header + contents
	char fmt[4];
	unsigned int flen;

	unsigned short dfmt;
	unsigned short nch;
	unsigned int sr;
	unsigned int bps;
	unsigned short cbps;
	unsigned short bd;

	// > data header
	char data[4];
	unsigned int dlen;
};

/* Note index :
 0  1   2  3  4   5  6   7  8  9   10  11
 A  Bb  B  C  C#  D  Eb  E  Fa F#  G   G
 La Sib Si Do Do# Re Mib Mi Fa Fa# Sol Lab
*/

double note (int n)
{
	return 440 * pow(2, ((double) n)/12);
}

int main (int argc, char* argv[])
{
	char* outfile = "out.wav";
	double f = note(0);

	if (argc > 1) { f = note(atoi(argv[1])); }
	if (argc > 2) { outfile = argv[2]; }

	unsigned int t = 7;

	unsigned short bd = 16, nch = 1;
	unsigned int sr = 48000;

	unsigned short flen = 2 + 2 + 4 + 4 + 2 + 2; // fmt contents
	unsigned int dlen = t * sr * nch * (bd/8);   // data contents

	unsigned int rlen = 4               // "WAVE"
	                  + (4 + 4) + flen  // fmt  (+hdr)
	                  + (4 + 4) + dlen; // data (+hdr)

	struct header hdr = {
	"RIFF", rlen,
		"WAVE",
		"fmt ", flen,
			0x1,
			nch,
			sr,
			nch*sr*(bd/8),
			nch*(bd/8),
			bd,
		"data", dlen
	};

	FILE* of = fopen(outfile, "w");

	fwrite (&hdr, sizeof(hdr), 1, of);

	float amp = 0.0;
	sample s = 0;
	for (unsigned i = 0 ; i < t * sr ; i++)
	{
		/* Undertale - (C) Toby Fox
		 . < v > ^
		 3 5 7 8 10
		
		 . ^  > . v v >
		 3 10 8 3 7 7 8... 3 8 3 7 7 8
		*/
		switch (20*i / sr)
		{
			case   0: f = note( 3); break;
			case  10: f = note(10); break;
			case  20: f = note( 8); break;
			case  30: f = note( 3); break;
			case  40: f = note( 7); break;
			case  50: f = note( 7); break;
			case  60: f = note( 8); break;
			case  70: f = note( 3); break;
			case  80: f = note( 8); break;
			case  90: f = note( 3); break;
			case 100: f = note( 7); break;
			case 110: f = note( 7); break;
			case 120: f = note( 8); break;
		}
		if (20*i / sr % 10 == 9) { f = 0; }

		amp = (float) sin(i * 2*M_PI * f / sr);
		s = amp * SAMPLE_MAX/8;
		fwrite (&s, sizeof(sample), 1, of);
	}

	fclose (of);

	return EXIT_SUCCESS;
}
