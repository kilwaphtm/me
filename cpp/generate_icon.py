import sys

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, "rb") as f:
    data = f.read()

with open(output_file, "w", encoding="utf-8") as f:
    f.write("extern const unsigned char g_icon_png[] = {\n")

    for i in range(0, len(data), 16):
        chunk = data[i:i + 16]
        f.write("    ")
        f.write(", ".join(f"0x{b:02X}" for b in chunk))
        f.write(",\n")

    f.write("};\n")
    f.write(f"extern const unsigned int g_icon_png_size = {len(data)};\n")