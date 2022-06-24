import argparse
import PIL.Image
import sys

def GetSprite8x8(image, x:int, y:int, bgColor):
	result = list()
	for yy in range(0,8):
		line = 0
		for xx in range(0,8):
			pixel = image.getpixel((x+xx, y+yy))
			if pixel != bgColor:
				line = line | (128 >> xx)
		result.append(line)
	return result

def GetSprite16x16(image, x:int, y:int, bgColor):
	result = list()
	pattern0 = GetSprite8x8(image, x,   y,   bgColor)
	pattern1 = GetSprite8x8(image, x,   y+8, bgColor)
	pattern2 = GetSprite8x8(image, x+8, y,   bgColor)
	pattern3 = GetSprite8x8(image, x+8, y+8, bgColor)

	result.extend(pattern0)
	result.extend(pattern1)
	result.extend(pattern2)
	result.extend(pattern3)
	return result

if __name__ == "__main__":
	argumentParser = argparse.ArgumentParser()
	argumentParser.add_argument('input', type=str)
	argumentParser.add_argument('--output', type=str)
	args = argumentParser.parse_args(sys.argv[1:])

	with open(args.output, 'w') as file:
		image = PIL.Image.open(args.input)

		bgColor = image.getpixel((0,0))

		xRange = range(0, int(image.width/16))
		yRange = range(0, int(image.height/16))

		for y in yRange:
			for x in xRange:
				pattern = GetSprite16x16(image, x*16, y*16, bgColor)
				line = "\t"
				for bits in pattern:
					line += '0x' + format(bits,'02x') + ","
				line += '\n'
				file.write(line)
