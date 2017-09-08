/*
 Copyright (c) 2014 Cromulence LLC
 
 Permission is hereby granted, __free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#include "file.h"

extern pfile root;

int bubble_sort( pfile parent )
{
    int outer_index = 0;
    int total_count = 0;
    pfile *list = NULL;
    pfile nl = NULL;
    int result = 0;
    
    if (parent == NULL) {
        return 0;
    }
    
    if ( parent->data == NULL ) {
        return 0;
    }
    
    /// Do an initial count on the list since it is possible
    /// that some have been freed. If this is the case then fix
    /// up the pointers and modify the length
    list = (pfile*)parent->data;
    
    while ( outer_index < parent->length ) {
        if ( list[outer_index] != NULL ) {
            total_count++;
        }
        
        outer_index++;
    }
    
    if (total_count < parent->length ) {
        outer_index = 0;
        total_count = 0;
        
        while ( outer_index < parent->length ) {
            if ( list[outer_index] != NULL ) {
                list[total_count] = list[outer_index];
                total_count++;
            }
            
            outer_index++;
        }
        
        outer_index = total_count;
        
        /// Clear out the remainders
        while ( total_count < parent->length) {
            list[total_count] = NULL;
            total_count++;
        }
        
#ifdef PATCHED
        /// Update the correct length
        parent->length = outer_index;
#endif
        
    }
    
    outer_index = 0;
    
    while ( outer_index < parent->length) {
        total_count = outer_index+1;
        
        while ( total_count < parent->length ) {
            result = __strcmp( list[outer_index]->name, list[total_count]->name);
            
            if ( result > 0 ) {
                nl = list[total_count];
                list[total_count] = list[outer_index];
                list[outer_index] = nl;
            }
            
            total_count++;
        }
        outer_index++;
    }
    
    return 1;
}

int remove_sub_file( pfile parent, char *name )
{
    pfile *list = NULL;
    int index = 0;
    pfile t = NULL;
    
    if ( parent == NULL || name == NULL ) {
        return 0;
    }
    
    if ( parent->data == NULL ) {
        return 0;
    }
    
    list = (pfile*)parent->data;
    
    while ( index < parent->length ) {
        t = list[index];
        
        if ( t == NULL ) {
            index++;
            continue;
        }
        
        if ( __strcmp( t->name, name ) == 0 ) {
            
            if ( t->type == DIR ) {
                __printf("[ERROR] Cannot delete a directory\n");
                return 0;
            }
            
            free_file( t );
            list[index] = NULL;
            return 1;
        }
        
        index ++;
    }
    
    return 0;
}

int delete_file( char *name )
{
    pfile base = root;
    char nm[256];
    int start = 0;
    int end = 0;
    int max = 0;
    
    if ( name == NULL ) {
        return 0;
    }
    
    /// Handle base case
    if ( name[0] != '/' ) {
        if ( does_sub_file_exist( base, name ) == 0 ) {
            __printf("[ERROR] Could not locate $s\n", name);
            return 0;
        }
        
        if ( remove_sub_file( base, name ) != 0 ) {
            __printf("[INFO] $s removed\n", name );
            return 1;
        } else {
            return 0;
        }
    }
    
    start = 1;
    max = __strlen(name);
   
    if ( max > 256 ) {
        __printf("[ERROR] Name too long\n");
        return 0;
    }
 
    while ( end != -1 ) {
        end = find_next_slash( name, start, max );
        
        __memset(nm, 0, 256);
        
        if ( end == -1 ) {

	    if ( (max - start) > 256 ) {
               __printf("[ERROR] Size calculation failed\n");
               return 0;
	    }

            __memcpy( nm, name+start, max-start);
            
            if ( does_sub_file_exist( base, nm ) == 0 ) {
                __printf("[ERROR] Could not locate $s\n", name );
                return 0;
            }
            
            if ( remove_sub_file( base, nm ) != 0 ) {
                __printf("[INFO] $s removed\n", nm );
                return 1;
            } else {
                return 0;
            }
        }
       
        if ( (end-start) > 256 ) {
            __printf("[ERROR] Size calculation failed\n");
            return 0;
        }
 
        __memcpy( nm, name+start, end-start);
        base = retrieve_sub( base, nm );
        
        if ( base == NULL ) {
            __printf("[ERROR] Failed to locate directory $s\n", nm);
            return 0;
        }

	if ( base->type != DIR ) {
	    return 0;
        }
        
        start = end + 1;
    
    }
    
    return 0;
}

pfile retrieve_sub( pfile pf, char *name )
{
    pfile *list = NULL;
    int index = 0;
    pfile t = NULL;
    
    if ( pf == NULL || name == NULL ) {
        return NULL;
    }
    
    if ( pf->type != DIR ) {
        return NULL;
    }

    list = (pfile*)pf->data;
    
    while ( index < pf->length ) {
        t = list[index];
        
        if ( t == NULL ) {
            index++;
            continue;
        }
        
        if ( __strcmp( t->name, name ) == 0 ) {
            return t;
        }
        
        index++;
    }
    
    return NULL;
}

int find_next_slash( char *str, int start, int max )
{
    int index = -1;
    
    if ( str == NULL ) {
        return -1;
    }
    
    while ( start < max ) {
        if ( str[start] == '/' ) {
            index = start;
            break;
        } else {
            start++;
        }
    }
    
    return index;
}

int fixup_dir_length( pfile d )
{
    char *nd = NULL;
    
    if ( d == NULL ) {
        return 0;
    }
    
    /// Base case
    if ( d->length == 0 ) {
        d->length = 1;
        
        /// 8 is used because it is rounded up anyway
        d->data = __malloc( 8 );
        
        if ( d->data == NULL ) {
            __printf("[ERROR] Failed to fixup dir data\n");
            d->length = 0;
            return 0;
        }
        
        __memset(d->data, 0, 8);
    } else {
        /// Increment it by 1
        d->length += 1;
        
        nd = __malloc( d->length * sizeof(pfile));
        
        if ( nd == NULL ) {
            d->length--;
            return 0;
        }
        
        __memset(nd, 0, d->length * sizeof(pfile));
        __memcpy(nd, d->data, (d->length-1)*sizeof(pfile));
        __free(d->data);
        d->data = nd;
    }
    
    return 1;
}

pfile get_file( char *name )
{
    pfile cbase = root;
    pfile rv = NULL;
    int start = 0;
    int end = 0;
    int max = 0;
    
    char sdir[256];
    
    if ( name == NULL ) {
        return NULL;
    }
    
    /// Handle the base case
    if ( name[0] != '/') {
        rv = retrieve_sub( cbase, name );
        
        return rv;
    }
    
    start = 1;
    max = __strlen(name);
    
    while ( end != -1 ) {
        end = find_next_slash( name, start, max );
        
        if ( end == -1 ) {
            __memset( sdir, 0, 256 );
            __memcpy( sdir, name+start, max-start);
            
            rv = retrieve_sub( cbase, sdir );
            
            return rv;
        }
        
        __memset( sdir, 0, 256 );
        __memcpy( sdir, name+start, end-start);
        
        cbase = retrieve_sub( cbase, sdir );
        
        start = end + 1;
        if ( cbase ) {
            if (cbase->type != DIR ) {
                __printf("[ERROR] $s is not a directory\n", sdir );
                return NULL;
            }
        } else {
            return cbase;
        }
    }
    
    return NULL;
}

int does_sub_file_exist( pfile pf, char *name)
{
    pfile *list = NULL;
    int index = 0;
    pfile t = NULL;
    
    list = (pfile*)pf->data;
    
    while ( index < pf->length ) {
        t = list[index];
        
        if ( t == NULL ) {
            index++;
            continue;
        }
        
        if ( __strcmp( t->name, name ) == 0 ) {
            return 1;
        }
        
        index++;
    }
    
    return 0;
}

int add_file_to_dir( pfile dir, pfile file )
{
    if ( dir == NULL || file == NULL ) {
        return 0;
    }
    
    if ( fixup_dir_length( dir ) == 0 ) {
        return 0;
    }
    
    ((pfile*)dir->data)[ dir->length-1] = file;
    
    return 1;
}

int add_file( pfile nf )
{
    int start = 0;
    int end = 0;
    char base[256];
    char *nd = NULL;
    int max = 0;
    pfile cbase_dir = root;
    pfile temp = NULL;
    
    if ( nf == NULL ) {
        return 0;
    }

    // If it does not start with a slash then it is just added to root
    // It is possible to have '/' in the name but it won't affect it
    if ( nf->name[0] != '/' ) {
        if ( add_file_to_dir( cbase_dir, nf ) == 0 ) {
            __printf("[ERROR] Failed to add file to root\n");
            return 0;
        }
        
        return 1;
    }
    
    start = 1;
    max = __strlen(nf->name);
    
    /// Handle the case where the name is just '/'
    if ( max == 1 ) {
        __printf("[ERROR] You cannot add '/'\n");
        return 0;
    }
    
    while ( end != -1 ) {
        end = find_next_slash( nf->name, start, max );
        
        /// If this is the end then copy out the name and add it in
        if ( end == -1 ) {
            __memset(base, 0, 256);
            __memcpy( base, nf->name+start, max-start );
            __memset( nf->name, 0, 256);
            __memcpy( nf->name, base, max - start );
            
            if ( does_sub_file_exist( cbase_dir, nf->name) == 1) {
                __printf("[ERROR] File already exists\n");
                return 0;
            }
            
            if ( add_file_to_dir( cbase_dir, nf) == 0 ) {
                __printf("[ERROR] Failed to add file to $s\n", cbase_dir->name);
                return 0;
            }
            
            return 1;
        } else {
            __memset( base, 0, 256);
            
            /// Copy the dir name and determine if it is valid
            __memcpy( base, nf->name+start, end-start);
            
            temp = retrieve_sub( cbase_dir, base );
            
            if ( temp == NULL ) {
                __printf("[ERROR] Directory $s does not exist.\n", base);
                return 0;
            } else if ( temp->type != DIR ) {
                __printf("[ERROR] $s is not a directory\n", base );
                return 0;
            } else {
                cbase_dir = temp;
            }
            
            start = end + 1;
        }
    }
    return 1;
}


void free_file( pfile f )
{
    if ( f == NULL ) {
        return;
    }
    
    if ( f->data != NULL ) {
        __free(f->data);
    }
    
    __free(f);
    
    return;
}

pfile init_file( void )
{
	pfile new_file = NULL;

	new_file = __malloc( sizeof(file) );

	if ( new_file == NULL ) {
		return new_file;
	}

	__memset( new_file, 0, sizeof(file) );

	return new_file;
}

int set_name( pfile pf, char *name)
{
    int length = 0;
    
    if (pf == NULL ) {
        return 0;
    }
    
    if ( name == NULL ) {
        return 0;
    }
    
    length = __strlen( name );
    
    if ( length > 255 ) {
        return 0;
    }
    
    __memcpy( pf->name, name, length );
    
    return length;
}

int set_type( pfile pf, int type )
{
    if ( pf == NULL ) {
        return 0;
    }
    
    if ( type != __FILE && type != DIR ) {
        return 0;
    }
    
    pf->type = type;
    
    return 1;
}

int set_data( pfile pf, int length, char *data )
{
    char *tn = NULL;
    
    if ( pf == NULL ) {
        return 0;
    }
    
    if ( data == NULL ) {
        return 0;
    }
    
    if ( length > 0x400 ) {
        return 0;
    }
    
    tn = __malloc( length + 1 );
    
    if ( tn == NULL ) {
        return 0;
    }
    
    __memcpy( tn, data, length + 1 );
    
    pf->length = length;
    pf->data = tn;
    
    return 1;
}
