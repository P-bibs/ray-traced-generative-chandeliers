# Filter

## Design Decisions

- I used a gaussian filter with sigam=radius/3 for the blur
- I used separable convolution for both blurring and edge detecting
- Since there isn't much repeated functionality/state, I used function composition instead of inheritance.
- To deal with edge-conditions in convolution, I keep track of the weights that are actually used and normalize by them.

## Extra Credit

None

## Known Bugs

N/A

## Formatting

I'm aware that some of the formatting choices are odd (e.g.: commas to start the line in member initializer lists). I used a clang-format style preset, so these are deliberate (albeit wacky) choices.
