#define __htobe32(x) (((((x) >> 24) & 0xff) <<  0) | \
                    ((((x) >> 16) & 0xff) <<  8) | \
                    ((((x) >>  8) & 0xff) << 16) | \
                    ((((x) >>  0) & 0xff) << 24))

#define __htobe16(x) (((((x) >>  8) & 0xff) <<  0) | \
                    ((((x) >>  0) & 0xff) <<  8))

#define __be32toh __htobe32
#define __be16toh __htobe16
