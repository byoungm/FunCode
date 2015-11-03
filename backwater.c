//  Created by Benjamin Young on 3/31/14.
//  Copyright (c) 2014 Benjamin Young. All rights reserved.
//

#include "stdio.h"
#include "stdlib.h"

#define TRUE 1
#define FALSE 0

typedef struct
{
    int found;
    int index;
}water_wall_t;

void fill_water(int *water, water_wall_t lw, water_wall_t rw)
{
    for (int i = lw.index+1; i < rw.index; i++)
    {
        water[i]++;
    }
}

void graph_lanscape(int *lanscape, int length)
{
    for(int i=0;i<length;i++)
    {
        printf("|");
        for(int j=0;j<lanscape[i];j++)
        {
            printf("= ");
        }
        printf("\n");       
    }   
    printf("\n");
}

int find_max_lanscape_height(int *lanscape, int length)
{
    int max = 0;
    for (int i=0; i<length; i++)
    {
        if (lanscape[i] > max)
        {
            max = lanscape[i];
        }
    }

    return max;
}

int backwater(int *lanscape, int lanscape_length)
{
    
    //Init water array
    int *water = (int *)malloc(sizeof(*lanscape) * lanscape_length);
    for (int n=0; n<lanscape_length; n++) 
    {
        water[n] = 0;
    }
    
    int max_lanscape_height = find_max_lanscape_height(lanscape, lanscape_length);
    
    for (int curr_height = 0; curr_height <= max_lanscape_height; curr_height++)
    {
        water_wall_t left_wall = {FALSE, 0};
        water_wall_t right_wall = {FALSE, 0};

        for (int i = 0; i < lanscape_length; i++)
        { 
            // If we are less than current height on rigth and left add water
            if (lanscape[i] >= curr_height)
            {
                if (left_wall.found)
                {   
                    right_wall.found = TRUE;
                    right_wall.index = i;
                }
                else
                {
                    left_wall.found = TRUE;
                    left_wall.index = i;
                }
            }
            if (left_wall.found && right_wall.found)
            {
                fill_water(water, left_wall, right_wall);
                right_wall.found = FALSE;
                // Right wall can now become left wall
                left_wall.index = right_wall.index;
            }
        }   
    }
    

    graph_lanscape(lanscape,lanscape_length);    

    // Print Water
    printf("water = {");
    for (int i=0; i < lanscape_length; i++) {
        if (i < lanscape_length - 1) printf("%i,",water[i]);
        else printf("%i}\n",water[i]);
    }
    
    return 0;
}

int main(int argc, const char * argv[])
{
    int a0[9] = {2,1,6,4,5,3,1,2,1}; backwater(a0,9);

    int a1[9] = {0,1,1,5,1,4,2,3,4}; backwater( a1 ,9);

    int a2[9] = {6,2,3,1,4,7,2,3,4}; backwater( a2 ,9);

    return 0;
}

