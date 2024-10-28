#!/bin/zsh

if ! command -v magick &> /dev/null
then
  echo "Please install the ImageMagick CLI and try again."
  echo "http://www.imagemagick.org/"
  exit 1
fi

if [ -z "$1" ] # check if arg 1 is present
then
  echo "Please provide a file as an argument."
  exit 1
fi

FILE=$1
filename="${FILE%%.*}"

if [ ! -d "resized" ]
then
  mkdir resized
fi

echo "Resizing $FILE..."

# Declare an associative array for dimensions and their corresponding output file names
typeset -A sizes
sizes=(
  icon_16x16 16
  icon_16x16@2x 32
  icon_32x32 32
  icon_32x32@2x 64
  icon_128x128 128
  icon_128x128@2x 256
  icon_256x256 256
  icon_256x256@2x 512
  icon_512x512 512
  icon_512x512@2x 1024
)

# Loop through the array and resize images accordingly
for file in "${(@k)sizes}"; do
  size=${sizes[$file]}
  magick "$FILE" -resize "${size}x${size}!" "resized/${file}.png"
done

echo "Resizing completed. Resized images are in the 'resized' directory."
