/*

Author: John Berry <hj@cromulence.co>

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

#include "planetParsers.h"

/**
 * Menu to manage a planet
 * @param pl Pointer to the planet to manage
 * @return Returns 0 if planet pointer should be NULL 1 otherwise
 **/
int planetMenu( pPlanet pl )
{
	int choice = 0;
	char selection[30];
	size_t length = 0;
	pCountry pc = NULL;

	if ( pl == NULL ) {
		return 0;
	}

	while ( 1 ) {
		__bzero(selection, 30);

		__printf("\nPlanet: @s\n", pl->name);
		__printf("1) Display planet information\n");
		__printf("2) Set Period\n");
		__printf("3) Set Orbit Speed\n");
		__printf("4) Set Aphelion\n");
		__printf("5) Set Perihelion\n");
		__printf("6) Set Mean Radius\n");
		__printf("7) Set Equatorial Radius\n");
		__printf("8) Set Mass\n");
		__printf("9) Set Gravity\n");
		__printf("10) Set population\n");
		__printf("11) Add Country\n");
		__printf("12) Select country\n");
		__printf("13) Delete Planet and __exit menu\n");
		__printf("14) Exit menu\n");
		__printf("Selection: ");

		__receive_until(selection, '\n', 3);
		choice = __atoi( selection );

		switch ( choice ) {
			case 1:
				printPlanetInfo( pl );
				break;
			case 2:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->period = __atof(selection);
				break;
			case 3:
				__printf("\n-> ");
				__receive_until(selection, '\n', 10);
				pl->orbitspeed = __atof(selection);
				break;
			case 4:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->aphelion = __atof(selection);
				break;
			case 5:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->perihelion = __atof(selection);
				break;
			case 6:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->radius = __atof(selection);
				break;
			case 7:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->eradius = __atof(selection);
				break;
			case 8:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->mass = __atof(selection);
				break;
			case 9:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->gravity = __atof(selection);
				break;
			case 10:
				__printf("\n-> ");
				__receive_until( selection, '\n', 10);
				pl->population = __atoi(selection);
				break;
			case 11:
				if ( pl->country_count >= COUNTRYMAX ) {
					__printf("Too many countries\n");
					continue;
				}

				__printf("\nNew Name: ");
				__receive_until(selection, '\n', 19);

				choice = 0;
				while ( choice < COUNTRYMAX ) {
					if ( pl->countries[choice] == NULL ) {
						break;
					}
					choice++;
				}

				if ( choice == COUNTRYMAX ) {
					__printf("!!No country slots\n");
					continue;
				}

				if ( allocate( sizeof(Country), 0, (void**)(&pc) ) != 0 ) {
					__printf("!!Failed to allocate structure\n");
					continue;
				}

				initCountry( pc );

				pl->countries[choice] = pc;

				length = 0;
	
				while ( __isalnum( selection[length] ) ) {
					pc->name[length] = selection[length];
					length++;
				}
				pc->name[length] = '\x00';

				pl->country_count++;
				break;
			case 12:
				__printf("\nCountries:\n");
				for ( choice = 0; choice < pl->country_count; choice++ ) {
					if ( pl->countries[choice] != NULL ) {
						__printf("@d) @s\n", choice + 1, pl->countries[choice]);
					}
				}

				__bzero(selection, 30);
				__printf("\n-> ");
				__receive_until( selection, '\n', 3 );

				choice = __atoi(selection);

				if ( choice < 1 || choice > COUNTRYMAX) {
					__printf("Invalid choice...\n");
					continue;
				} else if ( pl->countries[ choice-1] == NULL ) {
					__printf("Invalid choice...\n");
					continue;
				}

				if ( countryMenu( pl->countries[choice-1]) == 0 ) {
					pl->countries[choice-1] = NULL;
					pl->country_count--;
				}
				break;
			case 13:
				freePlanet( pl );
				return 0;
				break;
			case 14:
				return 1;
				break;
			default:
				__printf("Invalid...\n");
				break;
		};
	}

end:
	return 1;
}

/**
 * Frees the planet specified by the argument
 * @param pl Pointer to the planet structure
 * @return Returns nothing
 **/
void freePlanet( pPlanet pl )
{
	int index = 0;

	if ( pl == NULL ) {
		return;
	}

	/// Free all of the associated countries
	while ( index < COUNTRYMAX ) {
		if ( pl->countries[ index] != NULL ) {

			/// This function also frees the pointer so freePlanet does not have to.
			freeCountry( pl->countries[index] );
			pl->countries[index] = NULL;

		}

		index++;
	}
	
	deallocate( pl, sizeof(Planet) );

	return;
}

/**
 * Print the planet information
 * @param pl Pointer to the planet structure
 * @return Returns nothing
 **/
void printPlanetInfo( pPlanet pl )
{
        int index = 0;

        if ( pl == NULL ) {
                return;
        }

        if ( pl->name[0] == '\x00' ) {
                __printf("Name: Unknown\n");
        } else {
                __printf("Name: @s\n", pl->name );
        }

        if ( pl->population >= 0 ) {
                __printf("\tPopulation: @d\n", pl->population);
        }

        if ( pl->period >= 0.0 ) {
                __printf("\tPeriod: @f\n", pl->period);
        }

        if ( pl->orbitspeed >= 0.0 ) {
                __printf("\tOrbit Speed: @f\n", pl->orbitspeed);
        }

        if ( pl->aphelion >= 0.0 ) {
                __printf("\tAphelion: @f\n", pl->aphelion);
        }

        if ( pl->perihelion >= 0.0 ) {
                __printf("\tPerihelion: @f\n", pl->perihelion);
        }

        if ( pl->radius >= 0.0 ) {
                __printf("\tRadius: @f\n", pl->radius);
        }

        if ( pl->eradius >= 0.0 ) {
                __printf("\tERadius: @f\n", pl->eradius);
        }
        if ( pl->mass >= 0.0 ) {
                __printf("\tMass: @f\n", pl->mass);
        }
	
	if ( pl->gravity >= 0.0 ) {
		__printf("\tGravity: @f\n", pl->gravity);
	}

	/// Print each country's information
        while ( index < COUNTRYMAX ) {
                if ( pl->countries[ index ] != NULL ) {
                        printCountryInfo( pl->countries[index] );
                }

                index++;
        }

        return;
}

/**
 * Initialize all the planets members
 * @param pl Pointer to the planet structure
 * @return 0 on failure 1 on success
 **/
void initPlanet( pPlanet pl )
{
        register int index = 0;

        if ( pl == NULL ) {
                return;
        }

        __bzero( pl->name, 20 );
        pl->period = -1.0;
        pl->orbitspeed = -1.0;
        pl->aphelion = -1.0;
        pl->perihelion = -1.0;
        pl->radius = -1.0;
        pl->eradius = -1.0;
        pl->mass = -1.0;
        pl->gravity = -1.0;
        pl->population = -1.0;

        pl->country_count = 0;

        while ( index < COUNTRYMAX ) {
                pl->countries[index] = NULL;
                index++;
        }

        return;
}

/**
 * Parse the planet string and its sub elements
 * @param topLevel Pointer to the string structure to be parsed
 * @return Returns a pointer to a planet structure with associated data. Null on failure
 **/
pPlanet planetTopLevel( pstring topLevel )
{
	int endIndex = 0;
        int temp_index = 0;
        pPlanet newPlanet = NULL;
        element el;
        char *fl = NULL;
        int lastGood = 0;

        if ( topLevel == NULL ) {
                goto end;
        }

        /// Skip any leading spaces
        skipWhiteSpace( topLevel );

	lastGood = topLevel->index;

	if ( topLevel->maxlength < 9 ) {
		goto end;
	}

	char *c = "{Planet}";

	for ( int i = 0; i < 8; i++ ) {
		if ( topLevel->buffer[ topLevel->index + i ] != c[i] ) {
			goto end;
		}
	}

        /// Skip 8 bytes of top level then any whitespace
        topLevel->index += 8;

	skipWhiteSpace( topLevel );

        /// Allocate a new planet structure
        if ( allocate( sizeof(Planet), 0, (void**)&newPlanet) != 0 ) {
                goto end;
        }

        initPlanet( newPlanet );

        /// Extract the next element name
        fl = pullNextElementName( topLevel );

        getIndex( topLevel, &lastGood);

        while ( fl != NULL ) {
                /// Convert the element name
                el = elementNameToEnum( fl );

                /// The name is no longer needed so __free it
                deallocate( fl, __strlen(fl) + 1 );

                switch (el) {
                        case name:
                                fl = extractName( topLevel );

                                if ( fl == NULL ) {
                                        goto error;
                                }

				__bzero( newPlanet->name, 20 );
                                __strncpy( newPlanet->name, fl, 19 );

                                deallocate(fl, __strlen(fl) + 1 );
                                fl = NULL;

                                break;
                        case population:
                                newPlanet->population = extractPopulation( topLevel );

                                if ( newPlanet->population < 0 ) {
                                        goto error;
                                }

                                break;
                        case period:
                                /// Extract period and check result
                                newPlanet->period = extractPeriod( topLevel );
                                if ( newPlanet->period < 0.0 ) {
                                        goto error;
                                }

                                break;
                        case orbitspeed:
                                newPlanet->orbitspeed = extractOrbitSpeed( topLevel );

                                if ( newPlanet->orbitspeed < 0.0 ) {
                                        goto error;
                                }
                                break;
                        case aphelion:
                                newPlanet->aphelion = extractAphelion( topLevel );

                                if ( newPlanet->aphelion < 0.0 ) {
                                        goto error;
                                }
                                break;
                        case perihelion:
                                newPlanet->perihelion = extractPerihelion( topLevel );

                                if ( newPlanet->perihelion < 0.0 ) {
                                        goto error;
                                }

                                break;
                        case radius:
                                newPlanet->radius = extractRadius( topLevel );

                                if ( newPlanet->radius < 0.0 ) {
                                        goto error;
                                }
                                break;

                        case eradius:
                                newPlanet->eradius = extractERadius( topLevel );

                                if ( newPlanet->eradius < 0.0 ) {
                                        goto error;
                                }
                                break;
                        case mass:
                                newPlanet->mass = extractMass( topLevel );

                                if ( newPlanet->mass < 0.0 ) {
                                        goto error;
                                }

                                break;

                        case gravity:
                                newPlanet->gravity = extractGravity( topLevel );

                                if ( newPlanet->gravity < 0.0 ) {
                                        goto error;
                                }

                                break;
                        case country:
                                if ( newPlanet->country_count >= COUNTRYMAX ) {
                                        __printf("!!Only @d countries allowed\n", COUNTRYMAX);
                                        goto error;
                                }

                                newPlanet->countries[ newPlanet->country_count] = countryTopLevel(topLevel);

                                if (newPlanet->countries[ newPlanet->country_count ] == NULL ) {
                                        goto error;
                                }

                                newPlanet->country_count++;
                                break;

                        default:
                                __printf("Not allowed under Planet\n", fl);
                                goto error;
                                break;
                };


                getIndex( topLevel, &lastGood );
                fl = pullNextElementName( topLevel );

        }

        /// Since no more valid elements need to be parsed, check the closing element
        skipWhiteSpace( topLevel );

        if ( !atChar( topLevel, '{' ) ) {
                __printf("!!Closing value failed for Planet\n");
                goto error;
        }

        /// Skip the open brace
        if ( incChar( topLevel ) < 0 ) {
                goto error;
        }

        skipWhiteSpace( topLevel );

        if ( !atChar( topLevel, '#' ) ) {
                __printf("!!Malformed closing element\n");
                goto error;
        }

        /// Skip past #
        if ( incChar( topLevel ) == -1 ) {
                goto error;
        }

        getIndex( topLevel, &temp_index );

        endIndex = skipAlpha( topLevel );

        if ( endIndex == -1 ) {
                endIndex = 0;
                goto error;
        }

        if ( temp_index == endIndex ) {
                goto error;
        }

        fl = copyData( topLevel, temp_index, endIndex );

        if ( fl == NULL ) {
                goto error;
        }

        if ( __strcmp( fl, "Planet") != 0 ) {
                __printf("!!Closing element malformed\n");
                deallocate( fl, (endIndex-temp_index) + 1 );
                goto error;
        }

        deallocate(fl, (endIndex-temp_index) + 1 );

        skipWhiteSpace( topLevel );

        if ( !atChar( topLevel, '}' ) ) {
                __printf("!!Failed to find final closing brace\n");
                goto error;
        }

        incChar(topLevel);
	
        goto end;

error:
        topLevel->index = lastGood;

        __printf("!!Error at: @s\n", topLevel->buffer + topLevel->index);

	if ( newPlanet != NULL ) {
		freePlanet( newPlanet );
		newPlanet = NULL;
	}

end:
        return newPlanet;
}


/**
 * Parse the Period element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Period should never be negative.
 **/
double extractPeriod( pstring str )
{
	char *temp = NULL;
	double period = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int ps = 0;
	int pe = 0;

	if (str == NULL ) {
		return period;
	}

	skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return period;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return period;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return period;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "Period" then this is the wrong function
	if ( __strcmp( temp, "Period") != 0 ) {
		__printf("!!Element id is not \"Period\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return period;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return period;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &ps );

	/// The period data must be a float
	pe = skipFloat( str );

	if ( pe == -1 ) {
		__printf("!!Failed to locate the end of the period data\n");
		return end;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return period;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return period;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );
#ifdef PATCHED
	if ( temp == NULL ) {
		return -1.0;
	}
#endif

	if ( __strcmp( temp, "Period") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return period;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the name element data
	temp = copyData( str, ps, pe );

	if ( temp == NULL ) {
		__printf("!!Failed to copy period data\n");
		return -1;
	}

	period = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return period;
}

/**
 * Parse the OrbitSpeed element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Orbit speed should never be negative.
 **/
double extractOrbitSpeed( pstring str )
{
	char *temp = NULL;
	double speed = -1.0;
	register int start = 0;
	register int end = 0;
	int ss = 0;
	int se = 0;

	if (str == NULL ) {
		goto reterrvalue;	
	}

	start = skipWhiteSpace(str);

	if ( !(atChar( str, '{' ) & 1) ) {
		__printf("!!Failed to locate opening brace\n");
		goto reterrspeed;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return speed;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		goto reterrvalue;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "OrbitSpeed" then this is the wrong function
	if ( __strcmp( temp, "OrbitSpeed") != 0 ) {
		__printf("!!Element id is not \"OrbitSpeed\"\n");
		__bzero( temp, __strlen(temp) );
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		goto reterrvalue;
	}

	/// The buffer is no longer needed so __free it
	__bzero( temp, __strlen(temp) + 1 );
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		goto reterrspeed;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		goto retspeed;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	ss = str->index;

	/// The period data must be a float
	se = skipFloat( str );

	if (se == -1 ) {
		__printf("!!Failed to locate the end of the period data\n");
		return -1.0;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		goto reterrvalue;
	}

	/// Skip past the brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		goto reterrspeed;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		goto reterrvalue;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

#ifdef PATCHED
	if ( temp == NULL ) {
		return -1.0;
	}
#endif

	if ( __strcmp( temp, "OrbitSpeed") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		__bzero(temp, __strlen(temp) + 1 );
		deallocate(temp, __strlen(temp)+1);
		return -1.0;
	}

	__bzero( temp, __strlen(temp) + 1 );
	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		goto reterrspeed;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the name element data
	temp = copyData( str, ss, se );

	if ( temp == NULL ) {
		__printf("!!Failed to copy period data\n");
		goto retspeed;
	}

	speed = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	goto retspeed;

reterrvalue:
	return -1.0; 

reterrspeed:
	speed = -1.0;
	return speed;

retspeed:
	return speed;
}

/**
 * Parse the Aphelion element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Aphelion should never be negative.
 **/
double extractAphelion( pstring str )
{
	char *temp = NULL;
	double aphelion = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int ps = 0;
	int pe = 0;

	if (str == NULL ) {
		return -1.0;
	}

	start = skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return aphelion;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return aphelion;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return aphelion;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "Aphelion" then this is the wrong function
	if ( __strcmp( temp, "Aphelion") != 0 ) {
		__printf("!!Element id is not \"Aphelion\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return aphelion;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return aphelion;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &ps );

	/// The period data must be a float
	pe = skipFloat( str );

	if ( pe == -1 ) {
		__printf("!!Failed to locate the end of the period data\n");
		return aphelion;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return aphelion;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return aphelion;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

#ifdef PATCHED
	if ( temp == NULL ) {
		return -1.0;
	}
#endif

	if ( __strcmp( temp, "Aphelion") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return aphelion;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the name element data
	temp = copyData( str, ps, pe );

	if ( temp == NULL ) {
		__printf("!!Failed to copy period data\n");
		return -1;
	}

	aphelion = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return aphelion;
}

/**
 * Parse the Perihelion element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Perihelion should never be negative.
 **/
double extractPerihelion( pstring str )
{
	char *temp = NULL;
	double perihelion = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int ps = 0;
	int pe = 0;

	if (str == NULL ) {
		return -1.0;
	}

	start = skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return perihelion;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return perihelion;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return perihelion;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "Perihelion" then this is the wrong function
	if ( __strcmp( temp, "Perihelion") != 0 ) {
		__printf("!!Element id is not \"Perihelion\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return perihelion;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return perihelion;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &ps );

	/// The period data must be a float
	pe = skipFloat( str );

	if ( pe == -1 ) {
		__printf("!!Failed to locate the end of the perihelion data\n");
		return perihelion;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( incChar( str) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return perihelion;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return perihelion;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

	if ( __strcmp( temp, "Perihelion") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return perihelion;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the name element data
	temp = copyData( str, ps, pe );

	if ( temp == NULL ) {
		__printf("!!Failed to copy perihelion data\n");
		return -1;
	}

	perihelion = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return perihelion;
}

/**
 * Parse the Radius element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Radius should never be negative.
 **/
double extractRadius( pstring str )
{
	char *temp = NULL;
	double radius = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int rs = 0;
	int re = 0;

	if (str == NULL ) {
		return -1.0;
	}

	start = skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return radius;
	}

	/// Skip past the curly brace
	if ( incChar( str ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return radius;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return radius;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "Radius" then this is the wrong function
	if ( __strcmp( temp, "Radius") != 0 ) {
		__printf("!!Element id is not \"Radius\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return radius;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( incChar( str) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return radius;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &rs );

	/// The radius data must be a float
	re = skipFloat( str );

	if ( re == -1 ) {
		__printf("!!Failed to locate the end of the radius data\n");
		return radius;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( incChar( str) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return radius;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = incChar( str );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return radius;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

	if ( __strcmp( temp, "Radius") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return radius;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the name element data
	temp = copyData( str, rs, re );

	if ( temp == NULL ) {
		__printf("!!Failed to copy radius data\n");
		return -1.0;
	}

	radius = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return radius;
}

/**
 * Parse the ERadius element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. ERadius should never be negative.
 **/
double extractERadius( pstring str )
{
	char *temp = NULL;
	double eradius = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int es = 0;
	int ee = 0;

	if (str == NULL ) {
		return -1.0;
	}

	start = skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return eradius;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return eradius;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return eradius;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "ERadius" then this is the wrong function
	if ( __strcmp( temp, "ERadius") != 0 ) {
		__printf("!!Element id is not \"ERadius\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return eradius;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return eradius;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &es );

	/// The eradius data must be a float
	ee = skipFloat( str );

	if ( ee == -1 ) {
		__printf("!!Failed to locate the end of the eradius data\n");
		return eradius;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( incChar( str) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return eradius;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return eradius;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

	if ( __strcmp( temp, "ERadius") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return eradius;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the eradius element data
	temp = copyData( str, es, ee );

	if ( temp == NULL ) {
		__printf("!!Failed to copy eradius data\n");
		return -1;
	}

	eradius = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return eradius;
}

/**
 * Parse the Mass element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Mass should never be negative.
 **/
double extractMass( pstring str )
{
	char *temp = NULL;
	double mass = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int es = 0;
	int ee = 0;

	if (str == NULL ) {
		return -1.0;
	}

	start = skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return mass;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return mass;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return mass;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "Mass" then this is the wrong function
	if ( __strcmp( temp, "Mass") != 0 ) {
		__printf("!!Element id is not \"Mass\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return mass;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return mass;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &es );

	/// The mass data must be a float
	ee = skipFloat( str );

	if ( ee == -1 ) {
		__printf("!!Failed to locate the end of the mass data\n");
		return mass;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( incChar( str) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return mass;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return mass;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

#ifdef PATCHED
	if ( temp == NULL ) {
		return -1.0;
	}
#endif

	if ( __strcmp( temp, "Mass") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return mass;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the mass element data
	temp = copyData( str, es, ee );

	if ( temp == NULL ) {
		__printf("!!Failed to copy mass data\n");
		return -1;
	}

	mass = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return mass;
}

/**
 * Parse the Gravity element and return the stored value
 * @param str Pointer to a string structure
 * @return Returns the double value or -1.0 on failure. Gravity should never be negative.
 **/
double extractGravity( pstring str )
{
	char *temp = NULL;
	double gravity = -1.0;
	int start = 0;
	int end = 0;

	/// These will be used specifically for the data
	int es = 0;
	int ee = 0;

	if (str == NULL ) {
		return -1.0;
	}

	start = skipWhiteSpace(str);

	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate opening brace\n");
		return gravity;
	}

	/// Skip past the curly brace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip opening brace\n");
		return gravity;
	}

	/// Skip any additional whitespace
	start = skipWhiteSpace(str);

	/// This should skip any to either whitespace or a closing '}'
	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the element id\n");
		return gravity;
	}

	/// Copy the element id from the string
	temp = copyData( str, start, end );

	if ( temp == NULL ) {
		__printf("!!Copy from @d to @d failed\n", start, end);
		return -1.0;
	}

	/// If the element id is not "Gravity" then this is the wrong function
	if ( __strcmp( temp, "Gravity") != 0 ) {
		__printf("!!Element id is not \"Gravity\"\n");
		deallocate( temp, __strlen(temp) + 1 );
		temp = NULL;
		return gravity;
	}

	/// The buffer is no longer needed so __free it
	deallocate(temp, __strlen(temp) + 1);

	/// Skip to the end of the element id
	skipWhiteSpace( str );

	/// If it is not a closing brace then this is improperly formatted.
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate initial closing brace\n");
		return -1.0;
	}

	/// Skip the closing brace as well as any whitespace
	if ( skipLength( str, 1 ) == -1 ) {
		__printf("!!Failed to skip initial closing brace\n");
		return gravity;
	}

	skipWhiteSpace( str );

	/// Copy the start index to store the data
	getIndex( str, &es );

	/// The gravity data must be a float
	ee = skipFloat( str );

	if ( ee == -1 ) {
		__printf("!!Failed to locate the end of the gravity data\n");
		return gravity;
	}

	/// The rest of this code is a check to ensure proper formatting except for the copy data
	skipWhiteSpace( str );

	/// If this is not an opening curly brace then fail
	if ( !atChar( str, '{' ) ) {
		__printf("!!Failed to locate the final opening brace\n");
		return -1.0;
	}

	/// Skip past the brace
	if ( incChar( str) == -1 ) {
		__printf("!!Failed to skip the final opening brace\n");
		return gravity;
	}
	
	skipWhiteSpace(str);

	/// If this is not a # indicating the closing brace then fail
	if ( !atChar( str, '#' ) ) {
		__printf("!!Failed to locate the closing mark\n");		
		return -1.0;
	}

	/// Skip past the # but save the start
	start = skipLength( str, 1 );

	if ( start == -1 ) {
		__printf("!!Failed to skip closing mark\n");
		return gravity;
	}

	end = skipToNonAlphaNum( str );

	if ( end == -1 ) {
		__printf("!!Failed to locate the end of the closing element id\n");
		return -1.0;
	}
	
	temp = copyData( str, start, end );

	if ( __strcmp( temp, "Gravity") != 0 ) {
		__printf("!!Invalid closing element id: @s\n", temp);
		deallocate(temp, __strlen(temp)+1);
		return gravity;
	}

	deallocate(temp, __strlen(temp)+1);

	skipWhiteSpace( str );

	/// Check the final curly brace
	if ( !atChar( str, '}' ) ) {
		__printf("!!Failed to locate final closing brace\n");
		return -1.0;
	}

	/// Skip past the closing brace
	skipLength( str, 1 );
		
	/// Copy the gravity element data
	temp = copyData( str, es, ee );

	if ( temp == NULL ) {
		__printf("!!Failed to copy gravity data\n");
		return -1;
	}

	gravity = __atof( temp );

	deallocate( temp, __strlen(temp) + 1 );

	return gravity;
}

