
// Bottom Row
LetterBlock("0,0,0", 0, 0, 0);
LetterBlock("1,0,0", 50, 0, 0);
LetterBlock("2,0,0", 100, 0, 0);

LetterBlock("0,1,0", 0, 50, 0);
LetterBlock("1,1,0", 50, 50, 0);
LetterBlock("2,1,0", 100, 50, 0);

LetterBlock("0,2,0", 0, 100, 0);
LetterBlock("1,2,0", 50, 100, 0);
LetterBlock("2,2,0", 100, 100, 0);

// Middle Row
LetterBlock("0,0,1", 0, 0, 50);
LetterBlock("1,0,1", 50, 0, 50);
LetterBlock("2,0,1", 100, 0, 50);

LetterBlock("0,1,1", 0, 50, 50);
LetterBlock("1,1,1", 50, 50, 50);
LetterBlock("2,1,1", 100, 50, 50);

LetterBlock("0,2,1", 0, 100, 50);
LetterBlock("1,2,1", 50, 100, 50);
LetterBlock("2,2,1", 100, 100, 50);

// Top Row
LetterBlock("0,0,2", 0, 0, 100);
LetterBlock("1,0,2", 50, 0, 100);
LetterBlock("2,0,2", 100, 0, 100);

LetterBlock("0,1,2", 0, 50, 100);
LetterBlock("1,1,2", 50, 50, 100);
LetterBlock("2,1,2", 100, 50, 100);

LetterBlock("0,2,2", 0, 100, 100);
LetterBlock("1,2,2", 50, 100, 100);
LetterBlock("2,2,2", 100, 100, 100);
// Module definition.
// size=30 defines an optional parameter with a default value.
module LetterBlock(letter, x, y, z, size=30) {
    difference() {
        translate([x,y,z + (size/2)]) cube([size,size,size/1.5], center=true);
        translate([x,y,z + (size/2)]) {
            // convexity is needed for correct preview
            // since characters can be highly concave
            linear_extrude(height=size, convexity=6) {
                text(letter, 
                     size=7, //size*8/40,
                     font="Bitstream Vera Sans",
                     halign="center",
                     valign="center");
            }
        }
    }
}

echo(version=version());
