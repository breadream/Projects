Brad Lim 

First of all, please change the path(dataDir) in config.hpp file into a proper path  
and run g++ main.cpp -o main `pkg-config --cflags --libs sdl2 glew`

Earthquake data representation 

- Color : if the magnitude is less than 3, its color will be deep yellow-ish 
          if the magnitude is greater than 8, its color will be deep red-ish
          if the magnitude between those, its color will be orange-ish
          the color will be slightly different according to
          its specific magnitude value

- Size : the size representing earthquake is proportional to its magnitude. 
         but if the magnitude is less than 3, its size will be more smaller and 
             if the magnitude is greater than 8, its size will be more bigger



