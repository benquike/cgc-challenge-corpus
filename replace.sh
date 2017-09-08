#!/bin/bash

words=(digittoint isascii ispunct iscntrl isxdigit freaduntil fflush fbuffered fxlat vfprintf vsprintf sprintf htobe32 htobe16 be32toh be16toh read sleep FILE mkdir rmdir exit fopen unlink link fread fwrite fclose write malloc calloc free isspace isdigit isnan isinf atof atoi islower isupper isalpha isalnum memcpy memcmp memeq vfdprintf fdprintf strcpy strncpy putc printf bzero memset strcmp strncat receive_until strcat strlen itoa puts strchr strtok strtol strtoul realloc strdup pause)

for w in "${words[@]}"; do
    echo "replacing $w"
    ag -l $w|xargs sed -i "s/\b$w\b/__cgc_$w/g"
    git checkout poller
    git checkout pov
done

ag -l "__cgc_malloc\.h" | xargs sed -i "s/__cgc_malloc\.h/malloc\.h/g"
ag -l "__cgc_printf\.h" | xargs sed -i "s/__cgc_printf\.h/printf\.h/g"
ag -l "__cgc_link\.h" | xargs sed -i "s/__cgc_link\.h/link\.h/g"
ag -l "__cgc_strdup\.h" | xargs sed -i "s/__cgc_strdup\.h/strdup\.h/g"
ag -l "__cgc_malloc\.h" | xargs sed -i "s/__cgc_malloc\.h/malloc\.h/g"
ag -l "__cgc_read\.h" | xargs sed -i "s/__cgc_read\.h/read\.h/g"
ag -l "__cgc_memset\.h" | xargs sed -i "s/__cgc_memset\.h/memset\.h/g"
ag -l "__cgc_memcmp\.h" | xargs sed -i "s/__cgc_memcmp\.h/memcmp\.h/g"
ag -l "__cgc_memcpy\.h" | xargs sed -i "s/__cgc_memcpy\.h/memcpy\.h/g"
ag -l "__cgc_strncpy\.h" | xargs sed -i "s/__cgc_strncpy\.h/strncpy\.h/g"
ag -l "__cgc_strlen\.h" | xargs sed -i "s/__cgc_strlen\.h/strlen\.h/g"
ag -l "__cgc_strtok\.h" | xargs sed -i "s/__cgc_strtok\.h/strtok\.h/g"
ag -l "__cgc_sleep\.h" | xargs sed -i "s/__cgc_sleep\.h/sleep\.h/g"
ag -l "__cgc_strtol\.c" | xargs sed -i 's/__cgc_strtol\.c/strtol\.c/g'
ag -l "__cgc_atoi\.h" | xargs sed -i 's/__cgc_atoi\.h/atoi\.h/g'
ag -l "^void operator delete.*$" | xargs sed -E -i 's/^(void operator delete.*)$/\1 throw\(\)/g'
ag -l __dso_handle | xargs sed -i -E 's/void \*__dso_handle/extern void \*__dso_handle/g'
ag -l _SC_PAGESIZE | xargs sed -i 's/\b_SC_PAGESIZE\b/__CGC_SC_PAGESIZE_/g'
