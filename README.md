# Markov
A basic Markov chain generator in C++

To use:
1. Download seed text to a file, store in source directory.
2. Set the file name in Source.cpp.
3. Set an output filename in Source.cpp.
4. Set the Markov generator up. Give it a random seed (not necessary) and the order of the markov chains to use.
5. Parse the seed text.
6. Request a Markov string and do whatever with it.

If you want to play with it then I suggest changing the regex expression used in the parsing operation or change the order of the generator.

It can use quite a bit of memory, as you might expect. Also, I suggest running in release mode as the regex parsing is interminably slow in debug.
