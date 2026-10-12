#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// checks if a string is ASCII, returns one if it is, zero if not
uint8_t is_ascii(char string[])
{
    int size = 0;
    while(1) {
        if (string[size]) {size++;} else {break;}
    }
    for (int i = 0; i < size; i++) {
        if ((string[i] & 0b10000000) == 0b10000000) {return 0;}
    }
    return 1;
}

// given char array, will capitalize ascii characters in place, and return # of chars changed
int32_t capitalize_ascii(char str[])
{
    int size = 0;
    int count = 0;
    while(1) {
        if (str[size]) {size++;} else {break;}
    }
    for (int i = 0; i < size; i++) {
        if (((str[i] & 0b10000000) != 0b10000000) && (str[i] >= 97 && str[i] <= 122)){
            str[i] -= 32;
            count++;
        }
    }
    return count;
}


// Returns 1, 2, 3, or 4 depending on the size of the first codepoint in the string.
// Returns 0 if the string is empty
// Returns -1 if the first byte is not a valid UTF-8 start byte
int8_t codepoint_size(char string[]) {
    if ((unsigned char)string[0] < 128) {
        return 1;
    } else if ((string[0] & 0b11000000) != 0b10000000) {
        int size = 1;
        while ((string[size] & 0b11000000) == 0b10000000) {
            size++;
        }
        return size;
    }
    return -1;
}


// Given a char[] representing a UTF-8 encoded string,
// return the number of UTF-8 codepoints (characters) in the string.
// The input will always be valid UTF-8 and will not exceed 2048 bytes.
int32_t utf8_strlen(char str[]) {
    int32_t count = 0;
    int size = 0;
    while (1) {
        if (str[size]) {size++;} else {break;}
    }
    for (int i = 0; i < size; i++) {
       if ((str[i] & 0b10000000) == 0b00000000) {
          count++;
       } else if ((str[i] & 0b11000000) != 0b10000000) {
            int byte_size = 1;
            for (int j = i+1; j < i+4; j++) {
                 if ((str[j] & 0b01111111) == 0b00000000) {byte_size++;} else {break;}
            }
            i += byte_size-1;
            count++;
       }
    }
    return count;
}


// Extracts the substring from codepoint index start (inclusive) to end (exclusive) and prints it
void utf8_substring(const char str[], int start, int end) {
    //Implement utf8_substring in the body of this function!
    if (start < 0 || end < 0 || start >= end) {
        return;
    }
    int index = 0;
    int curr = 0;
    while (curr < end && str[index] != '\0') {
        int size = 1;
        unsigned char first_byte = (unsigned char)str[index];
        if ((first_byte & 0xF8) == 0b11110000) {
            size = 4;
        } else if ((first_byte & 0xE0) ==  0b11100000) {
            size = 3;
        } else if ((first_byte & 0xC0) == 0b11000000) {
            size = 2;
        }
        if (curr >= start) {
            for (int i = 0; i < size; i++) {
                printf("%c", str[i+index]);
            }
        }
        index += size;
        curr++;
    }
}

//given string and byte index, return codepoint value of char starting at that index
int32_t codepoint_at(char str[], int32_t byte_index) {
    // TODO: Implement this function
    if ((unsigned char)str[byte_index] < 128) {
        return str[byte_index];
    } else if (((unsigned char)str[byte_index] & 0b11000000) != 0b10000000) {
        int size = 1;
        while ((str[byte_index+size] & 0b11000000) == 0b10000000) {
            size++;
        }
        uint32_t mask = (unsigned)0b11111111 >> (size+1);
        uint32_t first_byte = ((unsigned char)str[byte_index] & mask);
        first_byte = first_byte << (6*(size-1));
        uint32_t codepoint = 0x00000000 | first_byte;
        for (int i = 1; i < size; i++) {
            uint32_t cont = ((unsigned char)str[byte_index+i] & 0b00111111);
            cont = cont << (6*(size-i-1));
            codepoint = codepoint | cont;
        }
        return codepoint;
    }
    return -1;
}


// It should return 1 if the codepoint at the given index is an animal emoji, 0 otherwise.
int is_animal_emoji_at(const char str[], int index){
    int size = 0;
    int index_check = -1;
    while (str[size]) {size++;}
    for (int i = 0; i < size;) {
        unsigned char first_byte = (unsigned char)str[i];
        unsigned int codepoint;
        int char_size;

        if (first_byte < 0b10000000) {
            codepoint = first_byte;
            char_size = 1;
        } else if ((first_byte & 0b11100000) == 0b11000000) {
            codepoint = first_byte & 0b00011111;
            char_size = 2;
        } else if ((first_byte & 0b11110000) == 0b11100000) {
            codepoint = first_byte & 0b00001111;
            char_size = 3;
        } else if ((first_byte & 0b11111000) == 0b11110000) {
            codepoint = first_byte & 0b00000111;
            char_size = 4;
        } else { return 0;}

        for (int j = 1; j < char_size; j++) {
            codepoint = (codepoint << 6) | ((unsigned char)str[i+j] & 0b00111111);
        }
        index_check++;
        if (index_check == index){
            if (((codepoint >= 0x1F400) && (codepoint <= 0x1F43F)) || ((codepoint >= 0x1F980) && (codepoint <= 0x1F9AE))) {
                    return 1;
                } else { return 0; }
            }
        i += char_size;
    }
    return 0;
}


int main(int argc, char **argv) {
    char name[100];
    printf("Enter a UTF-8 encoded string: ");
    while (fgets(name, sizeof(name), stdin)) {
    name[strcspn(name, "\n")] = 0;
    printf("\n");

    if (is_ascii(name) == 0) {
        printf("Valid ASCII: %s\n", "false");
    } else { printf("Valid ASCII: %s\n", "true"); }

    char nameTemp[100];
    int i = 0;
    while (name[i] != '\0') {
        nameTemp[i] = name[i];
        i++;
    }
    nameTemp[i] = '\0';
    int32_t ret = capitalize_ascii(nameTemp);
    printf("Uppercased ASCII: %s", nameTemp);
    printf("\n");

    //printf("Length in bytes: %d\n", codepoint_size(name)); //wrong
    printf("Length in bytes: %d\n", i);

    printf("Number of codepoints: %d\n", utf8_strlen(name));

    printf("Bytes per code point: ");
    for (int i = 0; name[i] != '\0';) {
        printf("%d ", codepoint_size(&name[i]));
        i += codepoint_size(&name[i]);
    }
    printf("\n");

    printf("Substring of the first 6 codepoints: \"");
    utf8_substring(name, 0, 6);
    printf("%s\n", "\"");

    printf("Code points as decimal numbers: ");
    for (int i = 0; name[i] != '\0'; i++) {
        if (codepoint_at(name, i) != -1) {
            printf("%d ", codepoint_at(name, i));
        }
    }
    printf("\n");

    printf("Animal emojis: ");
    int cp_index = 0;
    for (int i = 0; name[i] != '\0';) {
        if (is_animal_emoji_at(name, cp_index) == 1) {
            for (int j = 0; j < codepoint_size(&name[i]); j++) {
                printf("%c", name[i+j]);
            }
        }
        i += codepoint_size(&name[i]);
        cp_index++;
    }
    printf("\n");
}
}
