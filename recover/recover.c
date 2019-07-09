#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


// constant var. defining block size
#define BLOCK_SIZE 512
//define unit as unsinged 8bits (1byte)
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Verify valid input count
    if(argc!=2)
    {
        fprintf(stderr,"Single argument as a filepath to recover pls. \n");
        return 1;
    }
    // name incoming file w/ str
    char  *infile = argv[0];
    printf("%s", argv[0]);
   
    // open an raw file
    FILE *inptr = fopen(infile, "r");
    
    //check filepath is of valid size.
    if( inptr == NULL) 
    {
        fprintf(stderr, "Please enter .raw file to read, could open %s. \n", infile);
        return 2;
    }
    // make unsinged 512 byte array to step through infile
    BYTE buffer [512];
    
    //filename array, ###.JPG = 8 char to name files.
    char filename[8];
    
    // bool condition defining the beginning of a .jpg
    bool isJPG = (buffer[0] == 0xff && buffer[1] == 0xd8 &&
    buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0);
    
    // outptr for img file initilized
    FILE *img = NULL;
    
    // Track image count. Append to filename
    int imgCt = 0;
    
    //read .raw infile
    while(fread(buffer, 512, 1,inptr)==1)
    {
        // measure block of infile in bytes
        size_t bytesRead = fread(buffer, sizeof(BYTE), BLOCK_SIZE, inptr);
        // troubleshooting seg fault 
        printf("Reading bytes %lu \n", sizeof(bytesRead));
        
        // close open img on found jpg
        if(isJPG && img != NULL)
        {
            fclose(img);
            // print out image closings (troubleshooting)
            printf("closed image %i", imgCt);
            imgCt++;
        }
        
        // Break condition if bytes read = 0 or end of file 
        if(bytesRead == 0 && feof(inptr))
        {
            break;
        }
        // if jpg is found, open and write img to file
        if(isJPG)
        {
            //Write filename, assign img to filename
            sprintf(filename, "%30i.jpg", imgCt);
            img = fopen(filename, "w");
            
            // Write to img to open file (now populated).
            if(img != NULL)
            {
                fwrite(&buffer, sizeof(BYTE), bytesRead, img);
            }
        }
        // close last img
        fclose(img);
        // close infile
        fclose(inptr);
        
        printf("success");
        
    }
    return 0;
}
