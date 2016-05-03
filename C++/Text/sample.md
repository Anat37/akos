#Targets simple markdown

## General description

Need to write a program in C ++, using the paradigm of the PLO, the on-screen formatted text.
In command line arguments specify the parameters that influence the style of the text.

To parse the parameters necessary to use the getopt or getopt_long parameters are specified as follows:

1. -f filename markup
2. -w width of the screen
3. -t indent the second and subsequent lines of the list (number of spaces)
4. -m symbol marker unordered lists
5. -r value of the red line
6. -h - help with the issue of describing the behavior of the program
7. -v - Displays the version of the program

Here it is assumed that the parameters of the keys -w, -t, -m, -r - not required and if the parameter is not specified, the default value is set.
Perhaps such determination of the width of the screen using the system call fstat and ioctl (man tty_ioctl).

## Description Format

Marked-up text consists of paragraphs. There are four types of paragraphs:

* Headers,
* Bulleted lists,
* Bulleted lists,
  * Nested list
  * Another level
    * Need more levels!
* Simple paragraphs.

Paragraphs are separated by two or more line breaks. Everywhere in paragraphs two consecutive whitespace characters are considered one space.

Header is defined as follows:

\ # Header text to end of line

Titles can be several levels, the level of the header specifies the number of characters '#' at the beginning of a row.

Numbered lists (the number before the point of no importance):

100. The first element of the list
  1. The first element of the second level of the list
  2. The second element of the second level of the list
    3. The third level of the first element of the list
    4. The second element of the third level of the list
200. The second element of the list

Bulleted list is defined by analogy with numbered, use '*' symbol.

Lists can be nested, nested list is given indicating the 2 space indentation
from the previous level.

Simple paragraphs is all the other paragraphs. Most often, they will start with just text.

An example of the markup text is on a mission.

## Description of the display method

For text markup in accordance with the parameters required to specify the main function screen is on display.

When displayed on the screen all the whitespace between words should be replaced with a single space (in memory to store in this form).
If the text does not fit on one line, it inserts hyphens. It is forbidden to text with translations get out abroad
the width of the text is also prohibited to transfer so that on the line remained one character. If you can not put a transfer
the string is terminated, and the word is written in the next line
(Issue a warning, and to terminate the program if the words were longer than the width of the displayed text).

To-do lists are prefixed by a marker, or the number of c point, then after a certain number of whitespace
measured from the beginning of the screen, goes the text associated with an element of the list. If the item in the list does not fit on one line,
then on the next line as well put whitespace, that was the effect of indentation from the edge (see. example below).
This continues until the next element. The following items all begins on new: we put a marker
or the next number in the numbering, whitespace, and text. If the list is attached, it is placed on each line
extra padding in accordance with the level of nesting.

Titles are displayed in the center of a predetermined width of the screen. In this case, the title is a box of '#' symbol.
The left and right boundary boxes framed in many reshёtochek what level heading, and the top and bottom displays
the entire width of the screen line of arrays. After the title have to be an empty string is displayed.

Simple paragraphs are indented first line given by the number of spaces. On the other lines indented anymore.

## Requirements to code

The code should be written in C ++ and may include an abstract base class Abstract_text, which method comprises:

unsigned long int countSymbols () const = 0;
unsigned long int countWords () const = 0;
void print () const = 0;

Each gate of the text to be stored as char *. Do not use std :: string class and std :: vector.

Each class should be described in the own module.

You need to create the heirs of this class, where each class corresponds to one of the types of paragraphs in the markup text.
In heirs must fully define the interface of a class parent, which will implement the
parameterized text printing.

The program must be a class Text_viewer, in which methods should be read any text file
in the vault of the text and its listing on the screen from the repository.
Storage text - an array of pointers to objects Abstract_text.

After printing the text to standard error should be displayed number of characters and the number of words in the source text,
with several consecutive whitespace characters are one sismolom and symbols defining markup
are not involved in counting the number of characters and words.

If you can not display text program should generate an error message with intelligible description of the problem.