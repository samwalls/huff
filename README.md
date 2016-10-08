# Adaptive Huffman Coding Compressor "huff"

#### Build Instructions

To make the executable, navigate to the root directory of the project and run:
- `cmake .`
- If all's well, then after that run `make`

This should generate the executable file `huff`.

#### Usage

```
huff [--puff] [-h|--help] <input-file> <output-file>
<input-file>    the file treated as input
<output-file>   the file treated as output (will be overwritten if already exists)
--puff          Tells huff to decompress the input file. Huff will compress files by default.
-h|--help       Print this usage message.
```