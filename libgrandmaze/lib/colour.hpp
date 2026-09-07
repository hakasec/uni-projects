#ifndef COLOUR_H
#define COLOUR_H

namespace GrandMaze
{
    // All colours
    enum Colour
    {
        None,
        Invert,

        Red,
        RedInvert,
        Green,
        GreenInvert,
        Blue,
        BlueInvert,
        Yellow,
        YellowInvert,
        Magenta,
        MagentaInvert,
        Cyan,
        CyanInvert,
        
        Black,

        White = None,
        WhiteInvert = Invert,
    };

    // Initialise colours
    void initColour(void);
};

#endif
