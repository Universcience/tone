/*  Quick and dirty WAV output tone generator.
 *  Copyright (C) 2017-2019 - Jérôme Kirman
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

struct header {
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

#define Note(n) (440 * pow(2, n/12.0))

#define T 7

#define BD 16
#define NCH 1
#define SR 48000

#define FLEN (2 + 2 + 4 + 4 + 2 + 2) // "fmt " contents
#define DLEN (T * SR * NCH * (BD/8)) // "data" contents

// "WAVE" + ("fmt " + FLEN + contents) + ("data" + DLEN + contents)
#define RLEN (4 + (4 + 4 + FLEN) + (4 + 4 + DLEN))

int main (int argc, char* argv[])
{
	const char* outfile = "out.wav";
	double f = Note(0);

	if (argc > 1) { f = Note(atoi(argv[1])); }
	if (argc > 2) { outfile = argv[2]; }

	struct header hdr = {
	"RIFF", RLEN,
		"WAVE",
		"fmt ", FLEN,
			0x1,
			NCH,
			SR,
			NCH*SR*(BD/8),
			NCH*(BD/8),
			BD,
		"data", DLEN
	};

	FILE* of = fopen(outfile, "w");

	fwrite (&hdr, sizeof(hdr), 1, of);

	double amp = 0.0;
	sample s = 0;
	for (unsigned i = 0 ; i < T * SR ; ++i)
	{
		/* Undertale - (C) Toby Fox
		 . < v > ^
		 3 5 7 8 10
		
		 . ^  > . v v >
		 3 10 8 3 7 7 8... 3 8 3 7 7 8
		*/
		switch (20*i / SR)
		{
			case   0: f = Note( 3); break;
			case  10: f = Note(10); break;
			case  20: f = Note( 8); break;
			case  30: f = Note( 3); break;
			case  40: f = Note( 7); break;
			case  50: f = Note( 7); break;
			case  60: f = Note( 8); break;
			case  70: f = Note( 3); break;
			case  80: f = Note( 8); break;
			case  90: f = Note( 3); break;
			case 100: f = Note( 7); break;
			case 110: f = Note( 7); break;
			case 120: f = Note( 8); break;
			default: break;
		}
		if (20*i / SR % 10 == 9) { f = 0; }

		amp = sin(i * 2*M_PI * f / SR);
		s = (sample) lrint(amp * SAMPLE_MAX/8);
		fwrite (&s, sizeof(sample), 1, of);
	}

	fclose (of);

	return EXIT_SUCCESS;
}
