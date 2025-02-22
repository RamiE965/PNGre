# PNG:re
PNGre is a command-line utility for encoding and decoding hidden messages within PNG files.

## Features
- Encode secret messages into PNG files
- Decode hidden messages from PNG files
- Remove encoded messages
- Print chunk information from PNG files
- Preserves original image quality and appearance

## Installation
Clone the repository and navigate to the project directory
```
git clone https://github.com/RamiE965/pngre
cd pngre
```
Build using Make
```
Make
```

## Usage
```
./pngre encode <image.png> <chunk-type> <message>    # Encode a message
./pngre decode <image.png> <chunk-type>              # Decode a message
./pngre remove <image.png> <chunk-type>              # Remove a message
./pngre print <image.png>                            # Print all "chunks"
```

### Examples
```
# Encode message "Hello World!" with the chunktype "TEST"
./pngre encode image.png TEST "Hello World!"

# Decode the first message associated with the chunktype "TEST"
./pngre decode image.png TEST

# Remove the first message associated with the chunktype "TEST"
./pngre remove image.png TEST

# View all image Chunk information
./pngre print image.png
```

## Testing
Run the test suite
```
./run_tests
```

