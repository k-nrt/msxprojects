#include <msx-bios-wrapper.h>

#include "macros.h"

u8 g_buffer[512];

void DiskRead(void *pAddress, u16 sector, u8 count)
{
    UNUSED(pAddress);
    UNUSED(sector);
    UNUSED(count);
    
    __asm
        push    ix
        ld      hl, #0x0004
        add     hl,sp

        ld      e,(hl)              ; de = transfer address
        inc     hl
        ld      d,(hl)
    	ld		c,#0x1a             ; c = BDOS set transfer address
	    call	0xf37d              ; call BDOS

        ld      hl,#0x0006
        add     hl,sp

        ld      e,(hl)              ; de = logical sector
        inc     hl
        ld      d,(hl)
        inc     hl
	    ld		h,(hl)              ; h = sector count
        ld      l,#0x00             ; l = 0 = drive A:
        ld		c,#0x2f             ; c = BDOS read sector
	    call	0xf37d              ; call BDOS
        pop     ix
    __endasm;
}

void VDPWriteBytes(u8 vramHigh, u16 vramLow, const void *pAddress, u16 size)
{
    UNUSED(vramHigh);
    UNUSED(vramLow);
    UNUSED(pAddress);
    UNUSED(size);
    __asm
    VDPWriteBytes:
        ld      hl,#0x0002       ; skip return address
        add     hl,sp

        ld      b,(hl)      ; b = addressHigh
        inc     hl
        ld      e,(hl)      ; de = addressLow
        inc     hl
        ld      d,(hl)

        call    VDPWriteBeginDI

        ld      hl,#5       ; skip ret adressHigh addressLow
        add     hl,sp
        ld      e,(hl)      ; de = values
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      a,(hl)
        inc     hl
        ld      h,(hl)      ; hl = size
        ld      l,a

    VDPWriteBytes_Loop:
        ld      a,(de)
        out     (c),a
        inc     de
        dec     hl
        ld      a,h
        or      l
        jr      nz,VDPWriteBytes_Loop
        ei
        ret

    VDPWriteBeginDI:
        VDPRead  = 0x0006
        VDPWrite = 0x0007

        ld      a,(#VDPWrite)
        inc     a
        ld      c,a             ; c = write control register
        ld      a,b             ; a = vram high 1

        rlc     d               ; ad <<= 2
        rla
        rlc     d
        rla
        and     #0x07
        di
        out     (c),a           ; a = vram address upper 3 bits
        ld      a,#0x80+#14     ; 0b10xxxxxx 
        out     (c),a           ; a = write control register #14

        ld      a,e
        out     (c),a           ; a = vram address lower 8 bits
        srl     d               ; d >>= 2
        srl     d               ; d = restore vram address and fill 0 to upper 2 bits
        ld      a,#0x40         ; 0b01xxxxxx
        or      d
        out     (c),a           ; a = write vram address middle 6 bits

        dec     c               ; c = i/o write vram

        ret

    __endasm;
}

void VDPPaletteWrite(u8 start, const u16 *p0GRB, u8 count)
{
    UNUSED(start);
    UNUSED(p0GRB);
    UNUSED(count);

    __asm
        ld      hl,#0x0002
        add     hl,sp

        ld      c,(hl)
        inc     hl
        
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        
        ld      b,(hl)
        ld      l,c
        call    VDPPaletteWrite
        ret
 
    ;------------------------------------------------------------------------------
    ; in L start
    ; in B count
    ; in DE palette data
    ;------------------------------------------------------------------------------
    VDPPaletteWrite:
        ld      a,(#VDPWrite)
        inc     a
        ld      c,a             ; c = write control register

        ld      a,l
        and     #0x0f           ; palette start

        di
        out     (c),a           ; a = palette start index
        ld      a,#0x80+#16     ; 0b10xxxxxx 
        out     (c),a           ; a = write control register #16

        inc     c               ; c = i/o write palette

    VDPPaletteWrite_Loop:
        ld      a,(de)
        out     (c),a
        inc     de
        ld      a,(de)
        out     (c),a
        inc     de

        djnz    VDPPaletteWrite_Loop

        ei
        ret

    __endasm;
}

//! DFPAGE(FAF5H, 1)
//! 内容	ディスプレイページ番号
SDCC_FIXED_ADDRESS(0xFAF5) u8 DFPAGE;

void VDPSetDisplayPage(u8 nPage)
{
    UNUSED(nPage);

    __asm
    	.globl	_DFPAGE

        ld		a,(#VDPWrite)
        inc		a
        ld      c,a             ; c = write control register

        ld		hl,#0x0002
        add		hl,sp
        ld		a,(hl)		; nPage
        ld		(_DFPAGE),a
        and		#0x03
        rrca
        rrca
        rrca
        or		#0x1f
        
        di
        out		(c),a
        ld		a,#0x80+#0x02	; R#2 = pattern name table address
        out		(c),a
        ei
    __endasm;
}

void VDPCommandHMMM(u16 sx, u16 sy, u16 dx, u16 dy, s16 nx, s16 ny)
{
    UNUSED(sx);
    UNUSED(sy);
    UNUSED(dx);
    UNUSED(dy);
    UNUSED(nx);
    UNUSED(ny);

    __asm
        di
        ld      a,(#VDPRead)
        inc     a
        ld      c,a
        ld      b,#2
        ld      d,#0x80+#15

    VDPCommandHmmm_Wait:
        out     (c),b
        out     (c),d
        in      a,(c)
        and     #0x01
        jp      nz,VDPCommandHmmm_Wait

        xor     a
        out     (c),a
        out     (c),d
        in      a,(c)

        ld      a,(#VDPWrite)
        ld      c,a
        inc     c               ; c = port#1
        ld      a,#32
        out     (c),a
        ld      a,#0x80+#17
        out     (c),a           ; R#17 = 32

        inc     c
        inc     c               ; c = port#3

        ld      hl,#0x0002
        add     hl,sp

        ld      a,(hl)          ; a = sx low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = sx high
        and     #0x01
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = sy low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = sy high
        and     #0x03
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = nx low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = nx high
        and     #0x01
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = ny low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; a = ny high
        and     #0x03
        out     (c),a
        inc     hl

        ld      b,#0x00         ; b = dix,diy

        ld      e,(hl)          ; ae = nx
        inc     hl
        ld      a,(hl)
        inc     hl

        or      a
        jp      p,VDPCommandHMMM_SetNX

        set     2,b             ; dix = left
        cpl                     ; ae = -ae
        ld      d,a
        ld      a,e
        cpl
        ld      e,a
        inc     de
        ld      a,d

    VDPCommandHMMM_SetNX:
        out     (c),e           ; nx low
        and     #0x01
        out     (c),a           ; nx high

        ld      e,(hl)          ; de = ny
        inc     hl
        ld      a,(hl)

        or      a
        jp      p,VDPCommandHMMM_SetNY

        set     3,b             ; diy = top
        cpl                     ; ae = -ae
        ld      d,a
        ld      a,e
        cpl
        ld      e,a
        inc     de
        ld      a,d

    VDPCommandHMMM_SetNY:
        out     (c),e           ; ny low
        and     #0x03
        out     (c),a           ; ny high          

        out     (c),b           ; skip
        out     (c),b           ; dix,diy

        ld      a,#0xd0         ; a = HMMM
        out     (c),a

        ei
    __endasm;
}

void VDPCommandHMMV(u16 x, u16 y, u16 w, u16 h, u8 fill)
{
    UNUSED(x);
    UNUSED(y);
    UNUSED(w);
    UNUSED(h);
    UNUSED(fill);

    __asm
        ld      a,(#VDPRead)
        inc     a
        ld      c,a
        ld      b,#2
        ld      d,#0x8f

    VDPCommandHMMV_Wait:
        di
        out     (c),b
        out     (c),d
        in      a,(c)

        and     #0x01
        jp      nz,VDPCommandHMMV_Wait

        xor     a
        out     (c),a
        out     (c),d
        in      a,(c)

    VDPCommandHMMV_Begin:
        ld      a,(#VDPWrite)
        ld      c,a
        inc     c               ; c = port#1
        ld      a,#36
        out     (c),a
        ld      a,#0x80+#17
        out     (c),a           ; R#17 = 32

        inc     c
        inc     c               ; c = port#3

        ld      hl,#0x0002
        add     hl,sp

        ld      a,(hl)          ; dx low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; dx high
        and     #0x01
        out     (c),a
        inc     hl

        ld      a,(hl)          ; dy low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; dy high
        and     #0x03
        out     (c),a
        inc     hl

        ld      a,(hl)          ; nx low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; nx high
        and     #0x01
        out     (c),a
        inc     hl

        ld      a,(hl)          ; ny low
        out     (c),a
        inc     hl

        ld      a,(hl)          ; ny high
        and     #0x03
        out     (c),a
        inc     hl

        ld      a,(hl)          ; clr
        out     (c),a

        xor     a               ; mxd, dix, diy
        out     (c),a

        ld      a,#0xc0
        out     (c),a

        ei
    __endasm;
}

void VDPCommandWait()
{
    __asm
        di
        ld      a,(#VDPRead)
        inc     a
        ld      c,a
        ld      b,#2
        ld      d,#0x8f

    VDPCommandWait_Loop:
        out     (c),b
        out     (c),d
        in      a,(c)

        and     #0x01
        jp      nz,VDPCommandWait_Loop

        xor     a
        out     (c),a
        out     (c),d
        in      a,(c)
        ei
    __endasm;
}

void VDPSetAdjust(s8 x, s8 y)
{
    UNUSED(x);
    UNUSED(y);
    __asm
        ld      a,(#VDPWrite)
        inc     a
        ld      c,a
        
        ld      hl,#0x0002
        add     hl,sp
        ld      a,(hl)
        inc     hl
        and     #0x0f
        ld      b,a
        
        ld      a,(hl)
        rlca
        rlca
        rlca
        rlca
        and     #0xf0
        or      b

        di
        out     (c),a
        ld      a,#0x80+#18
        out     (c),a
        ei
    __endasm;
}

void VDPSet192()
{
    __asm
        ld      a,(#VDPWrite)
        inc     a
        ld      c,a

        ld      a,(#0xffe7+#9-#8)
        and     #0x7f
        ld      (#0xffe7+#9-#8),a

        di
        out     (c),a
        ld      a,#0x80+#9
        out     (c),a
        ei
    __endasm;
}

void DiskReadVram(u8 vramHigh, u16 vramLow, u16 sector, u16 sectorCount)
{
    u16 i;
    for (i = 0; i < sectorCount; i++)
    {
        DiskRead(g_buffer, sector, 1);
        VDPWriteBytes(vramHigh, vramLow, g_buffer, 512);
        
        sector++;
        vramLow += 512;
    }
}

void Print(u8 x, u8 y, const char* pText, u16 size)
{
    if (size == 0)
    {
        const char* pPos;
        for (pPos = pText; (*pPos != 0) && (size < 65535); pPos++)
        {
            size++;
        }
    }

    if (0 < size)
    {
        u16 address = ((u16)y)*40 + ((u16)x);
        //msxBiosCopyRAMtoVRAM((u16) pText, address, size);
        VDPWriteBytes(0, address, pText, size);
    }
}

void PrintHex16(u8 x, u8 y, u16 hex)
{
    static char text[4];
    const char hexs[] = "0123456789abcdef";

    text[0] = hexs[(hex >> 12) & 0x0f];
    text[1] = hexs[(hex >> 8) & 0x0f];
    text[2] = hexs[(hex >> 4) & 0x0f];
    text[3] = hexs[(hex >> 0) & 0x0f];

    Print(x, y, text, 4);
}

u8 g_names[1024];

void DrawBG(u8 *pName, s16 dx, u16 offsetY)
{
    u8 y;
    u16 dy = offsetY;

    if (dx < 0)
    {
        u16 offset = -dx;
        u16 width = 40 - offset;
        for (y = 0; y < 15; y++)
        {
            u8  name = *pName;
            u16 sx,sy;

            sx = (((u16)name)%6)*40;
            sy = (((u16)name)/6)*12;

            VDPCommandWait();
            VDPCommandHMMM(sx+offset,sy + 512,0,dy,width,12);

            pName++;
            dy += 12;
        }
    }
    else if (dx < 200)
    {
        for (y = 0; y < 15; y++)
        {
            u8  name = *pName;
            u16 sx,sy;

            sx = (((u16)name)%6)*40;
            sy = (((u16)name)/6)*12;

            VDPCommandWait();
            VDPCommandHMMM(sx,sy + 512,dx,dy,40,12);

            pName++;
            dy += 12;
        }
    }
    else if (dx < 240)
    {
        u16 width = 240 - dx;
        for (y = 0; y < 15; y++)
        {
            u8  name = *pName;
            u16 sx,sy;

            sx = (((u16)name)%6)*40;
            sy = (((u16)name)/6)*12;

            VDPCommandWait();
            VDPCommandHMMM(sx,sy + 512,dx,dy,width,12);

            pName++;
            dy += 12;
        }
    }
}

void DrawBGChip(u8 name, s16 dx, u16 dy)
{
    u16 sx = (((u16)name)%6)*40;
    u16 sy = 512 + (((u16)name)/6)*12;
    if (dx < 0)
    {
        u16 offset = -dx;
        u16 width = 40 - offset;
        VDPCommandWait();
        VDPCommandHMMM(sx + offset, sy, 8, dy, width, 12);
    }
    else if (dx < 200)
    {
        VDPCommandWait();
        VDPCommandHMMM(sx, sy, dx + 8, dy, 40, 12);
    }
    else if (dx < 240)
    {
        u16 width = 240 - dx;
        VDPCommandWait();
        VDPCommandHMMM(sx, sy, dx + 8, dy, width, 12);
    }
}

void DrawBGHorizontal(u8 *pName, s16 offsetX, u16 offsetY)
{
    u8 x;
    s16 dx = offsetX;
    for(x = 0; x < 7; x++)
    {
        if (240 <= dx)
        {
            break;
        }

        DrawBGChip(*pName, dx, offsetY);
        dx += 40;
        pName += 15;
    }
}

void DrawScreen(u8 mapX, s16 offsetX, u16 offsetY)
{
    u8 y;
    u8 *pName = g_names;
    u16 dy = offsetY;
    pName += mapX*15;

    for (y = 0; y < 15; y++)
    {
        DrawBGHorizontal(pName, offsetX, dy);
        pName++;
        dy += 12;
    }
    /*
    u8 x;
    u8 *pName = g_names;
    s16 dx = offsetX;
    pName += mapX*15;

    for(x = 0; x < 7; x++)
    {
        if (240 <= dx)
        {
            break;
        }

        DrawBG(pName,dx,offsetY);
        dx += 40;
        pName += 15;
    }
    */
}


void main()
{
    u16 offset, offsetY;
   	msx2BiosChangeModePalette(5);
	msxBiosChangeColor(5, 15, 0, 0);

    VDPSetDisplayPage(2);
    DiskReadVram(1, 0, 64, 64);

    VDPSetDisplayPage(3);
    DiskReadVram(1, 0x8000, 128, 64);

    //VDPSetDisplayPage(2);
    DiskRead(g_buffer, 60, 1);
    VDPPaletteWrite(0, g_buffer, 16);

    VDPSetDisplayPage(0);
    VDPCommandHMMM(0, 512, 0, 0, 256, 120);

    DiskRead(g_names, 62, 2);

    DrawScreen(0, 0, 0);

    VDPCommandHMMV(0,0, 256, 512, 0x00);
    VDPSet192();

    offsetY = 256;
    for(;;)
    {
        s16 x = 0;
        for (offset = 0; offset < 54; offset++)
        {
            for (; x < 40; x+=16)
            {
                u8 y;
                u8 *pName = g_names;
                u16 dy = offsetY;
                s8 ax = -8;
                pName += offset*15;

                for (y = 0; y < 15; y++)
                {
                    DrawBGHorizontal(pName, -x, dy);
                    pName++;
                    dy += 12;

                    VDPSetAdjust(ax,0);
                    ax += 1;
                }

                DrawBGHorizontal(pName - 1, -x, dy-12);
                VDPSetAdjust(ax,0);

                ax = -8;
                if (offsetY == 0)
                {
                    VDPSetAdjust(ax,0);
                    VDPSetDisplayPage(0);
                    offsetY = 256;
                }
                else
                {
                    VDPSetAdjust(ax,0);
                    VDPSetDisplayPage(1);
                    offsetY = 0;
                }

                //while(!msxBiosGetTrigger(0))
                //{}

            }
            x -= 40;
        }
    }


    for(;;)
    {}

    /*
    msxBiosChangeScreenMode(0);
    msxBiosChangeColor(0,15,0,0);

    Print(2,4,"Hello World", 0);

    {
        u16 sector = 0;
        for(sector = 0; sector < 128; sector++)
        {
            PrintHex16(0,5,sector);
            DiskRead(g_buffer, sector, 1);
            Print(0,6,g_buffer, 512);
        }

        for(;;)
        {}
    }
    */

}
