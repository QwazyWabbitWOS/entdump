// From http://www.r1ch.net/stuff/r1q2/ - thanks R1ch.
//GPL etc.
//
// entdump is used for extracting entities from quake2 bsp files in text
// format for usage with the added ent file support in Xatrix+
//
// Build like: gcc -o entdump entdump.c
// Use like:  # ./entdump map.bsp > map.ent
// Nick I fixed so that the ent.file didn't have a double newline at the end.

// January 11, 2010, QwazyWabbit added texture file name output, usage info

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define	LUMP_ENTITIES		0
#define	LUMP_PLANES			1
#define	LUMP_VERTEXES		2
#define	LUMP_VISIBILITY		3
#define	LUMP_NODES			4
#define	LUMP_TEXINFO		5
#define	LUMP_FACES			6
#define	LUMP_LIGHTING		7
#define	LUMP_LEAFS			8
#define	LUMP_LEAFFACES		9
#define	LUMP_LEAFBRUSHES	10
#define	LUMP_EDGES			11
#define	LUMP_SURFEDGES		12
#define	LUMP_MODELS			13
#define	LUMP_BRUSHES		14
#define	LUMP_BRUSHSIDES		15
#define	LUMP_POP			16
#define	LUMP_AREAS			17
#define	LUMP_AREAPORTALS	18
#define	HEADER_LUMPS		19

#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'I')
// little-endian "IBSP"

#define BSPVERSION	38


// upper design bounds
// leaffaces, leafbrushes, planes, and verts are still bounded by
// 16 bit short limits
#define	MAX_MAP_MODELS		1024
#define	MAX_MAP_BRUSHES		8192
#define	MAX_MAP_ENTITIES	2048
#define	MAX_MAP_ENTSTRING	0x40000
#define	MAX_MAP_TEXINFO		8192

#define	MAX_MAP_AREAS		256
#define	MAX_MAP_AREAPORTALS	1024
#define	MAX_MAP_PLANES		65536
#define	MAX_MAP_NODES		65536
#define	MAX_MAP_BRUSHSIDES	65536
#define	MAX_MAP_LEAFS		65536
#define	MAX_MAP_VERTS		65536
#define	MAX_MAP_FACES		65536
#define	MAX_MAP_LEAFFACES	65536
#define	MAX_MAP_LEAFBRUSHES 65536
#define	MAX_MAP_PORTALS		65536
#define	MAX_MAP_EDGES		128000
#define	MAX_MAP_SURFEDGES	256000
#define	MAX_MAP_LIGHTING	0x200000
#define	MAX_MAP_VISIBILITY	0x100000

typedef struct
{
	int		fileofs;
	int		filelen;
} lump_t;

typedef struct
{
	int			ident;
	int			version;	
	lump_t		lumps[HEADER_LUMPS];
} dheader_t;

typedef struct texinfo_s
{
	float		vecs[2][4];		// [s/t][xyz offset]
	int			flags;			// miptex flags + overrides
	int			value;			// light emission, etc
	char		texture[32];	// texture name (textures/*.wal)
	int			nexttexinfo;	// for animations, -1 = end of chain
} texinfo_t;

typedef struct csurface_s
{
	char		name[16];
	int			flags;
	int			value;
} csurface_t;

typedef struct mapsurface_s  // used internally due to name len probs //ZOID
{
	csurface_t	c;
	char		rname[32];
	int			dupe;	//QW// added
} mapsurface_t;

typedef unsigned char byte;

byte	*cmod_base;
int		numtexinfo;
char	map_entitystring[MAX_MAP_ENTSTRING];
mapsurface_t	map_surfaces[MAX_MAP_TEXINFO];

// arg state from zardoru
int print_textures = 0;
int print_entities = 1;

void CMod_LoadEntityString (lump_t *l)
{
	char *newline; // Nick - new pointer for newline.
	
	if (l->filelen > MAX_MAP_ENTSTRING) {
		fprintf(stderr, "Map has too large entity lump (%d > %d)", l->filelen, MAX_MAP_ENTSTRING);
		exit(EXIT_FAILURE);
	}

	memcpy (map_entitystring, cmod_base + l->fileofs, l->filelen);
	
	newline = &map_entitystring[strlen(map_entitystring)-1];	
	if (strcmp(newline, "\n") == 0)		// Nick - check if already a newline present
		map_entitystring[strlen(map_entitystring)-1] = '\0';	// Nick - Clear it if there is
	
	printf ("%s\n", map_entitystring); // Newline here (so only one).
}

/*
//QW// pulled this from quake2 engine source and modified
*/
void CMod_LoadSurfaces (lump_t *l)
{
	texinfo_t	*in;
	mapsurface_t	*out, *list;
	int		i, j, count;
	int		uniques;
	
	in = (void *)(cmod_base + l->fileofs);
	if (l->filelen % sizeof(*in)) {
		fprintf(stderr, "CMod_LoadSurfaces: funny lump size");
		exit(EXIT_FAILURE);
	}
	count = l->filelen / sizeof(*in);
	if (count < 1) {
		fprintf(stderr, "CMod_LoadSurfaces: Map with no surfaces");
		exit(EXIT_FAILURE);
	}
	if (count > MAX_MAP_TEXINFO) {
		fprintf(stderr, "CMod_LoadSurfaces: Map has too many surfaces");
		exit(EXIT_FAILURE);
	}
	
	numtexinfo = count;
	out = map_surfaces;
	uniques = 0;
	
	for (i=0; i<count; i++, in++, out++)
	{
		strncpy (out->c.name, in->texture, sizeof(out->c.name)-1);
		strncpy (out->rname, in->texture, sizeof(out->rname)-1);
		out->c.flags = in->flags;
		out->c.value = in->value;
		out->dupe = 0;

		list = map_surfaces;
		for (j=0; j<count; j++, list++)	// identify each unique texture name
		{
			if (strcmp(list->rname,"") != 0 && list != out 
				&& strcmp(list->rname, out->rname) == 0)
					out->dupe = 1;	//flag the duplicate
		}
		if (out->dupe == 0) 
		{
			uniques++;
			printf ("%s\n", out->rname);
		}
	}
	printf("Map uses %i unique textures %i times\n", uniques, count);
}

// Nice option parser from zardoru's entdump rewrite.
void parse_args(char** argv, int argc) {
	int print_textures_seen = 0;

	for (int i = 1; i < argc - 1; i++) {
		if (argv[i][0] == '-') {
			size_t arglen = strlen(argv[i]);

			// parse t, e, whatever
			for (size_t j = 1; j < arglen; j++) {
				char opt = argv[i][j];
				switch (opt) {
				case 't':
					print_textures = 1;
					if (!print_textures_seen) {
						print_entities = 0;
						print_textures_seen = 1;
					}
					break;
				case 'e':
					print_entities = 1;
					break;
				default:
					printf("unknown option '%c'\n", opt);
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	byte		*buf;
	int			len;
	dheader_t	header;
	FILE		*in = NULL;
	
	if (argc > 1)
	{
		in = fopen(argv[argc - 1], "rb");
		if (!in) 
		{
			fprintf (stderr, "FATAL ERROR: fopen() on %s failed.\n", argv[1]);
			exit (EXIT_FAILURE);
		}
	}
	else
	{
		//print usage info
		printf("Entdump is used for extracting entities from quake2 bsp files in text format\n");
		printf("for usage with the added ent file support in Xatrix+ and other mods.\n");
		printf("Usage: entdump mapname.bsp \n");
		printf("   or: entdump mapname.bsp > mapname.ent \n");
		printf("   or: entdump mapname.bsp | more \n");
		printf("   or: entdump -t mapname.bsp | more \n");
		printf("   or: entdump -t -e mapname.bsp | more \n");
		exit(EXIT_FAILURE);
	}
	
	parse_args(argv, argc);

	fseek (in, 0, SEEK_END);
	len = ftell (in);
	fseek (in, 0, SEEK_SET);
	
	buf = malloc(len);
	if (!buf)
	{
		fprintf(stderr, "Memory allocation failed!\n");
		exit(EXIT_FAILURE);
	}

	fread (buf, len, 1, in);
	
	//map header structs onto the buffer
	header = *(dheader_t *)buf;
	
	if (header.version != BSPVERSION) {
		fprintf(stderr, "This is not a valid BSP file.");
		exit(EXIT_FAILURE);
	}

	cmod_base = buf;
	
	if (print_textures) {
		CMod_LoadSurfaces(&header.lumps[LUMP_TEXINFO]);
		printf("Map entities:\n");
	}
		if (print_entities) {
		CMod_LoadEntityString(&header.lumps[LUMP_ENTITIES]);
	}

	free (buf);
	fclose (in);
	return EXIT_SUCCESS;
}
