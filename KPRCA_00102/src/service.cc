/*
 * Copyright (c) 2016 Kaprica Security, Inc.
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <cstdlib.h>
#include <cstring.h>
#include "playlist.h"
#include "tag_and_file.h"
#include "print.h"

#define MAX_USER_PLAYLISTS 10
#define SECRET_PAGE_SIZE 4096

unsigned int g_song_idx = 0;

bool RecvNewSong(tag_and_file *new_song)
{
    new_song->file = new MgcFile();
    if (new_song->file->ReadMgcFile(stdin))
    {
        __fflush(stdout);
        if (__fread(&new_song->tag, new_song->tag.header_size(), stdin) != new_song->tag.header_size())
        {
            delete new_song;
            return false;
        }
        new_song->tag.id = g_song_idx++;
        return true;
    }

    return false;
}

void PrintPlaylist(Playlist *playlist, unsigned int playlist_id)
{
    if (!playlist)
        return;

    __printf("Playlist ID: " ESC "d" NL, playlist_id);
    __printf("Number of songs added to playlist: " ESC "d" NL, playlist->length());
    __fflush(stdout);
}

extern "C" int __attribute__((fastcall)) main(int secret_page_i, char *unused[])
{
    unsigned char *secret_page = (unsigned char *)secret_page_i;
    /* If you want to obfuscate input/output, uncomment and change */
    __fxlat(stdin, "393748225");
    __fxlat(stdout, "393748225");

    unsigned int remix_idx;
    remix_idx = secret_page[2];
    __fbuffered(stdout, 1);

    __printf("--One Amp--" NL);
    __printf(NL NL);
    __fflush(stdout);

    Playlist *master = new Playlist();
    Playlist *selected_playlist = master;
    Playlist *user_playlists = new Playlist[MAX_USER_PLAYLISTS];
    unsigned int num_playlists = 0;

    char input[1024];
    int exit_program = false;
    int choice = 0;
    while(!exit_program)
    {
        __printf("Main Menu:" NL);
        if (selected_playlist == master)
        {
            __printf("1. Add Song To Library" NL);
            __printf("2. Delete Song From Library" NL);
            __printf("3. Select Playlist" NL);
        }
        else
        {
            __printf("1. Add Song To Playlist" NL);
            __printf("2. Delete Song From Playlist" NL);
            __printf("3. Return to Main Library" NL);
        }
        __printf("4. List Songs" NL);
        __printf("5. Sort By Song ID (Default Sort)" NL);
        __printf("6. Sort By Song Title" NL);
        __printf("7. Sort By Artist and Album" NL);
        __printf("8. Sort By Artist and Song Title" NL);
        __printf("9. Sort By Album" NL);
        __printf("10. Sort By Album and Song Title" NL);
        __printf("11. Remix Track" NL);
        if (selected_playlist == master)
        {
            __printf("12. Create Playlist" NL);
            __printf("13. Delete Playlist" NL);
            __printf("14. List Playlists" NL);
            __printf("15. Exit" NL);
        }
        else
        {
            __printf("12. List All Songs In Library" NL);
            __printf("13. Exit" NL);
        }

        __printf("[::]  ");
        __fflush(stdout);

        if (__freaduntil(input, sizeof(input), *NL, stdin) < 0)
            break;
        if (*input == 0)
            continue;
        choice = __strtol(input, NULL, 10);
        if (choice == 1 && selected_playlist == master)
        {
            tag_and_file new_song;
            if (RecvNewSong(&new_song) && master->AddSong(&new_song))
                __printf("Added song to library" NL);
            else
                __printf("Could not add song to library" NL);
            __fflush(stdout);
        }
        else if (choice == 2 && selected_playlist == master)
        {
            tag_and_file song_to_remove;
            __printf("Enter Song ID to delete from library" NL);
            __printf("[::] ");
            __fflush(stdout);

            if (__freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = __strtol(input, NULL, 10);
            if (master->RemoveSong(song_id, &song_to_remove))
            {
                for (unsigned int i = 0; i < num_playlists; i++)
                    user_playlists[i].RemoveSong(song_id, (tag_and_file *)NULL);

                delete song_to_remove.file;
                __printf("Successfully removed song from library" NL);
            }
            else
            {
                __printf("Could not remove song from library" NL);
            }
            __fflush(stdout);

        }
        else if (choice == 3 && selected_playlist == master)
        {
            __printf("Select Playlist ID" NL);
            __printf("[::] ");
            __fflush(stdout);

            if (__freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int playlist_id = __strtol(input, NULL, 10);
            if(playlist_id < num_playlists)
                selected_playlist = &user_playlists[playlist_id];
            else
                __printf("Bad Playlist ID" NL);
            __fflush(stdout);
        }
        else if (choice == 1)
        {
            __printf("Enter Song ID to add to playlist" NL);
            __printf("[::] ");
            __fflush(stdout);

            if (__freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = __strtol(input, NULL, 10);
            tag_and_file *playlist_song = master->GetSong(song_id);
            tag_and_file *song_already_added = selected_playlist->GetSong(song_id);
            if (playlist_song && !song_already_added && selected_playlist->AddSong(playlist_song))
                __printf("Added song to playlist" NL);
            else
                __printf("Could not add song to playlist" NL);
            __fflush(stdout);
        }
        else if (choice == 2)
        {
            __printf("Enter Song ID to delete from playlist" NL);
            __printf("[::] ");
            __fflush(stdout);

            if (__freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = __strtol(input, NULL, 10);
            if (selected_playlist->RemoveSong(song_id, (tag_and_file *)NULL))
                __printf("Successfully removed song from playlist" NL);
            else
                __printf("Could not remove song from playlist" NL);
            __fflush(stdout);

        }
        else if (choice == 3)
        {
            selected_playlist = master;
        }
        else if (choice == 4)
        {
            selected_playlist->ListAllSongs();
        }
        else if (choice == 5)
        {
            selected_playlist->SortById();
            __printf("Successfully sorted list by Song ID" NL);
            __fflush(stdout);
        }
        else if (choice == 6)
        {
            selected_playlist->SortByTitle();
            __printf("Successfully sorted list by Song Title" NL);
            __fflush(stdout);
        }
        else if (choice == 7)
        {
            selected_playlist->SortByArtistAndAlbum();
            __printf("Successfully sorted list by Artist Name and Album" NL);
            __fflush(stdout);
        }
        else if (choice == 8)
        {
            selected_playlist->SortByArtistAndTitle();
            __printf("Successfully sorted list by Artist Name and Song Title" NL);
            __fflush(stdout);
        }
        else if (choice == 9)
        {
            selected_playlist->SortByAlbum();
            __printf("Successfully sorted list by Album" NL);
            __fflush(stdout);
        }
        else if (choice == 10)
        {
            selected_playlist->SortByAlbumAndTitle();
            __printf("Successfully sorted list by Album and Song Title" NL);
            __fflush(stdout);
        }
        else if (choice == 11)
        {
            __printf("Enter Song ID to remix" NL);
            __printf("[::] ");
            __fflush(stdout);

            if (__freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int song_id = __strtol(input, NULL, 10);
            tag_and_file *song = selected_playlist->GetSong(song_id);
            if (song)
            {
                __printf("Original Track Data:" NL);
                song->file->PrintFrameData();
                unsigned int secret_page_size = SECRET_PAGE_SIZE;
                song->file->Remix(secret_page, &remix_idx, secret_page_size);
                __printf("Successfully remixed song" NL);
                __printf("Printing New Track Data:" NL);
                song->file->PrintFrameData();
            }
            else
            {
                __printf("Could not remix track" NL);
            }
            __fflush(stdout);
        }
        else if (choice == 12 && selected_playlist == master)
        {
            if(num_playlists < MAX_USER_PLAYLISTS)
            {
                user_playlists[num_playlists].ClearPlaylist(true);
                __printf("Created a new playlist with ID [" ESC "d]" NL, num_playlists++);
            }
            else
            {
                __printf("Maximum number of playlists reached" NL);
            }
            __fflush(stdout);
        }
        else if (choice == 13 && selected_playlist == master)
        {
            __printf("Enter Playlist ID to delete");
            __printf("[::] ");
            __fflush(stdout);

            if (__freaduntil(input, sizeof(input), *NL, stdin) <= 0)
                continue;
            unsigned int playlist_id = __strtol(input, NULL, 10);
            if(playlist_id < num_playlists)
            {
                user_playlists[playlist_id].ClearPlaylist(true);
                if (playlist_id < num_playlists - 1)
                {
                    memmove(&user_playlists[playlist_id], &user_playlists[playlist_id+1],
                            sizeof(user_playlists[0]) * (num_playlists - (playlist_id+1)));
                }
                --num_playlists;
                user_playlists[num_playlists].ClearPlaylist(false);
                __printf("Deleted Playlist ID: " ESC "d" NL, playlist_id);
            }
            else
            {
                __printf("Bad Playlist ID" NL);
            }
            __fflush(stdout);
        }
        else if (choice == 14 && selected_playlist == master)
        {
            for (unsigned int i = 0; i < num_playlists; i++)
                PrintPlaylist(&user_playlists[i], i);
        }
        else if (choice == 15 && selected_playlist == master)
        {
            exit_program = true;
        }
        else if (choice == 12)
        {
            master->ListAllSongs();
        }
        else if (choice == 13)
        {
            exit_program = true;
        }
    }

    __printf("--Exited One Amp--" NL);
    __fflush(stdout);

    return 0;
}
