from PIL import Image

# Open WHITE logo and resize with CORRECT aspect ratio
img = Image.open("images/golabs_logow.png").convert("RGBA")
print(f"Original size: {img.width}x{img.height}")

# Resize to 150x150 (square, maintains aspect ratio)
new_size = 150
img = img.resize((new_size, new_size), Image.Resampling.LANCZOS)
print(f"Resized to: {img.width}x{img.height}")

# Background color (UI_COLOR_BG_DARK = 0x0F172A)
bg_r = 0x0F
bg_g = 0x17
bg_b = 0x2A

# Generate RGB565 data
pixels = []
for y in range(img.height):
    for x in range(img.width):
        r, g, b, a = img.getpixel((x, y))
        
        # Alpha blend with background
        if a < 255:
            alpha = a / 255.0
            r = int(r * alpha + bg_r * (1 - alpha))
            g = int(g * alpha + bg_g * (1 - alpha))
            b = int(b * alpha + bg_b * (1 - alpha))
        
        # Convert to RGB565 (5 bits red, 6 bits green, 5 bits blue)
        rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
        
        # Swap bytes for little endian
        pixels.append(rgb565 & 0xFF)
        pixels.append((rgb565 >> 8) & 0xFF)

# Write C header file
with open("src/ui/golabs_logo.c", "w") as f:
    f.write(f'''
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

// Golabs Logo: {new_size}x{new_size} pixels, RGB565 format
static const LV_ATTRIBUTE_MEM_ALIGN uint8_t golabs_logo_map[] = {{
''')
    
    # Write pixel data
    for i, byte in enumerate(pixels):
        if i % 20 == 0:
            f.write("    ")
        f.write(f"0x{byte:02x},")
        if (i + 1) % 20 == 0:
            f.write("\n")
    
    f.write(f'''
}};

const lv_img_dsc_t golabs_logo = {{
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = {new_size},
    .header.h = {new_size},
    .data_size = sizeof(golabs_logo_map),
    .data = golabs_logo_map,
}};
''')

print(f"Generated golabs_logo.c ({new_size}x{new_size}, {len(pixels)} bytes)")
