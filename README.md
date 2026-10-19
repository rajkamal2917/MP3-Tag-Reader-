# MP3 Tag Reader and Editor in C

## Overview

MP3 Tag Reader and Editor is a command-line application developed using C to read and modify ID3v2.3 metadata in MP3 files.

The application allows users to view existing metadata and edit selected tag information, including the song title, artist, album, year, genre, and comment.

## Features

- Read ID3v2.3 metadata from MP3 files.
- Display song title, artist, album, year, genre, and comment.
- Edit existing metadata fields.
- Handle tag sizes and byte-order conversions.
- Use dynamic memory allocation for metadata storage.
- Provide command-line options for easy operation.

## Technologies Used

- Programming Language: C
- File Handling: `fopen()`, `fread()`, `fwrite()`, `fseek()`
- Dynamic Memory Allocation: `malloc()`, `free()`
- Data Structures: Structures and pointers
- Concepts: Binary file handling, endian conversion, ID3 metadata

## Project Structure

- `main.c` – Main program and command-line interface
- `main.h` – Common declarations and macros
- `id3_reader.c` – Reads ID3 metadata
- `id3_reader.h` – Reader function declarations
- `id3_writer.c` – Edits metadata
- `id3_writer.h` – Writer function declarations
- `id3_utils.c` – Metadata allocation and cleanup
- `id3_utils.h` – Metadata structure and utility declarations

## Compilation

Compile the source files using GCC:

```bash
gcc main.c id3_reader.c id3_writer.c id3_utils.c -o mp3tagreader
```

## Usage

### Display help

```bash
./mp3tagreader -h
```

### View metadata

```bash
./mp3tagreader -v sample.mp3
```

### Edit the song title

```bash
./mp3tagreader -e -t "New Title" sample.mp3
```

### Edit the artist

```bash
./mp3tagreader -e -a "New Artist" sample.mp3
```

### Edit the album

```bash
./mp3tagreader -e -A "New Album" sample.mp3
```

### Edit the year

```bash
./mp3tagreader -e -y 2026 sample.mp3
```

### Edit the genre

```bash
./mp3tagreader -e -g "Rock" sample.mp3
```

### Edit the comment

```bash
./mp3tagreader -e -c "New Comment" sample.mp3
```

## Learning Outcomes

- Understanding binary file operations in C
- Working with structures and pointers
- Managing dynamically allocated memory
- Processing metadata using file offsets and sizes
- Implementing command-line argument handling

## Author

Rajkamal V
