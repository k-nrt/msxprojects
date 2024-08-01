
from array import array
from email.mime import image
import PIL.Image

class IndexImage:
	def __init__(self, width, height) -> None:
		self.width = width
		self.height = height
		self.data = [int(0)]*width*height
		return

	def __pos__(self, x:int, y:int) -> int:
		if x < 0 or self.width <= x or y < 0 or self.height <= y:
			return -1
		else:
			return x + y*self.width

	def Set(self, x:int, y:int, value:int):
		pos = self.__pos__(x,y)
		if pos < 0:
			return
		self.data[pos] = value

	def Get(self, x:int, y:int, failure:int) -> int:
		pos = self.__pos__(x,y)
		if pos < 0:
			return failure
		return self.data[pos]

	def CreateImage(self, palette:list) -> PIL.Image.Image:
		image = PIL.Image.new("RGB", (self.width, self.height), (0,0,0))
		for y in range(0, self.height):
			for x in range(0, self.width):
				pos = x + y*self.width
				value = self.data[pos]
				if 0 <= value and value < len(palette):
					image.putpixel((x,y),palette[value])
		return image

	def Copy(self, dx:int, dy:int, src:any, sx:int, sy:int, sw:int, sh:int):
		for y in range(0, sh):
			for x in range(0, sw):
				value = src.Get(sx+x,sy+y,0)
				pos = self.__pos__(dx+x, dy+y)
				if 0 <= pos:
					self.data[pos] = value
		return

class SpriteImage:
	def __init__(self, pattern:bytearray, color:bytearray) -> None:
		self.pattern = pattern
		self.color = color
		return

	def ToIndexImage(self) -> IndexImage:
		image = IndexImage(16, 16)
		for i, value in enumerate(self.pattern):
			x = int(i/16)*8
			y = i % 16
			for j in range(0, 8):
				if int(value) & (0x80 >> j) != 0:
					image.Set(x+j, y, int(self.color[y]) & 0xf)
		return image

def DumpHexText(data, path):
	with open(path,'w') as file:
		for i,value in enumerate(data):
			if (i % 16) == 0:
				file.write('{0:04x} '.format(i))
			file.write('{0:02x}'.format(int(value)))
			if (i % 16) <15:
				file.write(' ')
			else:
				file.write('\n')

		file.close()
	return

def LoadPalette(data:bytearray, offset:int) -> list:
	palette = list()
	palette.append((0,0,0))
	palette.append((0,0,0))

	for i in range(0,13):
		pos = i*2 + offset
		rg = int(data[pos])
		bb = int(data[pos+1])

		r = ((rg & 0xf0) << 1)
		b = ((rg & 0x0f) << 5)
		g = ((bb & 0x0f) << 5)
		palette.append((r,g,b))

	palette.append((0xff,0xff,0xff))
	return palette

def LoadMapChip(data:bytearray, offset:int) -> IndexImage:
	image = IndexImage(96,112)
	for cx in range(0,12):
		for cy in range(0,14):
			basePos = cx * 32 + cy * 32 * 12 + offset
			for px in range(0,4):
				for py in range(0,8):
					pos = basePos + py*4 + px
					c = int(data[pos])
					c0 = (c & 0xf0) >> 4
					c1 = (c & 0x0f)
					x = cx*8 + px*2
					y = cy*8 + py
					image.Set(x,y, c0)
					image.Set(x+1,y, c1)
	return image

def LoadMap(data:bytearray, offset) -> IndexImage:
	width = 32
	height = 24*9+4
	image = IndexImage(width, height)
	for y in range(0, height):
		for x in range(0, width):
			pos = x + y*32 + offset
			c = int(data[pos])
			image.Set(x, y, c)

	return image

def ResolveMap(map:IndexImage, mapChip:IndexImage) -> IndexImage:
	image = IndexImage(map.width*8, map.height*8)
	for y in range(0, map.height):
		for x in range(0, map.width):
			id = map.Get(x,y,0)
			mx = (id%12)*8
			my = int(id/12)*8
			image.Copy(x*8, (map.height-1-y)*8, mapChip, mx, my, 8, 8)
	return image

def LoadEnemyEntry(data:bytearray, offset:int) -> list:
	entry = list()
	height = 24*9 + 4 - 22
	for y in range(0,height):
		pos = 4*y+offset
		entry.append((int(data[pos]), int(data[pos+1]), int(data[pos+2]), int(data[pos+3])))
	return entry

def LoadSprite(data:bytearray, count:int, offset:int) -> list:
	sprites = list()
	for sp in range(0,count):
		pattern = sp*48 + offset
		color = pattern + 32
		sprites.append(SpriteImage(data[pattern:pattern+32], data[color:color+16]))
	return sprites

def CompressMap(map:IndexImage):
	chip16x16 = dict()
	width = 32
	height = 24*9+4
	for y in range(0, int(height/2)):
		for x in range(0, int(width/2)):
			c0 = map.Get(x*2,   height - 2 - y*2, 0xff)
			c1 = map.Get(x*2+1, height - 2 - y*2, 0xff)
			c2 = map.Get(x*2,   height - 2 - y*2 + 1, 0xff)
			c3 = map.Get(x*2+1, height - 2 - y*2 + 1, 0xff)
			key = c0 << 24 | c1 << 16 | c2 << 8 | c3
			if key in chip16x16.keys():
				chip16x16[key] += 1
			else:
				chip16x16[key] = 1
	
	return chip16x16

if __name__ == "__main__":
	with open('projects/yk/dsk/file0000.dat','rb') as file:
		data = file.read()
		file.close()

	data = data[7:]

	DumpHexText(data, 'projects/yk/doc/dump0000.txt')

	#0x0000 + 18 = 0x0012
	DumpHexText(data[0x0000:0x0012],'projects/yk/doc/dump0x0000-0x0012.txt')

	#0x0012 + (13*2) = 0x002c
	palette = LoadPalette(data, 0x0012)

	#0x002c + 64 = 0x006c
	DumpHexText(data[0x002c:0x006c],'projects/yk/doc/dump0x002c-0x006c.txt')

	#0x006c + (12*14*32) = 0x156c
	mapChip = LoadMapChip(data, 0x006c)

	# 0x156c + ((24*9+4)*32) = 0x30ec
	map = LoadMap(data, 0x006C + 12*14*32)

	compressedMap = CompressMap(map)
	print(len(compressedMap))
	with open('projects/yk/doc/cmap0000.txt', 'w') as file:
		file.write('{0}\n'.format(len(compressedMap)))
		for key in compressedMap.keys():
			file.write('{0:08x} = {1}\n'.format(key, compressedMap[key]))
		file.close()

	# 0x30ec + ((24*9+4-22)*4) = 0x3040
	enemyEntries = LoadEnemyEntry(data, 0x30ec)

	# 0x3404 + 48*30 = 0x39a4
	enemySpriteImages = LoadSprite(data, 30, 0x3404)

	# 0x39a4 + 48*16 = 0x3ca4
	bossSpriteImages = LoadSprite(data, 16, 0x39a4)

	# map chips.
	mapChip.CreateImage(palette).save('projects/yk/doc/mapchip0000.png')

	# map.
	resolvedMapImage = ResolveMap(map, mapChip)
	resolvedMapImage.CreateImage(palette).save('projects/yk/doc/map0000.png')

	# enemy sprite.
	resolvedSpriteImage = IndexImage(16*3, 16*10)
	for i, spriteImage in enumerate(enemySpriteImages):
		indexImage = spriteImage.ToIndexImage()
		x = (i % 3)*16
		y = int(i/3)*16
		resolvedSpriteImage.Copy(x, y, indexImage, 0, 0, 16, 16)
	resolvedSpriteImage.CreateImage(palette).save('projects/yk/doc/sprite0000.png')

	# enemy entry.
	for i, entry in enumerate(enemyEntries):
		y = (len(enemyEntries) - 1 -i)*8
		id = entry[0]
		x = entry[1]*8
		if 0 <= id and id < 10:
			resolvedMapImage.Copy(x, y, resolvedSpriteImage, 0, id*16, 16, 16) 
		id = entry[2]
		x = entry[3]*8
		if 0 <= id and id < 10:
			resolvedMapImage.Copy(x, y, resolvedSpriteImage, 0, id*16, 16, 16) 
	resolvedMapImage.CreateImage(palette).save('projects/yk/doc/entry0000.png')

	# boss sprite.
	resolvedBossSpriteImage = IndexImage(16*4, 16*4)
	for i, spriteImage in enumerate(bossSpriteImages):
		indexImage = spriteImage.ToIndexImage()
		x = (i % 4)*16
		y = int(i/4)*16
		resolvedBossSpriteImage.Copy(x, y, indexImage, 0, 0, 16, 16)
	resolvedBossSpriteImage.CreateImage(palette).save('projects/yk/doc/boss0000.png')
